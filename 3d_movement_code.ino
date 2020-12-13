#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
 int a=0;
 int b=0;
 int c=0;
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char messageFromPC[numChars] = {0};
int RGB1 = 0;
int RGB2 = 0;
int RGB3 = 0;
int XCOR = 0;
int YCOR = 0;


boolean newData = false;

//============

void setup() {
      AFMS.begin();

    Serial.begin(9600);
    Serial2.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    motor2->setSpeed(150);
    motor3->setSpeed(150);
    motor4->setSpeed(150);

}

//============

void loop() {
  a=0;
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
  

        //GO AWAY FROM THE OBJECT
        if (XCOR<370 && YCOR<225 && RGB3<150){ 
          while(a<1){
            motor3->run(FORWARD);      
            delay(200);
            motor3->run(RELEASE);
            delay(200);
            motor2->run(FORWARD);      
            delay(200);
            motor2->run(RELEASE);
            delay(200);
            motor4->run(BACKWARD);     
            delay(200);
            motor4->run(RELEASE);
            delay(100);
            a++;
          }
         a=0;
        }
        if (XCOR<370 && YCOR>235 && RGB3<150){ 
          while(a<1){
            motor3->run(FORWARD);     
            delay(200);
            motor3->run(RELEASE);
            delay(200);
            motor2->run(BACKWARD);    
            delay(200);
            motor2->run(RELEASE);
            delay(200);
            motor4->run(BACKWARD);      
            delay(200);
            motor4->run(RELEASE);
            delay(100);
            a++;
          }
         a=0;
        }
        else if (XCOR>380 && YCOR<225 && RGB3<150){ 
          while(a<1){
            motor3->run(BACKWARD);     
            delay(450);
            motor3->run(RELEASE);
            delay(200);
            motor2->run(FORWARD);      
            delay(200);
            motor2->run(RELEASE);
            delay(200);
            motor4->run(BACKWARD);      
            delay(200);
            motor4->run(RELEASE);
            delay(100);
            a++;
          }
         a=0;
        }
        if (XCOR>380 && YCOR>235 && RGB3<150){ 
          while(a<1){
            motor3->run(BACKWARD);     
            delay(450);
            motor3->run(RELEASE);
            delay(200);
            motor2->run(BACKWARD);      
            delay(200);
            motor2->run(RELEASE);
            delay(200);
            motor4->run(BACKWARD);      
            delay(200);
            motor4->run(RELEASE);
            delay(100);
            a++;
          }
         a=0;
        }
     
       //GO TOWARDS THE OBJECT
       
         if (XCOR<370 && YCOR<225 && RGB3>160){ 
          while(a<1){
            motor3->run(FORWARD);      
            delay(200);
            motor3->run(RELEASE);
            delay(200);
            motor2->run(FORWARD);      
            delay(200);
            motor2->run(RELEASE);
            delay(200);
            motor4->run(FORWARD);      
            delay(200);
            motor4->run(RELEASE);
            delay(100);
            a++;
          }
         a=0;
        }
        if (XCOR<370 && YCOR>235 && RGB3>160){ 
          while(a<1){
            motor3->run(FORWARD);      
            delay(200);
            motor3->run(RELEASE);
            delay(200);
            motor2->run(BACKWARD);      
            delay(200);
            motor2->run(RELEASE);
            delay(200);
            motor4->run(FORWARD);      
            delay(200);
            motor4->run(RELEASE);
            delay(100);
            a++;
          }
         a=0;
        }
        else if (XCOR>380 && YCOR<225 && RGB3>160){ 
          while(a<1){
            motor3->run(BACKWARD);      
            delay(450);
            motor3->run(RELEASE);
            delay(200);
            motor2->run(FORWARD);      
            delay(200);
            motor2->run(RELEASE);
            delay(200);
            motor4->run(FORWARD);      
            delay(200);
            motor4->run(RELEASE);
            delay(100);
            a++;
          }
         a=0;
        }
        if (XCOR>380 && YCOR>235 && RGB3>160){ 
          while(a<1){
            motor3->run(BACKWARD);      
            delay(450);
            motor3->run(RELEASE);
            delay(200);
            motor2->run(BACKWARD);      
            delay(200);
            motor2->run(RELEASE);
            delay(200);
            motor4->run(FORWARD);      
            delay(200);
            motor4->run(RELEASE);
            delay(100);
            a++;
          }
         a=0;
        }
        newData = false;
        b=0;
        c=0;
    }
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index
    
    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx);
    
    strtokIndx = strtok(NULL,",");      // get the first part - the string
    RGB1=atoi(strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    RGB2 = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    RGB3 = atoi(strtokIndx);     // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    XCOR = atoi(strtokIndx);     // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    YCOR = atoi(strtokIndx);     // convert this part to a float

}
