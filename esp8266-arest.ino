#include <ESP8266WiFi.h>
#include <aREST.h>
#include <DHT.h>
#include "env.h"


int relayPin = 2;
int sensorPin = 0;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

String deviceId = "1";
String deviceName = "Hall";
String hostName = "hall";

#define DHTPIN sensorPin
#define DHTTYPE DHT11

aREST rest = aREST();

DHT dht(DHTPIN, DHTTYPE);
WiFiServer server(80);

float temperature, humidity, heatindex;
int relayState;

unsigned long sensorPreviousMillis = 0;
const long sensorInterval = 2000; 

int relayControl(String command);
int espControl(String command);


void setup(void)
{
  Serial.begin(115200);
  dht.begin();

  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, LOW);
  relayState = LOW;

  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);
  rest.variable("heatindex",&heatindex);
  rest.variable("relay",&relayState);
  rest.function("relay-switch",relayControl);
  rest.function("esp",espControl);

  rest.set_id(deviceId);
  rest.set_name(deviceName);

  Serial.println();

  WiFi.hostname(hostName);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.begin();
  Serial.println();
  Serial.print(WiFi.localIP());
  Serial.println();
}


void loop()
{
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while(!client.available()){
    delay(1);
  }

  processSensorData();
  rest.handle(client);
  delay(1);
}


int relayControl(String command)
{
  int state = digitalRead(relayPin);
  state = state == LOW ? HIGH : LOW;
  digitalWrite(relayPin, state);
  relayState = state;
  return relayState;
}


int espControl(String command)
{
  if(command == "reset") {
    Serial.println("Reset ESP.");
    ESP.reset();
  }
  return 0;
}


void processSensorData() {
  unsigned long sensorCurrentMillis = millis();
 
  if(sensorCurrentMillis - sensorPreviousMillis >= sensorInterval) {
    sensorPreviousMillis = sensorCurrentMillis;   
 
    float ht  = dht.readHumidity();
    float tt  = dht.readTemperature();
    float hit = dht.computeHeatIndex(tt, ht, false);

    if (isnan(ht) || isnan(tt) || isnan(hit)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      humidity = ht;
      temperature = tt;
      heatindex = hit;
    }
  }
}
