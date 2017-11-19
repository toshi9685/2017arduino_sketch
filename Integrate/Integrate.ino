#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <LSM303.h>

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;

#define SPEED 150 // Zumoのモータに与える回転力の基準値 

float red_G, green_G, blue_G; // カラーセンサーで読み取ったRGB値（0-255）
int zoneNumber_G; // ゾーン番号を表す状態変数
int mode_G; // 各ゾーンでのモードを表す状態変数
unsigned long timeInit_G, timeNow_G, timeZonestart_G; //  スタート時間，経過時間
int motorR_G, motorL_G;  // 左右のZumoのモータに与える回転力unsigned long timePrev = 0;
float direction_G;//座標
int c_G; // -32768~32767の範囲の整数(2バイト)
unsigned int b_G; // 0~65535の範囲の整数(2バイト)

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  button.waitForButton();
  setupColorSensor(); // カラーセンサーのsetup
  calibrationColorSensorWhite(); // カラーセンサーのキャリブレーション
  button.waitForButton();
  calibrationColorSensorBlack(); // カラーセンサーのキャリブレーション
  //calibrationColorSensorManual(); // カラーセンサーのキャリブレーション（手動設定）

  setupCompass(); // 地磁気センサーのsetup
  calibrationCompass(); // 地磁気センサーのキャリブレーション
  // calibrationCompassManual(); // 地磁気センサーのキャリブレーション（手動設定）

  zoneNumber_G = 1;
  mode_G = 3; // スタート時はzoneToZoneの途中から
  
  button.waitForButton();
  timeInit_G = millis();
  timeZonestart_G = millis();
}

void loop()
{
  readRGB(); // カラーセンサでRGB値を取得(0-255)
  direction_G = averageHeadingLP();//現在位置 12.2課題1
  timeNow_G = millis() - timeInit_G; // 経過時間
  motors.setSpeeds(motorL_G, motorR_G); // 左右モーターへの回転力入力
  //motors.setSpeeds(0,0);//左右のモーターの回転を止める
  sendData(); // データ送信

  if ( button.isPressed() ) { // Zumo button が押されていればtrue，そうでなければ false
    zoneNumber_G = 0; // zoneToZone に移行
    mode_G = 0;
    timeZonestart_G = millis();
    delay(100);
  }

  timerCount();//タイマー60s
  
  switch ( zoneNumber_G ) {
    case 0:
      zoneToZone(); // zone to zone (start to zone)
      break;
    case 1:
      zone_linetrace(); // zone 1
      break;
    case 2:
      zone(); // zone 2
      break;
    case 3:
      zone3beta(); // zone 3 山登り
      break;
    case 4:
      motorL_G = 0; motorR_G = 0;
      //zone(); // winning action
      break;
    default:
      break;
  }
  
}
//タイマー60sカウント
void timerCount() {
  if (timeNow_G - (timeZonestart_G - timeInit_G) >= 50000) { //60s経過
    zoneNumber_G = 0;//ゾーンナンバーを0にして次のエリアへ移動させる
    mode_G = 0;
    timeZonestart_G = millis();
    delay(100);
  }
}
// 通信方式２
void sendData()
{
  static unsigned long timePrev = 0;
  static int inByte = 0;

  // 50msごとにデータ送信（通信方式２），500msデータ送信要求なし-->データ送信．
  if ( inByte == 0 || timeNow_G - timePrev > 500 || (Serial.available() > 0 && timeNow_G - timePrev > 50)) { // 50msごとにデータ送信
    inByte = Serial.read();
    inByte = 1;

    Serial.write('H');
    /*
    Serial.write(dataR_max >> 8);
    Serial.write(dataR_max & 255);
    Serial.write(dataG_max >> 8);
    Serial.write(dataG_max & 255);
    Serial.write(dataB_max >> 8);
    Serial.write(dataB_max & 255);
    Serial.write(dataR_min >> 8);
    Serial.write(dataR_min & 255);
    Serial.write(dataG_min >> 8);
    Serial.write(dataG_min & 255);
    Serial.write(dataB_min >> 8);
    Serial.write(dataB_min & 255);
    */
    //sendcolor();
    Serial.write(zoneNumber_G);
    Serial.write(mode_G);
    Serial.write((int)red_G );
    Serial.write((int)green_G );
    Serial.write((int)blue_G );
    
    b_G = direction_G;
    c_G = direction_G;
    Serial.write(b_G >> 8); // 値の範囲が0~65535の場合
    Serial.write(b_G & 255);
    Serial.write(c_G >> 8); // 値の範囲が-32768~32767の場合
    Serial.write(c_G & 255);
    Serial.write((int)(100 * direction_G) >> 8); // 100倍して整数化(小数点2位まで送信)
    Serial.write((int)(100 * direction_G) & 255); // 整数化した時の値が-32768~32767の範囲
    timePrev = timeNow_G;
  }
}




