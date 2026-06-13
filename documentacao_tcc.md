# Documentação Completa do Projeto - TCC
## Solar Control: Automação Residencial com Energia Solar e IoT

---

## 1. VISÃO GERAL DO PROJETO

### 1.1 Objetivo
Desenvolver um sistema de automação residencial controlado remotamente via internet, utilizando um microcontrolador ESP32 e protocolo MQTT, demonstrando como a energia solar pode ser integrada com automação inteligente para criar uma casa sustentável e eficiente.

### 1.2 Conceito
O projeto permite que qualquer pessoa, de qualquer lugar do mundo, controle dispositivos elétricos (LEDs e lâmpada real) em tempo real através de um site acessível pelo celular. O sistema foi projetado para demonstrações em feiras de ciências, com controle de sessão, fila de espera e segurança.

### 1.3 Arquitetura do Sistema
```
Celular do usuário → Site (GitHub Pages) → Broker MQTT (broker.emqx.io) → ESP32 → LEDs/Relé
```

O fluxo de dados é bidirecional:
- Comandos vão do site para o ESP32
- Status dos dispositivos volta do ESP32 para o site

---

## 2. COMPONENTES DE HARDWARE

### 2.1 Lista de Materiais

| Componente | Especificação | Função |
|---|---|---|
| ESP32 Dev Module | Microcontrolador dual-core com WiFi/Bluetooth | Unidade central de processamento e comunicação |
| LED 1 | LED comum 5mm | Simula iluminação da Sala |
| LED 2 | LED comum 5mm | Simula iluminação da Cozinha |
| LED 3 | LED comum 5mm | Simula iluminação do Quarto |
| Módulo Relé 1 canal | 5V, contatos para 10A/250VAC | Controla lâmpada real (110/220V) |
| Lâmpada | 110V ou 220V | Demonstração de controle real de carga |
| Resistores | 220Ω (3 unidades) | Limitam corrente dos LEDs |
| Carregador USB | 5V 1A mínimo | Alimentação do ESP32 |
| Cabo micro-USB | - | Conexão e programação |
| Protoboard | - | Montagem do circuito |
| Jumpers | Macho-macho e macho-fêmea | Conexões elétricas |

### 2.2 Pinagem do ESP32

| Pino GPIO | Dispositivo Conectado | Tipo de Sinal |
|---|---|---|
| GPIO 4 | LED 1 (Sala) | Saída digital |
| GPIO 5 | LED 2 (Cozinha) | Saída digital |
| GPIO 21 | LED 3 (Quarto) | Saída digital |
| GPIO 22 | Módulo Relé (IN) | Saída digital |
| 5V (VIN) | Módulo Relé (VCC) | Alimentação |
| GND | Módulo Relé (GND) + LEDs (cátodo) | Referência |

### 2.3 Esquema de Conexão do Relé
```
ESP32 GPIO 22  →  IN (pino de sinal do módulo relé)
ESP32 VIN (5V) →  VCC (alimentação do módulo relé)
ESP32 GND      →  GND (terra do módulo relé)

Lado de alta tensão (lâmpada):
Fase da tomada   →  COM (comum do relé)
NO (normalmente aberto) →  Fio da lâmpada
Lâmpada          →  Neutro da tomada
```

### 2.4 Esquema de Conexão dos LEDs
```
ESP32 GPIO 4  →  Resistor 220Ω  →  Anodo LED 1  →  Cátodo  →  GND
ESP32 GPIO 5  →  Resistor 220Ω  →  Anodo LED 2  →  Cátodo  →  GND
ESP32 GPIO 21 →  Resistor 220Ω  →  Anodo LED 3  →  Cátodo  →  GND
```

---

## 3. TECNOLOGIAS UTILIZADAS

### 3.1 Hardware
- **ESP32**: Microcontrolador da Espressif com WiFi 802.11 b/g/n integrado, processador dual-core Xtensa LX6 a 240MHz, 520KB SRAM, GPIO programáveis.

