#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <InfluxDb.h>

#define LED D0 // set onboard LED for activity-blink
#define INFLUXDB_HOST "2a04:0000:0000:100:33b3:3340:3333:338f" // insert IPv6 (or IPv4 (boo!))
#define INFLUXDB_USER "admin" // insert InfluxDB username
#define INFLUXDB_PASS "lolpassword" // insert InfluxDB password

Adafruit_BME280 bme;

Influxdb influx(INFLUXDB_HOST);

const char* ssid = "lolWLAN"; // insert WLAN SSID
const char* password = "123456789101112"; // insert WLAN password
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  Serial.print("Connect to Wifi ");
 
  WiFi.begin(ssid, password);
  WiFi.hostname("esptemphalle");
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  pinMode(LED, OUTPUT);
  bool status;
  status = bme.begin(0x76);
  influx.setDbAuth("bme280", INFLUXDB_USER, INFLUXDB_PASS); // insert database or create a database named bme280
  Serial.println("Parameter gesetzt.");
}
 
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Your WiFi-Stuff here :-)
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
    Serial.println(bme.readTemperature());
    InfluxData row("temperature");
    row.addTag("device", "bme280-1");
    row.addValue("value", bme.readTemperature());
    influx.write(row);
    delay(5000);
  }
}


 
