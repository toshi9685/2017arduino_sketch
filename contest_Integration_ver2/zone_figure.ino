void zone_figure_trace()
{
  
  //static float lightMax =  255;//(255 + 255 + 255) / 3.0;// 各自で設定
  static float speed = 150; // パラメーター
  float speedDiff;
  float Kp = 1.7; // パラメーター
  static int TurnCount = 0;
  
  int done = 0;
  float lightNow;//現在のRGB値の平均値

  static float lightMin =  24;// ライントレースする線上のlightNowの値.
  static float lightMax = 255;// 白地のlightNowの値.

  //ライントレース時に
  static float lightMin1 = (183 + 40 + 24) / 3.0;// 赤色のlightNowの値
  static float lightMin2 = (12 + 33 + 104) / 3.0;// 青色のlightNowの値.
  static float lightMin3 = (84 + 179 + 89) / 3.0;// 緑色のlightNowの値.
  static float lightMin4 = (134 + 55 + 89) / 3.0;// ピンクのlightNowの値.

  //各色を発見できたかのフラグ(初期値:0,発見時:1)
  static int RED_F = 0;
  static int BLUE_F = 0;
  static int GREEN_F = 0;
  static int PINK_F = 0;

  int time_flag = 0;
  int count = 0;
  int wait_flag = 0;

  
  switch(mode_G){

    case 0://セットアップ
      mode_G = 1;
      break;
    
    case 1://各色を検知するまで直進
      if ( identifyColor( 183, 32, 24 ) ) { // 赤を検知
        RED_F = 1;
        mode_G = 2;
      }
      else if ( identifyColor( 15, 31, 112 ) ) { //青を検知
        BLUE_F = 1;
        mode_G = 2;
      }
      else if ( identifyColor( 56, 144, 64 ) ) { //緑を検知
        GREEN_F = 1;
        mode_G = 2;
      }
      else if ( identifyColor( 181, 83, 64 ) ) { //ピンクを検知
        PINK_F = 1;
        mode_G = 2;
      }         
      else if (identifyColor(255,255,97)){//黄色を検知
        mode_G = 3;
      }
      else if ( identifyColor( 0, 0, 0 ) ) { //黒を検知
        mode_G = 10;
      }
      
      goStraight( 120 );
      
     break;


  case 2://白を見つけるまで左回転
        if (identifyColor(255, 255, 255)) {
        mode_G = 3;
      }
      motorL_G = -90;
      motorR_G = 90;
      break;

    case 3://P制御によるライントレース
     time_flag = steadyState(5000); 
     lightNow = (red_G + green_G + blue_G ) / 3.0;

       //赤をライントレース
       if (RED_F == 1) {      
        if ( lightNow < (lightMin1 + lightMax) / 2.0 ) // 左回転
          speedDiff = map(lightNow, lightMin1, lightMax, -Kp*speed, Kp*speed);
        else // 右回転
          speedDiff = map(lightNow, lightMin1, lightMax, -Kp*speed, Kp*speed);
      }

       //青をライントレース
       if (BLUE_F == 1) {      
        if ( lightNow < (lightMin2 + lightMax) / 2.0 ) // 左回転
          speedDiff = map(lightNow, lightMin2, lightMax, -Kp*speed, Kp*speed);
        else // 右回転
          speedDiff = map(lightNow, lightMin2, lightMax, -Kp*speed, Kp*speed);
      }

      //緑をライントレース
      if (GREEN_F == 1) {      
        if ( lightNow < (lightMin3 + lightMax) / 2.0 ) // 左回転
          speedDiff = map(lightNow, lightMin3, lightMax, -Kp*speed, Kp*speed);
        else // 右回転
          speedDiff = map(lightNow, lightMin3, lightMax, -Kp*speed, Kp*speed);
      }     
      
      //ピンクをライントレース
      if (PINK_F == 1) {      
        if ( lightNow < (lightMin4 + lightMax) / 2.0 ) // 左回転
          speedDiff = map(lightNow, lightMin4, lightMax, -Kp*speed, Kp*speed);
        else // 右回転
          speedDiff = map(lightNow, lightMin4, lightMax, -Kp*speed, Kp*speed);
      }

        //移動
        if( speedDiff < 0 ){
          motorL_G = speed + speedDiff;
          motorR_G = speed;// + speedDiff;
        }
        else{
          motorL_G = speed;//+ - speedDiff;
          motorR_G = speed - speedDiff;
        }
        
        //5秒間回ると脱出  
        if(time_flag ==1){
          mode_G = 1;
          time_flag = 0;
          }
        break;


    case 10://黒を検知した時の回避
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

      if(count%2==0)
        turnRight(200);
      else
        turnRight(-200);
        //時間を計測
      wait_flag = steadyState(400 + count*400);
      //指定時間経過
      if(wait_flag == 1)
      {
        if(identifyColor(0,0,0)==0)
        {
          mode_G = 1;
          count = 0;
          }
        count++;
        }
      break;
       
      case 11://zoneTozoneへ
      zoneNumber_G = 0;
      mode_G = 0;
      break;

      default:
        break;
    }//switch終
  
  }//全体


