#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define SENSOR_PIN 5                   // GPIO2 (D4 on NodeMCU)
const char* ssid = "E5576_47AB";       // Replace with your WiFi SSID
const char* password = "iN5iRhdt67F";  // Replace with your WiFi Password
// const char* ssid = "EdNet";       // Replace with your WiFi SSID
// const char* password = "Huawei@123";  // Replace with your WiFi Password
// const char* ssid = "RCA-OFFICE";       // Replace with your WiFi SSID
// const char* password = "RCA@2024";  // Replace with your WiFi Password


// const char* ssid ="GROUND";
// const char* password ="RCA@2024";

const char* serverUrl = "http:// 192.168.8.85:6001/flow-data";
volatile int pulseCount = 0;
float flowRate = 0.0;
unsigned long oldTime = 0;

// Interrupt service routine for flow sensor       
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);

  //Starting Wi-fi Connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  pinMode(SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), pulseCounter, FALLING);
}

void loop() {
  if ((millis() - oldTime) > 1000) {  // Calculate flow rate every 9 second
    detachInterrupt(digitalPinToInterrupt(SENSOR_PIN));

    // Convert pulses to liters per minute (L/min)
    flowRate = (pulseCount / 7.5);  // YF-S201: 7.5 pulses per liter

    Serial.print("Flow Rate: ");
    Serial.print(flowRate);
    Serial.println(" L/min");

    sendFlowRateToServer(flowRate);

    pulseCount = 0;  // Reset pulse count
    oldTime = millis();

    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), pulseCounter, FALLING);
  }
}

void sendFlowRateToServer(float flow) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<100> JsonDoc;
    JsonDoc["flow-rate"] = flow;

    String jsonPayload;
    serializeJson(JsonDoc, jsonPayload);     

    Serial.print("Sending payload: ");
    Serial.println(jsonPayload);

    int httpResponseCode = http.POST(jsonPayload);
    String response = http.getString();

    Serial.print("Response: ");
    Serial.println(response);

    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully: " + jsonPayload);
    } else {
      Serial.println("Failed to send data");
      Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected!");
  }
}

