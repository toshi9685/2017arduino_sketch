

#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <LSM303.h>

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;

#define SPEED 140 // Zumoのモータに与える回転力の基準値 

float red_G, green_G, blue_G; // カラーセンサーで読み取ったRGB値（0-255）
int zoneNumber_G; // ゾーン番号を表す状態変数
int mode_G; // 各ゾーンでのモードを表す状態変数
unsigned long timeInit_G, timeNow_G; //  スタート時間，経過時間
static unsigned long time_Zonestart_G = 0;
int motorR_G, motorL_G;  // 左右のZumoのモータに与える回転力
static LSM303::vector<int32_t> direction_V = {
  0, 0, 0
};
int distance_G;
float direction_G;
int nowcolor_G = 2;
static float X = 0;
static float Y = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  setupDistance();
  button.waitForButton();
  setupColorSensor(); // カラーセンサーのsetup
  //calibrationColorSensorWhite(); // カラーセンサーのキャリブレーション
  //button.waitForButton();
  //calibrationColorSensorBlack(); // カラーセンサーのキャリブレーション
  calibrationColorSensorManual(); // カラーセンサーのキャリブレーション（手動設定）

  setupCompass(); // 地磁気センサーのsetup
  calibrationCompass(); // 地磁気センサーのキャリブレーション
  //CalibrationCompassManual(); // 地磁気センサーのキャリブレーション（手動設定）

  zoneNumber_G = 0;
  mode_G = 2; // スタート時はzoneToZoneの途中から
  button.waitForButton();
  timeInit_G = millis();
  time_Zonestart_G = millis();
}

void loop()
{
  readRGB(); // カラーセンサでRGB値を取得(0-255)
  timeNow_G = millis(); // 経過時間
  direction_G = compass_value();
  getDistance();
  position_estimation(motorL_G , motorR_G);

  motors.setSpeeds(motorL_G, motorR_G); // 左右モーターへの回転力入力
  //motors.setSpeeds(0, 0); // 左右モーターへの回転力入力
  sendData(); // データ送信

  if ( button.isPressed() ) { // Zumo button が押されていればtrue，そうでなければ false
    zoneNumber_G = 0; // zoneToZone に移行
    mode_G = 0;
    time_Zonestart_G = millis();
    delay(100);
  }

  timerCount();//タイマー60s

  switch ( zoneNumber_G ) {
    case 0:
      zoneToZone(); // zone to zone (start to zone)
      break;
    case 1:
      zone_figure_trace(); // zone 1
      break;
    case 2:
      zone_curling(); // zone 2
      break;
    case 3:
      zone3beta(); // zone 3
      break;
    case 4:
      winning_action(); // winning action
      break;
    default:
      break;
  }

}

void timerCount() {
  if (timeNow_G - time_Zonestart_G >= 45000 && nowcolor_G == 1 && zoneNumber_G == 3) { //60s経過
    zoneNumber_G = 0;//ゾーンナンバーを0にして次のエリアへ移動させる
    mode_G = 0;
    time_Zonestart_G = timeNow_G;
  }

  if (timeNow_G - time_Zonestart_G >= 50000 && zoneNumber_G == 2) {
    //zoneNumber_G = 0;
    mode_G = 88;
    time_Zonestart_G = timeNow_G;
  }
}


// 通信方式２
void sendData()
{

  static unsigned long timePrev = 0;
  static int inByte = 0;
  unsigned int send_time;


  // 50msごとにデータ送信（通信方式２），500msデータ送信要求なし-->データ送信．
  if ( inByte == 0 || timeNow_G - timePrev > 500 || (Serial.available() > 0 && timeNow_G - timePrev > 50)) { // 50msごとにデータ送信
    inByte = Serial.read();
    inByte = 1;

    send_time = (int)((timeNow_G - time_Zonestart_G));

    Serial.write('H');

    Serial.write((int)red_G );
    Serial.write((int)green_G );
    Serial.write((int)blue_G );

    write2byte((int)direction_V.x);  write2byte((int)direction_V.y);  write2byte((int)direction_V.z);

    write2byte(compass.a.x);  write2byte(compass.a.y);  write2byte(compass.a.z);

    write2byte(motorR_G); write2byte(motorL_G);

    Serial.write(distance_G);

    Serial.write(zoneNumber_G);
    Serial.write(mode_G);

    write2byte(direction_G);

    write2byte(send_time);

    write2byte((int)(X));
    write2byte((int)(Y));

    if (zoneNumber_G == 3)Serial.write(nowcolor_G);
    timePrev = timeNow_G;
  }
}

void write2byte(int x) {
  Serial.write(x >> 8);
  Serial.write(x & 255);
}



