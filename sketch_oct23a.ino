#include <ESP8266HTTPClient.h>
#include "DHT.h"
#include <FirebaseArduino.h> // Install the FirebaseESP8266 library via Arduino Library Manager


#define DHTPIN D2        // ESP8266 D2 pin map as 4 in Arduino IDE
#define DHTTYPE DHT11    // DHT sensor type
#define MQ7PIN D0        // Digital pin for MQ-7 sensor
#define FLAME_PIN D1     // Digital pin for IR flame sensor
#define MOISTURE_PIN A0  // Analog pin for soil moisture sensor

// Your Firebase project credentials
#define FIREBASE_HOST "wildlife-9afd0-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "XHUCXht5OQDUbiv0jP0pfXZ5hKLhDLc3mtmtA6ch"

 
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while (!Serial) { }

  dht.begin();

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running DHT, MQ-7, IR Flame, and Soil Moisture Sensors with Firebase");
  Serial.println("-------------------------------------");

  pinMode(MQ7PIN, INPUT); // Set the MQ7 sensor pin as input
  pinMode(FLAME_PIN, INPUT); // Set the IR flame sensor pin as input

  // Initialize the Firebase library
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int timeSinceLastRead = 0;

void loop() {
  // Report every 2 seconds.
  if (timeSinceLastRead > 2000) {
    // Reading temperature or humidity from DHT sensor
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);

    // Reading values from MQ-7 sensor
    int mq7Value = digitalRead(MQ7PIN);

    // Reading values from IR flame sensor
    int flameValue = digitalRead(FLAME_PIN);

    // Reading values from soil moisture sensor
    int moistureValue = analogRead(MOISTURE_PIN);

    // Check if any DHT reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    // Print DHT sensor readings
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");

    // Print MQ-7 sensor reading
    Serial.print("MQ-7 Sensor Value: ");
    Serial.println(mq7Value);

    // Print IR Flame sensor reading
    Serial.print("IR Flame Sensor Value: ");
    Serial.println(flameValue);

    // Print soil moisture sensor reading
    Serial.print("Soil Moisture Sensor Value: ");
    Serial.println(moistureValue);

    // Upload data to Firebase
    Firebase.setFloat("Temp",t);
    Firebase.setFloat("Humidity",h);
    Firebase.setInt("Flame",flameValue);
    Firebase.setInt("Moist",moistureValue);
   Firebase.setInt("Smoke",mq7Value);


    timeSinceLastRead = 0;

  }
  delay(100);
  timeSinceLastRead += 100;
}
