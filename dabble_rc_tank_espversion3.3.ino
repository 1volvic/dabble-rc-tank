#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>
#include <ESP32Servo.h>

//Lights
int red_led=17;
int white_led=16;

//Servo
Servo turret;
int turretServo=21;       //Turret servo digital

//Right motor
int enableRightMotor=32;  //ENA
int rightMotorPin1=25;    //IN1
int rightMotorPin2=26;    //IN2
//Left motor
int enableLeftMotor=33;   //ENB
int leftMotorPin1=27;     //IN3
int leftMotorPin2=14;     //IN4

#define MAX_MOTOR_SPEED 255

int led_mode = 0;
void led_mode_switch() {
  if (led_mode == 0) {
    led_mode = 1;
  } else if (led_mode == 1) {
    led_mode = 0;
  }
  delay(500);
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }
  
  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }
  
  // Without ledcSetup(), just turn enable pins ON/OFF (speed control disabled)
  if (rightMotorSpeed != 0) digitalWrite(enableRightMotor, HIGH);
  else digitalWrite(enableRightMotor, LOW);

  if (leftMotorSpeed != 0) digitalWrite(enableLeftMotor, HIGH);
  else digitalWrite(enableLeftMotor, LOW);
}

void setUpPinModes()
{ 
  pinMode(red_led,OUTPUT);
  pinMode(white_led,OUTPUT);
  pinMode(turretServo,OUTPUT);
  
  pinMode(enableRightMotor,OUTPUT);
  pinMode(rightMotorPin1,OUTPUT);
  pinMode(rightMotorPin2,OUTPUT);
  
  pinMode(enableLeftMotor,OUTPUT);
  pinMode(leftMotorPin1,OUTPUT);
  pinMode(leftMotorPin2,OUTPUT);

  // NO ledcSetup() calls here
  // NO ledcAttachPin() calls either (since no PWM)
  // Just digital outputs for enable pins
  
  // Start motors stopped
  //rotateMotor(0,0); 

  ESP32PWM::allocateTimer(2);
  turret.setPeriodHertz(50);
  turret.attach(turretServo, 500, 2400);    // Attach servo to pin 21
  turret.write(90);
}

void setup()
{
  setUpPinModes();
  Dabble.begin("rc TANK"); 
}

void loop()
{
  delay(1);
  int rightMotorSpeed=0;
  int leftMotorSpeed=0;
  Dabble.processInput();
  if (GamePad.isUpPressed())
  {
    rightMotorSpeed = MAX_MOTOR_SPEED;
    leftMotorSpeed = MAX_MOTOR_SPEED;
  }

  if (GamePad.isDownPressed())
  {
    rightMotorSpeed = -MAX_MOTOR_SPEED;
    leftMotorSpeed = -MAX_MOTOR_SPEED;
  }

  if (GamePad.isLeftPressed())
  {
    rightMotorSpeed = MAX_MOTOR_SPEED;
    leftMotorSpeed = -MAX_MOTOR_SPEED;
  }

  if (GamePad.isRightPressed())
  {
    rightMotorSpeed = -MAX_MOTOR_SPEED;
    leftMotorSpeed = MAX_MOTOR_SPEED;
  }

  rotateMotor(rightMotorSpeed, leftMotorSpeed);

  // lighting code

  if (GamePad.isTrianglePressed())
  {
    led_mode_switch();
  }

  if (led_mode == 0)
  {
   digitalWrite(red_led,LOW);
   digitalWrite(white_led,LOW); 
  }

  if (led_mode == 1)
  {
   digitalWrite(red_led,HIGH);
   digitalWrite(white_led,HIGH); 
  }
  
  

  // servo code

  if (GamePad.isSquarePressed())
  {
    turret.writeMicroseconds(1850); // turn right
  }
  else if (GamePad.isCirclePressed())
  {
    turret.writeMicroseconds(1200);  // turn left
  }
  else
  {
    turret.writeMicroseconds(1500);  // stop
  }
}
