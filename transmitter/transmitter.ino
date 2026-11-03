#include <SPI.h>                             // Library for SPI communication  
#include <RF24.h>                            // Library for nRF24L01 radio module  
#include "Adafruit_Sensor.h"                 // Base library for Adafruit sensors  
#include "Adafruit_AM2320.h"                 // Library for AM2320 temperature & humidity sensor            

Adafruit_AM2320 am2320 = Adafruit_AM2320();  // Initialize AM2320 sensor  

RF24 radio(9, 8);                            // Initialize RF24 module (CE, CSN)  
const byte address[6] = "1Node";             // Unique address for communication  

struct SensorData {                          
  float TEMP;                                // Temperature reading  
  float HUM;                                 // Humidity reading  
  float UV;                                  // UV sensor value  
};                                          
SensorData data;                             // Create an instance of SensorData  

void setup() {                               
  radio.begin();                             // Initialize RF24 module  
  am2320.begin();                            // Initialize AM2320 sensor  

  radio.openWritingPipe(address);            // Set the transmission address  
  radio.setPALevel(RF24_PA_LOW);             // Set power level to low  
  radio.setDataRate(RF24_1MBPS);             // Set data rate  
  radio.stopListening();                     // Set to transmit mode  
}                                            

void loop() {                                
  data.TEMP = am2320.readTemperature();      // Read temperature from AM2320  
  data.HUM  = am2320.readHumidity();         // Read humidity from AM2320  

  float VOLTS = analogRead(A0) * (5.0 / 1023.0); // Read analog value and convert to voltage  
  data.UV = VOLTS / 0.1;                     // Convert voltage to UV index  

  radio.write(&data, sizeof(data));          // Send sensor data via RF24  
}               
