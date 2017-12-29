// 方針１で書かれている

/*
void zone()
{
  motors.setSpeeds(0, 0);
  delay(500);
  motors.setSpeeds(SPEED, SPEED);
  delay(500);
  motors.setSpeeds(0, 0);
  delay(500);

  for ( int i = 0; i < zoneNumber_G; ++i ) {
    motors.setSpeeds(-SPEED, SPEED);
    delay(200);
    motors.setSpeeds(0, 0);
    delay(200);
  }

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
    case 4: // 200ms左回転
      motorL_G = -SPEED;
      motorR_G = SPEED;
      done = steadyState(200);
      if ( done == 1 )
        mode_G = 5;
      break;
    case 5: // 200ms停止
      motorL_G = 0;
      motorR_G = 0;
      done = steadyState(200);
      if ( done == 1 ) {
        ++countTurn;
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
