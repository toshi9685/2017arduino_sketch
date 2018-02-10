//競技前に確認すること一覧
//青と赤の位置関係 escape[]
//どの色の時脱出するか nowcolor_G 0:青 1:赤
//最初に探索する色 mount
float avex = 0, avey = 0;
static int count = 0;
static int mount = 0;//探索する色　0:青,1:赤
int action[] = {5, 2};//mode
int escape[] = {245, 50}; //山からの脱出方向
const int buzzerPin = 3;
bool esc = false;


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

void zone_hillclimb()
{
  static int entryAngle = 0;
  int done;
  float speed0, diff;

  avex = 0.9 * avex + 0.1 * compass.a.x;
  avey = 0.9 * avey + 0.1 * compass.a.y;
  switch (mode_G) {
    case 0://setup
      mode_G = 1;
      break;
    case 1://山探し
      if (avex > 4000) { //登り始めたらmode2か5へ
        mode_G = action[mount];
        if (0 <= direction_G && direction_G <= 180) {
          entryAngle = direction_G + 180;
        } else {
          entryAngle = direction_G - 180;
        }
      }
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知
        entryAngle = direction_G;
        mode_G = 20;
      }
      speed0 = 200;
      diff = -0.02 * compass.a.y; //p-制御(最急勾配方向へ)
      //diff = -0.02 * (compass.a.x + compass.a.y);
      break;
    case 2://中腹まで前進
      if (steadyState(900) == 1) { //秒後(mode 3)へ
        speed0 = 0;
        diff = 0;
        mode_G = 3;
      }
      speed0 = 200;
      diff = -0.02 * (compass.a.y); //p-制御(最急勾配方向へ)

      break;
    case 3://山腹探索
      if (steadyState(7000) == 1) { //7秒探索後(mode 4)へ
        speed0 = 0;
        diff = 0;
        mode_G = 4;
        entryAngle = direction_G;
      } else {
        speed0 = 200;
        //diff = -0.02 * (compass.a.x + 0.5 * compass.a.y); //山腹周回時計回り
        diff = 0.02 * (compass.a.x + 0.5 * -compass.a.y); //山腹周回反時計回り
        if (identifyColor_wide(183, 40, 24 , 70) == 1) {
          mount = 0;
          nowcolor_G = 1;//赤
          mode_G = 10;
          steadyState(0);
          entryAngle = direction_G;
        }
      }

      break;
    case 4://山頂へ向きを変える
      diff = turnTo(entryAngle - 90);
      if (abs(diff) <= 50) {
        mode_G = 5;
      }

      break;
    case 5://山腹から頂上へ
      if ((avex < 1500) && (avex > -1500) && (avey < 1500) && (avey > -1500)) { //登頂したら頂上の中心部まで進む
        //done = steadyState(100);//200msは要調整
        mode_G = 6;
        speed0 = 0;
        diff = 0;
      } else {
        speed0 = 200;
        diff = -0.02 * compass.a.y; //p-制御(最急勾配方向へ)
      }

      break;
    case 6://青のスポットを探す
      if (identifyColor_wide(12, 33, 104 , 70) == 1) {
        nowcolor_G = 0;//青
        mode_G = 10;
        mount = 1;
        steadyState(0);
      } else if (steadyState(300)) {
        mode_G = 7;
      }

      break;
    case 7://停止下山方向
      speed0 = 0;
      diff = turnTo(escape[count % 2]); //p-制御
      //diff = turnTo(escape[mount]); //p-制御
      if (abs(diff) <= 50) {
        count++;
        if (mount == 0) {
          mode_G = 25;
        } else {
          mode_G = 8;
        }
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
      speed0 = 200;
      //斜面に向かって斜め右を向くように降りる.
      //diff = 0.02 * (compass.a.x + compass.a.y);//斜め降り
      //diff = -0.02 * (compass.a.x + compass.a.y);//斜め登り
      if ((avex < 1500) && (avex > -1500) && (avey < 1500) && (avey > -1500)) { //下山したらmode_G=0.
        speed0 = 0;
        diff = 0;
        mode_G = 0;
      }

      break;
    case 10://ブザー鳴らす
      tone(buzzerPin, 440);
      if (steadyState(500) == 1) {
        noTone(buzzerPin);
        mode_G = 7;
      }
      break;
    case 20://少し後退
      speed0 = -200;
      diff = 0;
      if (identifyColor( 250, 250, 250 ) || identifyColor( 251, 236, 81 )) {
        mode_G = 21;
      }
      break;
    case 21://黒を脱却するまで回転
      speed0 = 0;
      diff = turnTo(entryAngle + 80); //p-制御
      if (abs(diff) <= 50) {
          mode_G = 1;
      }
      break;
    case 25:
      speed0 = 200;
      diff = 0;
      if (steadyState(1500)) {
        mode_G = 0;
      }
      break;
    case 30:
      esc = true;
      mode_G = 31;
      break;
    case 31:
      if (steadyState(800)) {
        mode_G = 32;
      } else {
        speed0 = 200;
        diff = 0.02 * (compass.a.x + 0.25 * -compass.a.y); //山腹周回反時計回り
      }
      break;
    case 32:
      zoneNumber_G = 0;
      mode_G = 0;
      break;
    default:
      break;
  }
  motorL_G = speed0 + diff;
  motorR_G = speed0 - diff;
}

