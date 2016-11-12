#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#define DHTPIN 2
#define DHTTYPE DHT22 //DHT11, DHT21, DHT22

byte mac[] {  0x09, 0x52, 0x2B, 0x38, 0xB7, 0xD1 };
const IPAddress ip(192, 168, 178, 205);
const IPAddress subnet(255, 255, 255, 0);
const IPAddress mqtt_server(192, 168, 178, 35);
const IPAddress dns_server(192, 168, 178, 1);
const IPAddress gate(192, 168, 178, 1);
const int mqtt_port = 1883;

EthernetClient ethClient;
PubSubClient client(ethClient);

DHT dht(DHTPIN, DHTTYPE);

int ldr1_value = 0;

//MQTT functions
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
    }
  }
}


void setup()
{
  Serial.begin(9600);
  dht.begin(); //init DHT sensor lib


  Ethernet.begin(mac, ip, dns_server, gate, subnet); //init ethernet
  // Allow the hardware to sort itself out
  delay(1500);
  client.setServer(mqtt_server, mqtt_port); //init MQTT lib
  Serial.println("Setup -> done");
}

void readDHT()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(t) || isnan(h))
  {
    Serial.println("DHT22 konnte nicht ausgelesen werden");
  }
  else
  {
    Serial.print("Luftfeuchte: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperatur: ");
    Serial.print(t);
    Serial.println(" C");
  }
}

void readLDR() {
  int current_ldr1_value = analogRead(0);
  if(current_ldr1_value != ldr1_value) {
    ldr1_value = current_ldr1_value;
    if(client.connected()) {
      // Once connected, publish an announcement...
      char buffer[10];
      snprintf(buffer, 10, "%d", ldr1_value);
      client.publish("ldrValue", buffer);
    }
    Serial.print("LDR Value:");
    Serial.println(ldr1_value);
  }

}

void loop()
{
  if (!client.connected()) {
  reconnect();
  }
  client.loop();
  delay(500);
  readDHT();
  readLDR();
}
