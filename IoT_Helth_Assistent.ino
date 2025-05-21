/* Comment this out to disable prints and save space */
#include "secrets.h"
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;

const byte RATE_SIZE = 4;
byte rates[4];
byte rateSpot = 0;
long lastBeat = 0;

bool isFingerPlaced = false;

int bufferLength = 100;
uint32_t irBuffer[100]; // Changed from uint16_t to uint32_t
uint32_t redBuffer[100]; // Changed from uint16_t to uint32_t



#define BLYNK_PRINT Serial



#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>



// Pin configuration
#define ONE_WIRE_BUS 19  // DS18B20
#define DHT_PIN 18       // DHT11

// Initialize sensors
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DHT dht(DHT_PIN, DHT11);

// Virtual pins
#define VIRTUAL_PIN_DS18B20_TEMP V1
#define VIRTUAL_PIN_DHT11_TEMP   V2
#define VIRTUAL_PIN_DHT11_HUMID  V3

#define VIRTUAL_PIN_HEART_RATE  V4
#define VIRTUAL_PIN_OXGEN_LEVEL  V5

int heart_rate;
int oxygen_level;


// Timer object for sensor readings
BlynkTimer timer;

void sendSensorData() {
  // Read DS18B20 temperature
  sensors.requestTemperatures();
  float tempDS18B20 = sensors.getTempCByIndex(0);
  
  // Read DHT11 data
  float humidity = dht.readHumidity();
  float tempDHT11 = dht.readTemperature();

  // Check if any reads failed
  if (isnan(tempDS18B20) || isnan(tempDHT11) || isnan(humidity)) {
    Serial.println("Failed to read from sensors!");
    return;
  }
  float tempFahrenheit = tempDS18B20 * 9.0 / 5.0 + 32;
  // Send data to Blynk
  Blynk.virtualWrite(VIRTUAL_PIN_DS18B20_TEMP, tempFahrenheit);
  Blynk.virtualWrite(VIRTUAL_PIN_DHT11_TEMP, tempDHT11);
  Blynk.virtualWrite(VIRTUAL_PIN_DHT11_HUMID, humidity);
  Blynk.virtualWrite(VIRTUAL_PIN_HEART_RATE, heart_rate);
  Blynk.virtualWrite(VIRTUAL_PIN_OXGEN_LEVEL, oxygen_level);

  // Print to Serial Monitor
  Serial.print("DS18B20 Temp: "); Serial.print(tempFahrenheit); Serial.println(" °F");
  Serial.print("DHT11 Temp: "); Serial.print(tempDHT11); Serial.println(" °C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
}

void setup() {
  // Debug console
  Serial.begin(115200);

  Wire.begin(21, 22); // Default ESP32 I2C: SDA = GPIO 21, SCL = GPIO 22

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }
  Serial.println(F("MAX30105 initialized successfully!"));

  particleSensor.setup(0x32, 4, 2, 400, 215, 4096);
  Serial.println(F("Place your finger on the sensor."));


  // Initialize sensors
  sensors.begin();
  dht.begin();

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Setup a function to be called every 2 seconds
  timer.setInterval(2000L, sendSensorData);
}

void loop() {




  float beatsPerMinute;
  int beatAvg;

  long irValue = particleSensor.getIR();
  if (irValue < 50000) {
    Serial.println(F("No finger detected. Please place finger on sensor."));
    isFingerPlaced = false;
  } else {
    isFingerPlaced = true;
    Serial.print(F("IR: "));
    Serial.print(irValue);
    Serial.println(F(" (Finger detected)"));
  }

  if (isFingerPlaced) {
    for (int i = 0; i < bufferLength; i++) {
      while (!particleSensor.check()) {
        delay(1);
      }
      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
    }

    Serial.print(F("IR[0]: "));
    Serial.print(irBuffer[0]);
    Serial.print(F(", Red[0]: "));
    Serial.println(redBuffer[0]);

    int32_t spo2;
    int8_t validSPO2;
    int32_t heartRate;
    int8_t validHeartRate;

    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    if (validHeartRate) {
      Serial.print(F("Heart Rate: "));
      Serial.print(heartRate);
      heart_rate = heartRate;
      Serial.println(F(" bpm"));
    } else {
      Serial.println(F("Heart Rate: Not valid"));
    }

    if (validSPO2) {
      Serial.print(F("SpO2: "));
      Serial.print(spo2);
      oxygen_level = spo2;
      Serial.println(F(" %"));
    } else {
      Serial.println(F("SpO2: Not valid"));
    }

  delay(1000);


  Blynk.run();
  timer.run();
}
}