#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22 //DHT11, DHT21, DHT22

DHT dht(DHTPIN, DHTTYPE);
int ldr1_value = 0;

void setup()
{
  Serial.begin(9600);
  dht.begin();
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
  ldr1_value = analogRead(0);
  Serial.print("LDR Value:");
  Serial.println(ldr1_value);
}

void loop()
{
  delay(100);
  readDHT();
  readLDR();
}
