/*   Basic Fill Box Replacement:
 *    Utilizes factory autofill probe (Nuova Simonelli Oscar)
 *    Utilizes G3MB-202P PCB SSR for autofill control
 *    Utilizes Arduino Uno R3
 *  
 *   USE AT YOUR ON RISK.
 *   I am posting this project for EDUCATIONAL USE ONLY.
 *   This project involves electricity, high heat, and water/steam.
 *   I will not be held liable damages and/or injuries resulting from the use of this code or from copying this project.
 *   
 *   This is a fork of:
 *   Andreja Premium Control Board Replacement 
 *   Written By Anthony DiPilato
 *   
 *   It has been simplified to have no buzzer or float switches for plumbed in models.
 *   Additionally, I have adapted the triggers for the pump/valve for use with a solid state relay (had to reverse readings).
 */

// Configuration
#include "Configuration.h"

// Include Melody library and Sensors class
#include "Sensors.h"

// Setup sensor pins
Sensors boiler(9);

// Status for heater valve and pump
int heat_state = 1;
int pump_state = 0; // pump and valve are shared

long pump_time = 0;

void setup() {
    Serial.begin(9600);
  
    // Output mode for relays
    pinMode(PUMP, OUTPUT);       
    pinMode(VALVE, OUTPUT);
    pinMode(HEAT, OUTPUT);

    // Initialize relays
    // Pump and valve off, Heat on
    // Heat is on N/O relay
    digitalWrite(PUMP, HIGH);
    digitalWrite(VALVE, HIGH);  
    digitalWrite(HEAT, HIGH);
  
}

// Serial Output for debugging
void debug_output(){
    Serial.print("Boiler: ");
    Serial.println(boiler.value);
    delay(250);
}

void toggle_heat(int state){
    if(state == heat_state){
        return;
    }
    switch(state){
        case 0:
            digitalWrite(HEAT, LOW); // LOW is on, N/O relay
            break;
        case 1:
            digitalWrite(HEAT, HIGH);
            break;
    }
    heat_state = state;
    return;
}

void toggle_pump(int state){
  if(state == pump_state){
    return;
  }
  if(state == 0){
      if ((millis() - pump_time) > PUMP_DELAY) {
          // Turn off pump before valve
          digitalWrite(PUMP, HIGH); // High is off
          digitalWrite(VALVE, HIGH);
          pump_state = state;
      }
  }else{
      // Turn on valve before pump
      digitalWrite(VALVE, LOW);
      digitalWrite(PUMP, LOW);
      pump_state = state;
  }
  return;
}

void check_sensors(){
    // Read Sensors
    boiler.check();
    // If boiler water is low turn on pump
    if(boiler.value == 0){
        toggle_pump(0);
    }else{
        toggle_pump(1);
    }
}

// Main Loop
void loop() {
    // read sensors
    check_sensors();  
    // Debug Output
    if(DEBUG == true){ debug_output();}
}
