/**
 * @file code-joystick.ino
 * @author lucusowl, robot-arm-team, siorTeam
 * @version 1.1
 * @date 2022-12-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
// http://adafruit.github.io/Adafruit-PWM-Servo-Driver-Library/html
#include <Adafruit_PWMServoDriver.h>

#define ARM_SH0     0  // 어깨 회전
#define ARM_SH1     1  // 어깨 상하
#define ARM_SH2     2  // 팔꿈치 상하
#define ARM_EL      3  // 손목 상하
#define ARM_WR      4  // 손목 회전
#define ARM_GR      5  // 손

#define ARM_SH_0    A0 // 1번 좌우 -> 어깨 회전
#define ARM_SH_1    A1 // 1번 상하 -> 어깨 상하
#define ARM_SH_2    A2 // 2번 좌우 -> 팔꿈치 상하
#define ARM_SH_3    A3 // 2번 상하 -> 손목 상하
#define ARM_SH_4    A6 // 3번 좌우 -> 손목 회전
#define ARM_SH_5    A7 // 3번 상하 -> 손

#define GR_VELOC    8  // 잡는 각도 단위
#define VELOC       2  // 관절 이동 속도

#define SERVOMIN    100 // MG996R   0도
#define SERVOMAX    620 // MG996R 180도
#define SERVO_FREQ  60  // MG996R signal frequency

// | pin-name | min ~ max | ini | right |
// +----------+-----------+-----+-------+
// |     0pin |   0 ~ 180 |  90 |    90 |
// |     1pin |   0 ~ 180 |  30 |    90 |
// |     2pin |   0 ~ 180 |  90 |    90 |
// |     3pin |   0 ~ 180 |  45 |    90 |
// |     4pin |   0 ~ 180 | 110 |   180 |
// |     5pin |   0 ~  90 |   0 |    45 |
// 현재 각도 Current Angle
int _arm[6] = {0,0,0,0,0,0};

Adafruit_PWMServoDriver servo_driver = Adafruit_PWMServoDriver();

// Fix & Convert range of pulse to SERVOMIN ~ SERVOMAX
// ang : (int) 타겟 각도값
int ang2pul(int ang) {
  return constrain(map(ang, 0, 180, SERVOMIN, SERVOMAX), SERVOMIN, SERVOMAX);
}

// 각도변화 설정
// ch  : (int) 모터 채널 id
// vel : (int) 각도 변화량
// return : (int) 성공여부(성공시 0, 실패시 1)
int writeRotate(int ch, int vel){
  int flag = true;
  int ang = _arm[ch] + vel;
  // 범위내의 각도인지 확인
  switch(ch){
    case 0:if( 0<=ang && ang<=180 ){flag=false;}break;
    case 1:if( 0<=ang && ang<=180 ){flag=false;}break;
    case 2:if( 0<=ang && ang<=180 ){flag=false;}break;
    case 3:if( 0<=ang && ang<=180 ){flag=false;}break;
    case 4:if( 0<=ang && ang<=180 ){flag=false;}break;
    case 5:if( 0<=ang && ang<= 90 ){flag=false;}break;
  }
  if(flag) return 1;
  // 문제가 없을 경우, 모터 각도를 해당값으로 설정
  servo_driver.setPWM(ch, 0, ang2pul(ang));
  _arm[ch] = ang;
  return 0;
}

void setup() {
  Serial.begin(9600);
  servo_driver.begin();
  servo_driver.setPWMFreq(SERVO_FREQ); // set freq

  // 초기설정
  writeRotate(0,  90);
  writeRotate(1,  30);
  writeRotate(2,  90);
  writeRotate(3,  45);
  writeRotate(4, 110);
  writeRotate(5,   0);
}

void loop() {
  int sig;
  int vel;

  sig = analogRead(ARM_SH_0);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(0, vel);

  sig = analogRead(ARM_SH_1);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(1, vel);

  sig = analogRead(ARM_SH_2);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(2, vel);

  sig = analogRead(ARM_SH_3);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(3, vel);

  sig = analogRead(ARM_SH_4);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(4, vel);

  sig = analogRead(ARM_SH_5);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(5, vel);

  delay(50);
}
