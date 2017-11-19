
// bang-bang
/*
  void zone_linetrace()
  {
  float lightNow;//現在のRGB値の平均値
  static float lightMin =  83;// ライントレースする線上のlightNowの値.
  static float lightMax = 255;// 白地のlightNowの値.
  static float speed = 80; // パラメーター
  float speedDiff;
  float Kp = 1.8; // パラメーター初期0.5曲がりきれない0.7ややオーバー0.9あと少し
  switch ( mode_G ) {
    case 0:
      mode_G = 1;
    break;
    case 1:
      lightNow = (red_G + green_G + blue_G ) / 3.0;
      if ( lightNow < (lightMin + lightMax) / 2.0 ) // 左回転
        speedDiff = -Kp * speed;
      else // 右回転
        speedDiff = Kp * speed;
      motorL_G = speed + speedDiff;
      motorR_G = speed - speedDiff;
      break;
    default:
      break;
  }
  }
*/
/*
  // p制御によるライントレース
  void zone_linetrace()
  {
  float lightNow;//現在のRGB値の平均値
  static float lightMin =  24;// ライントレースする線上のlightNowの値.
  static float lightMax = 255;// 白地のlightNowの値.
  static float speed = 100; // パラメーター 100 or 170
  float speedDiff;
  float Kp = 2.4; // パラメーター初期1.8 or 1.2 or 2.4
  switch ( mode_G ) {
    case 0:
      mode_G = 1;
    break;
    case 1:
      if(identifyColor( 183, 40, 24 )){
        mode_G = 2;
      }
      motorL_G = 80;
      motorR_G = 80;
    break;
    case 2:
      if(identifyColor(255,255,255)){
        mode_G = 3;
      }
      motorL_G = -80;
      motorR_G = 80;
    break;
    case 3:
      //lightNow = (red_G + green_G + blue_G ) / 3.0;
      lightNow = blue_G;
      speedDiff = map(lightNow,lightMin,lightMax,-Kp * speed,Kp * speed);
      motorL_G = speed + speedDiff;
      motorR_G = speed - speedDiff;
      if(identifyColor(79,36,73)){
        mode_G = 4;
      }
      break;
      case 4:
        motorL_G = 80;
        motorR_G = 80;
        if(identifyColor(84, 179, 89)){
          mode_G = 5;
        }
      break;
      case 5:
        motorL_G = -100;
        motorR_G = 100;
        if(steadyState(1000) == 1){
          mode_G = 6;
        }
      break;
      case 6:
       zoneNumber_G = 0;
       mode_G = 0;
      break;
    default:
      break;
  }
  }
*/
// p制御によるライントレース(発展課題)
void zone_linetrace()
{
  float lightNow;//現在のRGB値の平均値
  static float lightMin =  24;// ライントレースする線上のlightNowの値.
  static float lightMax = 255;// 白地のlightNowの値.
  static float speed = 150; // パラメーター 100 or 170
  float speedDiff;
  float Kp = 2.4; // パラメーター初期1.8 or 1.2 or 2.4
  switch ( mode_G ) {
    case 0://セットアップ
      mode_G = 1;
      break;
    case 1://赤を見つけるまで前進
      if (identifyColor( 183, 40, 24 )) {
        mode_G = 2;
      }
      motorL_G = 80;
      motorR_G = 80;
      break;
    case 2://白を見つけるまで左回転
      if (identifyColor(255, 255, 255)) {
        mode_G = 3;
      }
      motorL_G = -80;
      motorR_G = 80;
      break;
    case 3://ライントレース
      //lightNow = (red_G + green_G + blue_G ) / 3.0;
      lightNow = green_G;
      speedDiff = map(lightNow, lightMin, lightMax, -Kp * speed, Kp * speed);
      if ( speedDiff < 0 ) {
        motorL_G = speed;
        motorR_G = speed + speedDiff;
      } else {
        motorL_G = speed - speedDiff;
        motorR_G = speed;
      }
      if (identifyColor(79, 36, 73)) {//終点を見つけたら終わり
        mode_G = 4;
      }
      break;
    case 4://緑を見つけるまで前進
      motorL_G = 80;
      motorR_G = 80;
      if (identifyColor(84, 179, 89)) {
        mode_G = 5;
      }
      break;
    case 5://一秒間左回転
      motorL_G = -100;
      motorR_G = 100;
      if (steadyState(1000) == 1) {
        mode_G = 6;
      }
      break;
    case 6://zoneTozoneへ
      zoneNumber_G = 0;
      mode_G = 0;
      break;
    default:
      break;
  }
}



