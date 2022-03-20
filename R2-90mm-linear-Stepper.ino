/*
 Stepper Motor Control - one revolution
##Stepper Motor

 - yellow and red
 - black and blue
 
 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 Stepper Module Proto Board
 IN B1 A   11 // 9
 IN B1 B   10 // 8
 IN A1 A   8  // 7
 IN A1 B   9  // 6
 Motor  
 BLK BLU 1
 YEL RED 2
 Leonardo
*/


#include <ezButton.h>
#include <AccelStepper.h>
#define MAX_POSITION        3200 // maximum of position we can set (long type)
#define DEBUG true
#define DIRECTION_CCW -1
#define DIRECTION_CW   1
int direction  = DIRECTION_CW;
long targetPos = 0;
ezButton limitSwitch(A1); // create ezButton object that attach to pin A0;
ezButton actionSwitch(A0); // create ezButton object that attach to pin A0;

//AccelStepper stepper(AccelStepper::FULL4WIRE, 9, 8, 7, 6);

AccelStepper stepper(AccelStepper::FULL4WIRE, 9, 8, 7, 6);

bool isStopped = false;
bool StartPos = false;

#define OUT 1
#define IN 2
#define STOP 3


String data ;
String cmd ;
int lcmd = IN;

int Speed = 500;  //700 OK  400 OK

float tast;

void setup() {
  // set the speed at 60 rpm:
  tast=analogRead(A1);
  actionSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
 
  limitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds

  stepper.setMaxSpeed(Speed);   // set the maximum speed
  stepper.setAcceleration(2000.0); // set acceleration
  stepper.setSpeed(Speed);         // set initial speed
  //stepper.setCurrentPosition(0); // set position
  targetPos = direction * MAX_POSITION;
  
  if ( tast >= 100) {
  
  stepper.moveTo(targetPos);
  //stepper.setCurrentPosition(0); // set position
  StartPos = false;
  Serial.println("Reset");
  } 
  //stepper.moveTo(MAX_POSITION);
  // initialize the serial port
  if (DEBUG){
  Serial.begin(9600);
  Serial.println("R2-Stepper-90mm-dome-02.ino");
  Serial.print("Tast");
  Serial.println(tast);
  Serial.print("Position Start ");
  Serial.println(StartPos);
  } 
}

void readCom(){
  
  if(Serial.available() > 0)
    {
        data = Serial.readStringUntil('\n');
        if (DEBUG){
            Serial.print (F( "I received from COM Serial: "));
            Serial.println(data);
           
        }
        Move(data);
        data = "";
        Serial.flush();
    } // end serial
}

void Move(String cmd) {

  //Serial.print("Data---");
  //Serial.print(stepsPerRevolution);
  //Serial.print(way);
  //Serial.println(cmd);
  
  if (cmd == "in") {  
  Serial.println("clockwise");
  StartPos = false;
  stepper.moveTo(targetPos);
  lcmd = IN;
  
  }
  if (cmd == "out") {
  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  stepper.moveTo(-targetPos);
  lcmd = OUT;
  }

  if (cmd == "stop") {
  // step one revolution in the other direction:
  Serial.println("Stop");
  stepper.setCurrentPosition(0); // set position
  lcmd = STOP;
  }

  cmd = "";
 
}


void loop() {
 
  readCom();

  limitSwitch.loop(); // MUST call the loop() function first
  actionSwitch.loop(); // MUST call the loop() function first

  if (actionSwitch.isPressed()) {
    Serial.println("Action");
       Serial.print("Startpunkt");
         Serial.println(lcmd);
         switch (lcmd){
            case (IN):
            Serial.println("Move Out");
            Move("out");
            break;
            case (OUT):
            Serial.println("Move IN");
            Move("in");
            break;
            case (STOP):
            Serial.println("Move IN");
            Move("in");
            break;

            default:
            Serial.println("Move IN default");
            Move("in");

            break;
          
         }
    
  }

  if (StartPos == false) {

  if (limitSwitch.isPressed()) {
    //Serial.println(F("The limit switch: TOUCHED"));
   
    stepper.setCurrentPosition(0); // set position
    //stepper.moveTo(targetPos);

    //Serial.print("position ");
    //Serial.println(targetPos);

    if (targetPos == MAX_POSITION){
      Serial.print("Startpunkt");
      StartPos = true;
    }
   
  }

  } //find default Pos
  // without this part, the move will stop after reaching maximum position
  /*
  if (stepper.distanceToGo() == 0) { // if motor moved to the maximum position
    stepper.setCurrentPosition(0);   // reset position to 0
    stepper.moveTo(targetPos);       // move the motor to maximum position again
  }
  */

  stepper.run(); // MUST be called in loop() function

  //Serial.print("position ");
  //Serial.println(targetPos);
  
}
