/*
  Demonstration of CNY70 Reflective Sensor being used as a tachometer.
  This simple circuit and sketch displays RPM on the Serial Monitor
  using a CNY70 Reflective Sensor to detect the passage of a reflector.
  https://www.addicore.com/Reflective-Optical-Sensor-CNY70-p/224.htm
  
  The sensor consists of an infrared LED emitter that reflects
  light off of a target onto a phototransistor reciever (CNY70) which
  sends an analog voltage signal to the microcontroller indicating 
  the strength of the reflection.  Each time the target revolves past
  the sensor an increase in voltage (rising level) indicates a
  completed rotation.
  
  For this application it is important that the sensor to target distance
  be fixed, that the target has strong reflective qualities, and that
  it reflect strongly to the CNY70 during only part of each revolution.
  White paper glued on a dark nonreflective surface works well. See the
  photos in the Wiki for how this might be done.
  
  The sketch was developed for the Texas Instruments MSP430F5529
  LaunchPad using Energia but should work with most LaunchPads.  It
  will also work on an Arduino Uno: change the analog pin to A0
  or the analog pin of your choice.  In the statement that follows
       pinMode(sensorPin, INPUT_PULLDOWN);
  remove ", INPUT_PULLDOWN".  The Arduino does not have an internal
  pulldown resistor.  Substitute 5V for 3V3 in the diagram when
  using the Arduino.
 
  You will probably need to modify the "lowLevel" and "highLevel"
  threshold variables to work with your environmental conditions.
  To do so, remove the // comment marks in front of these lines
      // Serial.print("Reading: ");
      // Serial.println(thisReading);
  and run the sketch.
  
  The readings on the serial monitor will
  let you know how high and low the readings are going. Move the 
  sensor back and forth, left and right, slowly until you get good
  separation between high and low. You may get good readings as
  much as an inch (2.5 cm) away from the target depending
  on enviromental light, target reflectivity, etc.
  
  Set the thresholds so that they are comfortably below the "lowLevel"
  and above the "highLevel".  Replace the comments to get rid of
  the clutter resulting from readings not associated with a rising
  edge.
  
  SAFETY NOTE: Take precautions not to place this sensor around high
  energy rotating equipment especially with your hands or other 
  body parts in the vicinity.
  
  Circuit diagram for LaunchPad
  -----------------------------
  
                  LaunchPad
                    Pin 6
                      |                    
         3V3          |--- 15k ohm --- GND
          |           |
        White       Green      Colors shown are arbitrary wire colors
          |           |        that can be used to keep things straight
        |-o----   ----o-|      
        |      ---      |      Looking down on CNY70 with
        |               |        - emitter LED on left
        |   O       O   |        - detector phototransistor on right
        |               |        - pins on bottom connect as shown
        |      ---      |        
        |-o----   ----o-|      Lettering is on right vertical face   
          |           |          with indents as shown
       Purple        Red       
          |           |
       100 ohm       3V3       
          |
         GND
         
  Arduino note: Use 5V instead of 3V3.  Use A0 or other analog pin for
  sensorPin.
  
  Created by Frank Milburn, 1 June 2015
  Released into the public domain
*/

const int sensorPin = 6;         // Specify an analog pin

bool fallen = false;             // Flag for falling/rising edge detection
int lowLevel = 400;              // Low level threshold for falling edge
int highLevel = 1000;            // High level threshold for rising edge
long lastTime = 0;               // Last time a rising edge was detected
long thisTime = 0;               // Current time

void setup() 
{
  Serial.begin(115200);          // Highest transmission the board allows
  Serial.print("Starting tachometer...");
  pinMode(sensorPin, INPUT_PULLDOWN);
  lastTime = micros();           // Using micros for higher resolution
  
}

void loop() 
{
  int thisReading = analogRead(sensorPin);    // Take a reading
  // Serial.print("Reading: ");
  // Serial.println(thisReading);
  if (thisReading > highLevel)                // check to see if above
  {                                           // the high level threshold,
    if (fallen == true)                       // if so, it was low on
    {                                         // previous pass which means a
                                              // rising edge was detected
                                              
      // Since there is only one rising edge per rotation on this
      // tachometer, and only the rising edges are being used in the RPM
      // calculation, the elapsed time since the last reading is equal to the
      // revolutions per microsecond.  To get RPM, divide (1,000,000
      // micros/second x 60 seconds/minute) by the elapsed time which
      // is equivalent to dividing 60000000 by the elasped time.
      
      thisTime = micros();
      long RPM = 60000000 / (thisTime - lastTime);
      Serial.print("   RPM: ");
      Serial.println(RPM);
      lastTime = thisTime;                      // Set the time to "last time" and
      fallen = false;                           // the edge to false so we won't count
                                                // this rise again
    }
  }
  if (thisReading < lowLevel)                   // When the edge first falls below the 
  {                                             // low level threshold, make fallen true                          
    if (fallen == false)                        
    {
      fallen = true;
    }
  }  
}
