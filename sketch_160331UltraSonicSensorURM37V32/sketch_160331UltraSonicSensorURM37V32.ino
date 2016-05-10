/*
 * このページを参考にやっていきましょう．
 * URM37v3.2はシリアルでコマンドを送るとそれに答えて結果を返してくれるそうなのでこの機能を使ってまいります．
 * http://www.dfrobot.com/wiki/index.php/URM37_V3.2_Ultrasonic_Sensor_(SKU:SEN0001)#Introduction
 * */

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

long pastMillis = millis();

//command
//{Command, value-High, value-Low, Checksum}
//checksum = low 8bit of the sum of command + data0 + data1
char READTEMP[] = {0x11, 0x00, 0x00, 0x11};
char READDIST[] = {0x22, 0x00, 0x00, 0x22};

int distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);

  for (int i = 0; i < 4; i++) {
    mySerial.write(READDIST[i]);
  }
}

void loop() {

//  if (millis() - pastMillis > 1000) {
//    pastMillis = millis();
//    for (int i = 0; i < 4; i++) {
//      mySerial.write(READDIST[i]);
//    }
//    //    mySerial.write('\n');
//  }

  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

  if (mySerial.available()) {
    char tmp = mySerial.read();
    Serial.print(tmp, HEX);
    if (tmp == 0x11 || tmp == 0x22) {
      int count = 0;
      char vh, vl, checksum;
      while (count < 3) {
        if (mySerial.available()) {
          if (count == 0) vh = mySerial.read(); //high value
          else if(count == 1) vl = mySerial.read(); //low value
          else checksum = mySerial.read();
          count++;
        }
      }
      distance = (int)vh * 255 + (int)vl;
      Serial.print(" 0x");
      Serial.print(vh, HEX);
      Serial.print(" 0x");
      Serial.print(vl, HEX);
      Serial.print(" ");
      Serial.print(distance);
      Serial.println("");

      for (int i = 0; i < 4; i++) {
        mySerial.write(READDIST[i]);
      }
    }
  }
}
