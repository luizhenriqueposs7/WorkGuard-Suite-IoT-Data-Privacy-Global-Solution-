# 🛡️ WorkGuard Suite: IoT, Segurança & LGPD

> **Global Solution FIAP** - Projeto de IoT focado em Segurança do Trabalho (NR-12/NR-17) com conformidade à LGPD.

### 📺 Demonstração e Pitch
[![Assista ao Vídeo no YouTube](https://img.youtube.com/vi/QYpTq7KOebo/0.jpg)](https://www.youtube.com/watch?v=QYpTq7KOebo)
*(Clique na imagem para assistir à explicação completa do projeto)*

---

## 📖 Sobre o Projeto
O **WorkGuard Suite** é um dispositivo de *Edge Computing* baseado em ESP32 desenvolvido para resolver um dilema moderno: como monitorar rigorosamente a segurança física dos colaboradores sem violar sua privacidade de dados?

O sistema monitora riscos ambientais e de acidentes em tempo real. Através de um botão físico, o dispositivo alterna entre:
* **Modo Admin:** Exibe dados brutos para manutenção.
* **Modo Privacidade (LGPD):** Aplica um algoritmo de *Hash* nos dados pessoais, permitindo que o monitoramento de segurança continue ativo sem expor a identidade ou dados sensíveis do trabalhador.

## ⚙️ Funcionalidades Técnicas
O código foi desenvolvido em **C++** utilizando a lógica de orientação a objetos e *namespaces* para organizar os sensores.

### 1. Monitoramento Inteligente (Sensores)
* **Distância (Ultrassônico HC-SR04):** Monitora a aproximação de máquinas perigosas (NR-12). Se < 30cm, aciona alerta crítico.
* **Conforto Térmico (DHT22):** Mede temperatura e umidade para garantir conformidade com a NR-17.
* **Ergonomia Visual (LDR):** Monitora a luminosidade do ambiente para evitar fadiga visual.
* **Qualidade do Ar (Potenciômetro/Simulação):** Simula um sensor de CO2/Gás para detectar insalubridade.

### 2. Privacidade por Design (Privacy by Design)
* **Algoritmo de Hash:** No modo usuário, os dados numéricos são convertidos para hexadecimal na saída Serial e LCD.
* **Indicador Físico:** Um **LED Azul** permanece aceso exclusivamente quando o modo de anonimização está ativo, dando feedback visual de segurança de dados ao usuário.

### 3. Feedback Multinível (Atuadores)
* **Semáforo de Risco:** LEDs Verde (Seguro), Amarelo (Atenção) e Vermelho (Perigo).
* **Display LCD 20x4:** Exibe status do sistema e leituras (anonimizadas ou reais).
* **Buzzer:** Alarme sonoro em caso de risco iminente (Nível 2).

## 🛠️ Tecnologias & Hardware
* **Microcontrolador:** ESP32 DevKit V1
* **Linguagem:** C++ (Wiring)
* **Simulação:** Wokwi
* **Componentes:**
    * 1x Sensor Ultrassônico HC-SR04
    * 1x Sensor DHT22
    * 1x LDR (Fotorresistor)
    * 1x Display LCD 20x4 (I2C)
    * LEDs, Resistores e Botão Pushbutton

## 🚀 Como Rodar (Simulação)
Este projeto está configurado para rodar no simulador **Wokwi**, dispensando hardware físico para testes.

1.  Clone este repositório.
2.  Abra a pasta do projeto no **VS Code**.
3.  Instale a extensão **Wokwi Simulator**.
4.  Abra o arquivo `diagram.json` e clique no botão "Play" da simulação.

## 👨‍💻 Autores (Time FIAP)
* **Luiz Henrique Poss** (RM: 562177)
* Gabriel Tonelli (RM: 564705)
* Vinícius Adrian (RM: 564962)

---
*Projeto acadêmico desenvolvido para a disciplina de Edge Computing & Computer Systems.*
