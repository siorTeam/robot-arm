#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver PWM = Adafruit_PWMServoDriver(0x40);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  PWM.begin();
  PWM.setPWMFreq(60); // set freq

  // **** DEBUG ****
  Serial.println("Ready!");
  // **** DEBUG ****
}

char buf[128];
int index = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    char a = Serial.read();
    if(a == '\n'){
      buf[index] = '\0';
      int num = atoi(buf);
      Serial.println(num);
      PWM.setPWM(2, 0, num);
      index = 0;
    }
    else{
      buf[index] = a;
      index++;
    }
//    
  }
}
