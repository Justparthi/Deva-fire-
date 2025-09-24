#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ---------------- OLED Setup ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------------- DS18B20 Setup ----------------
#define ONE_WIRE_BUS 8   // Sensor connected to digital pin 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ---------------- Buzzer Setup ----------------
#define BUZZER_PIN 9     // Buzzer connected to digital pin 9

// ---------------- Temperature Limit ----------------
#define TEMP_LIMIT 50.0  // Fire alert threshold in °C

void setup() {
  Serial.begin(9600);
  delay(200);
  Serial.println("DS18B20 Temperature Display on OLED");

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();
  delay(1000);

  // Initialize DS18B20
  sensors.begin();
  if (sensors.getDeviceCount() == 0) {
    Serial.println("❌ No DS18B20 sensors found!");
  } else {
    Serial.print("✅ DS18B20 sensors found: ");
    Serial.println(sensors.getDeviceCount());
  }

  // Setup buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // keep buzzer off initially
}

void loop() {
  sensors.requestTemperatures();               // Ask DS18B20 to measure
  float tempC = sensors.getTempCByIndex(0);    // Read temperature from first sensor
  float tempF = sensors.toFahrenheit(tempC);

  // Debug to Serial
  Serial.print("Raw TempC: ");
  Serial.println(tempC);

  // Check for sensor error
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("❌ ERROR: Device disconnected or read failed");
    display.clearDisplay();
    display.setCursor(0, 20);
    display.setTextSize(1);
    display.println("Sensor Error!");
    display.display();

    // Beep to indicate error
    tone(BUZZER_PIN, 1000, 300); // short error beep
  } else {
    // Print to Serial
    Serial.print("Temperature: ");
    Serial.print(tempC, 2);
    Serial.print(" °C | ");
    Serial.print(tempF, 2);
    Serial.println(" °F");

    // Display on OLED
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print(tempC, 2);
    display.println(" C");

    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print(tempF, 2);
    display.println(" F");

    // 🔥 Fire alert display
    if (tempC > TEMP_LIMIT) {
      display.setTextSize(2);
      display.setCursor(0, 35);
      display.println("Fire ALERT");
      tone(BUZZER_PIN, 2000, 500); // Buzzer ON
    } else {
      noTone(BUZZER_PIN); // Buzzer OFF
    }

    display.display();
  }

  delay(1000); // Update every second
}
