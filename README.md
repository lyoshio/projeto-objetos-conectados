# Sistema de Detecção de Incêndio com IoT

## 1. Descrição Geral do Projeto

Este projeto implementa um sistema de detecção de incêndio utilizando tecnologias IoT. Ele combina hardware e software para identificar chamas, emitir alertas locais (LED) e transmitir dados para monitoramento remoto via protocolo MQTT. O objetivo é fornecer uma solução acessível, eficiente e com resposta em tempo real.

### Funcionalidade:

1. O **sensor KY-026** detecta a presença de fogo.
2. O **NodeMCU ESP8266** processa o sinal e aciona o LED de alerta.
3. Os dados são transmitidos via **protocolo MQTT** para o servidor **io.adafruit.com**.
4. O monitoramento em tempo real ocorre por meio de um dashboard remoto.

---

## 2. Software Desenvolvido

O sistema foi programado em C++ para a plataforma Arduino, utilizando bibliotecas específicas para Wi-Fi e MQTT.

### Código Principal:

````cpp
#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define pinoSensorIncendio D2
#define pinoLedIncendio D6

const char* wifiSSID = "LUCAS 2.4G";
const char* wifiSenha = "07071994";

#define AIO_SERVIDOR "io.adafruit.com"
#define AIO_PORTA 1883
#define AIO_USUARIO "Lucas_N"
#define AIO_CHAVE "aio_mrud24f9wQbS2pEz43ZYqv7FVFYh"

WiFiClient wifiCliente;
Adafruit_MQTT_Client mqttCliente(&wifiCliente, AIO_SERVIDOR, AIO_PORTA, AIO_USUARIO, AIO_CHAVE);
Adafruit_MQTT_Publish topicoStatusIncendio = Adafruit_MQTT_Publish(&mqttCliente, AIO_USUARIO "/feeds/alert");

void setup() {
  Serial.begin(9600);
  conectarNoWiFi();
  conectarNoMQTT();
  pinMode(pinoSensorIncendio, INPUT);
  pinMode(pinoLedIncendio, OUTPUT);
}

void loop() {
  if (!mqttCliente.connected()) {
    conectarNoMQTT();
  }
  mqttCliente.ping();
  monitorarSensor();
}

```

## Documentação do Código:

* Wi-Fi: Configurado para conectar à rede local.
* MQTT: Envia mensagens para o servidor Adafruit IO.
* Monitoramento do Sensor: Lê o estado do sensor e publica alertas em intervalos definidos.
* LED: Indica visualmente a detecção de fogo.

## 3. Hardware Utilizado
Componentes:
 * NodeMCU ESP8266: Microcontrolador com conectividade Wi-Fi.
 * Protoboard: Usada para montagem temporária do circuito.
 * Sensor de Chama KY-026: Detecta chamas e fontes de calor.
 * LED Vermelho: Alerta visual para situações de incêndio.


# Especificações:

| Componente       | Descrição                                   |
|-------------------|--------------------------------------------|
| NodeMCU ESP8266   | Processamento e conectividade Wi-Fi        |
| Protoboard        | Conexões temporárias                       |
| Sensor KY-026     | Faixa de detecção de chamas de até 1 metro |
| LED Vermelho      | Consumo de 20mA, tensão de 2V              |


## 4. Documentação das Interfaces e Protocolos
 # Comunicação:
 * Wi-Fi (IEEE 802.11): Conexão do NodeMCU ao roteador local.
 * MQTT (Message Queuing Telemetry Transport):
    * Broker: Adafruit IO
     * Porta: 1883
     * Tópico: /feeds/alert
 # Estrutura de Mensagens MQTT:
     * Formato: Texto
     * Exemplos:
         * "Alerta de Incêndio!" (fogo detectado)
         * "Nenhum incêndio detectado." (status normal)
# Interface do Dashboard:
 * Adafruit IO:
     * Permite monitoramento em tempo real.
     * Gráficos e indicadores visuais para acompanhamento do sistema.


## 5. Comunicação e Controle via Internet
Este projeto utiliza o protocolo MQTT para comunicação, garantindo leveza e eficiência. A conectividade é mantida pela rede Wi-Fi do NodeMCU.

# Fluxo de Comunicação:
1. Sensor KY-026 detecta a chama.
2. Dados transmitidos para o servidor via MQTT.
3. Dashboard Adafruit IO exibe os dados para o usuário em tempo real.

## 6. Expansões Futuras
 * Alarmes Sonoros: Inclusão de alertas sonoros em caso de incêndio.
 * Sistema de Supressão de Fogo: Integração com atuadores para combate automático ao fogo.
 * Detecção Multisensorial: Uso de sensores adicionais para maior precisão.


 #Autores: Lucas Nicoli Batista || Lucas Yoshio Gonçalves
 #Última Atualização: Novembro/2024
````
