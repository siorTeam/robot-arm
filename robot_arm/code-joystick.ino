/**
 * @file code-serial.ino
 * @author lucusowl, robot-arm-team, siorTeam
 * @version 0.2
 * @date 2022-11-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Adafruit_PWMServoDriver.h>

#define ARM_SH0         0  // 어깨 회전
#define ARM_SH1         1  // 어깨 상하1
#define ARM_SH2         2  // 팔꿈치 상하
#define ARM_EL          3  // 손목 상하
#define ARM_WR          4  // 손목 회전
#define ARM_GR          5  // 손

#define ARM_SH_0        A0 // 1번 좌우 -> 어깨상하  1
#define ARM_SH_1        A1 // 1번 상하 -> 팔꿈치    2
#define ARM_SH_2        A2 // 2번 좌우 -> 어깨회전  0
#define ARM_SH_3        A3 // 2번 상하 -> 손목상하  3
#define ARM_SH_4        A6 // 3번 좌우 -> 손목회전  4
#define ARM_SH_5        A7 // 3번 상하 -> 손        5

#define GR_VELOC        8  // 잡는 각도 단위
#define VELOC           2  // 관절 이동 속도

#define SERVOMIN 100
#define SERVOMAX 620
#define SERVO_FREQ 60

// 현재 각도 저장
int _arm[6] = {0,0,0,0,0,0};

Adafruit_PWMServoDriver servo_driver = Adafruit_PWMServoDriver();

// fix & convert range of pulse to SERVOMIN ~ SERVOMAX
int ang2pul(int ang) {
  return constrain(map(ang, 0, 180, SERVOMIN, SERVOMAX), SERVOMIN, SERVOMAX);
}

int writeRotate(int ch, int ang, int prev){
  int flag = true;
  switch(ch){
    case 0:if( 0<=ang && ang<=180){flag=false;}break;
    case 1:if( 0<=ang && ang<=180){flag=false;}break;
    case 2:if( 0<=ang && ang<=180){flag=false;}break;
    case 3:if( 0<=ang && ang<=180){flag=false;}break;
    case 4:if( 0<=ang && ang<=180){flag=false;}break;
    case 5:if( 0<=ang && ang<= 90){flag=false;}break;
  }
  if(flag) return 1;

  _arm[ch] = ang;
  servo_driver.setPWM(ch, 0, ang2pul(ang));
  return 0;
}

void setup() {
  Serial.begin(9600);
  servo_driver.begin();
  servo_driver.setPWMFreq(SERVO_FREQ); // set freq

  // | pin-name | min ~ max | ini | right |
  // +----------+-----------+-----+-------+
  // |     0pin |   0 ~ 180 |  90 |    90 |
  // |     1pin |   0 ~ 180 |  90 |    90 |
  // |     2pin |   0 ~ 180 |  90 |    90 |
  // |     3pin |   0 ~ 180 |  90 |    10 |
  // |     4pin |   0 ~ 180 |  90 |   180 |
  // |     5pin |   0 ~  90 |   0 |    10 |

  // 초기설정
  writeRotate(0,  90, _arm[0]);
  writeRotate(1,  30, _arm[1]);
  writeRotate(2,  90, _arm[2]);
  writeRotate(3,  45, _arm[3]);
  writeRotate(4, 110, _arm[4]);
  writeRotate(5,   0, _arm[5]);
}

void loop() {
  int sig;
  int vel;

  sig = analogRead(ARM_SH_0);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(0, _arm[0]+vel, _arm[0]);
  sig = analogRead(ARM_SH_1);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(1, _arm[1]+vel, _arm[1]);

  sig = analogRead(ARM_SH_2);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(2, _arm[2]+vel, _arm[2]);
  sig = analogRead(ARM_SH_3);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(3, _arm[3]+vel, _arm[3]);

  sig = analogRead(ARM_SH_4);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(4, _arm[4]+vel, _arm[4]);
  sig = analogRead(ARM_SH_5);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(5, _arm[5]+vel, _arm[5]);

  delay(50);
}
