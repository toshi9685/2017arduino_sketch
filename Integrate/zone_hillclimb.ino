float avex = 0;
float turnTo(float dir) {
  float heading, diff;
  heading = direction_G;
  if (heading > 180) heading -= 360;
  heading -= dir;
  if (heading > 180) heading -= 360;
  if (heading < -180) heading += 360;
  diff = -heading * 5;//p-制御
  if (diff > 200) diff = 200;//飽和
  if (diff < -200) diff = -200; //飽和
  return diff;
}
void zone3beta()
{
  static int countTurn = 0;
  static int entryAngle = 0;
  int done;
  float speed0, diff;

  avex = 0.9 * avex + 0.1 * compass.a.x;
  switch (mode_G) {
    case 0://setup
      countTurn = 0; mode_G = 1;
      break;
    case 1://山探し
      if (avex > 4000) { //登り始めたらmode2へ
        mode_G = 2;
      }
      speed0 = 150;
      diff = -0.02 * compass.a.y; //p-制御(最急勾配方向へ)
      break;
    case 2://山登り中
      if ((avex < 1500) && (avex > -1500)) { //登頂したら頂上の中心部まで進む
        done = steadyState(200);//200msは要調整
      }
      if (done == 1) { //頂上の中心部まできたら停止(mode 3)へ
        speed0 = 0;
        diff = 0;
        mode_G = 3;
      } else {
        speed0 = 150;
        diff = -0.02 * compass.a.y; //p-制御(最急勾配方向へ)
      }
      break;
    case 3://500ms停止
      speed0 = 0;
      diff = 0;
      done = steadyState(500);
      if(0 <= direction_G && direction_G <=180){
        entryAngle = direction_G + 180;
      }else{
        entryAngle = direction_G - 180; 
      }
      if (done == 1) {
        mode_G = 4;
      }
      break;
    case 4://青のスポットを探す
      if(identifyColor(12, 33, 104)){
        mode_G = 5;
      }
      break;
    case 5://停止
      speed0 = 0;
      diff = turnTo(entryAngle); //p-制御
      if (abs(diff) <= 50) {
        //mode_G = 6;
      }
      break;
    case 6://山下り始め
      if (avex > -3000) { //山を下り始めるまで
        speed0 = 120;
        diff = 20;
      } else {//山を下り始めたら
        mode_G = 7;
      }
      break;
    case 7://山の斜面を走る時
      speed0 = 100;
      //斜面に向かって斜め右を向くように降りる.
      diff = -0.02 * (compass.a.x+compass.a.y);
      /*
      if (abs(compass.a.x) > abs(compass.a.y)) {
        diff = 20;
      } else if (abs(compass.a.x) < abs(compass.a.y)) {
        diff = -20;
      }
      */
      if ((avex < 1500) && (avex > -1500)) { //下山したら一旦止まる.zoneToZone()に移行する.
        speed0 = 0;
        diff = 0;
        mode_G = 0;
        zoneNumber_G = 0;
      }

      break;
    default:
      break;
  }
  motorL_G = speed0 + diff;
  motorR_G = speed0 - diff;
}

