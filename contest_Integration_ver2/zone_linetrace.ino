
// bang-bang
void zone_linetrace()
{
  float lightNow;
  static float lightMin = 18 ;// 各自で設定
  static float lightMax = 255;// 各自で設定
  static float speed = 100; // パラメーター
  float speedDiff;
  float Kp = 2.3; // パラメーター
  int done;

  switch ( mode_G ) {
    case 0:
      mode_G = 1;
      break;
    case 1:
      motorL_G = 70;
      motorR_G = 70;
      if ( identifyColor_wide( 194,  48,  28 , 40) ) { // 赤を検知
        mode_G = 2;
      }
      break;
    case 2:
      motorL_G = -SPEED;
      motorR_G = SPEED;
      if ( identifyColor( 255, 255, 255 ) ) { // 白を検知
        mode_G = 3;
      }
      break;
    case 3:
      lightNow = blue_G;
      speedDiff = map(lightNow , lightMin , lightMax , -Kp * speed , Kp * speed);
      motorL_G = speed + speedDiff;
      motorR_G = speed - speedDiff;
      if ( identifyColor( 92, 41, 72 ) ) { // 赤と青の混合色を検知
        mode_G = 4;
      }
      break;
    case 4:
      motorL_G = 70;
      motorR_G = 70;
      if ( identifyColor_wide(  82, 169,  87 , 40) ) { // 緑を検知
        mode_G = 5;
      }
      break;
    case 5:
      motorL_G = -SPEED;
      motorR_G = SPEED;
      done = steadyState(800);
      if ( done == 1 )
        mode_G = 6;
      break;
    case 6:
      zoneNumber_G = 0;     // 状態変数の更新
      mode_G = 0;           // 状態変数の更新
      break;
    default:
      break;
  }
}



