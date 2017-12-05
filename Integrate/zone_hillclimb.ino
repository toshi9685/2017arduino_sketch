float avex = 0;
int mount = 0;//探索する色　0:青,1:赤
int action[] = {5, 2};//mode
int escape[] = {0, 180}; //山からの脱出方向 青が北側、赤が南側を想定
const int buzzerPin = 3;
int target = 1500;
float Kp = 0.02, Ki = 0.02;


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
  static int entryAngle = 0;
  static float sum_e = 100;
  static unsigned long pretime = 0;
  int done;
  float speed0, diff, e = 0;

  avex = 0.9 * avex + 0.1 * compass.a.x;
  avex_G = avex;
  switch (mode_G) {
    case 0://setup
      mode_G = 1;
      break;
    case 1://山探し
      if (avex > 4000) { //登り始めたらmode2,5へ
        mode_G = action[mount];
        if (0 <= direction_G && direction_G <= 180) {
          entryAngle = direction_G + 180;
        } else {
          entryAngle = direction_G - 180;
        }
      }
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知
        mode_G = 20;
      }
      speed0 = 200;
      diff = -0.02 * compass.a.y; //p-制御(最急勾配方向へ)
      //diff = -0.02 * (compass.a.x + compass.a.y);
      break;
    case 2://中腹まで前進
      if (steadyState(1000) == 1) { //3秒後(mode 3)へ
        speed0 = 0;
        diff = 0;
        mode_G = 3;
        pretime = millis();
      }
      speed0 = 200;
      diff = -0.02 * (compass.a.y); //p-制御(最急勾配方向へ)
      break;
    case 3://山腹探索
      if (steadyState(6000) == 1) { //6秒探索後(mode 4)へ
        speed0 = 0;
        diff = 0;
        mode_G = 4;
        entryAngle = direction_G;
      } else {
        speed0 = 200;
        diff = -0.02 * (compass.a.x + 0.5 * compass.a.y); //p-制御(最急勾配方向へ)
        if (identifyZone() == 1) {
          mount = 0;
          mode_G = 10;
          steadyState(0);
          entryAngle = direction_G;
        }
        /*
          //PI制御
          e = target-avex;//目標との差
          sum_e += e * (millis() - pretime) / 1000;//積分
          diff = Kp*e + Ki*sum_e;//diffの決定
          pretime = millis();
        */
      }
      break;
    case 4://山頂へ向きを変える
      diff = turnTo(entryAngle + 90);
      if (abs(diff) <= 50) {
        mode_G = 5;
      }
      break;
    case 5://山腹から頂上へ
      if ((avex < 1000) && (avex > -1000)) { //登頂したら頂上の中心部まで進む
        done = steadyState(200);//200msは要調整
        mode_G = 6;
        speed0 = 0;
        diff = 0;
      } else {
        speed0 = 200;
        diff = -0.02 * compass.a.y; //p-制御(最急勾配方向へ)
      }
      break;
    case 6://青のスポットを探す
      if (identifyZone() == 3) {
        mode_G = 10;
        mount = 1;
      } else {
        mode_G = 7;
        mount = 0;
      }
      break;
    case 7://停止下山方向

      speed0 = 0;
      diff = turnTo(escape[mount] - 20); //p-制御
      if (abs(diff) <= 50) {
        mode_G = 8;
      }
      break;
    case 8://山下り始め
      if (avex > -1500) { //山を下り始めるまで
        speed0 = 150;
      } else {//山を下り始めたら
        mode_G = 9;
      }
      break;
    case 9://山の斜面を走る時
      speed0 = 150;
      //斜面に向かって斜め右を向くように降りる.
      diff = 0.02 * (compass.a.x + compass.a.y);//斜め降り
      //diff = -0.02 * (compass.a.x + compass.a.y);//斜め登り
      if ((avex < 1500) && (avex > -1500)) { //下山したら一旦止まる.zoneToZone()に移行する.
        speed0 = 0;
        diff = 0;
        //mode_G = 0;
        //zoneNumber_G = 0;
      }

      break;
    case 10://ブザー鳴らす
      tone(buzzerPin, 440);
      if (steadyState(500) == 1) {
        noTone(buzzerPin);
        mode_G = 7;
      }
      break;
    case 11://山間の移動
      //改めて向きを変える
      diff = turnTo(escape[mount]); //p-制御
      if (abs(diff) <= 50) {
        mode_G = 0;
      }
      break;
    case 20://黒を脱却するまで回転
      BreakBLine(0);
      break;
    default:
      break;
  }
  motorL_G = speed0 + diff;
  motorR_G = speed0 - diff;
}

