// Plotclock
// cc - by Johannes Heberlein 2014
// v 1.01
// thingiverse.com/joo   wiki.fablab-nuernberg.de
// units: mm; microseconds; radians
// origin: bottom left of drawing surface

// Make: 
// SandUhr 2.0 Addition
// Set "Board" menu to Ardunino Nano if MikroE MINI AT is used

// time library see http://playground.arduino.cc/Code/time 

// delete or mark the next line as comment when done with calibration  
//#define CALIBRATION

// When in calibration mode, adjust the following factors until the servos move exactly 90 degrees
#define SERVOFAKTORLEFT 560
#define SERVOFAKTORRIGHT 580

// Zero-position of left and right servo
// When in calibration mode, adjust the NULL-values so that the servo arms are at all times parallel
// either to the X or Y axis

//von der Servo-Seite aus gesehen!
#define SERVOLEFTNULL 1650
#define SERVORIGHTNULL 820

//#define SERVOLEFTNULL 2000
//#define SERVORIGHTNULL 1150

#define SERVOPINLIFT  3
#define SERVOPINLEFT  4
#define SERVOPINRIGHT 5

// lift positions of lifting servo
#define LIFT0 1350 // on drawing surface
#define LIFT1 1100  // between numbers
#define LIFT2 900  // going towards sweeper

#define NUMBERSCALE 1.5

// speed of liftimg arm, higher is slower
#define LIFTSPEED 1500

// length of arms
#define L1 42.0
#define L2 55.0
#define L3 9.0

// origin points of left and right servo 
#define O1X 22
#define O1Y -25
#define O2X 47
#define O2Y -25

#define idleX 60
#define idleY 15

float OffsetLR = -25;

//#include <Time.h> // see http://playground.arduino.cc/Code/time 
#include <Servo.h>
#include <Wire.h> // I2C-Bibliothek einbinden
#include "RTClib.h" // RTC-Bibliothek einbinden

RTC_DS1307 RTC;      // RTC Modul

int servoLift = LIFT2;

Servo ServoLift_1;  // 
Servo ServoRight_2;  // 
Servo ServoLeft_3;  // 

volatile double lastX = idleX;
volatile double lastY = idleX;

int last_min = 0;

void setup() 
{ 
  // Set current time only the first to values, hh,mm are needed
  pinMode(A0, OUTPUT);	// Servos Enable
  pinMode(A1, OUTPUT);  // Vibrationsmotor
  digitalWrite(A0, HIGH);  // enable Servos

  ServoLift_1.attach(SERVOPINLIFT);   //  lifting servo
  ServoLift_1.writeMicroseconds(LIFT1);				
  delay(500);
  ServoRight_2.attach(SERVOPINLEFT);   //  left servo
  ServoRight_2.writeMicroseconds(1500);
  delay(500);
  ServoLeft_3.attach(SERVOPINRIGHT);  //  right servo
  ServoLeft_3.writeMicroseconds(1500);				
  delay(500);

#ifdef CALIBRATION
  motor_on(1000);
  ServoLift_1.writeMicroseconds(LIFT0);				
  delay(2000);
#endif

  drawTo(idleX, idleY);

  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    
    // Aktuelles Datum und Zeit setzen, falls die Uhr noch nicht läuft
    RTC.adjust(DateTime(__DATE__, __TIME__));
    
  }


} 

