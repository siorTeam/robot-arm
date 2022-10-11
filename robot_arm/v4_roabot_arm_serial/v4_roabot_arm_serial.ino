#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
//라이브러리

#define BT_TX 2 //Bluetooth Transmit
#define BT_RX 3 //Bluetooth Receive
SoftwareSerial BT(BT_TX, BT_RX); // Arduino - RX, TX 아두이노랑 블루투스 TX-RX, RX-TX
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

//버퍼 설정
#define BUF_MAX 128 //버퍼 최대 크기 define
#define COM_SEP ',' //컴마 define
#define COM_END '\n' //줄바꿈 define
char buf[BUF_MAX]; //버퍼의 버퍼 맥스 할당함 command
int idx = 0; //인덱스 설정
/*추후에 버퍼에 인덱스 번호에 맞는 문자 추가해줄 예정
   컴퓨터 또는 블루투스 시리얼로 문자(0,180)가 들어오면 (0 , 1 8 0) 이렇게 들어옴
   위 문자들을 (0,180)라는 문자(문자열은 아니다.)로 바꿔주기 위해 버퍼가 필요함
*/

// 원하는 명령어 (핀번호,각도)  나왔을 때 동작하는 커맨드(channel comma, degree command)
void chnl_com_deg_com() {
  buf[idx] = '\0'; //버퍼가 형식 만족해서 끝났을 때
  idx = 0; //인덱스 번호 0으로 재설정
  String com_raw(buf); //문자들을 문자로 바꿈

  int comma = com_raw.indexOf(COM_SEP); // , 위치를 찾겠다
  int len = com_raw.length(); // \0 (종결 신호) 까지의 문자의 전체 길이 인식

  int channel = com_raw.substring(0, comma).toInt(); //채널번호는 0번부터 , 전까지
  int deg = com_raw.substring(comma + 1, len).toInt(); // 각도(deg)는 ,다음부터 \0 전까지

  PWM.setPWM(channel, 0, deg2pul(deg)); // 서보모터 동작 함수 (채널번호, 0(뭔지 모름), 각도->펄스)

  // **** DEBUG ****
  Serial.print('(');
  Serial.print(channel);
  Serial.print(',');
  Serial.print(deg);
  Serial.println(')');
  delay(10);
  // **** DEBUG ****

}


void setup() {
  Serial.begin(9600); // 시리얼 9600포트
  BT.begin(9600); // 블루투스 9600포트
  PWM.begin(); // ?
  PWM.setPWMFreq(SERVO_FREQ); // 진동수 설정

  // **** DEBUG ****
  Serial.println("Ready!"); // 작동 준비 되면 시리얼에 나옴
  // **** DEBUG ****

}

void loop() {
  if (Serial.available()) { // 시리얼에서 읽을 수 있으면 (=문자 있으면)
    char a = Serial.read(); // 문자 하나하나씩 읽는다 (예: 0 , 1 8 0)
    if (idx == BUF_MAX || a == COM_END) chnl_com_deg_com(); // 인덱스 값이 버퍼의 최댓값 초과시
    else buf[idx] = a; //버퍼 맥스가 아니라면 버퍼의 인덱스 번호에 값 추가하고 인덱스 값 1 추가
    idx += 1;
  }
}

/*
  void loop() {
   if (BT.available()) { // 블루투스에서 읽을 수 있으면 (=문자 있으면)
      char A = BT.read(); // 문자 하나하나씩 읽는다 (예: 0 , 1 8 0)
      if(idx == BUF_MAX || A == COM_END) chnl_com_deg_com(); // 인덱스 값이 버퍼의 최댓값 초과시
      else buf[idx] = A; //버퍼 맥스가 아니라면 버퍼의 인덱스 번호에 값 추가하고 인덱스 값 1 추가
      idx+=1;
   }

   // **** DEBUG ****
   if (Serial.available()) {
      BT.write(Serial.read());
   }
   // **** DEBUG ****
  }
*/
