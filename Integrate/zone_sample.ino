// 方針１で書かれている
/*
  void zone()
  {
  static int dic = -1;//目標角度初期化
  motors.setSpeeds(0, 0);
  delay(500);
  motors.setSpeeds(SPEED, SPEED);
  delay(500);
  motors.setSpeeds(0, 0);
  delay(500);

  for ( int i = 0; i < zoneNumber_G*2; ++i ) {
    if (dic == -1) {//-1なら目標角度を代入
        dic = 90 + direction_G;//現在の角度に９０度足す
        if (dic >= 360) {//360度以上は0~359度に補正
          dic = dic - 360;
        }
    }
    while(targetDirection(dic) == 0){//目標角度になるまで回転
      motors.setSpeeds(SPEED, -SPEED);
      direction_G = averageHeadingLP();//現在の角度を更新
    }
    dic = -1;//初期化
    motors.setSpeeds(SPEED, SPEED);
    delay( 1000);
  }
  motors.setSpeeds(0, 0);
  button.waitForButton(); // Zumo bottun が押されるまで停止
  zoneNumber_G = 0;
  mode_G = 0;
  }
*/

// 方針２で書かれている
void zone()
{
  static int countTurn = 0;
  int done;
  static int dic = -1;//目標角度
  static int turnTimes = 0;//方向転換回数//課題4
  int n;

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
      if ( done == 1 )
        mode_G = 4;
      break;
    case 4: // 90度右回転
      motorL_G = SPEED;
      motorR_G = -SPEED;
      if (dic == -1) {//-1なら目標角度を代入
        dic = 90 + direction_G;//現在の角度に９０度足す
        if (dic >= 360) {//360度以上は0~359度に補正
          dic = dic - 360;
        }
      }
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知//発展課題12.2.1
        mode_G = 7;
      }
      done = targetDirection(dic);
      if ( done == 1 ) {
        mode_G = 5;
        dic = -1;
      }
      break;
    case 5: // 1000ms直進
      if ( identifyColor( 0, 0, 0 ) ) { // 黒を検知//発展課題12.2.1
        mode_G = 7;
      }
      motorL_G = SPEED;
      motorR_G = SPEED;
      done = steadyState(1000);
      if ( done == 1 ) {
        ++countTurn;
        if ( countTurn == zoneNumber_G * 100 ) // 条件満たすまでmode_G=4-5の繰り返し
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
    case 7://黒を脱却するまで回転////発展課題12.2.1
      if (turnTimes <= 2) {
        turnRight(SPEED);
      } else if (turnTimes > 2) {
        turnRight(-SPEED);
      }
      if (identifyColor( 255, 255, 255 )) {
        goStraight( SPEED );
        mode_G = 5;
        turnTimes = 0;
      }
      if (steadyState(50) == 1) {
        turnTimes++;
      }

      break;
    default:
      break;
  }
}

//ダミーzone()
/*
  void zone() {
  static int countTurn = 0;
  int done;
  static int dic = -1;

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
      if ( done == 1 )
        mode_G = 4;
      break;
    case 4:
      mode_G = 5;
      break;
    case 5: // 1000ms直進
      motorL_G = SPEED;
      motorR_G = SPEED;
      done = steadyState(100);
      if ( done == 1 ) {
        ++countTurn;
        if ( countTurn == zoneNumber_G * 2 ) // 条件満たすまでmode_G=4-5の繰り返し
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
*/

