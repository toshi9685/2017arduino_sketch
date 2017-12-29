const int trig = 5;
const int echo = 4;
unsigned long interval;

void setupDistance() {
  pinMode(trig , OUTPUT);
  pinMode(echo , INPUT);
}

void getDistance() {
  static unsigned long timeDistance_G = 0; //  距離計測を行ったタイミングを保持

  if (timeNow_G - timeDistance_G > 60) {
    digitalWrite(trig , HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    interval = pulseIn(echo, HIGH, 23068); //Echo 信号がHIGH である時間(μs)を計測
    distance = 340 * interval / 10000 / 2; //距離(cm)に変換
    timeDistance_G = millis() - timeInit_G; // 経過時間
  }
}

