#include <Servo.h>
#include <IRremote.h>

#define IR_RECEIVE_PIN 3

Servo servo;
int servopin = 4;

int redled = 8;
int greenled = 7;

int motor1_IN1 = 9;
int motor1_IN2 = 2;
int motor1_EN = 6;

int motor2_IN1 = 11;
int motor2_IN2 = 4;
int motor2_EN = 5;

bool systemeOn = false;
int degres = 90;
int speed = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("=== Initialisation ===");
  
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  
  pinMode(motor1_IN1, OUTPUT);
  pinMode(motor1_IN2, OUTPUT);
  pinMode(motor1_EN, OUTPUT);
  
  pinMode(motor2_IN1, OUTPUT);
  pinMode(motor2_IN2, OUTPUT);
  pinMode(motor2_EN, OUTPUT);
  
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  
  digitalWrite(redled, HIGH);
  digitalWrite(greenled, LOW);
  
  servo.attach(servopin);
  servo.write(degres);
  
  stopMotors();
  
  delay(1500);
  Serial.println("=== Pret a l'utilisation ===");
}

void loop() {
  if (IrReceiver.decode()) {
    uint32_t code = IrReceiver.decodedIRData.command;
    Serial.print("Code recu : 0x");
    Serial.println(code, HEX);
    
    if (code == 0x0) {
      systemeOn = !systemeOn;
      degres = 90;
      servo.write(degres);
      speed = 0;
      stopMotors();
      
      if (systemeOn) {
        digitalWrite(redled, LOW);
        digitalWrite(greenled, HIGH);
        Serial.println("=== Systeme ON ===");
      } else {
        digitalWrite(greenled, LOW);
        digitalWrite(redled, HIGH);
        Serial.println("=== Systeme OFF ===");
      }
    }
    
    if (systemeOn) {
      if (code == 0x14) {
        degres = constrain(degres + 15, 40, 140);
        Serial.print("left : ");
        Serial.println(degres);
        servo.write(degres);
      } 
      else if (code == 0x16) {
        degres = constrain(degres - 15, 40, 140);
        Serial.print("right : ");
        Serial.println(degres);
        servo.write(degres);
      }
      
      if (code == 0x11) {
        speed = constrain(speed + 10, 0, 255);
        Serial.print("speed up: ");
        Serial.println(speed);
        moveForward(speed);
      }
      else if (code == 0x19) {
        speed = constrain(speed - 10, 0, 255);
        Serial.print("slow: ");
        Serial.println(speed);
        if (speed > 0) {
          moveForward(speed);
        } else {
          stopMotors();
        }
      }
    }
    
    IrReceiver.resume();
  }
}
 

void moveForward(int motorSpeed) {
  digitalWrite(motor1_IN1, HIGH);
  digitalWrite(motor1_IN2, LOW);
  analogWrite(motor1_EN, motorSpeed);
  
  digitalWrite(motor2_IN1, HIGH);
  digitalWrite(motor2_IN2, LOW);
  analogWrite(motor2_EN, motorSpeed);
}

void moveBackward(int motorSpeed) {
  digitalWrite(motor1_IN1, LOW);
  digitalWrite(motor1_IN2, HIGH);
  analogWrite(motor1_EN, motorSpeed);
  
  digitalWrite(motor2_IN1, LOW);
  digitalWrite(motor2_IN2, HIGH);
  analogWrite(motor2_EN, motorSpeed);
}

void turnLeft(int motorSpeed) {
  digitalWrite(motor1_IN1, LOW);
  digitalWrite(motor1_IN2, HIGH);
  analogWrite(motor1_EN, motorSpeed);
  
  digitalWrite(motor2_IN1, HIGH);
  digitalWrite(motor2_IN2, LOW);
  analogWrite(motor2_EN, motorSpeed);
}

void turnRight(int motorSpeed) {
  digitalWrite(motor1_IN1, HIGH);
  digitalWrite(motor1_IN2, LOW);
  analogWrite(motor1_EN, motorSpeed);
  
  digitalWrite(motor2_IN1, LOW);
  digitalWrite(motor2_IN2, HIGH);
  analogWrite(motor2_EN, motorSpeed);
}

void stopMotors() {
  digitalWrite(motor1_IN1, LOW);
  digitalWrite(motor1_IN2, LOW);
  analogWrite(motor1_EN, 0);
  
  digitalWrite(motor2_IN1, LOW);
  digitalWrite(motor2_IN2, LOW);
  analogWrite(motor2_EN, 0);
  
  speed = 0;
}
