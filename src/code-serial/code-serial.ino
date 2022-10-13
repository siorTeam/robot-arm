/**
 * @file cdoe-serial.ino
 * @author robot-arm-team, siorTeam
 * @version 0.1
 * @date 2022-10-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
//라이브러리

Adafruit_PWMServoDriver PWM = Adafruit_PWMServoDriver(0x40); //PWM 함수 설정

#define SERVOMIN 100 //서보모터 펄스 최솟값 (0도) 실험해서 구함
#define SERVOMAX 620 //서보모터 펄스 최솟값 (180도) 실험해서 구함
#define SERVO_FREQ 60 //통신 진동수 60 (노후되면 60-a)

int deg2pul(int deg) {
  //각도에서 펄스로
  // fix range of deg to 0 ~ 180
  // convert to range of pulse
  return constrain(map(deg, 0, 180, SERVOMIN, SERVOMAX), SERVOMIN, SERVOMAX);
  //각도 들어오면(0~180)-> 펄스로 (100~620)
}

void setup() {
  Serial.begin(9600); // 시리얼 9600포트
  PWM.begin(); // ?
  PWM.setPWMFreq(SERVO_FREQ); // 진동수 설정

  // **** DEBUG ****
  Serial.println("Ready!"); // 작동 준비 되면 시리얼에 나옴
  // **** DEBUG ****

}

void loop() {
  PWM.setPWM(4, 0, deg2pul(10));
}
