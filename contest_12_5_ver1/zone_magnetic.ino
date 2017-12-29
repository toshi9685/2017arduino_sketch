// 方針１で書かれている

/*
  void zone_magnetic()
  {
  motors.setSpeeds(0, 0);
  delay(500);
  motors.setSpeeds(SPEED, SPEED);
  delay(500);
  motors.setSpeeds(0, 0);
  delay(500);

  for ( int i = 0; i < zoneNumber_G; ++i ) {
    motors.setSpeeds(SPEED, -SPEED);
    int direction_now = averageHeadingLP();
    if (direction_now > 270) {
      direction_now -= 360;
    }

    while ( targetDirection(direction_now + 90) == 0 ) {
      direction_G = averageHeadingLP();
    }

    motors.setSpeeds(SPEED, SPEED);
    delay(1000);
  }


  motors.setSpeeds(0, 0);
  delay(1000);

  button.waitForButton(); // Zumo bottun が押されるまで停止
  zoneNumber_G = 0;
  mode_G = 0;
  }

*/



// 方針２で書かれている
void zone_magnetic()
{
  static int countTurn = 0;
  static int direction_now = 0;
  int done;

  switch ( mode_G ) {
    case 0: // setupはここ
      countTurn = 0;
      mode_G = 1;
    case 1: // 500ms停止
      motorL_G = 0;
      motorR_G = 0;
      done = steadyState(500);
      if ( done == 1 )
        mode_G = 2;
      break;
    case 2: // 500ms直進
      motorL_G = SPEED;
      motorR_G = SPEED;
      done = steadyState(500);
      if ( done == 1 )
        mode_G = 3;
      break;
    case 3: // 500ms停止
      motorL_G = 0;
      motorR_G = 0;
      done = steadyState(500);
      if ( done == 1 ) {
        mode_G = 4;
        direction_now = direction_G;
      }
      break;
    case 4: // 90度右回転
      motorL_G = SPEED;
      motorR_G = -SPEED;
      done = targetDirection(direction_now + 90);
      if ( done == 1 )
        mode_G = 5;
      break;
    case 5: // 500ms直進
      motorL_G = SPEED;
      motorR_G = SPEED;
      done = steadyState(1000);
      if ( done == 1 ) {
        ++countTurn;
        direction_now = direction_G;
        if ( countTurn == zoneNumber_G ) // 条件満たすまでmode_G=4-5の繰り返し
          mode_G = 6;
        else
          mode_G = 4;
      }
      break;
    case 6: // 停止
      motorL_G = 0;
      motorR_G = 0;
      countTurn = 0;
      break;
    default:
      break;
  }
}

// 方針２で書かれている
void zone_loopTurn()
{
  static int turn_count = 1;
  static int direction_now = 0;
  int done;

  switch ( mode_G ) {
    case 0: // setupはここ
      mode_G = 1;
    case 1: // 500ms停止
      motorL_G = 0;
      motorR_G = 0;
      done = steadyState(500);
      if ( done == 1 )
        mode_G = 2;
      break;
    case 2: // 500ms直進
      motorL_G = SPEED;
      motorR_G = SPEED;
      done = steadyState(500);
      if ( done == 1 )
        mode_G = 3;
      break;
    case 3: // 500ms停止
      motorL_G = 0;
      motorR_G = 0;
      done = steadyState(500);
      if ( done == 1 ) {
        mode_G = 4;
        direction_now = direction_G;
      }
      break;
    case 4: // 90度右回転
      motorL_G = SPEED;
      motorR_G = -SPEED;
      done = targetDirection(direction_now + 90);
      if ( done == 1 ) {
        mode_G = 5;
      }
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知
        mode_G = 6;
      }
      break;
    case 5: // 500ms直進
      motorL_G = SPEED;
      motorR_G = SPEED;
      done = steadyState(1000);
      if ( done == 1 ) {
        direction_now = direction_G;
        mode_G = 4;
      }
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知
        mode_G = 6;
      }
      break;
    case 6: // コースアウトしないように復帰
      int done;
      if (turn_count % 2 == 1) {
        turnRight( SPEED );
      }
      else if (turn_count % 2 == 0) {
        turnRight( -SPEED );
      }
      done = steadyState(300 * turn_count);
      if ( done == 1 ) {
        done = 0;
        turn_count++;
        if ( !(identifyColor( 0, 0, 0 )) ) { // 黒以外を検知
          mode_G = 5;
          turn_count = 1;
        }
      }
      break;
    case 7:
      motorL_G = SPEED;
      motorR_G = SPEED;
      done = steadyState(1000);
      if ( done == 1 ) {
        direction_now = direction_G;
        mode_G = 5;
      }
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知
        mode_G = 6;
      }
    default:
      break;
  }
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

