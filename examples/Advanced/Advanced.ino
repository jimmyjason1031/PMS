
#include "PMS.h"
#include "ArduinoJson.h"

PMS pms(Serial);
PMS::DATA data;

void setup()
{
  // Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
  pms.passiveMode();    // Switch to passive mode
}

void loop()
{
  Serial.println("Wake up, wait 30 seconds for stable readings...");
  pms.wakeUp();
  delay(30000);

  Serial.println("Send request read...");
  pms.requestRead();

  Serial.println("Wait max. 10 seconds for read...");
  if (pms.readUntil(data, 10000))
  {
    Serial.println("Data:");
    
    StaticJsonDocument<200> doc;
    doc["action"] = "status";

    JsonObject component = doc.createNestedObject("component");
    component["temperature"] = data.temperature;
    component["humidity"] = data.humidity;
    component["particle0.3um"] = data.PM_TOTALPARTICLES_0_3;
    component["particle0.5um"] = data.PM_TOTALPARTICLES_0_5;
    component["particle1.0um"] = data.PM_TOTALPARTICLES_1_0;
    component["particle2.5um"] = data.PM_TOTALPARTICLES_2_5;
    component["concentration_PM1.0"] = data.PM_SP_UG_1_0;
    component["concentration_PM2.5"] = data.PM_SP_UG_2_5;
    component["concentration_PM10"] = data.PM_SP_UG_10_0;
    component["concentration_aPM1.0"] = data.PM_AE_UG_1_0;
    component["concentration_aPM2.5"] = data.PM_AE_UG_2_5;
    component["concentration_aPM10"] = data.PM_AE_UG_10_0;

    serializeJson(doc, Serial);
    Serial.println();
  }
  else
  {
    Serial.println("No data.");
  }

  Serial.println("Going to sleep for 10 seconds.");
  pms.sleep();
  delay(10000);
}
