#include "OneButton.h"        // To detect multiple and long button presses
#include <WiFi.h>             // TO conenct to wifi 
#include <WebServer.h>        // To host local web server
#include <Preferences.h>      // To store the infos collected from webpage
#include <FastLED.h>          // LED coloured automation library

//defining i/p pins for soil moisture data
#define soilSensor1 15
#define soilSensor2 7
#define soilSensor3 6
#define soilSensor4 5
#define soilSensor5 4

//defining o/p pins for driving relays
#define RELAY_LED1 8
#define RELAY_LED2 18
#define RELAY_LED3 17
#define RELAY_LED4 16
#define RELAY_LED5 3

#define indication_LED 9        // Single color LED connected at outside of the box 
#define LED_PIN   48            // Pin connected to the LED
#define RELAY_LED_INDICATOR 14            // Pin connected to LED that indicates RELAY ON
#define COLOR     CRGB::Green   // Color of the LED
CRGB leds[1];                   // Define LED array

unsigned long previousMillis = 0;   // Variable to store the previous time
const long interval = 1000;         // Interval at which to blink (milliseconds)
bool ledState = false;              // Current state of the LED


//#define GLED1 21
//#define GLED2 38
//#define GLED3 39
//#define GLED4 1
//#define GLED5 2

// arrays to hold values of multiple sensors
const int num_moisture_sensors = 5; 
const int soil_moisture_pins[num_moisture_sensors] = {soilSensor1, soilSensor2, soilSensor3, soilSensor4, soilSensor5};
uint32_t  soil_moisture_adcValue[num_moisture_sensors];
uint8_t   soil_moisture_percent[num_moisture_sensors]; 
const int relay_pins[num_moisture_sensors] = {RELAY_LED1, RELAY_LED2, RELAY_LED3, RELAY_LED4, RELAY_LED5};
//const int led_pins[num_moisture_sensors] = {GLED1, GLED2, GLED3, GLED4, GLED5};
int realy_state = false; // TO track whether the relay is ON or Not

uint threashold_min;
uint threashold_max;
long sensor_test_timer;
long serial_print_timer;
const int btn = 13;  //13;              // User Switch, long press to activate hotspot



//Function Prototypes
void test_moisture_sensors();
void hotspot(); 
bool hotspot_activated = false; 

OneButton button(btn, true);            // [Active LOW]   //attach a button on pin 13 to the library
WebServer server(80);
Preferences preferences;                // Flash Memory

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(indication_LED, OUTPUT);
  pinMode(RELAY_LED_INDICATOR, OUTPUT);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, 1);
  FastLED.setBrightness(60);
  leds[0] =  CRGB::Green;
  FastLED.show();

  Serial.begin(115200);
  pinMode(btn, INPUT_PULLUP);

  preferences.begin("moisture_config", false);
  //preferences.clear();
  threashold_min = preferences.getInt("moisture_min", 20);
  threashold_max = preferences.getInt("moisture_max", 80);
  preferences.end();
  
  Serial.printf("\n\n---------------------Received Data Constrained---------------\n");
  Serial.printf("[Info] Threashold Min: %d\n[Info] Threashold Max: %d\n\n", threashold_min, + threashold_max);



 for(int i =0; i<num_moisture_sensors; i++){
  pinMode(soil_moisture_pins[i], INPUT);
  pinMode(relay_pins[i], OUTPUT);
  //pinMode(led_pins[i],OUTPUT);
  }

  // Put relay into OFF state when woken up
  for(int j=0; j<num_moisture_sensors; j++){
  //  digitalWrite(relay_pins[j], HIGH);
   digitalWrite(relay_pins[j], LOW);
  }

  button.attachLongPressStart(hotspot);           // link the function to be called on a long click event.
  button.setPressMs(5000);                        // 3+2 = 5 seconds button hold duration.
}

