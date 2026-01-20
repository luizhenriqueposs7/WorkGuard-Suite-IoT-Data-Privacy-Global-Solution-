/*
 * GLOBAL SOLUTION 2025.2 - WORKGUARD SUITE
 * TEMA: Tecnologias Inteligentes para o Futuro do Trabalho
 * * ADAPTAÇÃO:
 * - Ultrassônico: Monitora proximidade de áreas de risco (Máquinas).
 * - DHT22: Conforto Térmico (NR-17).
 * - LDR: Ergonomia Visual.
 * - Potenciômetro 1: Simula Sensor de Gás/CO2.
 * - Botão: Alterna entre modo ADMIN (Dados Reais) e USER (Anonimizado).
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// --- 1. DEFINIÇÕES DE HARDWARE ---
// Sensores
#define ULTRASONIC_TRIG_PIN 5
#define ULTRASONIC_ECHO_PIN 18
#define DHT_PIN 32
#define DHT_TYPE DHT22  // Se usar o azul, mude para DHT11
#define LDR_PIN 34
#define POT_CO2_PIN 26       // Antigo Pot Chuva
#define POT_RUIDO_PIN 25     // Antigo Pot Pressao

// Atuadores
#define LED_VERMELHO_PIN 16
#define LED_AMARELO_PIN 17
#define LED_VERDE_PIN 27
#define BUZZER_PIN 19

// Entrada
#define BUTTON_PIN 35

// LCD
#define LCD_COLUMNS 20
#define LCD_ROWS 4
#define LCD_ADDRESS 0x27

// Objetos
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
DHT dht(DHT_PIN, DHT_TYPE);

// Variáveis Globais de Sistema
bool modoAdmin = false;       // false = Anonimizado, true = Real
bool lastButtonState = HIGH;  // Controle do botão

// --- 2. NAMESPACE WORKGUARD (Requisito: Organização) ---
namespace WorkGuardSystem {

    // Variáveis de Ambiente
    float temperatura = 0.0;
    float umidade = 0.0;
    int luminosidade = 0;
    int distanciaMaquina = 0; // Em cm
    int nivelCO2 = 0;         // Simulado (0-100%)
    
    // Níveis de Alerta: 0=Seguro, 1=Atenção, 2=Perigo
    int statusGeral = 0; 

    // -- FUNÇÃO DE HASHING (Requisito: Anonimização) --
    String anonimizar(float valor, String prefixo) {
        // Cria um código hexadecimal baseado no valor para "esconder" o dado real
        long calc = (long)(valor * 17); 
        String hash = String(calc, HEX);
        hash.toUpperCase();
        return prefixo + "-" + hash + "X";
    }

    // -- LEITURA DO ULTRASSÔNICO --
    int lerDistancia() {
        digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
        long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 30000);
        int cm = duration * 0.034 / 2;
        if (cm == 0 || cm > 400) return 400; // Filtro de erro
        return cm;
    }

    // -- ATUALIZAÇÃO DOS DADOS --
    void atualizarSensores() {
        temperatura = dht.readTemperature();
        umidade = dht.readHumidity();
        luminosidade = map(analogRead(LDR_PIN), 0, 4095, 0, 100); // 0 a 100% de luz
        distanciaMaquina = lerDistancia();
        
        // Simuladores nos Potenciômetros
        nivelCO2 = map(analogRead(POT_CO2_PIN), 0, 4095, 0, 100); // ppm simulado em %
        
        // Tratamento de erro DHT
        if (isnan(temperatura)) temperatura = 0.0;
    }

    // -- LÓGICA DE INTELIGÊNCIA (Alertas Multinível) --
    void processarRiscos() {
        // Regra 1: Proximidade Perigosa de Máquina (NR-12)
        // Se estiver a menos de 30cm da máquina -> PERIGO
        if (distanciaMaquina < 30) {
            statusGeral = 2; 
            return; // Prioridade máxima
        }

        // Regra 2: Qualidade do Ar (CO2) ou Calor Excessivo
        if (nivelCO2 > 70 || temperatura > 30.0) {
            statusGeral = 2; // Perigo
        }
        // Regra 3: Desconforto (Luz baixa ou calor leve)
        else if (nivelCO2 > 40 || temperatura > 26.0 || luminosidade < 20) {
            statusGeral = 1; // Atenção
        }
        // Regra 4: Ambiente Ideal
        else {
            statusGeral = 0; // Seguro
        }
    }
}

// --- 3. FUNÇÕES AUXILIARES DE HARDWARE ---

void controlarAtuadores() {
    // Reseta Leds
    digitalWrite(LED_VERDE_PIN, LOW);
    digitalWrite(LED_AMARELO_PIN, LOW);
    digitalWrite(LED_VERMELHO_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    switch (WorkGuardSystem::statusGeral) {
        case 0: // Seguro
            digitalWrite(LED_VERDE_PIN, HIGH);
            break;
        case 1: // Atenção
            digitalWrite(LED_AMARELO_PIN, HIGH);
            break;
        case 2: // Perigo
            digitalWrite(LED_VERMELHO_PIN, HIGH);
            // Buzzer Intermitente (Simulado simples)
            if (millis() % 500 < 250) digitalWrite(BUZZER_PIN, HIGH);
            break;
    }
}

void atualizarLCD() {
    lcd.setCursor(0, 0);
    if (modoAdmin) lcd.print("ADMIN MODE [UNLOCKED]");
    else           lcd.print("USER MODE  [PRIVACY]");

    lcd.setCursor(0, 1);
    lcd.print("Status: ");
    if(WorkGuardSystem::statusGeral == 2) lcd.print("PERIGO!   ");
    else if(WorkGuardSystem::statusGeral == 1) lcd.print("ATENCAO   ");
    else lcd.print("SEGURO    ");

    lcd.setCursor(0, 2);
    lcd.print("Temp: "); 
    if(modoAdmin) { lcd.print(WorkGuardSystem::temperatura, 1); lcd.print("C"); }
    else lcd.print("**.**"); // Oculta no LCD também se quiser, ou mostra hash

    lcd.setCursor(0, 3);
    lcd.print("Prox.Maq: ");
    lcd.print(WorkGuardSystem::distanciaMaquina); lcd.print("cm");
}

// --- SETUP ---
void setup() {
    Serial.begin(115200);
    
    // Configuração de Pinos
    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);
    pinMode(LED_VERMELHO_PIN, OUTPUT);
    pinMode(LED_AMARELO_PIN, OUTPUT);
    pinMode(LED_VERDE_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    pinMode(LDR_PIN, INPUT);
    pinMode(POT_CO2_PIN, INPUT);
    pinMode(POT_RUIDO_PIN, INPUT);

    dht.begin();
    lcd.init();
    lcd.backlight();

    lcd.setCursor(0,0);
    lcd.print("WorkGuard Suite");
    lcd.setCursor(0,1);
    lcd.print("Inicializando...");
    delay(2000);
    lcd.clear();
}

// --- LOOP PRINCIPAL ---
void loop() {
    
    // 1. Leitura do Botão (Toggle Admin/User)
    bool btnReading = digitalRead(BUTTON_PIN);
    if (btnReading == LOW && lastButtonState == HIGH) {
        modoAdmin = !modoAdmin; // Inverte modo
        delay(50); // Debounce
    }
    lastButtonState = btnReading;

    // 2. Executar Lógica do Sistema (Namespace)
    WorkGuardSystem::atualizarSensores();
    WorkGuardSystem::processarRiscos();

    // 3. Atualizar Hardware (Leds e LCD)
    controlarAtuadores();
    atualizarLCD();

    // 4. DASHBOARD SERIAL (Requisito: Tabela + Anonimização)
    // Limpa o console visualmente
    for(int i=0; i<3; i++) Serial.println();

    Serial.println("=============================================================");
    Serial.print(" WORKGUARD IOT DASHBOARD | MODE: ");
    if(modoAdmin) Serial.println("ADMINISTRADOR (DADOS REAIS)");
    else          Serial.println("COLABORADOR (PRIVACIDADE ATIVA)");
    Serial.println("=============================================================");
    
    Serial.println("| SENSOR           | VALOR LIDO / HASH       | STATUS       |");
    Serial.println("|------------------|-------------------------|--------------|");

    // -- Linha Temperatura --
    Serial.print("| Temp. Ambiente   | ");
    if(modoAdmin) {
        Serial.print(WorkGuardSystem::temperatura); Serial.print(" C             ");
    } else {
        Serial.print(WorkGuardSystem::anonimizar(WorkGuardSystem::temperatura, "TMP"));
        Serial.print("             ");
    }
    Serial.print("| ");
    if(WorkGuardSystem::temperatura > 26) Serial.println("ALERTA       |"); else Serial.println("OK           |");

    // -- Linha Proximidade (Ultrassônico) --
    Serial.print("| Prox. Maquina    | ");
    Serial.print(WorkGuardSystem::distanciaMaquina); Serial.print(" cm              "); // Distância geralmente não precisa de hash
    Serial.print("| ");
    if(WorkGuardSystem::distanciaMaquina < 30) Serial.println("PERIGO!      |"); else Serial.println("SEGURO       |");

    // -- Linha CO2 (Potenciômetro) --
    Serial.print("| Qualidade Ar CO2 | ");
    if(modoAdmin) {
        Serial.print(WorkGuardSystem::nivelCO2); Serial.print(" %               ");
    } else {
        Serial.print(WorkGuardSystem::anonimizar(WorkGuardSystem::nivelCO2, "GAS"));
        Serial.print("             ");
    }
    Serial.print("| ");
    if(WorkGuardSystem::nivelCO2 > 70) Serial.println("RUIM         |"); else Serial.println("BOM          |");

    Serial.println("=============================================================");
    Serial.print("ALERTA GLOBAL DO SISTEMA: ");
    if(WorkGuardSystem::statusGeral == 0) Serial.println("VERDE (SEGURO)");
    if(WorkGuardSystem::statusGeral == 1) Serial.println("AMARELO (ATENCAO)");
    if(WorkGuardSystem::statusGeral == 2) Serial.println("VERMELHO (EVACUAR/PARAR)");
    Serial.println("=============================================================");

    delay(1000); // Atualiza a cada 1 segundo
}