void loop() 
{ 
  DateTime datetime = RTC.now();
  int minute = datetime.minute();
  int hour = datetime.hour();

  lift(2);

#ifdef CALIBRATION

// Servohorns will have 90° between movements, parallel to x and y axis
  drawTo(idleX, idleY);
//  lift(0);
  drawTo(-3, 29.2);
  delay(1000);
//  lift(1);
  drawTo(74.1, 28);
  delay(1000);
//  lift(2);
//  delay(1000);

#else 


  int i = 0;
  if (last_min != minute) {

		motor_on(3000);

    if (!ServoLift_1.attached()) ServoLift_1.attach(SERVOPINLIFT);
    if (!ServoRight_2.attached()) ServoRight_2.attach(SERVOPINLEFT);
    if (!ServoLeft_3.attached()) ServoLeft_3.attach(SERVOPINRIGHT);

    lift(1);

    //hour();
    while ((i+1)*10 <= hour)
    {
      i++;
    }

    number(0 + OffsetLR, 22, i, NUMBERSCALE);
    number(21 + OffsetLR, 22, (hour-i*10), NUMBERSCALE);
    number(40 + OffsetLR, 22, 11, NUMBERSCALE); // Doppelpunkt

    i=0;
    while ((i+1)*10 <= minute)
    {
      i++;
    }
    number(54 + OffsetLR, 18, i, NUMBERSCALE+0.1);
    number(70 + OffsetLR, 16, (minute-i*10), NUMBERSCALE+0.13);
//    number(54, 18, i, NUMBERSCALE+0.1);
//    number(70, 16, (minute()-i*10), NUMBERSCALE+0.13);
    lift(2);
    last_min = minute;

  	drawTo(idleX, idleY);
  	
    ServoLift_1.detach();
    ServoRight_2.detach();
    ServoLeft_3.detach();
  }

#endif

} 

// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals a 20 mm high font.
// The structure follows this principle: move to first startpoint of the numeral, lift down, draw numeral, lift up

void number(float bx, float by, int num, float scale) {

  switch (num) {

  case 0:
    drawTo(bx + 12 * scale, by + 6 * scale);
    lift(0);
    bogenGZS(bx + 7 * scale, by + 10 * scale, 10 * scale, -0.8, 6.7, 0.5);
    lift(1);
    break;
  case 1:

    drawTo(bx + 3 * scale, by + 15 * scale);
    lift(0);
    drawTo(bx + 10 * scale, by + 20 * scale);
    drawTo(bx + 10 * scale, by + 0 * scale);
    lift(1);
    break;
  case 2:
    drawTo(bx + 2 * scale, by + 12 * scale);
    lift(0);
    bogenUZS(bx + 8 * scale, by + 14 * scale, 6 * scale, 3, -0.8, 1);
    drawTo(bx + 1 * scale, by + 0 * scale);
    drawTo(bx + 14 * scale, by + 0 * scale);
    lift(1);
    break;
  case 3:
    drawTo(bx + 2 * scale, by + 17 * scale);
    lift(0);
    bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 3, -2, 1);
    bogenUZS(bx + 5 * scale, by + 5 * scale, 5 * scale, 1.57, -3, 1);
    lift(1);
    break;
  case 4:
    drawTo(bx + 10 * scale, by + 0 * scale);
    lift(0);
    drawTo(bx + 10 * scale, by + 20 * scale);
    drawTo(bx + 2 * scale, by + 6 * scale);
    drawTo(bx + 12 * scale, by + 6 * scale);
    lift(1);
    break;
  case 5:
    drawTo(bx + 2 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 6 * scale, 6 * scale, -2.5, 2, 1);
    drawTo(bx + 5 * scale, by + 20 * scale);
    drawTo(bx + 12 * scale, by + 20 * scale);
    lift(1);
    break;
  case 6:
    drawTo(bx + 2 * scale, by + 10 * scale);
    lift(0);
    bogenUZS(bx + 7 * scale, by + 6 * scale, 6 * scale, 2, -4.4, 1);
    drawTo(bx + 11 * scale, by + 20 * scale);
    lift(1);
    break;
  case 7:
    drawTo(bx + 2 * scale, by + 20 * scale);
    lift(0);
    drawTo(bx + 12 * scale, by + 20 * scale);
    drawTo(bx + 2 * scale, by + 0);
    lift(1);
    break;
  case 8:
    drawTo(bx + 5 * scale, by + 10 * scale);
    lift(0);
    bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 4.7, -1.6, 1);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 5 * scale, -4.7, 2, 1);
    lift(1);
    break;

  case 9:
    drawTo(bx + 9 * scale, by + 11 * scale);
    lift(0);
    bogenUZS(bx + 7 * scale, by + 15 * scale, 5 * scale, 4, -0.5, 1);
    drawTo(bx + 5 * scale, by + 0);
    lift(1);
    break;

  case 11: // Doppelpunkt
    drawTo(bx + 5 * scale, by + 15 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 15 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    drawTo(bx + 5 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    break;

  }
}


