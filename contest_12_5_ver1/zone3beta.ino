float avex = 0;

void zone3beta()
{
  static int countTurn = 0;
  int done;
  float speed0, diff;
  avex = 0.9 * avex + 0.1 * compass.a.x;
  switch ( mode_G ) {
    case 0: // setup はここ
      countTurn = 0; mode_G = 1;
    case 1: // 山探し(ここでは直進)
      if (avex > 4000) {
        mode_G = 2;  // 登り始めたらmode 2 へ
      }
      speed0 = 150;
      diff = -0.02 * compass.a.y; // P-制御(最急勾配方向へ)
      break;
    case 2: // 山登り中
      if ((avex < 1500) && (avex > -1500)) { // 登頂したら頂上の中心部まで進む
        done = steadyState(200); // 200ms は要調整
      }
      if (done == 1) { // 頂上の中心部まで来たら停止(mode 3 へ)
        speed0 = 0; diff = 0;
        mode_G = 3;
      } else {
        speed0 = 150;
        diff = -0.02 * compass.a.y; // P-制御(最急勾配方向へ)
      }
      break;
    case 3: // 500ms 停止
      speed0 = 0; diff = 0;
      done = steadyState(500);
      if ( done == 1 )
        mode_G = 4;
      break;
    case 4: // その場回転で東を向く
      speed0 = 0;
      diff = turnTo(90 * countTurn); // P-制御
      if (abs(diff) <= 50) {
        diff = 0;
        countTurn++;
        mode_G = 5;
      }
      break;
    case 5: // 停止
      speed0 = 0; diff = 0;
      done = steadyState(500);
      if (done == 1 && countTurn < 5) {
        mode_G = 4;
        speed0 = 150;
      }
      else if (done == 1 && countTurn >= 5) {
        mode_G = 6;
      }
      break;
    case 6: // 停止
      diff = 0;
      if (avex < -4000) {
        mode_G = 7;  // 登り始めたらmode 2 へ
      }
      speed0 = 90;
      diff = 0.03 * compass.a.y + 0.03 * compass.a.x; // P-制御(最急勾配方向へ)
      break;
    case 7: // 山登り中
      if ((avex < 1500) && (avex > -1500)) { // 登頂したら頂上の中心部まで進む
        done = steadyState(200); // 200ms は要調整
      }
      if (done == 1) { // 頂上の中心部まで来たら停止(mode 3 へ)
        speed0 = 0; diff = 0;
        mode_G = 8;
      } else {
        speed0 = 90;
        diff = 0.03 * compass.a.y + 0.03 * compass.a.x; // P-制御(最急勾配方向へ)
      }
      break;
    case 8: // 停止
      speed0 = 0; diff = 0;
      diff = turnTo(0); // P-制御
      if (abs(diff) <= 50) {
        diff = 0;
        mode_G = 9;
      }
      break;
    case 9:
      speed0 = 100 ; diff = 0;
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知
        mode_G = 10;
      }
      break;
    case 10:
      speed0 = 0; diff = 0;
      break;
    default:
      break;
  }
  motorL_G = speed0 + diff;
  motorR_G = speed0 - diff;
}

float turnTo(float dir) {
  float heading, diff;
  heading = direction_G;
  if (heading > 180) heading -= 360;
  heading -= dir;
  if (heading > 180) heading -= 360;
  if (heading < -180) heading += 360;
  diff = -heading * 5; // P-制御
  if (diff > 200) diff = 200; // 飽和
  if (diff < -200) diff = -200; // 飽和
  return diff;
}
