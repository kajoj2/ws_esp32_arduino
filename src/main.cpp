#include <Arduino.h>
#include <WiFi.h>
#define I2C_SDA 21
#define I2C_SCL 22
#include "sensors.h"
#include "mqtt_client.h"
#include <ArduinoJson.h>

#define MESSURE_GLOBAL_DEALY_MS 1000

const char *device = "DEVELOPMENT_DEVICE_001";
const char *ntpServer = "pool.ntp.org";

esp_mqtt_client_handle_t client;

void vTMP112_proc(void *parameter);
void vBH1730_proc(void *parameter);
void vDSP310_proc(void *parameter);
void vSHTC3_proc(void *parameter);

void setup()
{

  WiFi.mode(WIFI_STA);
  WiFi.setHostname("DEV BOARD");
  WiFi.begin("iot", "Tajne1234%^");
  // Initialise I2C communication as MASTER
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin(I2C_SDA, I2C_SCL);
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(115200);

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
      .uri = "mqtt://s0.kajoj.com",
  };

  client = esp_mqtt_client_init(&mqtt_cfg);
  esp_mqtt_client_start(client);

  xTaskCreate(
    vTMP112_proc,    // Function that should be called
    "TMP112 proc",   // Name of the task (for debugging)
    15000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );
    xTaskCreate(
    vBH1730_proc,    // Function that should be called
    "BH1730 proc",   // Name of the task (for debugging)
    15000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );
    xTaskCreate(
    vDSP310_proc,    // Function that should be called
    "DSP310 proc",   // Name of the task (for debugging)
    15000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );
    xTaskCreate(
    vSHTC3_proc,    // Function that should be called
    "SHTC3 proc",   // Name of the task (for debugging)
    15000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );


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


void loop()
{


 delay(500);
}

void vTMP112_proc(void *parameter)
{
  StaticJsonDocument<100> doc;
  String json;
  for (;;)
  {
    Serial.print("Temperature in Celsius  TMP112 ");
    float TMP112_temperature = TMP112_get_temperature_c();
    Serial.print(TMP112_temperature);
    Serial.println(" C");
    if (TMP112_temperature > 100 || TMP112_temperature < -10 ){
       Serial.println("TMP112_temperature limit bard reading");
       continue;
    }

    doc["device"] = device;
    doc["time"] = getTime();
    doc["sensor"] = "TMP112";
    doc["temperature"] = TMP112_temperature;

    json = "";
    serializeJson(doc, json);
    esp_mqtt_client_publish(client, String("/v1/data/" + String(device) + "/temperature/").c_str(), json.c_str(), 0, 2, 0);
    delay(MESSURE_GLOBAL_DEALY_MS);
  }
}
void vBH1730_proc(void *parameter)
{
  StaticJsonDocument<100> doc;
  String json;
  for (;;)
  {
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
    delay(MESSURE_GLOBAL_DEALY_MS);
  }
}
void vDSP310_proc(void *parameter)
{
  StaticJsonDocument<100> doc;
  String json;
  for (;;)
  {
    float pressure = DSP310_measure_pressure();

      Serial.println("pressure values: ");
      Serial.println(pressure);
 

      if (pressure > 0){
          doc["device"] = device;
          doc["time"] = getTime();
          doc["sensor"] = "DSP310";
          doc["pressure"] = pressure;

          json = "";
          serializeJson(doc, json);
          esp_mqtt_client_publish(client, String("/v1/data/" + String(device) + "/pressure/").c_str(), json.c_str(), 0, 2, 0);

    }

    delay(MESSURE_GLOBAL_DEALY_MS);
  }
}
void vSHTC3_proc(void *parameter)
{
  StaticJsonDocument<100> doc;
  String json;
  for (;;)
  {
    Serial.print("Temperature in Celsius:  SHTC3  ");
    float SHTC3_temperature = SHTC3_measure_temperature_c();
    Serial.print(SHTC3_temperature);
    Serial.println(" C");

    if (SHTC3_temperature > 100 || SHTC3_temperature < -10 ){
       Serial.println("SHTC3_temperature limit bard reading");
       continue;
    }

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

    if (SHTC3_huminidity > 99 || SHTC3_huminidity < 1 ){
       Serial.println("SHTC3_huminidity limit bard reading");
       continue;
    }

    doc["device"] = device;
    doc["time"] = getTime();
    doc["sensor"] = "SHTC3";
    doc["huminidity"] = SHTC3_huminidity;

    json = "";
    serializeJson(doc, json);
    esp_mqtt_client_publish(client, String("/v1/data/" + String(device) + "/huminidity/").c_str(), json.c_str(), 0, 2, 0);
    delay(MESSURE_GLOBAL_DEALY_MS);
  }
}
