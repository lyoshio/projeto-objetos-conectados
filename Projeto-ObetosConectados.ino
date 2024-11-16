#include <ESP8266WiFi.h> 

#include <Adafruit_MQTT.h> 

#include <Adafruit_MQTT_Client.h> 

  

#define pinoSensorIncendio D2 

#define pinoLedIncendio D6  // Define o pino do LED 

  

const char* wifiSSID = "LUCAS 2.4G"; 

const char* wifiSenha = "07071994"; 

  

#define AIO_SERVIDOR      "io.adafruit.com" 

#define AIO_PORTA         1883                   

#define AIO_USUARIO       "Lucas_N" 

#define AIO_CHAVE         "aio_mrud24f9wQbS2pEz43ZYqv7FVFYh" 

  

WiFiClient wifiCliente; 

Adafruit_MQTT_Client mqttCliente(&wifiCliente, AIO_SERVIDOR, AIO_PORTA, AIO_USUARIO, AIO_CHAVE); 

Adafruit_MQTT_Publish topicoStatusIncendio = Adafruit_MQTT_Publish(&mqttCliente, AIO_USUARIO "/feeds/alert"); 

  

unsigned long ultimoTempoPublicacao = 0; 

unsigned long ultimoTempoAlarme = 0; 

  

const unsigned long intervaloPublicacaoPadrao = 5000; 

const unsigned long intervaloPublicacaoAlerta = 2000; 

  

void conectarNoMQTT() { 

  int8_t resultado; 

  

  while ((resultado = mqttCliente.connect()) != 0) { 

    mqttCliente.disconnect(); 

    delay(5000); 

  } 

} 

  

void monitorarSensor() { 

  int leituraSensorIncendio = digitalRead(pinoSensorIncendio); 

  

  if (leituraSensorIncendio == LOW) {  // Incêndio detectado 

    if (millis() - ultimoTempoAlarme >= intervaloPublicacaoAlerta) { 

      Serial.println("Alerta de Incêndio!"); 

      topicoStatusIncendio.publish("Alerta de Incêndio!"); 

      ultimoTempoAlarme = millis(); 

      digitalWrite(pinoLedIncendio, HIGH);  // Acende o LED 

    } 

  } else {  // Nenhum incêndio detectado 

    if (millis() - ultimoTempoPublicacao >= intervaloPublicacaoPadrao) { 

      Serial.println("Nenhum incêndio detectado."); 

      topicoStatusIncendio.publish("Nenhum incêndio detectado."); 

      ultimoTempoPublicacao = millis(); 

      ultimoTempoAlarme = 0; 

      digitalWrite(pinoLedIncendio, LOW);  // Apaga o LED 

    } 

  } 

} 

  

void conectarNoWiFi() { 

  Serial.print("Conectando ao Wi-Fi"); 

  WiFi.begin(wifiSSID, wifiSenha); 

  while (WiFi.status() != WL_CONNECTED) { 

    delay(500); 

    Serial.print("."); 

  } 

  Serial.println(); 

  Serial.println("Conexão Wi-Fi bem-sucedida!"); 

  Serial.print("Endereço de IP: "); 

  Serial.println(WiFi.localIP()); 

} 

  

void setup() { 

  Serial.begin(9600); 

  

  conectarNoWiFi(); 

  conectarNoMQTT(); 

  

  pinMode(pinoSensorIncendio, INPUT); 

  pinMode(pinoLedIncendio, OUTPUT);  // Configura o pino do LED como saída 

  digitalWrite(pinoLedIncendio, LOW);  // Inicialmente, apaga o LED 

  

  monitorarSensor(); 

} 

  

void loop() { 

  if (!mqttCliente.connected()) { 

    conectarNoMQTT(); 

  } 

  

  mqttCliente.ping(); 

  monitorarSensor(); 

} 