void motor_on(int vibtime) {
  digitalWrite(A1, HIGH); 	// Motor
  delay(vibtime);
  digitalWrite(A1, LOW);
  delay(250);
}



void lift(char lift) {
  switch (lift) {
    // room to optimize  !

  case 0: //850

      if (servoLift >= LIFT0) {
      while (servoLift >= LIFT0) 
      {
        servoLift--;
        ServoLift_1.writeMicroseconds(servoLift);				
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT0) {
        servoLift++;
        ServoLift_1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);

      }

    }

    break;

  case 1: //150

    if (servoLift >= LIFT1) {
      while (servoLift >= LIFT1) {
        servoLift--;
        ServoLift_1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);

      }
    } 
    else {
      while (servoLift <= LIFT1) {
        servoLift++;
        ServoLift_1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }

    }

    break;

  case 2:

    if (servoLift >= LIFT2) {
      while (servoLift >= LIFT2) {
        servoLift--;
        ServoLift_1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT2) {
        servoLift++;
        ServoLift_1.writeMicroseconds(servoLift);				
        delayMicroseconds(LIFTSPEED);
      }
    }
    break;
  }
}


void bogenUZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = -0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) > ende);

}

void bogenGZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = 0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) <= ende);
}


void drawTo(double pX, double pY) {
  double dx, dy, c;
  int i;

  // dx dy of new point
  dx = pX - lastX;
  dy = pY - lastY;
  //path lenght in mm, times 4 equals 4 steps per mm
  c = floor(4 * sqrt(dx * dx + dy * dy));

  if (c < 1) c = 1;

  for (i = 0; i <= c; i++) {
    // draw line point by point
    set_XY(lastX + (i * dx / c), lastY + (i * dy / c));

  }

  lastX = pX;
  lastY = pY;
}

double return_angle(double a, double b, double c) {
  // cosine rule for angle between c and a
  return acos((a * a + c * c - b * b) / (2 * a * c));
}

void set_XY(double Tx, double Ty) 
{
  delay(1);
  double dx, dy, c, a1, a2, Hx, Hy;

  // calculate triangle between pen, servoLeft and arm joint
  // cartesian dx/dy
  dx = Tx - O1X;
  dy = Ty - O1Y;

  // polar lemgth (c) and angle (a1)
  c = sqrt(dx * dx + dy * dy); // 
  a1 = atan2(dy, dx); //
  a2 = return_angle(L1, L2, c);

  ServoRight_2.writeMicroseconds(floor(((a2 + a1 - M_PI) * SERVOFAKTORLEFT) + SERVOLEFTNULL));

  // calculate joinr arm point for triangle of the right servo arm
  a2 = return_angle(L2, L1, c);
  Hx = Tx + L3 * cos((a1 - a2 + 0.621) + M_PI); //36,5°
  Hy = Ty + L3 * sin((a1 - a2 + 0.621) + M_PI);

  // calculate triangle between pen joint, servoRight and arm joint
  dx = Hx - O2X;
  dy = Hy - O2Y;

  c = sqrt(dx * dx + dy * dy);
  a1 = atan2(dy, dx);
  a2 = return_angle(L1, (L2 - L3), c);

  ServoLeft_3.writeMicroseconds(floor(((a1 - a2) * SERVOFAKTORRIGHT) + SERVORIGHTNULL));

}




