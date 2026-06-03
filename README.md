# ☀️ Solar Control - Automação Residencial com Energia Solar

Projeto de automação residencial controlado remotamente via internet, utilizando ESP32 e protocolo MQTT. Desenvolvido para a **Feira de Ciências 2026**.

## 📋 Sobre o Projeto

O Solar Control permite controlar dispositivos elétricos (LEDs e lâmpada real via relé) de qualquer lugar do mundo usando apenas um celular. O sistema demonstra como a automação residencial pode ser integrada com energia solar para uma casa inteligente e sustentável.

## 🛠️ Componentes

| Componente | Função |
|---|---|
| ESP32 Dev Module | Microcontrolador com WiFi |
| 3x LEDs | Simulam iluminação dos cômodos |
| Módulo Relé 1 canal | Controla lâmpada real (110/220V) |
| Lâmpada | Demonstração de controle real |
| Carregador USB 5V | Alimentação do ESP32 |

## 📌 Pinagem

| GPIO | Dispositivo |
|---|---|
| 4 | LED 1 (Sala) |
| 5 | LED 2 (Cozinha) |
| 21 | LED 3 (Quarto) |
| 22 | Relé (Lâmpada) |

## 🌐 Como Funciona

```
Celular → Site (GitHub Pages) → MQTT (broker.emqx.io) → ESP32 → LEDs/Relé
```

1. O ESP32 conecta no WiFi (hotspot do celular) e no broker MQTT
2. O site público envia comandos via MQTT
3. O ESP32 recebe e liga/desliga os dispositivos em tempo real

## 🔗 Links

- **Demonstração:** `https://davidlindoetec.github.io/arduino-web-page/?t=feira2026`
- **Admin (logs):** `https://davidlindoetec.github.io/arduino-web-page/admin.html`

## 🚀 Como Usar

### Configurar o ESP32
1. Abra `arduino_solar_control.ino` no Arduino IDE
2. Instale a biblioteca **PubSubClient** (by Nick O'Leary)
3. Configure o WiFi no código (nome e senha do hotspot)
4. Selecione a placa "ESP32 Dev Module"
5. Faça upload

### Na Feira
1. Ligue o hotspot do celular
2. Conecte o ESP32 na tomada (carregador USB)
3. O ESP32 conecta automaticamente
4. Pessoas escaneiam o QR Code e controlam os dispositivos

## 🔒 Segurança

- Acesso ao controle só via QR Code (token na URL)
- Sessão de **2 minutos** por pessoa
- **Cooldown de 3 minutos** entre sessões (anti-spam)
- Apenas 1 pessoa usa por vez
- Logs registram todas as ações

## 📊 Funcionalidades do Site

- **Dispositivos:** Controle individual de LEDs e relé
- **Energia:** Simulação de geração solar, consumo e economia
- **Rotinas:** Temporizadores e automações pré-definidas
- **Admin:** Painel com logs em tempo real

## 📁 Estrutura de Arquivos

```
├── index.html          # Página principal (público)
├── admin.html          # Painel de logs (administrador)
├── arduino_solar_control/
│   └── arduino_solar_control.ino  # Código do ESP32
└── README.md
```

## 🔧 Tecnologias

- **Hardware:** ESP32, Relé, LEDs
- **Comunicação:** MQTT (broker.emqx.io)
- **Frontend:** HTML, CSS, JavaScript
- **Hospedagem:** GitHub Pages
- **Protocolo:** WebSocket (wss)

## 👤 Autor

Projeto desenvolvido para a Feira de Ciências 2026 - ETEC

---

*🌱 Energia solar é o futuro!*
