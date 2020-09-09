// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2    // Digital DHT sensor PIN   
#define RELAY1 7    // MotionSensor Relay PIN
#define RELAY2 4    // Humidty Relay PIN
#define MOTION 8    // Motion Sensor PIN
#define LIGHTREQ 3    // Digital Light Sensor 

// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  Serial.begin(9600);
  while (!Serial) ;
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(MOTION, INPUT);
  pinMode(LIGHTREQ, INPUT);

  digitalWrite(RELAY1,HIGH);
  digitalWrite(RELAY2,HIGH);

  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  sensor_t sensor2;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));


  
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

float hum_reading_sec[30] = {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100};
float hum_reading_min[10] = {100,100,100,100,100,100,100,100,100,100};
float temp_reading_sec[30] = {100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100};
int relay2_on_off = 0;
unsigned long relay2_on_time = 0;
int on_time_min = 2;

unsigned long start_seconds = millis()/1000;

void loop() {

  // Delay between measurements.
  delay(100);
 
  unsigned long seconds = millis()/1000;
  int sec_counter = seconds % 30;
  int min_counter = (seconds/60) % on_time_min;

  float hum;
  float temp;
  
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
//  if (isnan(event.temperature)) {
//    Serial.println(F("Error reading temperature!"));
//  }
//  else {
//    Serial.print(F("Temperature: "));
//    Serial.print(event.temperature);
//    Serial.println(F("°C"));
//  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
//    Serial.print(F("Humidity: "));
//    Serial.print(event.relative_humidity);
//    Serial.println(F("%"));
      hum = event.relative_humidity;
      hum_reading_sec[sec_counter] = hum;
      hum_reading_min[min_counter] = hum;
      temp = event.temperature;
      temp_reading_sec[sec_counter] = temp;
  }
  int i_light_req =  digitalRead(LIGHTREQ);
  int last_30_sec = 29 - sec_counter;
  int last_10_min = (on_time_min - 1) - min_counter;

   Serial.print(F("Version: 1.1"));
   Serial.print(F("Humidity: "));
   Serial.println(hum);

   Serial.print(F("Last30SecHum: "));
   Serial.println(hum_reading_sec[last_30_sec]);

   Serial.print(F("Last10MinHum: "));
   Serial.println(hum_reading_min[last_10_min]);

   Serial.print(F("SecCounter: "));
   Serial.println(sec_counter);


   Serial.print(F("Temperatur: "));
   Serial.println(hum);

   Serial.print(F("Last30SecTemperatur: "));
   Serial.println(temp_reading_sec[last_30_sec]);

    Serial.println( millis() );
    Serial.print("start seconds: ");
    Serial.println(start_seconds);

    Serial.print("seconds: ");
    Serial.println(seconds);
    Serial.println((seconds - start_seconds));
    
    // If relay2 is off and humidity has increased in last 30 seconds to more than 1% then turn on relay
    if( (relay2_on_off == 0) && ((hum - hum_reading_sec[last_30_sec]) > 1.5 ) && ((temp - temp_reading_sec[last_30_sec]) > 1.5 ) && seconds > 30 ) {
      // Turn on Relay 2
      relay2_on_time = seconds;
      relay2_on_off = 1;
    }
    // If relay2 is on for last 10 minutes and difference of humidity of last 10 minutes is less than 0.5% then turn of relay2
    if( (relay2_on_off == 1) && ((seconds - relay2_on_time) > 600) && ( abs(hum - hum_reading_min[last_10_min]) <= 0.5 ))
    {
      // Turn Off Relay2
      relay2_on_off = 0;
    }

    if(relay2_on_off == 0)
      digitalWrite(RELAY2,HIGH);
    else
      digitalWrite(RELAY2,LOW);



    /////////////////// RELAY1 CONTROL MOTION SENSOR /////////////////////
    int motion = digitalRead(MOTION);
//  int a =  analogRead(A0);
    Serial.println(motion);
    if(motion == HIGH){
      if(i_light_req == 1){
        digitalWrite(RELAY1,LOW);
      }
    }
    else
    {
    digitalWrite(RELAY1, HIGH);
    }

}
