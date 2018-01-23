void position_estimation(float V_L , float V_R) {

  static float V_C = 0;   //中心の速さ
  static float V_C_x1 = 0;   //中心の速さのx方向成分
  static float V_C_y1 = 0;   //中心の速さのy方向成分
  static float V_C_x = 0;   //中心の速さのx方向成分
  static float V_C_y = 0;   //中心の速さのy方向成分
  static float X1 = 0;  //x方向成分の1つ先
  static float Y1 = 0;  //y方向成分の1つ先
  static float dt = 0.03; //0.1秒ごとに現在位置を更新
  static float X_Prev = 0;     //過去のX座標
  static float Y_Prev = 0;     //過去のY座標
  static float X_Now = 0;   //現在のX座標
  static float Y_Now = 0;   //現在のY座標
  static float X_PPrev = 0;
  static float Y_PPrev = 0;
  static float count = 0;

  static float X_PPP = 0;
  static float X_PPPP = 0;
  static unsigned long timePrev2 = 0;
  static bool flag = false;


  count++;

  if (flag == false) {
    timePrev2 = timeNow_G;
    flag = true;
  }

  else if (flag == true) {
    //dt = (timeNow_G - timePrev2) / 1000;
    timePrev2 = timeNow_G;

    V_C = ((V_L + V_R) / 10) / 2;

    V_C_x1 = V_C * cos( (direction_G / 180) * 3.14 );
    V_C_y1 = V_C * sin( (direction_G / 180) * 3.14 );

    X1 = X + V_C_x1 * dt;
    Y1 = Y + V_C_y1 * dt;

    X = X1;
    Y = Y1;

    //X = 1.0;
    /*
      X1 = X + (V_C_x + V_C_x1) / 2 * dt;
      Y1 = Y + (V_C_y + V_C_y1) / 2 * dt;

      V_C_x = V_C_x1;
      V_C_y = V_C_y1;

      X = X1;
      Y = Y1;

    */


  }
}


void suitei() {
  float omega, speed, p, kakudo;
  float dt = 0.03;
  float now_time;
  static float Theta;

  speed = (motorR_G * dt + motorL_G * dt) / 2;
  kakudo = (motorR_G * dt - motorL_G * dt) / (2 * 6);
  p = speed / kakudo;
  if (motorR_G == motorL_G || abs(kakudo) < 0.01) {
    X = X + speed * cos(Theta + (kakudo / 2));
    Y = Y + speed * sin(Theta + (kakudo / 2));
  }

  
  else {

    X = X + (2 * p * sin(kakudo)) * cos(Theta + (kakudo / 2));
    Y = Y + (2 * p * sin(kakudo)) * sin(Theta + (kakudo / 2));
    Theta = Theta + kakudo;
  }
  //println(p);
  //println("kakudo : " + kakudo / 3.14 * 180);
  //println("Theta : " + Theta / 3.14 * 180);
}

