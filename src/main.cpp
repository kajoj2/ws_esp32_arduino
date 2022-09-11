#include <Arduino.h>
#include <WiFi.h>
#define I2C_SDA 21
#define I2C_SCL 22
#include "sensors.h"
#include "mqtt_client.h"
#include <ArduinoJson.h>

const char *device = "DEVELOPMENT_DEVICE_001";
const char *ntpServer = "pool.ntp.org";

esp_mqtt_client_handle_t client;

void setup()
{

  WiFi.mode(WIFI_STA);
  WiFi.setHostname("DEV BOARD");
  WiFi.begin("iot", "Tajne1234%^");
  // Initialise I2C communication as MASTER
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin(I2C_SDA, I2C_SCL);
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  BH1730_init();
  TMP112_init_conitonus();
  DSP310_init_continous();
  SHTC3_init();

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(200);
  }

  Serial.println(WiFi.localIP());
  configTime(0, 0, ntpServer);

  esp_mqtt_client_config_t mqtt_cfg = {
      .uri = "mqtt://10.128.50",
  };

  client = esp_mqtt_client_init(&mqtt_cfg);
  esp_mqtt_client_start(client);
}

unsigned long getTime()
{
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    // Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

StaticJsonDocument<100> doc;
String json;

void loop()
{
  // Output data to serial monitor
  Serial.print("Temperature in Celsius  TMP112 ");
  float TMP112_temperature = TMP112_get_temperature_c();
  Serial.print(TMP112_temperature);
  Serial.println(" C");

  doc["device"] = device;
  doc["time"] = getTime();
  doc["sensor"] = "TMP112";
  doc["temperature"] = TMP112_temperature;

  json = "";
  serializeJson(doc, json);
  esp_mqtt_client_publish(client, String("/v1/data/" + String(device) + "/temperature/").c_str(), json.c_str(), 0, 2, 0);

  Serial.print("");
  float lux = BH1730_oneTimeMessurments();
  Serial.print(lux);
  Serial.println(" lux");

  doc["device"] = device;
  doc["time"] = getTime();
  doc["sensor"] = "BH1730";
  doc["illuminance"] = lux;

  json = "";
  serializeJson(doc, json);
  esp_mqtt_client_publish(client, String("/v1/data/" + String(device) + "/illuminance/").c_str(), json.c_str(), 0, 2, 0);

  uint8_t pressureCount = 20;
  float pressure[pressureCount];
  uint8_t temperatureCount = 20;
  float temperature[temperatureCount];

  // This function writes the results of continuous measurements to the arrays given as parameters
  // The parameters temperatureCount and pressureCount should hold the sizes of the arrays temperature and pressure when the function is called
  // After the end of the function, temperatureCount and pressureCount hold the numbers of values written to the arrays
  // Note: The Dps310 cannot save more than 32 results. When its result buffer is full, it won't save any new measurement results
  int16_t ret = DSP310_get_results(temperature, temperatureCount, pressure, pressureCount);

  if (ret != 0)
  {
    Serial.println();
    Serial.println();
    Serial.print("FAIL! ret = ");
    Serial.println(ret);
  }
  else
  {
    Serial.println();
    Serial.println();
    Serial.print(temperatureCount);
    Serial.println(" temperature values found: ");
    for (int16_t i = 0; i < temperatureCount; i++)
    {
      Serial.print(temperature[i]);
      Serial.println(" degrees of Celsius");
    }

    Serial.println();
    Serial.print(pressureCount);
    Serial.println(" pressure values found: ");
    for (int16_t i = 0; i < pressureCount; i++)
    {
      Serial.print(pressure[i]);
      Serial.println(" Pascal");
    }
  }

  Serial.print("Temperature in Celsius:  SHTC3  ");
  float SHTC3_temperature = SHTC3_measure_temperature_c();
  Serial.print(SHTC3_temperature);
  Serial.println(" C");

  doc["device"] = device;
  doc["time"] = getTime();
  doc["sensor"] = "SHTC3";
  doc["temperature"] = SHTC3_temperature;

  json = "";
  serializeJson(doc, json);
  esp_mqtt_client_publish(client, String("/v1/data/" + String(device) + "/temperature/").c_str(), json.c_str(), 0, 2, 0);

  Serial.print("huminidity SHTC3  ");
  float SHTC3_huminidity = SHTC3_measure_huminidity();
  Serial.print(SHTC3_huminidity);
  Serial.println(" %");

  doc["device"] = device;
  doc["time"] = getTime();
  doc["sensor"] = "SHTC3";
  doc["huminidity"] = SHTC3_huminidity;

  json = "";
  serializeJson(doc, json);
  esp_mqtt_client_publish(client, String("/v1/data/" + String(device) + "/huminidity/").c_str(), json.c_str(), 0, 2, 0);

  //delay(500);
}