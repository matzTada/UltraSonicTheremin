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

int LED_PIN[] = {A0, A1, A2, A3, A4, A5, 4, 5, 6, 7};
int state[10];

#define SPEAKER_MODE_PIN 9
#define SPEAKER_BUTTON_PIN 10

#define SPEAKER_PIN 11
#define C 262
#define D 294
#define E 330
#define F 349
#define G 392
#define A 440
#define B 494
#define C2 523
#define REST 100


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);

  for (int i = 0; i < 4; i++) {
    mySerial.write(READDIST[i]);
  }

  for (int i = 0; i < 10; i++) {
    pinMode(LED_PIN[i], OUTPUT);
    state[i] = 0;
  }

  pinMode(SPEAKER_MODE_PIN, INPUT);
  pinMode(SPEAKER_BUTTON_PIN, INPUT);
}

void loop() {
  if (millis() - pastMillis > 20) {
    pastMillis = millis();
    for (int i = 0; i < 4; i++) {
      mySerial.write(READDIST[i]);
    }

    //    mySerial.write('\n');
  }

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
          else if (count == 1) vl = mySerial.read(); //low value
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

      //      for (int i = 0; i < 4; i++) {
      //        mySerial.write(READDIST[i]);
      //      }
    }
  }

  //LED line function
  for (int i = 0; i < 10; i++) {
    if (i < distance / 5) digitalWrite(LED_PIN[i], HIGH);
    else digitalWrite(LED_PIN[i], LOW);
  }

  Serial.println(digitalRead(SPEAKER_BUTTON_PIN));
  if (digitalRead(SPEAKER_BUTTON_PIN) == HIGH) {
    if (digitalRead(SPEAKER_MODE_PIN) == HIGH) {
      //piezzo sound
      int sound = (((float)distance) / 25.0) * 440.0;
      tone(SPEAKER_PIN, sound);
      Serial.println(sound);
    } else {
      //piezzo sound separated
      if (distance < 10) {
        tone(SPEAKER_PIN, C);
        Serial.println("C");
      } else if (10 <= distance && distance < 15) {
        tone(SPEAKER_PIN, D);
        Serial.println("D");
      } else if (15 <= distance && distance < 20) {
        tone(SPEAKER_PIN, E);
        Serial.println("E");
      } else if (20 <= distance && distance < 25) {
        tone(SPEAKER_PIN, F);
        Serial.println("F");
      } else if (25 <= distance && distance < 30) {
        tone(SPEAKER_PIN, G);
        Serial.println("G");
      } else if (30 <= distance && distance < 35) {
        tone(SPEAKER_PIN, A);
        Serial.println("A");
      } else if (35 <= distance && distance < 40) {
        tone(SPEAKER_PIN, B);
        Serial.println("B");
      } else if (40 <= distance && distance < 45) {
        tone(SPEAKER_PIN, C2);
        Serial.println("C2");
      }
    }
  } else {
    noTone(SPEAKER_PIN);
  }
}
