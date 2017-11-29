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
float direction_G;//方角
float avex_G;


void setup()
{
  Serial.begin(9600);
  Wire.begin();

  button.waitForButton();
  setupColorSensor(); // カラーセンサーのsetup
  //calibrationColorSensorWhite(); // カラーセンサーのキャリブレーション
  //button.waitForButton();
  //calibrationColorSensorBlack(); // カラーセンサーのキャリブレーション
  calibrationColorSensorManual(); // カラーセンサーのキャリブレーション（手動設定）

  setupCompass(); // 地磁気センサーのsetup
  calibrationCompass(); // 地磁気センサーのキャリブレーション
  // calibrationCompassManual(); // 地磁気センサーのキャリブレーション（手動設定）

  zoneNumber_G = 0;
  mode_G = 2; // スタート時はzoneToZoneの途中から
  
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
  sendData(); // データ送信

  if ( button.isPressed() ) { // Zumo button が押されていればtrue，そうでなければ false
    zoneNumber_G = 0; // zoneToZone に移行
    mode_G = 0;
    timeZonestart_G = millis();
    delay(100);
  }

  //timerCount();//タイマー60s
  
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

void write2byte(int x) {
  Serial.write(x>>8);
  Serial.write(x&255);
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
    float mx,my,mz;
    
    Serial.write('H');
    Serial.write((int)red_G );
    Serial.write((int)green_G );
    Serial.write((int)blue_G );
    mx = map(compass.m.x,compass.m_min.x,compass.m_max.x,-100,100);
    my = map(compass.m.y,compass.m_min.y,compass.m_max.y,-100,100);
    mz = map(compass.m.z,compass.m_min.z,compass.m_max.z,-100,100);
    write2byte((int)mx);  
    write2byte((int)my);  
    write2byte((int)mz);
    write2byte(compass.a.x);  
    write2byte(compass.a.y);  
    write2byte(compass.a.z);
    write2byte(motorR_G);
    write2byte(motorL_G);
    Serial.write(0);
    Serial.write(zoneNumber_G);
    Serial.write(mode_G);
    write2byte((int)(100 * direction_G));
    if(zoneNumber_G == 3)write2byte(avex_G);
    timePrev = timeNow_G;
  }
}




