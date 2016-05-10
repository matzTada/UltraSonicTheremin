int LED_PIN[] = {A0, A1, A2, A3, A4, A5, 4, 5, 6, 7};
int state[10];

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 10; i++) {
    pinMode(LED_PIN[i], OUTPUT);
    state[i] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_PIN[i], state[i]);
    state[i] = 1 - state[i];

    delay(50);
  }
}
