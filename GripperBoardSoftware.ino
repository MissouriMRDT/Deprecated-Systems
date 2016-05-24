#include "DualVNH5019MotorShield.h"

 // Default shield Pin map
 // _INA1 = 2;
 // _INB1 = 4;
 // _EN1DIAG1 = 6;
 // _CS1 = A0; 
 // DualVNH5019MotorShield GripperMotor;
 
#define SPEED_INC_DELAY 2

#define BRAKE_DELAY 250
 
#define WATCH_TRIGGER_MILLIS    500     
#define BRAKE_INDUCTIVE_5019    400

#define SPEED_5019_MAX_CCW     -400
#define SPEED_5019_MAX_CW       400

#define SPEED_NEUTRAL_ZENITH    128

#define SPEED_ZENITH_MAX_CCW    0
#define SPEED_ZENITH_MAX_CW     255

DualVNH5019MotorShield GripperMotor;

byte command_speed = 0;
int  motor_speed   = 0;
int  LED_UNO       = 13;

unsigned long last_watch_clear_millis = 0;

////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
 
  pinMode(LED_UNO, OUTPUT); 
  
  GripperMotor.init();
  last_watch_clear_millis = millis();
}//end fnctn

/////////////////////////////////////////////////////////////
void loop()
{
  while(Serial.available())
  {
    command_speed = Serial.read();
     
    if(command_speed == SPEED_NEUTRAL_ZENITH)
    {
      GripperMotor.setM1Brake(BRAKE_INDUCTIVE_5019);
      stopIfFault();
      delay(BRAKE_DELAY);   
    }else{
      
      motor_speed = map( (int)command_speed, SPEED_ZENITH_MAX_CCW, SPEED_ZENITH_MAX_CW, SPEED_5019_MAX_CCW, SPEED_5019_MAX_CW);
      motor_speed = constrain(motor_speed,   SPEED_5019_MAX_CCW,   SPEED_5019_MAX_CW);    

      GripperMotor.setM1Speed(motor_speed); 
      stopIfFault(); 
      delay(SPEED_INC_DELAY);      
    }//end if  
    
    last_watch_clear_millis = millis();
  }//end while
  
  if( (millis() - last_watch_clear_millis ) > WATCH_TRIGGER_MILLIS)
  {
    GripperMotor.setM1Brake(BRAKE_INDUCTIVE_5019);
    stopIfFault();
    delay(BRAKE_DELAY);
  }//end if 
  
}//end loop


///////////////////////////////////////////////////////////////
//todo
void stopIfFault()
{
  if (GripperMotor.getM1Fault())
  {
    while(1)
    {
      digitalWrite(LED_UNO, HIGH);
    }//end while
  }//end if
}//end fnctn