### 3.2 Software e Protocolos
- **Arduino IDE**: Ambiente de desenvolvimento para programação do ESP32 em C/C++
- **MQTT (Message Queuing Telemetry Transport)**: Protocolo de comunicação leve para IoT, padrão publish/subscribe
- **WebSocket (WSS)**: Protocolo de comunicação bidirecional em tempo real entre navegador e servidor
- **HTML5/CSS3/JavaScript**: Tecnologias web para interface do usuário
- **GitHub Pages**: Hospedagem gratuita de sites estáticos

### 3.3 Serviços Externos
- **broker.emqx.io**: Broker MQTT público e gratuito da EMQX (porta 1883 para TCP, porta 8084 para WebSocket seguro)
- **GitHub Pages**: Hosting do site público (HTTPS)
- **Hotspot móvel**: Provedor de internet para o ESP32 na feira

### 3.4 Bibliotecas Utilizadas
- **WiFi.h**: Biblioteca nativa do ESP32 para conexão WiFi
- **PubSubClient** (by Nick O'Leary): Cliente MQTT para Arduino/ESP32
- **mqtt.min.js** (via unpkg CDN): Cliente MQTT JavaScript para o navegador

---

## 4. ETAPAS DE DESENVOLVIMENTO

### Etapa 1: Prototipação Inicial
**Objetivo**: Validar o controle básico dos LEDs via ESP32.

- Código inicial simples com `digitalWrite()` para piscar LEDs nos pinos GPIO 4, 5 e 21
- Teste de funcionamento dos LEDs no circuito
- Validação da alimentação via USB

### Etapa 2: Controle via Serial (USB)
**Objetivo**: Implementar controle por comandos de texto.

- Desenvolvimento do protocolo de comandos: `LED1ON`, `LED1OFF`, `LED2ON`, `LED2OFF`, `LED3ON`, `LED3OFF`, `ALLON`, `ALLOFF`, `STATUS`
- Comunicação Serial a 115200 baud
- Resposta em formato JSON para integração com software

### Etapa 3: Interface Web Local (Web Serial API)
**Objetivo**: Criar página web que controla via cabo USB.

- Desenvolvimento de interface HTML com design responsivo (dark theme)
- Implementação da Web Serial API para comunicação direta navegador → USB → ESP32
- Limitação encontrada: Web Serial API requer HTTPS ou localhost, não funciona abrindo arquivo direto
- Solução: servidor local via Node.js (`npx serve`)

### Etapa 4: Migração para WiFi + MQTT
**Objetivo**: Permitir controle remoto de qualquer lugar via internet.

- Abandono da abordagem Serial (limitada a conexão física)
- Escolha do protocolo MQTT por ser leve, eficiente e ideal para IoT
- Implementação no ESP32: conexão WiFi + cliente MQTT
- Implementação no site: cliente MQTT via WebSocket
- Broker utilizado: primeiro HiveMQ (instável), depois migrado para EMQX (mais confiável)

### Etapa 5: Hospedagem e Acesso Público
**Objetivo**: Disponibilizar o site na internet para acesso de qualquer dispositivo.

- Criação de repositório no GitHub
- Ativação do GitHub Pages para hospedagem gratuita com HTTPS
- URL final: `https://davidlindoetec.github.io/arduino-web-page-2/`
- Geração de QR Code para acesso rápido

### Etapa 6: Sistema de Sessão e Fila
**Objetivo**: Gerenciar múltiplos usuários na feira de ciências.

- Implementação de sessão de 2 minutos por pessoa
- Sistema de fila: quando alguém está usando, outros veem "Aguarde sua vez"
- Cooldown de 3 minutos: impede a mesma pessoa de usar repetidamente
- Desligamento automático de todos os dispositivos ao fim da sessão

### Etapa 7: Segurança
**Objetivo**: Proteger o sistema contra uso indevido.

Camadas de proteção implementadas:
1. **Token de acesso**: URL requer parâmetro `?t=feira2026` para funcionar. Sem o token, mostra "Acesso Restrito"
2. **Ofuscação do token**: Token armazenado como array de char codes, não como texto legível
3. **Bloqueio de DevTools**: Desabilita F12, Ctrl+Shift+I, Ctrl+U e botão direito do mouse
4. **Rate limiting**: Máximo de 30 comandos por sessão para evitar flood/spam
5. **Cooldown**: 3 minutos entre sessões da mesma pessoa
6. **Sessão temporária**: Acesso expira automaticamente em 2 minutos

### Etapa 8: Módulo Relé (Lâmpada Real)
**Objetivo**: Demonstrar controle de carga real (110/220V).

- Adição do módulo relé no GPIO 22
- Integração com o sistema de comandos MQTT
- Lâmpada real controlada pelo site

### Etapa 9: Simulação de Energia Solar
**Objetivo**: Demonstrar conceitos de geração e economia de energia.

Cálculos implementados:
- **Consumo real**: Soma da potência dos dispositivos ligados (LED = 5W, Lâmpada = 60W)
- **Geração solar simulada**: Baseada na hora real do dia, com curva senoidal (pico ao meio-dia = 150W, zero à noite)
- **Economia mensal**: `Geração × 10h/dia × 30 dias × R$0,85/kWh`
- **Custo evitado**: Energia gerada acumulada na sessão × tarifa
- **Gráfico**: 24 barras representando cada hora do dia com geração estimada

### Etapa 10: Temporizadores e Rotinas
**Objetivo**: Implementar funcionalidades de automação real.

**Temporizadores**:
- Interface para criar temporizadores personalizados
- Campos: nome, dispositivo, ação (ligar/desligar), tempo em segundos
- Contagem regressiva visual em tempo real
- Execução automática do comando quando o tempo zera
- Possibilidade de cancelar

**Rotinas pré-definidas**:
- "Chegando em Casa": Liga sala + cozinha + lâmpada (com delay entre cada)
- "Boa Noite": Desliga todos os dispositivos
- "Modo Economia": Desliga tudo exceto quarto (essencial)
- Todas reutilizáveis (podem ser executadas múltiplas vezes)

### Etapa 11: Sistema de Logs (Admin)
**Objetivo**: Monitorar todas as ações realizadas no sistema.

- Página administrativa separada (`admin.html`)
- Logs em tempo real via MQTT
- Informações registradas: ID do usuário, ação, hora
- Contadores: total de sessões, comandos, rotinas, usuários únicos
- Classificação por tipo: comando, sessão, rotina

---

## 5. CÓDIGO-FONTE DO ESP32

### 5.1 Estrutura do Código

```cpp
// Bibliotecas
#include <WiFi.h>          // Conexão WiFi nativa do ESP32
#include <PubSubClient.h>  // Cliente MQTT

// Configurações
- WiFi: SSID e senha do hotspot móvel
- MQTT: broker.emqx.io, porta 1883
- Tópicos: feira_solar_2026/cmd (receber), feira_solar_2026/status (enviar)

// Funções principais
- setup(): Configura pinos, conecta WiFi, conecta MQTT
- loop(): Mantém conexão MQTT, envia status periodicamente
- onMessage(): Recebe e processa comandos do site
- sendStatus(): Envia estado dos dispositivos em JSON
- connectMQTT(): Gerencia reconexão automática
```

### 5.2 Protocolo de Comunicação

**Comandos recebidos (tópico: feira_solar_2026/cmd)**:
| Comando | Ação |
|---|---|
| LED1ON | Liga LED 1 (GPIO 4) |
| LED1OFF | Desliga LED 1 |
| LED2ON | Liga LED 2 (GPIO 5) |
| LED2OFF | Desliga LED 2 |
| LED3ON | Liga LED 3 (GPIO 21) |
| LED3OFF | Desliga LED 3 |
| RELEON | Liga relé (GPIO 22) |
| RELEOFF | Desliga relé |
| ALLON | Liga todos |
| ALLOFF | Desliga todos |

**Status enviado (tópico: feira_solar_2026/status)**:
```json
{"led1":true,"led2":false,"led3":false,"rele":true}
```

---

## 6. INTERFACE WEB

### 6.1 Telas do Sistema

1. **Tela de Acesso Negado**: Exibida quando não há token válido na URL
2. **Tela Inicial**: Botão "Iniciar Demonstração" com informações do projeto
3. **Tela de Espera**: Mostrada quando outro usuário está usando
4. **Tela de Controle**: Interface principal com 3 abas
5. **Tela de Expiração**: Mostrada quando os 2 minutos acabam

### 6.2 Abas de Controle

**Aba Dispositivos**:
- 3 botões de LED (Sala, Cozinha, Quarto) com feedback visual
- Botão do Relé com indicador ON/OFF
- Botões "Ligar Tudo" e "Desligar Tudo"

**Aba Energia**:
- Geração Solar (W) - simulada pela hora do dia
- Consumo Atual (W) - calculado pelos dispositivos ligados
- Economia (R$/mês) - projeção mensal baseada na geração
- Custo Evitado (kWh) - energia gerada acumulada
- Gráfico de barras: geração por hora do dia

**Aba Rotinas**:
- Temporizadores personalizáveis (criar, visualizar, cancelar)
- Rotinas rápidas pré-definidas com execução real

### 6.3 Design e UX
- Dark theme profissional
- Design responsivo (mobile-first)
- Glassmorphism (transparência com blur)
- Gradientes e sombras para profundidade
- Animações suaves em interações
- Timer circular com animação de progresso
- Feedback tátil em dispositivos móveis

---

## 7. INFRAESTRUTURA DE REDE

### 7.1 Topologia na Feira
```
[Celular do visitante]  ←(4G/WiFi próprio)→  [Internet]
                                                  ↓
                                    [broker.emqx.io - MQTT]
                                                  ↓
[ESP32] ←(WiFi hotspot)→ [Celular do apresentador - Hotspot] ←(4G)→ [Internet]
```

### 7.2 Portas e Protocolos
- ESP32 → Broker: TCP porta 1883 (MQTT não criptografado)
- Site → Broker: WSS porta 8084 (WebSocket sobre TLS/HTTPS)
- Site hospedado: HTTPS porta 443 (GitHub Pages)

### 7.3 Consumo de Dados
- Cada comando MQTT: ~50 bytes
- Status enviado a cada 2 segundos: ~60 bytes
- Estimativa para 1 hora de feira: < 1 MB total
- Impacto no plano de dados: insignificante

---

## 8. TESTES REALIZADOS

### 8.1 Teste de Hardware
- Verificação individual de cada LED com código simples
- Teste do módulo relé com lâmpada
- Teste de alimentação via carregador USB (sem computador)

### 8.2 Teste de Comunicação
- Teste Serial Monitor: envio de comandos e recebimento de JSON
- Teste de conexão WiFi com hotspot móvel
- Teste de conexão MQTT com broker EMQX
- Teste de latência: comandos executados em < 500ms

### 8.3 Teste de Interface
- Teste em diferentes navegadores (Chrome, Firefox, Safari mobile)
- Teste de responsividade em diferentes tamanhos de tela
- Teste do sistema de sessão com múltiplos dispositivos
- Teste de expiração e cooldown

### 8.4 Problemas Encontrados e Soluções

| Problema | Causa | Solução |
|---|---|---|
| ESP32 não conecta no WiFi | Espaço invisível no nome da rede | Remoção do espaço extra |
| ESP32 não conecta no WiFi | Hotspot em 5GHz | Configurar hotspot para 2.4GHz |
| Web Serial não funciona | Página aberta via file:// | Migração para MQTT via internet |
| HiveMQ WebSocket falha | Broker instável | Migração para EMQX |
| Porta COM ocupada | Serial Monitor aberto | Fechar Serial Monitor antes de conectar |
| Upload falha | Porta em uso | Fechar programas que usam a COM |
| Biblioteca não encontrada | PubSubClient não instalada | Instalação via Library Manager |

---

## 9. CONSIDERAÇÕES DE SEGURANÇA

### 9.1 Vulnerabilidades Conhecidas
- Token visível no código-fonte (mitigado por ofuscação)
- Broker MQTT público (qualquer pessoa pode escutar os tópicos se souber o nome)
- Sem autenticação no broker MQTT

### 9.2 Mitigações Implementadas
- Tópicos MQTT com nome específico e difícil de adivinhar
- Token de acesso obrigatório na URL
- Rate limiting (30 comandos/sessão)
- Sessão temporária (2 minutos)
- Cooldown entre sessões (3 minutos)
- Bloqueio de ferramentas de inspeção no navegador

### 9.3 Melhorias Futuras (não implementadas)
- Broker MQTT privado com autenticação
- Token dinâmico gerado por servidor backend
- HTTPS em toda comunicação MQTT
- Banco de dados para persistência de logs

---

## 10. CUSTOS DO PROJETO

| Item | Custo Estimado |
|---|---|
| ESP32 Dev Module | R$ 35-50 |
| LEDs (3x) + Resistores | R$ 5 |
| Módulo Relé 1 canal | R$ 10-15 |
| Protoboard + Jumpers | R$ 20 |
| Lâmpada | R$ 8 |
| Carregador USB | R$ 15 |
| Hospedagem (GitHub Pages) | Gratuito |
| Broker MQTT (EMQX público) | Gratuito |
| Domínio | Não necessário |
| **TOTAL ESTIMADO** | **R$ 93-113** |

---

## 11. RESULTADOS ESPERADOS

- Demonstração funcional de automação residencial controlada por celular
- Controle em tempo real de dispositivos elétricos via internet
- Interface intuitiva acessível por QR Code
- Sistema de gestão de visitantes (sessão, fila, tempo)
- Visualização de dados de energia solar simulados
- Registro de logs para análise posterior

---

## 12. CONCLUSÃO

O projeto Solar Control demonstra a viabilidade de sistemas de automação residencial de baixo custo utilizando tecnologias IoT. Com um investimento inferior a R$120, é possível criar um sistema funcional que controla dispositivos remotamente de qualquer lugar do mundo.

A integração entre ESP32, MQTT e tecnologias web mostra como a Internet das Coisas pode tornar residências mais inteligentes e sustentáveis, contribuindo para a economia de energia e o uso de fontes renováveis como a solar.

---

## 13. REFERÊNCIAS TÉCNICAS

- Espressif ESP32 Datasheet
- Protocolo MQTT v3.1.1 (OASIS Standard)
- EMQX Public MQTT Broker Documentation
- PubSubClient Library (Nick O'Leary)
- GitHub Pages Documentation
- MDN Web Docs: WebSocket API
- Tarifa média de energia elétrica ANEEL 2026

---

## ANEXOS

### Anexo A: URL do Projeto
- Repositório: https://github.com/davidlindoetec/arduino-web-page-2
- Site público: https://davidlindoetec.github.io/arduino-web-page-2/?t=feira2026
- Painel admin: https://davidlindoetec.github.io/arduino-web-page-2/admin.html

### Anexo B: QR Code
Gerar QR Code com a URL: `https://davidlindoetec.github.io/arduino-web-page-2/?t=feira2026`

### Anexo C: Credenciais do Sistema
- WiFi (hotspot): Nome "David", Senha "12895050"
- MQTT Broker: broker.emqx.io (público, sem autenticação)
- Tópicos MQTT: feira_solar_2026/cmd, feira_solar_2026/status, feira_solar_2026/session, feira_solar_2026/log
- Token de acesso: feira2026 (parâmetro ?t= na URL)
