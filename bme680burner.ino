/***************************************************************************
  This project is designed for running on a device with the BME680 sensor.
  It is for testing and running the burn-in period.
  
  Based on the Adafruit sample code.
  
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
***************************************************************************/

#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

//* BME680 pin definitions
#define BME_CS                          48                    // BME680->CS
#define BME_MOSI                        13                    // BME680->SDA
#define BME_SCK                         21                    // BME680->SCL
#define BME_MISO                        47                    // BME680->SDO

//* Time between reads (seconds)
#define READ_INTERVAL                   30

//* LED pin definitions
#define LED_RUN                         18                    // Show LED when running loop.
#define LED_ACTIVE                      8                     // Show LED when reading sensor.

//Adafruit_BME680 bme;                                        // I2C
//Adafruit_BME680 bme(BME_CS);                                // hardware SPI
Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);    // custom SPI


unsigned long LoopMillis = 0;                                 // Loop timing


void setup() {

  Serial.begin(115200);
  Serial.print(F("\r\n"));
  Serial.println(F("BME680 tester/burn-in program."));

  Serial.print(F("Turning on BME680... "));
  if (!bme.begin()) {
    Serial.print(F("ERROR!!"));
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  Serial.println(F("OK!!"));

  //* Turn on LED to show we are running the loop.
  pinMode(LED_RUN, OUTPUT);
  digitalWrite(LED_RUN, HIGH);

  pinMode(LED_ACTIVE, OUTPUT);
  digitalWrite(LED_ACTIVE, LOW);

  Serial.print(F("Configuring BME680 sensor.. "));

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 300); // 320*C for 300 ms

  Serial.println(F("OK!!"));
  Serial.println("Entering main loop...");
  Serial.print(F("\r\n"));

  //* Start Loop timing
  LoopMillis = millis();
}

void loop() {

  //* Get the current time in milliseconds since the program started.
  unsigned long MillisTemp = millis(); 

  //* Check if its time to read the sensor.
  if (MillisTemp - LoopMillis >= READ_INTERVAL * 1000) {

    //* Turn on LED to show we are reading the sensor.
    digitalWrite(LED_ACTIVE, HIGH);

    //* Perform a reading of the sensor.
    Serial.print(F("Reading BME680... "));
    if (! bme.performReading()) {
      //WE FAILED!!
      Serial.println(F("ERROR!! Failed to perform reading."));
      //* Turn off LED to show we are done reading.
      digitalWrite(LED_ACTIVE, LOW);
      //* Reset the timer
      LoopMillis = millis();
      return;
    }

    Serial.println(F("OK!!"));

    //* Show the results.
    Serial.print("Temperature = ");
    Serial.print(bme.temperature);
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bme.pressure / 100.0);
    Serial.println(" hPa");

    Serial.print("Humidity = ");
    Serial.print(bme.humidity);
    Serial.println(" %");

    Serial.print("Gas = ");
    Serial.print(bme.gas_resistance / 1000.0);
    Serial.println(" KOhms");

    Serial.println();
    
    //* Turn off LED to show we are done reading.
    digitalWrite(LED_ACTIVE, LOW);

    //* Reset the timer
    LoopMillis = millis();

  }

}
