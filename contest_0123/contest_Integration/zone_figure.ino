void zone_figure_trace()
{

  /*Kpとspeedを調節して終わり*/


  //static float lightMax =  255;//(255 + 255 + 255) / 3.0;// 各自で設定
  static float speed = 100; // パラメーター
  float speedDiff;
  float Kp = 3.0; // パラメーター
  static int TurnCount = 0;
  int done = 0;
  int done1 = 0;//色をライントレースして2秒数える(重複処理)
  static int tflag = 0;//色をライントレースして2秒数える(重複処理)
  static int secflag = 0;//5秒経過したらその色のライントレースを強制終了
  float lightNow;//現在のRGB値の平均値

  static float lightMin =  24;// ライントレースする線上のlightNowの値.
  static float lightMax = 255;// 白地のlightNowの値.

  //ライントレース時に
  static float lightMin1 = (183 + 40 + 24) / 3.0;// 赤色のlightNowの値
  static float lightMin2 = (12 + 33 + 104) / 3.0;// 青色のlightNowの値.
  static float lightMin3 = (84 + 179 + 89) / 3.0;// 緑色のlightNowの値.
  static float lightMin4 = (134 + 55 + 89) / 3.0;// ピンクのlightNowの値.

  //各色を発見できたかのフラグ(初期値:0,発見時:1)
  static int RED_F;
  static int BLUE_F;
  static int GREEN_F;
  static int PINK_F;

  //色を発見できた時の座標を記録
  static float red_x;
  static float red_y;
  static float blue_x;
  static float blue_y;
  static float green_x;
  static float green_y;
  static float pink_x;
  static float pink_y;

  static float color_direction;//現在いる色に入った角度を一時記憶
  static float color_x;//現在いる色に入ったx座標を記憶
  static float color_y;//現在いる色に入ったy座標を記憶
  static int figure_count = 0;//図形をカウント
  static float colorCenter_x;//中心xを図形の座標から計算
  static float colorCenter_y;//中心yを図形の座標から計算
  static float targetDir;//黒検知位置とトレース座標の角度を計算
  int time_flag = 0;
  int count = 0;
  int wait_flag = 0;

  static unsigned long timePrev_figure = 0;


  switch (mode_G) {

    case 0://セットアップ
      mode_G = 1;
      timePrev_figure = timeNow_G;
      break;


    /*************************
      case 1~10 各色のライントレース
     *************************/

    case 1://各色を検知するまで直進


      if ( identifyColor( 160, 38, 20 ) ) { // 赤を検知
        if (RED_F == 0) {
          RED_F = 1;
          //mode_G = 2;
        }
        mode_G = 3;
        red_x = X;
        red_y = Y;
        color_x = 0.0;//現在いる色に入ったx座標を記憶
        color_y = 0.0;//現在いる色に入ったy座標を記憶
        // color_direction = direction_G;
        figure_count++;
      }
      else if ( identifyColor( 15, 31, 112 ) ) { //青を検知
        if (BLUE_F == 0)
        {
          BLUE_F = 1;
          mode_G = 2;
        }
        blue_x = X;
        blue_y = Y;
        color_x = 0.0;//現在いる色に入ったx座標を記憶
        color_y = 0.0;//現在いる色に入ったy座標を記憶
        color_direction = direction_G;
        figure_count++;
      }
      else if ( identifyColor( 48, 127, 48 ) ) { //緑を検知
        if (GREEN_F == 0)
        {
          GREEN_F = 1;
          mode_G = 2;
        }
        //mode_G = 5;
        green_x = X;
        green_y = Y;
        color_x = 0.0;//現在いる色に入ったx座標を記憶
        color_y = 0.0;//現在いる色に入ったy座標を記憶
        // color_direction = direction_G;
        figure_count++;
      }
      else if ( identifyColor( 188, 100, 139 ) ) { //ピンクを検知
        if (PINK_F == 0)
        {
          PINK_F = 1;
          //mode_G = 2;
        }
        mode_G = 6;
        pink_x = X;
        pink_y = Y;
        color_x = 0.0;//現在いる色に入ったx座標を記憶
        color_y = 0.0;//現在いる色に入ったy座標を記憶
        //color_direction = direction_G;
        figure_count++;
      }
      else if ( identifyColor( 0, 0, 0 ) ) { //黒を検知
        mode_G = 20;
      }

      goStraight( 160 );

      break;


    case 2://白を見つけるまで左回転
      if (identifyColor(255, 255, 255)) {
        //if (RED_F == 1) mode_G = 3;
        if (BLUE_F == 1) mode_G = 4;
        if (GREEN_F == 1) mode_G = 5;
        //if (PINK_F == 1) mode_G = 6;
      }
      motorL_G = 100;
      motorR_G = -100;
      break;



    case 3://P制御による赤色ライントレース
      if ( identifyColor( 0, 0, 0 ) ) { //黒を検知
        mode_G = 20;
      }

      lightNow = (red_G + green_G + blue_G ) / 3.0;
      if (RED_F == 2) mode_G = 1;

      //赤をライントレース
      if (RED_F == 1) {
        /*
          if ( lightNow < (lightMin1 + lightMax) / 2.0 ) // 左回転
          speedDiff = map(lightNow, lightMin1, lightMax, -Kp * speed, Kp * speed);
          else { // 右回転
          speedDiff = map(lightNow, lightMin1, lightMax, -Kp * speed, Kp * speed);
          }

          //移動
          if ( speedDiff < 0 ) {
          motorL_G = speed + speedDiff;
          motorR_G = speed;// + speedDiff;
          }
          else {
          motorL_G = speed;//+ - speedDiff;
          motorR_G = speed - speedDiff;
          }*/
        speed = 100;
        Kp = 2.0;
        speedDiff = map(lightNow, lightMin2, lightMax, -Kp * speed, Kp * speed);
        motorL_G = speed - speedDiff;
        motorR_G = speed + speedDiff;

        secflag = steadyState(8000);
        if (secflag == 1) {
          RED_F = 2;
          mode_G = 1;
        }
        /*
          //一周判定
          done1 = steadyState(3000);
          if (done1 == 1) {
          tflag = 1;
          }
          if (identifyDistance( red_x, red_y, X, Y) == 1 && tflag == 1) {
          RED_F = 2;
          mode_G = 1;
          tflag = 0;
          done1 = 0;
          }
        */
      }
      //強制終了



      break;



    case 4://P制御による青色ライントレース
      if ( identifyColor( 0, 0, 0 ) ) { //黒を検知
        mode_G = 20;
      }

      lightNow = (red_G + green_G + blue_G ) / 3.0;

      if (BLUE_F == 2) mode_G = 1;

      //青をライントレース
      if (BLUE_F == 1) {
        /*
          if ( lightNow < (lightMin2 + lightMax) / 2.0 ) // 左回転
          speedDiff = map(lightNow, lightMin2, lightMax, -Kp * speed, Kp * speed);
          else // 右回転
          speedDiff = map(lightNow, lightMin2, lightMax, -Kp * speed, Kp * speed);

          //移動
          if ( speedDiff < 0 ) {
          motorL_G = speed + speedDiff;
          motorR_G = speed;// + speedDiff;
          }
          else {
          motorL_G = speed;//+ - speedDiff;
          motorR_G = speed - speedDiff;
          }*/
        speed = 100;
        Kp = 2.0;
        speedDiff = map(lightNow, lightMin2, lightMax, -Kp * speed, Kp * speed);
        motorL_G = speed - speedDiff;
        motorR_G = speed + speedDiff;

        secflag = steadyState(6500);
        if (secflag == 1) {
          BLUE_F = 2;
          mode_G = 1;
        }
        /*
                done1 = steadyState(3000);
                if (done1 == 1) {
                  tflag = 1;
                  steadyState(0);
                }
                if (identifyDistance( blue_x, blue_y, X, Y) == 1 && tflag == 1) {
                  BLUE_F = 2;
                  mode_G = 1;
                  tflag = 0;
                  done1 = 0;
                }
        */
      }



      break;



    case 5://P制御による緑色ライントレース
      if ( identifyColor( 0, 0, 0 ) ) { //黒を検知
        mode_G = 20;
      }

      lightNow = (red_G + green_G + blue_G ) / 3.0;
      if (GREEN_F == 2) mode_G = 1;

      //緑をライントレース
      if (GREEN_F == 1) {
        /*
          if ( lightNow < (lightMin3 + lightMax) / 2.0 ) // 左回転
          speedDiff = map(lightNow, lightMin3, lightMax, -Kp * speed, Kp * speed);
          else // 右回転
          speedDiff = map(lightNow, lightMin3, lightMax, -Kp * speed, Kp * speed);

          //移動
          if ( speedDiff < 0 ) {
          motorL_G = speed + speedDiff;
          motorR_G = speed;// + speedDiff;
          }
          else {
          motorL_G = speed;//+ - speedDiff;
          motorR_G = speed - speedDiff;
          }
        */
        speed = 90;
        Kp = 3.5;
        speedDiff = map(lightNow, lightMin2, lightMax, -Kp * speed, Kp * speed);
        motorL_G = speed - speedDiff;
        motorR_G = speed + speedDiff;

        secflag = steadyState(8000);
        if (secflag == 1) {
          GREEN_F = 2;
          mode_G = 1;
        }
        /*
                done1 = steadyState(3000);
                if (done1 == 1) {
                  tflag = 1;
                }
                if (identifyDistance( green_x, green_y, X, Y) == 1 && tflag == 1) {
                  GREEN_F = 2;
                  mode_G = 1;
                  tflag = 0;
                  done1 = 0;
                }*/
      }


      break;


    case 6:////P制御によるピンク色ライントレース
      if ( identifyColor( 0, 0, 0 ) ) { //黒を検知
        mode_G = 20;
      }
      lightNow = (red_G + green_G + blue_G ) / 3.0;
      //lightNow = green_G;
      if (PINK_F == 2) mode_G = 1;

      //ピンクをライントレース
      if (PINK_F == 1) {
        Kp = 5.0;
        speed = 80;
        /*
                if ( lightNow < (lightMin4 + lightMax) / 2.0 ) // 左回転
                  speedDiff = map(lightNow, lightMin4, lightMax, -Kp * speed, Kp * speed);
                else // 右回転
                  speedDiff = map(lightNow, lightMin4, lightMax, -Kp * speed, Kp * speed);

                //移動
                if ( speedDiff < 0 ) {
                  motorL_G = speed + speedDiff;
                  motorR_G = speed;// + speedDiff;
                }
                else {
                  motorL_G = speed;//+ - speedDiff;
                  motorR_G = speed - speedDiff;
                }*/


        speedDiff = map(lightNow, lightMin2, lightMax, -Kp * speed, Kp * speed);
        motorL_G = speed + speedDiff;
        motorR_G = speed - speedDiff;

        /*
          if ( lightNow < (lightMin4 + lightMax) / 2.0 ) // 左回転
          speedDiff = -Kp * speed;
          else
          speedDiff = Kp * speed;

          if ( speedDiff < 0 ) {
          motorL_G = speed + speedDiff;
          motorR_G = speed;// + speedDiff;
          }
          else {
          motorL_G = speed;//+ - speedDiff;
          motorR_G = speed - speedDiff;
          }*/

        secflag = steadyState(8500);
        if (secflag == 1) {
          PINK_F = 2;
          mode_G = 1;
        }
        /*
                done1 = steadyState(3000);
                if (done1 == 1) {
                  tflag = 1;
                }
                if (identifyDistance( pink_x, pink_y, X, Y) == 1 && tflag == 1) {
                  speed = 140;
                  Kp = 2.0;
                  PINK_F = 2;
                  mode_G = 1;
                  tflag = 0;
                  done1 = 0;
                }*/
      }


      break;

    /*
        case 7://一周後,ライントレースした図形の中心方向に回転
          if (turnToDirection1(color_direction)) {
            zoneNumber_G = 0;
          }
          break;


                case 8://ライントレースした図形の中心方向に移動
                  motorL_G = SPEED;
                  motorR_G = SPEED;
                  done3 = steadyState(500);
                  if (done3 == 1) {
                    done3 = 0;
                    steadyState(0);
                    mode_G = 9;
                  }

                  break;


                case 9://次の図形の方向に回転し,mode_G=1へ遷移
                  if (figure_count == 1 ) {
                    //左回転
                    motorL_G = -SPEED;
                    motorR_G = SPEED;
                    done4 = targetDirection(color_direction - 66);
                    if (done4 == 1) {
                      done4 = 0;
                      mode_G = 1;
                      steadyState(0);
                    }
                  }

                  if (figure_count == 2 || figure_count == 3) {
                    //右回転
                    motorL_G = SPEED;
                    motorR_G = -SPEED;
                    done4 = targetDirection(color_direction - 66);
                    if (done4 == 1) {
                      done4 = 0;
                      mode_G = 1;
                    }
                  }

                  //全部のトレースが終わった時,回転
                  if (figure_count == 4) {
                    //左回転
                    motorL_G = -SPEED;
                    motorR_G = SPEED;
                    done4 = targetDirection(color_direction - 180);
                    if (done4 == 1) {
                      done4 = 0;
                      colorCenter_x = (red_x + blue_x + green_x + pink_x) / 4;
                      colorCenter_y = (red_y + blue_y + green_y + pink_y) / 4;
                      mode_G = 10;
                    }
                  }

                  break;


                case 10://コースの中心位置まで直進
                  goStraight( 120 );
                  done5 = positionDistance(colorCenter_y,  Y);
                  if (done5 == 1)
                  {
                    done5 = 0;
                    mode_G = 11;
                  }

                  break;


                case 11://右に90度回転し,zoneTozoneへ
                  motorL_G = SPEED;
                  motorR_G = -SPEED;
                  done6 = targetDirection(90);
                  if (done6 == 1)
                  {
                    done6 = 0;
                    mode_G = 21;
                  }
                  break;


    */

    case 20://黒を検知した時の回避(コースアウト処理)
      speed = 100;
      Kp = 3.0;
      if (count % 2 == 0)
        turnRight(250);
      else
        turnRight(-250);
      //時間を計測
      wait_flag = steadyState(400 + count * 400);
      //指定時間経過
      if (wait_flag == 1)
      {
        if (identifyColor(0, 0, 0) == 0)
        {
          mode_G = 1;
          count = 0;
        }
        count++;
      }
      break;

    case 21://zoneTozoneへ
      zoneNumber_G = 0;
      mode_G = 0;
      break;


    /********************************************
      黒を検知した時ライントレースしていた図形の原点方向へ回転
    *********************************************/
    /*
      case 31://黒を検知した時,direction_G=0になるまで回転
      motorL_G = SPEED;
      motorR_G = -SPEED;
      if (direction_G == 0) {
        targetDir = getDegree(X, Y, color_x, color_y);
        mode_G = 32;
      }
      break;

      case 32://目標座標の方へ回転
      if (targetDir > 0) { //右回転
        motorL_G = SPEED;
        motorR_G = -SPEED;
      } else if (targetDir < 0) { //左回転
        motorL_G = -SPEED;
        motorR_G = SPEED;
      }
      done2 = targetDirection(targetDir);
      if (done2 == 1) {
        done2 = 0;
        mode_G = 33;
      }
      break;

      case 33://目標座標まで前進
      goStraight( 120 );
      if (identifyDistance( color_x, color_y, X, Y) == 1)
      {
        if (RED_F == 1) mode_G = 3;
        if (BLUE_F == 1) mode_G = 4;
        if (GREEN_F == 1) mode_G = 5;
        if (PINK_F == 1) mode_G = 6;
      }
      break;
    */

    /*****************************
      case 90~ 座標の確認
     ******************************/
    /*
      case 90://x座標測定用
      goStraight( 120 );
      if ( identifyColor( 0, 0, 0 ) ) { //黒を検知
       mode_G = 99;
      }
      break;

      case 91://y座標測定用1
      goStraight(120);
      done2 = steadyState(3000);
      if (done2 == 1) {
       done2 = 0;
       mode_G = 92;
      }
      break;

      case 92://y座標測定2
      //右回転
      motorL_G = SPEED;
      motorR_G = -SPEED;
      done2 = targetDirection(90);
      if (done2 == 1) {
       done2 = 0;
       mode_G = 90;
      }

      case 99://座標測定用停止
      motorL_G = 0;
      motorR_G = 0;
      break;
    */

    default:
      break;
  }//switch終

}//全体





