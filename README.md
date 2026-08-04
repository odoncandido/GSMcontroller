# GSMController

Biblioteca C++ para Arduino/ESP32 desenvolvida para facilitar o controle de módulos GSM baseados no **SIM800L** através de comandos AT.

A biblioteca fornece uma camada simples para comunicação com o modem, permitindo enviar comandos AT, realizar chamadas, desligar chamadas e processar automaticamente eventos como:

- 📱 SMS recebido
- 📞 Chamada recebida
- ❌ Chamada encerrada
- 🔴 Linha ocupada
- 📵 Sem linha
- ⏱️ Chamada não atendida
- ℹ️ Outras respostas do modem

---

## 📋 Características

- Comunicação através de `HardwareSerial`
- Compatível com ESP32 e placas Arduino que possuem `HardwareSerial`
- Envio de comandos AT
- Realização de chamadas
- Encerramento de chamadas
- Identificação do número de chamadas recebidas
- Processamento de SMS recebidos
- Detecção automática de eventos GSM
- Interface simples baseada em classes C++

---

## 🔧 Requisitos

### Hardware

- ESP32 ou Arduino compatível
- SIM800L
- SIM card válido
- Fonte de alimentação adequada para o SIM800L

> ⚠️ O SIM800L possui picos de corrente elevados durante a comunicação com a rede GSM. 
> Evite alimentar o módulo diretamente a partir de uma porta GPIO ou de uma fonte incapaz de fornecer corrente suficiente.

### Software

- Arduino IDE ou PlatformIO
- C++11 ou superior
- Biblioteca `HardwareSerial` disponível na plataforma utilizada

---

# 📦 Instalação

## Arduino IDE

Baixe ou clone este repositório:

```bash
git clone https://github.com/odoncandido/GSMController.git

# Estrutura
```
GSMController/
├── src/
│   ├── GSMController.cpp
│   └── GSMController.h
│
├── examples/
│   ├── Basic/
│   │   └── Basic.ino
│   │
│   ├── SendSMS/
│   │   └── SendSMS.ino
│   │
│   └── ReceiveSMS/
│       └── ReceiveSMS.ino
│
├── library.properties
├── README.md
├── LICENSE
└── keywords.txt
```
