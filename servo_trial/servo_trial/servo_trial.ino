#include <Wire.h>

#include <Servo.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads; 


Servo servo;    // Create an instance of the Servo library

int flexThreshold = 250;    // Adjust this value according to your flex sensor readings
int servoAngle = 10;    // The angle to rotate the servo when flexing is detected
int servoCenter = 90;    // The mean position of the servo
int servoRange = 90;    // The range of servo motion from the center position
int ambient0,dif0;

bool servoForward = true;    // Flag to track the servo direction


void setup() {
  Serial.begin(115200);
  Wire.begin();
  ads.begin();

  servo.attach(D0);    // Attach the servo to pin D0
  servo.write(servoCenter);    // Move the servo to the center position
  ambient0 = ads.readADC_SingleEnded(0);
  delay(500);

  Serial.println("Setup completed!");
}

void loop() {
  int flexValue = ads.readADC_SingleEnded(0);
  dif0 = flexValue-ambient0;

  if (dif0 > flexThreshold) {
    // Flexing detected, rotate servo by servoAngle in the appropriate direction
    int newServoPosition = servo.read();
    if (servoForward) {
      newServoPosition += servoAngle;
    } else {
      newServoPosition -= servoAngle;
    }

    if (newServoPosition >= (servoCenter - servoRange) && newServoPosition <= (servoCenter + servoRange)) {
      // Check if the servo is within the allowed range
      servo.write(newServoPosition);
      Serial.print("Servo angle: ");
      Serial.println(newServoPosition);

      delay(300);  // Delay to allow servo to reach the desired position
    }
  }
 
  
  if (servo.read() >= (servoCenter + servoRange)) {
    // Servo reached the maximum range, reverse direction
    servoForward = false;
    Serial.println("Servo reached maximum range. Reversing direction.");
  } else if (servo.read() <= (servoCenter - servoRange)) {
    // Servo reached the minimum range, reverse direction
    servoForward = true;
    Serial.println("Servo reached minimum range. Reversing direction.");
  }

  delay(100);
}
