const float color[4][3] = { // 各ゾーン（1-4)の識別色（RGB値）（各自で設定）//課題1
  { 84, 179, 89 },//緑
  { 183, 40, 24 },//赤
  { 12, 33, 104 },//青
  { 251, 236, 81 }//黄
};

const float direction_exit[3] = { // 各ゾーン(1-3)の脱出方向の角度（各自で設定）
  270, 90, 180
};
float minDistance; // identifyZone()用のグローバル変数

// zoneからzoneへの移動（mode_G=0からスタート)
// startからzoneへの移動（mode_G=2からスタート）
void zoneToZone()
{
  int zoneNumber;
  float direction_target;
  static int zoneNumber_in; // ゾーンに入った時のゾーン番号を記録
  static int turnTimes = 0;//方向転換回数//課題4
  switch ( mode_G ) {
    case 0:  // setup（必要がなくても形式的に置いておく）
      mode_G = 1;
      break;
    case 1: // direction_targetの方向を向く
       direction_target = direction_exit[zoneNumber_in-1];
       turnRight(SPEED);
       if( turnToDirection( direction_target ) ){
        mode_G = 2;
       }
      //mode_G = 2; // とりあえず，無条件にmode_Gの値を更新
      break;
    case 2: // 黄を検知するまで直進
      goStraight( SPEED );
      if ( identifyColor( 251, 236, 81 ) ) { // 黄を検知
        mode_G = 3;
      }else if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知//課題2
        mode_G = 5;
      }
      break;
    case 3: // 黒を検知するまで直進
      goStraight( SPEED );
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知
        minDistance = 9999999; // identifyZone()関数で使う変数の初期化
        mode_G = 4;
      }
      break;
    case 4: // 白を検知するまで直進（その間ゾーン番号を検知）
      goStraight( SPEED );
      zoneNumber = identifyZone();
      if ( identifyColor( 255, 255, 255 ) ) { // 白を検知
        goStraight( 0 );
        zoneNumber_G = zoneNumber; // 状態変数の更新
        mode_G = 0;                // 状態変数の更新
        zoneNumber_in = zoneNumber; // どのゾーンに入ったのか記録
        timeZonestart_G = millis();
      }
      break;
    case 5://黒を脱却するまで回転//課題3
    /*
      turnRight( SPEED );
      if (identifyColor( 255, 255, 255 )) {
        mode_G = 2;
      }
      */
      //課題4
      
      if(turnTimes <= 2){
        turnRight(SPEED);
      }else if(turnTimes > 2){
        turnRight(-SPEED);
      }
      if (identifyColor( 255, 255, 255 ) || identifyColor( 251, 236, 81 )) {
        goStraight( SPEED );
        mode_G = 2;
        turnTimes = 0;
      }
      if(steadyState(50) == 1){
        turnTimes++;
      }
      
      break;
    default:
      break;
  }
}

// 直進
void goStraight( int speed )
{
  motorL_G = speed;
  motorR_G = speed;
}

// 右回転（負の値は左回転）
void turnRight( int speed )
{
  motorL_G = speed;
  motorR_G = -speed;
}

// 指定した方向いたら1を返す（それ以外0)
int turnToDirection( int direction_target )
{
  float direction_diff;
  direction_diff = direction_G - direction_target; // ターゲット方向からの差異 (direction_G の値は loop() 関数で取得)
  // direction_diff の値は-360~360(度)の範囲を取り得るが，-180~180(度)の範囲 に変換したい.
  if ( direction_diff > 180.0 )
    direction_diff -= 360.0;
  else if ( direction_diff < -180.0 )
    direction_diff += 360.0;
  // この時点でdirection_diff の値は-180~180(度)になっている. 
  if ( fabs(direction_diff) < 5.0 ) // 5.0はパラメーター
    return 1;
  else
    return 0;
}


// KNNで現在最も近い番号を返す
//カラーセンサーで読み込んだRGB値が引数と一定以下の距離であれば1,それ以外は0を返す
//閾値は関数内のパラメータで指定
int identifyZone()
{
  // float minDistance; グローバル変数で定義
  static int zoneNumber; // 現状で一番近い識別色の番号
  float distance; // 距離の2乗

  for ( int i = 0; i < 4; ++i ) {
    distance = (red_G - color[i][0]) * (red_G - color[i][0])
               + (green_G - color[i][1]) * (green_G - color[i][1])
               + (blue_G - color[i][2]) * (blue_G - color[i][2]);
    if ( distance < minDistance ) {
      minDistance = distance;
      zoneNumber = i;
    }
  }

  return zoneNumber + 1; // zone番号は1-4なので+1
}

int identifyColor( int red, int green, int blue )
{
  float d2;
  float d2_max = 30; // パラメーター

  d2 = pow(red - red_G, 2) + pow(green - green_G, 2) + pow(blue - blue_G, 2);
  if ( d2 < d2_max * d2_max )
    return 1;
  else
    return 0;
}


