//一周判定
int identifyDistance( float color_x, float color_y, float x, float y)
{
  float d2;
  float d2_max = 5; // パラメーター

  d2 = pow(color_x - x, 2) + pow(color_y - y, 2);
  if ( d2 < d2_max * d2_max )
    return 1;
  else
    return 0;
}


//x,y座標の片方を目標
int positionDistance(float color_y,  float y)
{

  if ( color_y - y < 5 && color_y - y > -5)
    return 1;
  else
    return 0;
}

//2点間の角度を計算
float getDegree(float x_now, float y_now, float target_x, float target_y)
{
  double degree, radian;
  radian = atan2((double)(x_now - target_x), (double)(y_now - target_y));
  degree = radian * 180 / PI;
  return (float)degree;
}


/*
  if ( identifyColor( 0, 0, 0 ) )  {
   if ( TurnCount == 0 ) {
    turnRight( SPEED );
    done = steadyState(200); //200ms回転
    if ( done == 1 ) TurnCount++;
   }
   else if ( TurnCount == 1 ) {
    turnRight( -SPEED );
    done = steadyState(400); //400ms回転
    if ( done == 1 ) TurnCount++;
   }
   else if ( TurnCount == 2 ) {
    turnRight( SPEED );
    done = steadyState(600); //600ms回転
    if ( done == 1 ) TurnCount++;
   }
   else if ( TurnCount == 3 ) {
    turnRight( -SPEED );
    done = steadyState(800); //800ms回転
    if ( done == 1 ) TurnCount++;
   }
   else if ( TurnCount == 4 ) {
    turnRight( SPEED );
    done = steadyState(1000); //1000ms回転
    if ( done == 1 ) TurnCount = 0;
   }

  }
  else if ( identifyColor( 255, 255, 255 ) )  { //白を検知
  mode_G = 1;
  TurnCount = 0;
  }
*/


// 指定した方向いたら1を返す（それ以外0)
int turnToDirection1( int direction_target )
{
  float direction_diff;
  direction_diff = direction_G - direction_target; // ターゲット方向からの差異（direction_G の値はloop() 関数で取得）
  // direction_diff の値は-360～360（度）の範囲を取り得るが，-180～180（度）の範囲に変換したい．
  if ( direction_diff > 180.0 )
    direction_diff -= 360.0;
  else if ( direction_diff < -180.0 )
    direction_diff += 360.0;
  // この時点でdirection_diff の値は-180～180（度）になっている．
  if ( fabs(direction_diff) < 1.5 ) // 5.0 はパラメーター
    return 1;
  else
    return 0;
}



