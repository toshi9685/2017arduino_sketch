void zone_curling()
{
  float lightNow;
  static float blue_lightMin = 20 ;// 各自で設定
  static float blue_lightMax = 255;// 各自で設定
  static float speed = 130; // パラメーター
  float speedDiff;
  float Kp = 0.8; // パラメーター
  int done;
  static int object_count;
  static int distance_Prev;
  float line_Kp = 1.5; // パラメーター
  static int direction_Prev , sum_direction , direction_diff;
  static int direction_now;
  static int turn_direction , turn_count = 1;
  static bool turn_flag = false;
  
  direction_Prev = 0;
  switch ( mode_G ) {
    case 0:
      done = steadyState(500);
      if ( done == 1 ) {
        mode_G = 1;
      }
      break;
    // ストーン探索で左回転
    case 1:
      if (turn_flag == false) {
        motorL_G = -90;
        motorR_G = 90;
      }
      else if (turn_flag == true) {
        motorL_G = 90;
        motorR_G = -90;
      }
      if (distance > 5 && distance < 80) {
        object_count++;
      }
      else {
        object_count = 0;
      }
      if (object_count > 10) {
        mode_G = 2;
        object_count = 0;
      }

      break;
    // 1秒直進　ストーンが取れればモード4へ　取れなければモード3へ
    case 2:
      motorL_G = 100;
      motorR_G = 100;
      done = steadyState(1000);
      if ( done == 1 ) {
        mode_G = 3;
      }
      if (distance > 0 && distance < 9) {
        object_count++;
      }
      else {
        object_count = 0;
      }
      if (object_count > 25) {
        mode_G = 4;
        object_count = 0;
      }
      break;
    // 少しだけ回転させる　物体がない方向に回転すると回転方向を反転させる
    case 3:
      if (turn_flag == false) {
        motorL_G = -90;
        motorR_G = 90;
      }
      else if (turn_flag == true) {
        motorL_G = 90;
        motorR_G = -90;
      }
      if (distance > 5 && distance < 80) {
        object_count++;
      }
      else {
        object_count--;
      }
      if (object_count > 10) {
        mode_G = 2;
        object_count = 0;
      }
      if (object_count < -10) {
        turn_flag = !turn_flag;
        object_count = 0;
      }
      break;

    // 反時計回りで2秒回転　回転後モード5へ
    case 4:
      motorL_G = -100;
      motorR_G = 110;
      done = steadyState(2000);
      if ( done == 1 ) {
        mode_G = 5;
      }
      break;
    // 色を検知するまで直進
    case 5:
      motorL_G = 100;
      motorR_G = 100;
      if ( identifyColor_wide( 20 , 43 , 137  , 30) ) { // 青を検知
        mode_G = 6;
      }
      if ( identifyColor_wide( 0, 0, 0 , 30) ) { // 黒を検知
        mode_G = 99;
      }
      break;
    // 白を検知するまで半時計周り
    case 6:
      motorL_G = -110;
      motorR_G = 110;
      if ( identifyColor( 255, 255, 255 ) ) { // 白を検知
        mode_G = 7;
      }
      break;
    // 青のラインをライントレース
    case 7:
      lightNow = red_G;
      speedDiff = map(lightNow , blue_lightMin , blue_lightMax , -line_Kp * speed , line_Kp * speed);
      motorL_G = speed + speedDiff;
      motorR_G = speed - speedDiff;
      direction_diff = direction_G - direction_Prev;
      if (direction_Prev != 0 && abs(direction_diff) < 10) {
        sum_direction = sum_direction + direction_diff;
      }
      direction_Prev = direction_G;
      done = steadyState(2500);
      if ( done == 1 ) {
        if (sum_direction < 0) {
          turn_direction = -90;
          motorL_G = -SPEED;
          motorR_G = +SPEED;
        }
        else {
          turn_direction = 90;
          motorL_G = +SPEED;
          motorR_G = -SPEED;
        }
        direction_now = direction_G;
        done = 0;
        mode_G = 8;
      }
      break;
    // 指定した方向へ回転  回転方向をモータスピードではなく方角で判定
    case 8:
      done = targetDirection(direction_now + turn_direction);
      if ( done == 1 )
        mode_G = 9;
      break;
    // ストーンを円の中心へ
    case 9:
      motorL_G = 70;
      motorR_G = 70;
      if ( identifyColor_wide( 194,  48,  28 , 40) ) { // 赤を検知
        mode_G = 10;
      }
      break;
    // ストーンを円の中心において後退する
    case 10:
      motorL_G = -100;
      motorR_G = -100;
      if ( identifyColor_wide( 20 , 43 , 137  , 30) ) { // 青を検知
        mode_G = 11;
        direction_now = direction_G;
      }
      break;
    // 左に90度回転
    case 11:
      motorL_G = -SPEED;
      motorR_G = +SPEED;
      done = targetDirection(direction_now - 90);
      if ( done == 1 ) {
        mode_G = 12;
      }
      break;
    // 青色のラインをライントレース
    case 12:
      lightNow = red_G;
      speedDiff = map(lightNow , blue_lightMin , blue_lightMax , -line_Kp * speed , line_Kp * speed);
      motorL_G = speed + speedDiff;
      motorR_G = speed - speedDiff;
      if (distance > 5 && distance < 40) {
        object_count++;
      }
      else {
        object_count = 0;
      }
      if (object_count > 20) {
        mode_G = 98;
        object_count = 0;
      }
      break;
    case 98:
      motorL_G = 0;
      motorR_G = 0;
      break;
    case 99:
      motorL_G = -110;
      motorR_G = 110;
      if ( !(identifyColor( 0, 0, 0 )) ) { // 黒以外を検知
        done = steadyState(700);
        if ( done == 1 ) {
          mode_G = 5;
        }
      }
      break;
      break;
    default:
      break;
  }
}
