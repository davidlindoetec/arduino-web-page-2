/*
 * ESP32 - Feira de Ciências com MQTT
 * Controle pela internet via site público
 * 
 * PINOS:
 *   GPIO 4  -> LED 1
 *   GPIO 5  -> LED 2
 *   GPIO 21 -> LED 3
 *   GPIO 22 -> RELÉ (lâmpada real) - opcional
 * 
 * BIBLIOTECAS NECESSÁRIAS (instalar pelo Arduino IDE):
 *   1. PubSubClient (por Nick O'Leary) - Sketch > Include Library > Manage Libraries > pesquise "PubSubClient"
 *   2. WiFi (já vem com ESP32)
 * 
 * COMO FUNCIONA:
 *   1. ESP32 conecta no WiFi da feira/escola
 *   2. ESP32 conecta no servidor MQTT gratuito (broker.hivemq.com)
 *   3. Pessoa escaneia QR code -> abre site
 *   4. Site envia comandos via MQTT -> ESP32 recebe e liga/desliga
 */

#include <WiFi.h>
#include <PubSubClient.h>

// ===== WIFI DA FEIRA/ESCOLA =====
const char* ssid = "David";               // Hotspot do celular
const char* password = "12895050";         // Senha do hotspot

// ===== MQTT (não precisa mudar) =====
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
// Tópicos únicos para seu projeto (não mude a menos que queira outro ID)
const char* topic_cmd = "feira_solar_2026/cmd";
const char* topic_status = "feira_solar_2026/status";

// ===== PINOS =====
const int LED1 = 4;
const int LED2 = 5;
const int LED3 = 21;
const int RELE = 22;

// ===== ESTADO =====
bool led1State = false;
bool led2State = false;
bool led3State = false;
bool releState = false;

WiFiClient espClient;
PubSubClient mqtt(espClient);

unsigned long lastStatusSend = 0;

void setup() {
  Serial.begin(115200);
  
  // Pinos
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(RELE, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(RELE, LOW);

  // Conectar WiFi
  Serial.print("Conectando WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi OK! IP: ");
  Serial.println(WiFi.localIP());

  // Configurar MQTT
  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(onMessage);
  
  connectMQTT();
  Serial.println("Pronto! Aguardando comandos do site...");
}

void loop() {
  if (!mqtt.connected()) {
    connectMQTT();
  }
  mqtt.loop();
  
  // Enviar status a cada 2 segundos
  if (millis() - lastStatusSend > 2000) {
    lastStatusSend = millis();
    sendStatus();
  }
}

// ===== CONECTAR MQTT =====
void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Conectando MQTT...");
    String clientId = "esp32_solar_" + String(random(0xffff), HEX);
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("OK!");
      mqtt.subscribe(topic_cmd);
      sendStatus();
    } else {
      Serial.print("Falhou (");
      Serial.print(mqtt.state());
      Serial.println(") tentando de novo em 3s...");
      delay(3000);
    }
  }
}

// ===== RECEBER COMANDOS =====
void onMessage(char* topic, byte* payload, unsigned int length) {
  String cmd = "";
  for (unsigned int i = 0; i < length; i++) {
    cmd += (char)payload[i];
  }
  cmd.toUpperCase();
  Serial.print("Comando recebido: ");
  Serial.println(cmd);

  if (cmd == "LED1ON") { led1State = true; digitalWrite(LED1, HIGH); }
  else if (cmd == "LED1OFF") { led1State = false; digitalWrite(LED1, LOW); }
  else if (cmd == "LED2ON") { led2State = true; digitalWrite(LED2, HIGH); }
  else if (cmd == "LED2OFF") { led2State = false; digitalWrite(LED2, LOW); }
  else if (cmd == "LED3ON") { led3State = true; digitalWrite(LED3, HIGH); }
  else if (cmd == "LED3OFF") { led3State = false; digitalWrite(LED3, LOW); }
  else if (cmd == "RELEON") { releState = true; digitalWrite(RELE, HIGH); }
  else if (cmd == "RELEOFF") { releState = false; digitalWrite(RELE, LOW); }
  else if (cmd == "ALLON") {
    led1State = true; led2State = true; led3State = true; releState = true;
    digitalWrite(LED1, HIGH); digitalWrite(LED2, HIGH); digitalWrite(LED3, HIGH); digitalWrite(RELE, HIGH);
  }
  else if (cmd == "ALLOFF") {
    led1State = false; led2State = false; led3State = false; releState = false;
    digitalWrite(LED1, LOW); digitalWrite(LED2, LOW); digitalWrite(LED3, LOW); digitalWrite(RELE, LOW);
  }

  sendStatus();
}

// ===== ENVIAR STATUS =====
void sendStatus() {
  String json = "{\"led1\":";
  json += led1State ? "true" : "false";
  json += ",\"led2\":";
  json += led2State ? "true" : "false";
  json += ",\"led3\":";
  json += led3State ? "true" : "false";
  json += ",\"rele\":";
  json += releState ? "true" : "false";
  json += "}";
  
  mqtt.publish(topic_status, json.c_str());
}
