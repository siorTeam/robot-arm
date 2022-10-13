/**
 * @file code-serial.ino
 * @author lucusowl, robot-arm-team, siorTeam
 * @version 0.1
 * @date 2022-10-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Adafruit_PWMServoDriver.h>

#define ARM_SH0         0  // 어깨 회전
#define ARM_SH1         1  // 어깨 상하1
#define ARM_SH2         2  // 어깨 상하2
#define ARM_EL          3  // 팔꿈치 상하
#define ARM_WR          4  // 손목 상하
#define ARM_GR          5  // gripper
#define ARM_SH_INPUT_0  A0 // 1번 좌우 -> 어깨 회전(ARM_SH0)
#define ARM_SH_INPUT_1  A1 // 1번 상하 -> 어깨 상하(ARM_SH1, ARM_SH2)
#define ARM_EL_INPUT    A2 // 2번 좌우 -> 팔꿈치 상하(ARM_EL)
#define ARM_WR_INPUT    A3 // 2번 상하 -> 손목 상하(ARM_WR)
#define ARM_GR_CATCH    8  // 1번 버튼 -> gripper catch button
#define ARM_GR_RELEASE  7  // 2번 버튼 -> gripper release button
#define GR_VELOC        8  // 잡는 각도 단위
#define VELOC           4  // 관절 이동 속도

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
    case 3:if( 0<=ang && ang<=140){flag=false;}break;
    case 4:if(65<=ang && ang<=180){flag=false;}break;
    case 5:if( 0<=ang && ang<=180){flag=false;}break;
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
  pinMode(ARM_GR_CATCH, INPUT_PULLUP);
  pinMode(ARM_GR_RELEASE, INPUT_PULLUP);

  // | pin-name | min ~ max | ini | right |
  // +----------+-----------+-----+-------+
  // |     0pin |   0 ~ 180 |  90 |    90 |
  // | 1 - 2pin |   0 ~ 180 | 135 |    90 |
  // |     3pin |   0 ~ 140 |  90 |    10 |
  // |     4pin |  65 ~ 180 |  90 |   180 |
  // |     5pin |   0 ~ 180 |   0 |    10 |

  // 초기설정
  writeRotate(0, 90, _arm[0]);
  writeRotate(1, 135, _arm[1]);
  writeRotate(2, 180-135, _arm[2]);
  writeRotate(3, 90, _arm[3]);
  writeRotate(4, 90, _arm[4]);
  writeRotate(5, 10, _arm[5]);
}

void loop() {
  int sig;
  int vel;

  sig = analogRead(ARM_SH_INPUT_0);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(0, _arm[0]+vel, _arm[0]);
  sig = analogRead(ARM_SH_INPUT_1);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(1, _arm[1]+vel, _arm[1]);
  writeRotate(2, 180-(_arm[2]+vel), _arm[2]);
  sig = analogRead(ARM_EL_INPUT);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(3, _arm[3]+vel, _arm[3]);
  sig = analogRead(ARM_WR_INPUT);
  vel = map(sig, 0, 1024, -VELOC, VELOC+1);
  writeRotate(4, _arm[4]+vel, _arm[4]);

  if(LOW == digitalRead(ARM_GR_CATCH)){
    if(LOW != digitalRead(ARM_GR_RELEASE)){
      writeRotate(5, _arm[5]+GR_VELOC, _arm[5]); // catch
    }
  }
  else if(LOW == digitalRead(ARM_GR_RELEASE)){
    writeRotate(5, _arm[5]-GR_VELOC, _arm[5]); // release
  }
  
  delay(50);
}
