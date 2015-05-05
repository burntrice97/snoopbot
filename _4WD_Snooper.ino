/*
 * Project: Snoopy
 */
 
const int speed = 80; // percent of maximum speed

#include <AFMotor.h>  // adafruit motor shield library (modified my mm)
AF_DCMotor Motor_Left_Front(4, MOTOR34_1KHZ);   // Motor 4 
AF_DCMotor Motor_Right_Front(3, MOTOR34_1KHZ);  // Motor 3
AF_DCMotor Motor_Left_Rear(1, MOTOR12_1KHZ);    // Motor 1 
AF_DCMotor Motor_Right_Rear(2, MOTOR12_1KHZ);   // Motor 2

int pwm;

#include <Servo.h>

//servo and sensor
Servo myservo;
#define trigPin 6
#define echoPin 5

void setup()
{
  //initializes serial
  Serial.begin(9600);
   
  // scale percent into pwm range (0-255) 
  pwm= map(speed, 0,100, 0,255);  
  Motor_Left_Front.setSpeed(pwm);
  Motor_Right_Front.setSpeed(pwm);
  Motor_Left_Rear.setSpeed(pwm);
  Motor_Right_Rear.setSpeed(pwm);
  
  //servo setup
  myservo.attach(9);
  myservo.write(80);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// run over and over
void loop(){ 
  int distance, distL, distR;              //variables to hold distances
 
  delay(700);                              //delay for accurate ping reading
  distance = getDistance();                //grabs initial distance
  
  
  driveForward();                          //drives forward
  
  if(distance < 15){                       //conditions for obsticles within 15cm 
    park();                                //stops the motors
    
    myservo.write(80);                     //angles servo to forward position
    
    //turn Right then scan
    myservo.write(10);                     //angles servo to right position
    delay(700);                            //delay for accurate ping reading
    distR = getDistance();                 //gets distance to the right
    
    //turn Left and scan
    myservo.write(170);                    //angles servo to left position
    delay(700);                            //delay for accurate ping reading
    distL = getDistance();                 //gets distance from the left
    
    
    while(distL < 15 && distR < 15){       //checks if distance to left and right is clear
      unsigned long start = millis();      //initialize starting milliseconds
      while (millis() - start <= 1525) {   //reverses for 1.5 seconds
        driveBackward();
      }
      
      park();                              //stops the motors
      
      //turn Right then scan
      myservo.write(10);                   //angle of the servo arm
      delay (700);                         //delay for accurate ping reading
      distR = getDistance();               //gets distance to the right
    
      //turn Left and scan
      myservo.write(170);                  //angle of the servo arm
      delay (700);                         //delay for accurate ping reading
      distL = getDistance();               //get distance to the left
    }
    
    
    if(distL > distR) {
      unsigned long start = millis();      //initializes starting milliseconds
      while (millis() - start <= 1525) {   //turns left for 1.5 seconds
        turnLeft();
      }
      park();                              //stops the motors
    } else {
      unsigned long start = millis();      //initializes starting milliseconds
      while (millis() - start <= 1525) {   //turns right for 1.5 seconds
        turnRight();
      }
      park();                              //stops the motors
    }
    
    myservo.write(80);                     //reposition sensor
  }
}

int getDistance(){                         //gets distance and converts to centimeters
  int distance;                            //distance variable
  long duration;                           //set as long to prevent return of negative value
  duration = pulseIn(echoPin, HIGH);       //sends ping
  distance = (duration/2) / 29.1;          //converts ping duration to cm
  
  return distance;                         //returns distance in centimeters
}


void driveForward(){                       //tells motors to drive forward
  Motor_Left_Front.run(FORWARD);
  Motor_Left_Rear.run(FORWARD);
  Motor_Right_Front.run(BACKWARD);
  Motor_Right_Rear.run(BACKWARD);
}

void driveBackward(){                      //tells motors to drive backward
  Motor_Left_Front.run(BACKWARD);
  Motor_Left_Rear.run(BACKWARD);
  Motor_Right_Front.run(FORWARD);
  Motor_Right_Rear.run(FORWARD);
}

void turnLeft(){                           //turns left
  Motor_Left_Front.run(BACKWARD);
  Motor_Left_Rear.run(BACKWARD);  
  Motor_Right_Front.run(BACKWARD);
  Motor_Right_Rear.run(BACKWARD);
}

void turnRight(){                          //turns right
  Motor_Left_Front.run(FORWARD);
  Motor_Left_Rear.run(FORWARD);
  Motor_Right_Front.run(FORWARD);
  Motor_Right_Rear.run(FORWARD);
}

void park(){                               //stops the motors
  Motor_Left_Front.run(RELEASE);            
  Motor_Right_Front.run(RELEASE);
  Motor_Left_Rear.run(RELEASE);            
  Motor_Right_Rear.run(RELEASE);
}
