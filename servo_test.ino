
#include <Servo.h>

int finger1pin = A5;
int finger2pin = A0;
int finger3pin = A1;
int finger4pin = A2;
int finger5pin = A4;

int min_servo = 5;
int max__servo = 175;

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
// create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = min_servo;
int pos1 = min_servo;
int pos2 = min_servo;
int pos3 = min_servo;
int pos4 = min_servo;
int pos5 = min_servo; // variable to store the servo position

void setup()
{
    myservo1.attach(finger1pin);
    myservo2.attach(finger2pin);
    myservo3.attach(finger3pin);
    myservo4.attach(finger4pin);
    myservo5.attach(finger5pin); // attaches the servo on pin 9 to the servo object
}

void loop()
{

    // close all fingers
    for (pos = min_servo; pos <= max__servo; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo1.write(pos);
        myservo2.write(pos);
        myservo3.write(170 - pos);
        myservo4.write(pos);
        myservo5.write(pos); // tell servo to go to position in variable 'pos'
        delay(15);           // waits 15 ms for the servo to reach the position
    }
    delay(400);
    // open all fingers
    for (pos = max__servo; pos >= min_servo; pos -= 1)
    { // goes from 180 degrees to 0 degrees
        myservo1.write(pos);
        myservo2.write(pos);
        myservo3.write(max__servo - pos);
        myservo4.write(pos);
        myservo5.write(pos); // tell servo to go to position in variable 'pos'
        delay(15);           // waits 15 ms for the servo to reach the position
    }
    delay(400);
    // close fingers 1 by 1
    for (pos = min_servo; pos <= max__servo; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo1.write(pos);
        delay(15);
    }
    for (pos = min_servo; pos <= max__servo; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo2.write(pos);
        delay(15);
    }
    for (pos = min_servo; pos <= max__servo; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo3.write(max__servo - pos);
        delay(15);
    }
    for (pos = min_servo; pos <= max__servo; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo4.write(pos);
        delay(15);
    }
    for (pos = min_servo; pos <= max__servo; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo5.write(pos);
        delay(15);
    }
    delay(400);
    // open fingers 1 by 1
    for (pos = max__servo; pos >= min_servo; pos -= 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo1.write(pos);
        delay(15);
    }
    for (pos = max__servo; pos >= min_servo; pos -= 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo2.write(pos);
        delay(15);
    }
    for (pos = max__servo; pos >= min_servo; pos -= 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo3.write(max__servo - pos);
        delay(15);
    }
    for (pos = max__servo; pos >= min_servo; pos -= 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo4.write(pos);
        delay(15);
    }
    for (pos = max__servo; pos >= min_servo; pos -= 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo5.write(pos);
        delay(15);
    }
    delay(400);
}