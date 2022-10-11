#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/**
 * Bluetooth TX = Arduino D2
 * Bluetooth RX = Arduino D3
 * PCA9685 SDA = Arduino A4
 * PCA9685 SCL = Arduino A5
 */
#define BT_TX 2
#define BT_RX 3
SoftwareSerial BT(BT_TX, BT_RX); // Arduino - RX, TX
Adafruit_PWMServoDriver PWM = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN 100
#define SERVOMAX 620
#define SERVO_FREQ 60
int deg2pul(int deg){
  // fix range of deg to 0 ~ 180
  // convert to range of deg
  return constrain(map(deg, 0, 180, SERVOMIN, SERVOMAX), SERVOMIN, SERVOMAX);
}

#define BUF_MAX 128
#define COM_SEP ','
#define COM_END '\n'
char buf[BUF_MAX]; // command
int idx = 0;

void deg_com(){
   buf[idx] = '\0';
   idx = 0;
   String com_raw(buf);

   int comma = com_raw.indexOf(COM_SEP);
   int len = com_raw.length();

   int channel = com_raw.substring(0,comma).toInt();
   int deg = com_raw.substring(comma+1,len).toInt();

   PWM.setPWM(channel, 0, deg2pul(deg));

   // **** DEBUG ****
   Serial.print('(');
   Serial.print(channel);
   Serial.print('-');
   Serial.print(deg);
   Serial.println(')');
   delay(10);
   // **** DEBUG ****
}




void setup() {
   Serial.begin(9600);
   BT.begin(9600);
   PWM.begin();
   PWM.setPWMFreq(SERVO_FREQ); // set freq

   // **** DEBUG ****
   Serial.println("Ready!");
   // **** DEBUG ****
}

void loop() {
   if (BT.available()) {
      char A = BT.read();
      if(idx == BUF_MAX || A == COM_END) deg_com();
      else buf[idx++] = A;
   }

   // **** DEBUG ****
   if (Serial.available()) {
      BT.write(Serial.read());
   }
   // **** DEBUG ****
}