void loop() {
 button.tick();              // Handle Button Press Events
 handle_webserver();         // Handle Webserver;
 
 // Test Moisture sensor every 5 seconds without Blocking loop
 if(millis() - sensor_test_timer > 5000){
  sensor_test_timer = millis(); 
  test_moisture_sensors();
  }
    // Display the measured parameters every 20 seconds
  if(millis() - serial_print_timer > 5000){
  serial_print_timer = millis();
  Serial.println("-------------------------------------");
  for(int i =0; i<num_moisture_sensors; i++)
  Serial.printf("Moisture %d Value: %d, Moisture %d Percent: %d\n",i, soil_moisture_adcValue[i], i, soil_moisture_percent[i]);
  }


 const int sample_size = 200;
 int driest_sensor_number; 
 

 // set o to every soil moisture at loop
 for(int i =0; i<num_moisture_sensors; i++){
   soil_moisture_adcValue[i] = 0;
 }


 for(int i =0; i<num_moisture_sensors; i++){
    for(int j=0; j<sample_size; j++){
     soil_moisture_adcValue[i] += analogRead(soil_moisture_pins[i]);
    }
  }

  // Take mean Moisture Value from all moisture Sensors
  for(int i =0; i<num_moisture_sensors; i++){
    soil_moisture_adcValue[i] /=  sample_size;
    soil_moisture_percent[i] =  map_adc_to_percent(soil_moisture_adcValue[i]);
  }

  // look for minimum moisture threashold at sensors to turn on Motor Pump
  for(int i = 0; i<num_moisture_sensors; i++){
    if (soil_moisture_percent[i] < threashold_min && soil_moisture_percent[i] > 5){
   //     digitalWrite(led_pins[i], LOW); // Turf off Green LED indicatind Dryness at that sensor
      
      if (realy_state != HIGH) {
        Serial.println("\n------------------- \"Dry Alert\" -------------------");
        Serial.printf("[Info] Dryness at Moisture Sensor: %d", i);
        driest_sensor_number = i;
        realy_state = HIGH; 
        digitalWrite(RELAY_LED_INDICATOR, !realy_state);
        
        // Turn each relay ON
        for(int j=0; j<num_moisture_sensors; j++)
          //digitalWrite(relay_pins[j], !realy_state);
          digitalWrite(relay_pins[j], realy_state);
        
      }
   } else {
    // digitalWrite(led_pins[i], HIGH); // Turn ON Green LED indicating Proper Moisture at sensor
      digitalWrite(RELAY_LED_INDICATOR , realy_state);
    }
  }
    
    


  //Detect occasional sensor noise
  if(soil_moisture_percent[driest_sensor_number] < 5 && realy_state != LOW) {
    realy_state = LOW;
    Serial.println("\n\n------------------- \"Fault Detected\" -------------------");
    Serial.printf("[Info] False Relay Triggred by Moisture Sensor: %d \n", driest_sensor_number);
    
    // Turn each relay OFF
    for(int j=0; j<num_moisture_sensors; j++){
      //digitalWrite(relay_pins[j], !realy_state);   
      digitalWrite(relay_pins[j], realy_state);
    }
  }

  // look for maximum moisture threashold at sensors to turn off the Motor Pump
  if (realy_state != LOW) {
    if (soil_moisture_percent[driest_sensor_number] > threashold_max){
      Serial.println("------------------- \"Irrigation Complete\" -------------------");
      Serial.printf("[Info] Threashold Reached by Moisture Sensor: %d", driest_sensor_number);
      realy_state = LOW;   
     
      // Turn each relay OFF
      for(int j=0; j<num_moisture_sensors; j++){
        //digitalWrite(relay_pins[j], !realy_state);
        digitalWrite(relay_pins[j], realy_state);
      }
    }
  }
  delay(400); // Delay after checking all sensors


}


int  active_sensor_count;  
int  Over_irrigated_sensor_count; 

void test_moisture_sensors(){
  active_sensor_count = 0;
  Over_irrigated_sensor_count = 0;
 
 for(int i =0; i<num_moisture_sensors; i++){
   if( soil_moisture_percent[i] > 5){ // All active sensors will have value >0. 
      active_sensor_count ++;
   }
   if(soil_moisture_percent[i] > threashold_max){
    Over_irrigated_sensor_count++;
   }
 }
 Serial.printf("\n\n-----------\"Sensor Test\"-------------\nTotal Active Sensors: %d,\nTotal Over Irrigated Sensors: %d \n\n", active_sensor_count, Over_irrigated_sensor_count);
}
int map_adc_to_percent(int moisture_adc_value){
  const int threashold_moisture_0 = 4095;
  const int threashold_moisture_100 = 1500;
  int moisture_percent;

  moisture_percent = map(moisture_adc_value, threashold_moisture_0, threashold_moisture_100, 0,100);
  moisture_percent = constrain(moisture_percent, 0, 100);
  return moisture_percent;
}


