#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Yellow wire on flow_meter will be connected on D2
#define FLOW_SENSOR_PIN 4  // GPIO4 (D2 on NodeMCU)

// WiFi Credentials
const char* ssid = "EdNet";
const char* password = "Huawei@123";

// MQTT Broker details
const char* mqtt_server = "157.173.101.159";
const int mqtt_port = 1883;

//Creating instance of ESPClient and that of MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

volatile int pulseCount = 0;
float flowRateL_min = 0.0;
unsigned long lastSendTime = 0;

// Function to count number of pulses
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

// Function to connect to WiFi
void setup_wifi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Function to reconnect to MQTT broker
void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection... ");

    if (mqttClient.connect("ESP8266Client")) {
      Serial.println("Connected To MQTT !");
      mqttClient.subscribe("/leak_alert/flow_rate");  //subscribing to the topic for scalability
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" Trying again in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  mqttClient.setServer(mqtt_server, mqtt_port);

  // using internal pullup resistor
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  //Time interval to send data to Broker
  if (millis() - lastSendTime > 1000) {
    lastSendTime = millis();

    // Convert pulse count to flow rate (L/min)
    flowRateL_min = (pulseCount / 7.5);  // YF-S201 flow_meter sensor generate 7.5 pulses per liter
    pulseCount = 0;


    Serial.print("Flow Rate: ");
    Serial.print(flowRateL_min);
    Serial.println(" L/min");

    // Publish flow rate data to MQTT topic
    String flowData = String(flowRateL_min);
    mqttClient.publish("/leak_alert/flow_rate", flowData.c_str());
  }
}
