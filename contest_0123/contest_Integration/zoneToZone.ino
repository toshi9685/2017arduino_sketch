const float color[4][3] = { // 各ゾーン（1-4)の識別色（RGB値）（各自で設定）
  { 160 , 30 , 14},
  { 63 , 148 , 63 },
  { 20 , 43 , 137 },
  { 255 , 240 , 82 },
};

const float direction_exit[3] = { // 各ゾーン(1-3)の脱出方向の角度（各自で設定）
  107, 90, 180
};
float minDistance; // identifyZone()用のグローバル変数
static int turn_count = 1;

// zoneからzoneへの移動（mode_G=0からスタート)
// startからzoneへの移動（mode_G=2からスタート）
void zoneToZone()
{
  int zoneNumber;
  float direction_target;
  static int zoneNumber_in = 0; // ゾーンに入った時のゾーン番号を記録
  int done;
  static bool exit_flag = false;

  switch ( mode_G ) {
    case 0:  // setup（必要がなくても形式的に置いておく）
      mode_G = 1;
      break;
    case 1: // direction_targetの方向を向く
      turnRight( SPEED );

      direction_target = direction_exit[ zoneNumber_in - 1 ];
      if ( turnToDirection( direction_target ) ) { // 各自で作成
        mode_G = 2;
      }


      break;

    case 2: // 黄を検知するまで直進
      goStraight( SPEED );
      if ( identifyColor( 255 , 240 , 82 ) ) { // 黄を検知
        mode_G = 3;
      }
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知
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

        zoneNumber_G = zoneNumber; // 状態変数の更新
        mode_G = 0;                // 状態変数の更新
        zoneNumber_in = zoneNumber; // どのゾーンに入ったのか記録
        Y = 0;
        time_Zonestart_G = millis();
      }
      break;
    case 5: // 白を検知するまで回転
      int done;
      if (turn_count % 2 == 1) {
        turnRight( SPEED );
      }
      else if (turn_count % 2 == 0) {
        turnRight( -SPEED );
      }
      done = steadyState(200 * turn_count);
      if ( done == 1 ) {
        done = 0;
        turn_count++;
        if ( !(identifyColor( 0, 0, 0 )) ) { // 黒以外を検知
          mode_G = 2;
          turn_count = 1;
        }
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
  direction_diff = direction_G - direction_target; // ターゲット方向からの差異（direction_G の値はloop() 関数で取得）
  // direction_diff の値は-360～360（度）の範囲を取り得るが，-180～180（度）の範囲に変換したい．
  if ( direction_diff > 180.0 )
    direction_diff -= 360.0;
  else if ( direction_diff < -180.0 )
    direction_diff += 360.0;
  // この時点でdirection_diff の値は-180～180（度）になっている．
  if ( fabs(direction_diff) < 5.0 ) // 5.0 はパラメーター
    return 1;
  else
    return 0;
}


// KNNで現在最も近い番号を返す
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
  float d2_max = 40; // パラメーター

  d2 = pow(red - red_G, 2) + pow(green - green_G, 2) + pow(blue - blue_G, 2);
  if ( d2 < d2_max * d2_max )
    return 1;
  else
    return 0;
}

int identifyColor_wide( int red, int green, int blue , float d2_max)
{
  float d2;

  d2 = pow(red - red_G, 2) + pow(green - green_G, 2) + pow(blue - blue_G, 2);
  if ( d2 < d2_max * d2_max )
    return 1;
  else
    return 0;
}

int steadyState( unsigned long period )
{
  static int flagStart = 0; // 0:待ち状態，1:現在計測中
  static unsigned long startTime = 0;

  if ( flagStart == 0 ) { 
    startTime = timeNow_G;
    flagStart = 1; // 現在計測中にしておく
  }

  if ( timeNow_G - startTime > period ) { // 計測開始からの経過時間が指定時間を越えた
    flagStart = 0; // 待ち状態に戻しておく
    startTime = 0; // なくても良いが，形式的に初期化
    return 1;
  }
  else
    return 0;
}


int targetDirection( int direction_target )
{
  float direction_diff;
  direction_diff = direction_G - direction_target; // ターゲット方向からの差異（direction_G の値はloop() 関数で取得）
  // direction_diff の値は-360～360（度）の範囲を取り得るが，-180～180（度）の範囲に変換したい．
  if ( direction_diff > 180.0 )
    direction_diff -= 360.0;
  else if ( direction_diff < -180.0 )
    direction_diff += 360.0;
  // この時点でdirection_diff の値は-180～180（度）になっている．
  if ( fabs(direction_diff) < 5.0 ) // 5.0 はパラメーター
    return 1;
  else
    return 0;
}







