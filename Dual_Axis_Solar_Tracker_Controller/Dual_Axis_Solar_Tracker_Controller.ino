/*
 * Dual_Axis_Tracker_V3.ino
 *
 * Brown Dog Gadgets <https://www.browndoggadgets.com/>
 * 
 */

// include Servo library
#include <Servo.h>

// horizontal servo
Servo horizontal;
int servoh = 90;

int servohLimitHigh = 120;
int servohLimitLow = 50;

Servo vertical;
int servov = 0;

int servovLimitHigh = 180;
int servovLimitLow = 0;


// LDR pin connections
int ldrTR = A1; // LDR top right
int ldrTL = A2; // LDR top left
int ldrBR = A3; // LDR bottom right
int ldrBL = A4; // LDR bottom left


void setup() {
  Serial.begin(9600);
  // servo connections
  horizontal.attach(A5);
  vertical.attach(3);
  // move servos
  horizontal.write(90);
  vertical.write(0);
  delay(1500);
}


void loop() {

  int tr = analogRead(ldrTR); // top right
  int tl = analogRead(ldrTL); // top left
  int br = analogRead(ldrBR); // bottom right
  int bl = analogRead(ldrBL); // bottom left

  int dtime = 45; // change for debugging only
  int tol = 50;

  int avt = (tl + tr) / 2; // average value top
  int avd = (bl + br) / 2; // average value bottom
  int avl = (tl + bl) / 2; // average value left
  int avr = (tr + br) / 2; // average value right

  int dvert = avt - avd;  // check the difference of up and down
  int dhoriz = avl - avr; // check the difference of left and right

  // check if the difference is in the tolerance else change vertical angle
  if (-1 * tol > dvert || dvert > tol) {
    if (avt > avd) {
      servov = ++servov;
      if (servov > servovLimitHigh) {
        servov = servovLimitHigh;
      }
    }
    else if (avt < avd) {
      servov = --servov;
      if (servov < servovLimitLow) {
        servov = servovLimitLow;
      }
    }
    vertical.write(servov);
  }

  // check if the difference is in the tolerance else change horizontal angle
  if (-1 * tol > dhoriz || dhoriz > tol) {
    if (avl > avr) {
      servoh = --servoh;
      if (servoh < servohLimitLow) {
        servoh = servohLimitLow;
      }
    }
    else if (avl < avr) {
      servoh = ++servoh;
      if (servoh > servohLimitHigh) {
        servoh = servohLimitHigh;
      }  
    }
    else if (avl = avr) {
      // nothing
    }
    horizontal.write(servoh);
  }
  
  delay(dtime);
  
}
