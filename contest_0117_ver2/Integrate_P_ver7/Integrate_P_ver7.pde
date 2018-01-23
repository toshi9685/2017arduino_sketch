
import processing.serial.*;
Serial port; 
float sonic, motor_L, motor_R, accel_X, accel_Y, accel_Z, geomag_X, geomag_Y, geomag_Z, direction;
int zoneNumber, mode, hcolor;
int green, red, blue;
int high, low;
int a, c;
int b;
float time, x, y, Theta;
float x_Prev, y_Prev;

float theta, psi, phi;
int arfa, diff;
float Mu_x;
static float prev_time = 0;
int stone_count, prev_mode;

float map_x;
float map_y;
float map_px;
float map_py;

int dfc, preframe = -180,cnum[]=new int[2];//drawfindcolor
boolean Fflag=false;//drawfindcolor


void setup() {
  //frame.setLocation(0, 500);
  //size(1024, 768, P3D);
  size(1024, 768, P3D);
  PFont font;//①変数fontがフォント変数であると定義
  font = createFont("MS Gothic", 48, true);//ＭＳ明朝 「MS Mincho」
  textFont(font);//③フォント，サイズを指定する

  background(255);
  println(Serial.list());
  //String arduinoPort = Serial.list()[1];
  //port = new Serial(this, arduinoPort, 9600 );
  //port = new Serial(this, "COM4", 9600 ); // シリアルポート名は各自の環境に合わせて適宜指定
  zoneNumber = 3;//テスト用
  mode = 0;

  arfa = 100;
  diff = 0;
  motor_L = 110;
  motor_R = 100;
  red = 255;
  Mu_x = 0;
  direction = 30;
  sonic = 50;
  x = 00;
  y = 30;
  stone_count = 0;
  accel_X = 100;
  accel_Y = 16100;
  accel_Z = 1400;
  geomag_X = 99;
  geomag_Y = 0;
  geomag_Z = 0;
}

void draw() {
  if (zoneNumber == 3) {
    //テスト
    if (frameCount%60 == 0) {
      mode++;
      if (mode == 15) {
        mode = 0;
        hcolor += 1;
        hcolor=hcolor%2;
      }
    }
    //テスト
    hint(ENABLE_DEPTH_SORT); // ←追加
    //draw_3d();
    aaaaaa();
    Draw_findcolor();
  }

  /*
  if (zoneNumber == 0) {
   hint(ENABLE_DEPTH_SORT); // ←追加
   get_Euler();
   Draw_3d();
   }
   */
  hint(DISABLE_DEPTH_SORT);
  fill(255);
  stroke(255);
  rect(0, 0, width, height * 0.46);
  Draw_map();
  Draw_speed();
  Draw_compass();
  Draw_color();
  if (zoneNumber == 2) {
    //zone_task();
    zone_curling();
  }
  if (zoneNumber == 1) {
    suitei();
    zone_zukei();
  }
  stroke(100, 100, 100);
  strokeWeight(1);
  line(0, height*0.44, width, height*0.44);
  line(width * 0.535, 0, width * 0.535, height*0.44);
  line(0, height - 20, width, height - 20);

  /*
  print("theta : ");
   println(theta * 180 / 3.14159265);
   print("psi : ");
   println(psi * 180 / 3.14159265);
   print("phi : ");
   println(phi);
   */
}

int read2byte(Serial p) {
  int x = p.read();  
  x <<= 8;   
  x |= p.read();
  if (x>32757) 
    x -= 65536;
  return x;
}

// 通信方式1
void serialEvent(Serial p) { 

  if ( p.available() >= 31 ) { 
    if ( p.read() == 'H' ) {

      //RGB値
      red =  p.read();
      green = p.read();
      blue =  p.read();
      //地磁気
      //geomag_X = 0.9 * geomag_X + 0.1 * read2byte(p);
      //geomag_Y = 0.9 * geomag_Y + 0.1 * read2byte(p);
      //geomag_Z = 0.9 * geomag_Z + 0.1 * read2byte(p);

      geomag_X = read2byte(p);
      geomag_Y = read2byte(p);
      geomag_Z = read2byte(p);

      //加速度
      //accel_X = 0.9 * accel_X + 0.1 * read2byte(p);
      //accel_Y = 0.9 * accel_Y + 0.1 * read2byte(p);
      //accel_Z = 0.9 * accel_Z + 0.1 * read2byte(p);


      accel_X = read2byte(p);
      accel_Y = read2byte(p);
      accel_Z = read2byte(p);

      //motorspeed
      motor_R = read2byte(p);
      motor_L = read2byte(p);

      //超音波
      sonic = p.read();

      //ゾーンナンバー
      zoneNumber = p.read();

      //ゾーンタスク
      prev_mode = mode;
      mode =  p.read();
      if (zoneNumber == 2 && mode == 10 && prev_mode == 9) {
        stone_count++;
      }

      //方角
      direction = read2byte(p);

      //経過時間 
      high = p.read();
      low = p.read();
      time = ((high << 8 ) + low) / 1000;
      //time = read2byte(p)/1000;

      //位置推定
      x = read2byte(p);
      y = read2byte(p);

      if (zoneNumber == 3)hcolor = p.read();


      p.clear(); // 念のためクリア
      p.write("A");

      //print("  RGB = ");
      // println(red, green, blue);
      // print("(geomag_X,geomag_Y,geomag_Z) = ");
      // println(geomag_X, geomag_Y, geomag_Z);
      // print("(accel_X,accel_Y,accel_Z) = ");
      // println(accel_Z, accel_Y, accel_Z);
      // print("motorspeed =(L,R)");
      // println(motor_L, motor_R);
      // print("距離");
      // println(sonic);
      // print("ゾーンナンバー = ");
      // println(zoneNumber);
      // print("タスクナンバー = ");
      // println(mode);
      //print("方角 = ");
      //println(direction);

      // print("theta : ");
      // println(theta * 180 / 3.14159265);
      // print("psi : ");
      // println(psi * 180 / 3.14159265);
      // print("phi : ");
      // println(phi  * 180 / 3.14159265);


      //print("x = ");
      //println(x);
      //print("y = ");
      //println(y);
    }
  }
}


void get_Euler() {
  float Normaliz_ax, Normaliz_ay, Normaliz_az;
  float Normaliz_mx, Normaliz_my, Normaliz_mz;
  float m0_x, m0_y, m0_z;
  float cos_theta, sin_theta, cos_psi, sin_psi, cos_phi, sin_phi = 0.0;

  float Size_a = sqrt(pow(accel_X, 2) + pow(accel_Y, 2) + pow(accel_Z, 2));
  float Size_m = sqrt(pow(geomag_X, 2) + pow(geomag_Y, 2) + pow(geomag_Z, 2));
  float Size_m0;

  Normaliz_ax = accel_X / Size_a;
  Normaliz_ay = accel_Y / Size_a;
  Normaliz_az = accel_Z / Size_a;
  Normaliz_mx = geomag_X / Size_m;
  Normaliz_my = geomag_Y / Size_m;
  Normaliz_mz = geomag_Z / Size_m;


  println(Normaliz_ax);
  println(Normaliz_ay);
  //println(Normaliz_az);
  //println(Normaliz_mx);
  //println(Normaliz_my);
  //println(Normaliz_mz);


  cos_theta = Normaliz_az;
  sin_theta = Math.signum(Normaliz_ay) * sqrt(1 - pow(Normaliz_az, 2));
  theta = atan2(sin_theta, cos_theta);

  if (abs(sin_theta) < 0.2) {
    println("aaaaaaaaaaaaaaaaaaaaaaaaaaa");
    //psi = atan2(-Normaliz_my, Normaliz_mx);
    psi = direction * 3.14 / 180;
  } else {
    sin_psi = Normaliz_ax / sin_theta;
    cos_psi = Normaliz_ay / sin_theta;
    println("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    psi = atan2(sin_psi, cos_psi);
    //println("psi : "  + psi * 180 / 3.14);
    phi = 0;
  }

  if (abs(accel_X) < 100 && abs(accel_Y) < 100) {
    m0_x = geomag_X;
    m0_y = geomag_Y;
    m0_z = geomag_Z;
    Size_m0 = sqrt(pow(m0_x, 2) + pow(m0_y, 2) + pow(m0_z, 2));
    Mu_x = (sqrt(pow(m0_x, 2) + pow(m0_y, 2))) / Size_m0;
    //println("Mu_x : " + Mu_x);
  }
  if (Mu_x > 0) {
    cos_phi = (1.0 / Mu_x) * 
      ((cos(psi) * (Normaliz_mx - (Mu_x * Normaliz_ax))) 
      + (-sin(psi) * (Normaliz_my - (Mu_x * Normaliz_ay))));
    sin_phi = (1.0 / Mu_x) * 
      (((-cos(theta) * sin(psi)) * (Normaliz_mx - (Mu_x * Normaliz_ax))) 
      +  ((-cos(theta) * cos(psi)) * (Normaliz_my - (Mu_x * Normaliz_ay)))
      +  ((sin(theta))             * (Normaliz_my - (Mu_x * Normaliz_ay))));

    phi = atan2(sin_phi, cos_phi);
  }
  /*
  print("theta : ");
   println(theta * 180 / 3.14159265);
   print("psi : ");
   println(psi * 180 / 3.14159265);
   print("phi : ");
   println(phi * 180 / 3.14159265);
   */
}

void Draw_map() {
  float zone_pos[][] = {
    {0, 0, 0, 0}, 
    {60, 20, 100, 110}, 
    {200, 20, 100, 110}, 
    {340, 20, 260, 240}, 
    {340, 300, 260, 60}};

  stroke(0);
  strokeWeight(1);
  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  scale(0.90, 0.90);
  fill(0);
  rect(30, 10, 150, 130);
  fill(255);
  rect(60, 20, 110, 110);
  fill(255, 0, 0);
  rect(40, 10, 10, 130);
  fill(255, 255, 0);
  rect(20, 10, 10, 130);
  rect(160, 20, 10, 110);
  translate(160, 0);  
  fill(0);
  rect(10, 10, 150, 130);
  fill(255);
  rect(40, 20, 110, 110);
  fill(0, 255, 0);
  rect(20, 10, 10, 130);
  fill(255, 255, 0);
  rect(140, 20, 10, 110);
  translate(160, 0);  
  fill(0);
  rect(10, 10, 270, 270);
  fill(255);
  rect(20, 20, 250, 250);
  fill(0, 0, 255);
  rect(0, 10, 10, 130);
  translate(0, 250);
  fill(255, 255, 0);
  rect(20, 10, 250, 10);
  rect(10, 30, 270, 10);
  fill(0);
  rect(10, 40, 270, 80);
  fill(255);
  rect(20, 50, 250, 60);
  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)

  if (zoneNumber != 0) {
    fill(255, 0, 255, arfa);
    rect(zone_pos[zoneNumber][0]  * 0.90, zone_pos[zoneNumber][1]  * 0.9, zone_pos[zoneNumber][2] * 0.9, zone_pos[zoneNumber][3] * 0.9);
    if (arfa >= 100) {
      diff = -2;
    } else if (arfa <= 10) {
      diff = 2;
    }
    arfa += diff;
  }

  stroke(255);
  fill(255);
  //rect(0, height * 0.45 -60, 200, 50);

  fill(0);
  textSize(34);
  text("経過時間: ", 10, height * 0.28);
  text(time, 165, height * 0.28);
  //textSize(40);
  text("ゾーン番号: ", 10, height * 0.35);
  text((int)zoneNumber, 220, height * 0.35);
  text("モード番号: ", 10, height * 0.45 -20);
  text((int)mode, 220, height * 0.45 -20);
  noFill();
}

void Draw_speed() {
  float right_theta, left_theta;
  float right_x, right_y, left_x, left_y;
  left_theta = map(motor_L, -300, 300, 0, -PI);
  right_theta = map(motor_R, -300, 300, 0, -PI);
  left_x = -cos(left_theta) * 85 + 120;
  left_y = sin(left_theta) * 85 + 110;
  right_x = -cos(right_theta) * 85 + 350;
  right_y = sin(right_theta) * 85 + 110; 
  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  translate(width * 0.535, 0);
  stroke(0);
  fill(255, 255, 255);
  strokeWeight(1);
  for (int i = 0; i <= 180; i++) {
    if (i % 30 == 0) {
      strokeWeight(3);
      line(85*cos(radians(i))+120, 85*sin(-radians(i))+110, 70*cos(radians(i))+120, 70*sin(-radians(i))+110);
      line(85*cos(radians(i))+350, 85*sin(-radians(i))+110, 70*cos(radians(i))+350, 70*sin(-radians(i))+110);
    } else if (i % 6 == 0) {
      strokeWeight(1);
      line(85*cos(radians(i))+120, 85*sin(-radians(i))+110, 75*cos(radians(i))+120, 75*sin(-radians(i))+110);
      line(85*cos(radians(i))+350, 85*sin(-radians(i))+110, 75*cos(radians(i))+350, 75*sin(-radians(i))+110);
    }
  }
  arc(120, 110, 170, 170, -PI, 0);
  arc(350, 110, 170, 170, -PI, 0);
  fill(0);
  ellipse(120, 110, 12, 12);
  ellipse(350, 110, 12, 12);
  line(35, 110, 200, 110);
  line(270, 110, 430, 110);
  stroke(255, 0, 0, 180);
  strokeWeight(7);
  line(120, 110, left_x, left_y);
  line(350, 110, right_x, right_y);
  strokeWeight(1);
  stroke(255);
  fill(255);
  rect(10, 120, 450, 30);
  textSize(22);
  fill(0);
  text("motor_L = " + motor_L, 30, 145);

  text("motor_R = " + motor_R, 260, 145);

  textSize(15);
  text(0, 115, 15);
  text(-100, 45, 28);
  text(-200, 5, 60);
  text(100, 165, 28);
  text(200, 196, 60);
  text(0, 345, 15);
  text(-100, 275, 28);
  text(-200, 235, 60);
  text(100, 395, 28);
  text(200, 426, 60);

  // 適応変換
  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)
}

void Draw_3d() {
  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  //translate(width * 0.6, height*0.45);
  /*
  camera(400.0, -300.0, 1000.0, // 視点X, 視点Y, 視点Z
   -600.0, -300.0, 0.0, // 中心点X, 中心点Y, 中心点Z
   0.0, 1.0, 0.0); // 天地X, 天地Y, 天地Z
   */

  camera(250.0, -300.0, 1000.0, // 視点X, 視点Y, 視点Z
    -500.0, -200.0, 0.0, // 中心点X, 中心点Y, 中心点Z
    0.0, 1.0, 0.0); // 天地X, 天地Y, 天地Z

  background(255);

  // x軸を示す赤色の線
  stroke(255, 0, 0);
  line(0, 0, 0, 200, 0, 0);
  // y軸を示す緑色の線
  stroke(0, 255, 0);
  line(0, 0, 0, 0, -200, 0);
  // x軸を示す青色の線
  stroke(0, 0, 255);
  line(0, 0, 0, 0, 0, 200);


  //rotateX(-theta);
  rotateY(-psi);
  //rotateY(direction * 3.14 / 180);
  //rotateY(-phi);


  fill(255, 255, 255);

  scale(100);    //拡大
  beginShape(QUADS);

  //正面
  stroke(0, 0, 0);
  strokeWeight(0.02);
  fill(255, 0, 0, 70);
  vertex(-1, 1, 1);
  vertex( 1, 1, 1);
  vertex( 1, -1, 1);
  vertex(-1, -1, 1);
  //右
  fill(128, 128, 128, 50);
  vertex( 1, 1, 1);
  vertex( 1, 1, -1);
  vertex( 1, -1, -1);
  vertex( 1, -1, 1);
  //後ろ
  fill(0, 0, 255, 50); 
  vertex( 1, 1, -1);
  vertex(-1, 1, -1);
  vertex(-1, -1, -1);
  vertex( 1, -1, -1);
  //左
  fill(128, 128, 128, 50);
  vertex(-1, 1, -1);
  vertex(-1, 1, 1);
  vertex(-1, -1, 1);
  vertex(-1, -1, -1);
  //下
  fill(128, 128, 128, 50);
  vertex(-1, 1, -1);
  vertex( 1, 1, -1);
  vertex( 1, 1, 1);
  vertex(-1, 1, 1);
  //上
  fill(128, 128, 128, 50);
  vertex(-1, -1, -1);
  vertex( 1, -1, -1);
  vertex( 1, -1, 1);
  vertex(-1, -1, 1);

  endShape();

  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)
}


void Draw_compass() {
  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  scale(0.93);
  translate(width * 0.68, height * 0.32);
  stroke(0);
  fill(187, 255, 255);
  ellipse(0, 0, 155, 155);
  fill(255);
  ellipse(0, 0, 140, 140);
  textSize(20);
  fill(0);
  text("N", -7, -50);
  text("S", -5, 68);
  text("E", 55, 5);
  text("W", -67, 5);
  text("方角:", -50, 110);
  if (direction > 348.75 || direction < 11.25) {
    text("北", 0, 110);
  } else if (direction > 11.25 && direction < 33.75) {
    text("北北東", 0, 110);
  } else if (direction > 33.75 && direction < 56.25) {
    text("北東", 0, 110);
  } else if (direction > 56.25 && direction < 78.75) {
    text("東北東", 0, 110);
  } else if (direction > 78.75 && direction < 101.25) {
    text("東", 0, 110);
  } else if (direction > 101.25 && direction < 123.75) {
    text("東南東", 0, 110);
  } else if (direction > 123.75 && direction < 146.25) {
    text("南東", 0, 110);
  } else if (direction > 146.25 && direction < 168.75) {
    text("南南東", 0, 110);
  } else if (direction > 168.75 && direction < 191.25) {
    text("南", 0, 110);
  } else if (direction > 191.25 && direction < 213.75) {
    text("南南西", 0, 110);
  } else if (direction > 213.75 && direction < 236.25) {
    text("南西", 0, 110);
  } else if (direction > 236.25 && direction < 258.75) {
    text("西南西", 0, 110);
  } else if (direction > 258.75 && direction < 281.25) {
    text("西", 0, 110);
  } else if (direction > 281.25 && direction < 303.75) {
    text("西北西", 0, 110);
  } else if (direction > 303.75 && direction < 326.25) {
    text("北西", 0, 110);
  } else if (direction > 326.25 && direction < 348.75) {
    text("北北西", 0, 110);
  }

  fill(255, 0, 0);
  rotate(radians(direction));
  triangle(-10, 0, 0, -50, 10, 0);
  fill(255);
  triangle(-10, 0, 0, 50, 10, 0);
  fill(200, 200, 200);
  ellipse(0, 0, 10, 10);

  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)
}

void Draw_color() {
  float red_length, green_length, blue_length;
  red_length = map(red, 0, 255, 0, -130);
  green_length = map(green, 0, 255, 0, -130);
  blue_length = map(blue, 0, 255, 0, -130);
  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  translate(width * 0.77, height * 0.24);
  fill(255);
  rect(0, 130, 15, -130);
  rect(30, 130, 15, -130);
  rect(60, 130, 15, -130);
  noStroke();
  fill(red, 0, 0);
  rect(0, 130, 15, red_length);
  fill(0, green, 0);
  rect(30, 130, 15, green_length);
  fill(0, 0, blue);
  rect(60, 130, 15, blue_length);
  textSize(17);
  fill(0);
  text("R", 3, -5);
  text("G", 33, -5);
  text("B", 63, -5);
  text(red, -6, 150);
  text(green, 27, 150);
  text(blue, 57, 150);
  fill(red, green, blue);
  stroke(0);
  rect(90, 0, 90, 130);
  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)
}


void zone_task() {
  float map_distance;

  map_distance = map(sonic, 0, 60, 0, 180);


  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  translate(0., height * 0.46);
  fill(205);
  stroke(255);
  rect(0., 0, width, height * 0.54);

  fill(245);
  stroke(255);
  strokeWeight(1);
  rect(0, 25, 280, 40);
  textSize(20);
  fill(0);
  text(sonic, 10, 50);

  translate(700, 210);
  stroke(0);
  strokeWeight(2);
  fill(255, 255, 255);
  for (int i = 3; i > 0; i--) {
    ellipse(0, 0, 120 * i, 120 * i);
  }


  stroke(0);
  strokeWeight(1);
  rotate(radians(direction));
  fill(0, 255, 0);
  triangle(-10, 5, 0, -15, 10, 5);
  fill(255, 0, 0);
  ellipse(0, -map_distance, 10, 10);

  //sonic -= 0.2;
  //direction += 1.0;

  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)
}






void zone_zukei() {


  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納


  /*
  fill(255);
   stroke(0, 0, 0);
   strokeWeight(10);
   rect(300, 15, 400, 380);
   */
  /*  stroke(0, 0, 0);
   strokeWeight(10);
   line(300, 15, 300, 395);
   line(300, 15, 700, 15);
   line(300, 395, 700, 395);
   line(700, 15, 700, 395);
   */

  //トレースした線の色
  if (mode == 1) {//白色(確認のため黒にしておく)
    stroke(255, 255, 255, 255);
    fill(255, 255, 255, 255);
  } else if (mode == 3) {//赤色
    stroke(255, 0, 0);
    //fill(255, 0, 0);
  } else if (mode == 4) {//青色
    stroke(0, 0, 255);
    //fill(0, 0, 255);
  } else if (mode == 5) {//緑色
    stroke(0, 255, 0);
    //fill(0, 255, 0);
  } else if (mode == 6) {//ピンク色
    stroke(255, 182, 193);
    //fill(181, 83, 64);
  }

  //ellipse(x + 500, y + 200, 6, 6);
  strokeWeight(5);
  translate(0, 384);
  if (x != 0 && x_Prev != 0 ) {
    map_x = map(x, -10, 150, 155, 340);
    map_y = map(y, 20, -150, 140, 325);
    map_px = map(x_Prev, -10, 155, 150, 340);
    map_py = map(y_Prev, 20, -150, 140, 325);
    line(map_x+300, map_y+240, map_px+300, map_py+240);

    //point(map_x+300, map_y+240);
    //line(x + 300, y + 307, x_Prev + 300, y_Prev + 307);
  }

  fill(245);
  stroke(255);
  strokeWeight(1);
  rect(10, 5, 280, 40);

  textSize(20);
  fill(0);
  text("(x , y) =  (" + (int)x + " , " + (int)y + ")", 20, 30);
  //text(x, 50, 30);
  //text(y, 50, 250);


  x_Prev = x;
  y_Prev = y;

  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)
}


void zone_curling() {
  //x += 0.2;

  float map_distance;
  float distance;
  float zumo_x, zumo_y;

  map_distance = map(sonic, 0, 80, 0, 360);
  zumo_x = map(x, 200, -80, 0, 427.5);
  zumo_y = map(y, 0, 280, 370, 10);
  distance = map(sonic, 0, 100, 0, 500);

  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  translate(0, height * 0.44);
  fill(255);
  stroke(255);
  rect(0., 0, width, height * 0.56);

  textSize(30);
  fill(0);

  text("X:" + x + "  Y:" + y, 10, 300);
  text("確保したストーン数 : " + stone_count, 10, 370);

  if (0 < sonic) {
    fill(0, 255, 0, 200);
    rect( 10, 80, distance, 70);
    fill(0);
    text("ストーンまでの距離 " + sonic + " cm", 10, 50);
  } else {
    fill(0);
    text("ストーン探索中、、、 ", 10, 50);
  }
  stroke(0);
  strokeWeight(3);
  noFill();
  rect( 10, 80, 500, 70);  


  textSize(20);
  fill(0);
  text(0, 7, 175);
  for (int i = 1; i <= 100; i++) {
    if (i % 10 == 0) {
      strokeWeight(2);
      line(5 * i + 10, 127, 5 * i + 10, 150);
      textSize(20);
      fill(0);
      text(i, 5 * i, 175);
    } else if (i % 5 == 0) {
      strokeWeight(2);
      line(5 * i + 10, 135, 5 * i + 10, 150);
    } else {
      strokeWeight(1);
      line(5 * i + 10, 140, 5 * i + 10, 150);
    }
  }




  translate(550, 5);


  strokeWeight(1);
  fill(0);
  noStroke();
  rect(0, 0, 447.5, 390);
  fill(255);
  rect(10, 10, 427.5, 370);
  fill(0, 255, 0);
  stroke(0);
  rect(0, 390, 447.5, 10);
  fill(255, 255, 0);
  rect(10, 10, 427.5, 10);

  translate(223.75, 200);

  noFill();

  //strokeWeight(8);
  fill(0, 0, 255);
  noStroke();
  ellipse(0, 0, 270, 270);
  fill(255);
  ellipse(0, 0, 255, 255);
  fill(255, 0, 0);
  ellipse(0, 0, 135, 135);
  fill(255);
  ellipse(0, 0, 120, 120);

  translate(-223.75, -200);

  //rotate(radians(direction));
  fill(255, 0, 0);
  stroke(0);
  strokeWeight(2);
  //popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)
  //pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  translate(zumo_x + 10, zumo_y + 20);
  rotate(radians(direction - 90));
  //triangle(-15, 20, 15, 20, 0, -20);
  beginShape();
  vertex(0, -25);
  vertex(-20, 25);
  vertex(0, 10);
  vertex(20, 25);

  endShape(CLOSE);

  fill(0, 255, 0);
  ellipse(0, -map_distance, 30, 30);


  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)
}


void suitei() {
  float omega, speed, p, kakudo;
  float dt = 0.002;
  float now_time;

  now_time = millis();
  if (now_time - prev_time > 2000) {
    println("OJ");
    prev_time = now_time;
    motor_L = (int)(Math.random()*100 + 80);
    motor_R = (int)(Math.random()*100 + 80);
  }

  speed = (motor_L * dt + motor_R * dt) / 2;
  kakudo = (motor_L * dt - motor_R * dt) / (2 * 6);
  p = speed / kakudo;

  //x = x + speed * cos(Theta + (kakudo / 2));
  //y = y + speed * sin(Theta + (kakudo / 2));

  x = x + (2 * p * sin(kakudo)) * cos(Theta + (kakudo / 2));
  y = y + (2 * p * sin(kakudo)) * sin(Theta + (kakudo / 2));
  Theta = Theta + kakudo;

  //println(p);
  //println("kakudo : " + kakudo / 3.14 * 180);
  //println("Theta : " + Theta / 3.14 * 180);
}


void draw_3d() {
  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  /*
  camera(250.0, -300.0, 1000.0, // 視点X, 視点Y, 視点Z
   -500.0, -200.0, 0.0, // 中心点X, 中心点Y, 中心点Z
   0.0, 1.0, 0.0); // 天地X, 天地Y, 天地Z
   */
  background(255);

  float a = sqrt(pow(accel_X, 2) + pow(accel_Y, 2) + pow(accel_Z, 2));
  float m = sqrt(pow(geomag_X, 2) + pow(geomag_Y, 2) + pow(geomag_Z, 2));

  float hx = geomag_Y * accel_Z - geomag_Z * accel_Y;
  float hy = geomag_Z * accel_X - geomag_X * accel_Z;
  float hz = geomag_X * accel_Y - geomag_Y * accel_X;
  float h = sqrt(hx*hx + hy*hy + hz*hz);

  float rx = accel_Y * hz - accel_Z * hy; 
  float ry = accel_Z * hx - accel_X * hz;
  float rz = accel_X * hy - accel_Y * hx;
  float r = sqrt(rx*rx + ry*ry + rz*rz);

  applyMatrix(hx/h, hy/h, hz/h, 0.0, 
    -rx/r, -ry/r, -rz/r, 0.0, 
    accel_X/a, accel_Y/a, accel_Z/a, 0.0, 
    0.0, 0.0, 0.0, 1.0);

  fill(255, 255, 255);

  scale(100);    //拡大
  beginShape(QUADS);

  //正面
  stroke(0, 0, 0);
  strokeWeight(0.02);
  fill(255, 0, 0, 70);
  vertex(-1, 1, 1);
  vertex( 1, 1, 1);
  vertex( 1, -1, 1);
  vertex(-1, -1, 1);
  //右
  fill(128, 128, 128, 50);
  vertex( 1, 1, 1);
  vertex( 1, 1, -1);
  vertex( 1, -1, -1);
  vertex( 1, -1, 1);
  //後ろ
  fill(0, 0, 255, 50); 
  vertex( 1, 1, -1);
  vertex(-1, 1, -1);
  vertex(-1, -1, -1);
  vertex( 1, -1, -1);
  //左
  fill(128, 128, 128, 50);
  vertex(-1, 1, -1);
  vertex(-1, 1, 1);
  vertex(-1, -1, 1);
  vertex(-1, -1, -1);
  //下
  fill(128, 128, 128, 50);
  vertex(-1, 1, -1);
  vertex( 1, 1, -1);
  vertex( 1, 1, 1);
  vertex(-1, 1, 1);
  //上
  fill(128, 128, 128, 50);
  vertex(-1, -1, -1);
  vertex( 1, -1, -1);
  vertex( 1, -1, 1);
  vertex(-1, -1, 1);

  endShape();




  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)
}


void aaaaaa() {

  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  translate(0, height * 0.5);
  background(255);

  // calculation of rotation matrix

  float a = sqrt(pow(accel_X, 2) + pow(accel_Y, 2) + pow(accel_Z, 2));
  float m = sqrt(pow(geomag_X, 2) + pow(geomag_Y, 2) + pow(geomag_Z, 2));

  float hx = geomag_Y * accel_Z - geomag_Z * accel_Y;
  float hy = geomag_Z * accel_X - geomag_X * accel_Z;
  float hz = geomag_X * accel_Y - geomag_Y * accel_X;
  float h = sqrt(hx*hx + hy*hy + hz*hz);

  float rx = accel_Y * hz - accel_Z * hy; 
  float ry = accel_Z * hx - accel_X * hz;
  float rz = accel_X * hy - accel_Y * hx;
  float r = sqrt(rx*rx + ry*ry + rz*rz);

  fill(255, 127, 63);
  // display heading info in degrees
  float head = atan2(hx/h, rx/r);
  text(int(degrees(head)), 100, 70);

  fill(63, 127, 255);
  text(accel_X, 100, 20);
  text(accel_Y, 200, 20);
  text(accel_Z, 300, 20);

  fill(255, 127, 63);
  text(geomag_X, 100, 40);
  text(geomag_Y, 200, 40);
  text(geomag_Z, 300, 40);
  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)


  pushMatrix(); //(0, 0)を原点とする座標軸をスタックに格納
  translate(width/2, height/2);

  stroke(255);
  lights();
  hint(DISABLE_DEPTH_SORT);



  // display data using vector
  translate(200, 80);
  fill(63, 127, 255);
  beginShape();
  vertex(0, 0, 0);
  vertex(0, 10, 0);
  vertex(accel_X/a*20, accel_Y/a*20, accel_Z/a*20);
  endShape(CLOSE);

  fill(255, 127, 63);
  beginShape();
  vertex(0, 0, 0);
  vertex(0, 10, 0);
  vertex(geomag_X/m*20, geomag_Y/m*20, geomag_Z/m*20);
  endShape(CLOSE);
  translate(-200, -80);

  // setup coordinates for board picture
  translate(width/2 - 300, height/2 - 150);
  rotateX(radians(90)); // move Z-axis up
  rotateZ(radians(180)); // move X-axis front


  //camera(200.0, -60.0, 250.0, // 視点X, 視点Y, 視点Z
  //  -50.0, -40.0, 0.0, // 中心点X, 中心点Y, 中心点Z
  //  0.0, 1.0, 0.0); // 天地X, 天地Y, 天地Z

  // x軸を示す赤色の線
  stroke(255, 0, 0);
  line(0, 0, 0, -200, 0, 0);
  // y軸を示す緑色の線
  stroke(0, 255, 0);
  line(0, 0, 0, 0, -200, 0);
  // z軸を示す青色の線
  stroke(0, 0, 255);
  line(0, 0, 0, 0, 0, 200);


  hint(ENABLE_DEPTH_SORT); // ←追加
  // rotate by the matrix
  applyMatrix(hx/h, hy/h, hz/h, 0.0, 
    -rx/r, -ry/r, -rz/r, 0.0, 
    accel_X/a, accel_Y/a, accel_Z/a, 0.0, 
    0.0, 0.0, 0.0, 1.0);

  // Draw board

  scale(2.0);
  fill(255, 63, 63, 130);
  box(50, 50, 10);
  translate(0, 0, 5);
  fill(0, 0, 0);
  box(10, 10, 3);
  translate(0, 0, -5);
  translate(28, 0, 0);
  fill(255, 0, 0);
  box(4, 10, 10);

  /*
  scale(60);    //拡大
   beginShape(QUADS);
   
   //正面
   stroke(0, 0, 0);
   strokeWeight(0.02);
   fill(128, 128, 128, 50);
   vertex(-1, 1, 1);
   vertex( 1, 1, 1);
   vertex( 1, -1, 1);
   vertex(-1, -1, 1);
   //右
   fill(255, 0, 0, 128);
   vertex( 1, 1, 1);
   vertex( 1, 1, -1);
   vertex( 1, -1, -1);
   vertex( 1, -1, 1);
   //後ろ
   fill(128, 128, 128, 50); 
   vertex( 1, 1, -1);
   vertex(-1, 1, -1);
   vertex(-1, -1, -1);
   vertex( 1, -1, -1);
   //左
   fill(0, 0, 255, 128);
   vertex(-1, 1, -1);
   vertex(-1, 1, 1);
   vertex(-1, -1, 1);
   vertex(-1, -1, -1);
   //下
   fill(128, 128, 128, 50);
   vertex(-1, 1, -1);
   vertex( 1, 1, -1);
   vertex( 1, 1, 1);
   vertex(-1, 1, 1);
   //上
   fill(128, 128, 128, 50);
   vertex(-1, -1, -1);
   vertex( 1, -1, -1);
   vertex( 1, -1, 1);
   vertex(-1, -1, 1);
   
   endShape();
   */
  popMatrix(); //座標軸の位置をスタックから取り出すし設定する ... この場合(0, 0)
}

void Draw_findcolor() {
  fill(0);
  textSize(34);

  text("検出した回数", width*0.4, height * 0.75);
  text("青:"+cnum[0], width*0.4, height * 0.8);
  text("赤:"+cnum[1], width*0.4, height * 0.85);
  if (frameCount < preframe+180) {//180frameの間見つけた色を表示
    text("発見!!", width*0.2, height * 0.65);
    if (hcolor==0) {
      fill(0, 0, 255);
    } else {
      fill(255, 0, 0);
    }
  } else {//探索中の描画に戻す
    Fflag=false;
  }
  if (mode == 10 && Fflag==false) {//見つけたとき
    Fflag=true;//見つけたフラグ立てる
    preframe = frameCount;//そのときのフレームを取得
    cnum[hcolor] += 1;
  } else if (Fflag==false) {
    if (dfc==0) {
      text("探索中", width*0.2, height * 0.65);
    } else if (dfc == 1) {
      text("探索中 .", width*0.2, height * 0.65);
    } else if (dfc == 2) {
      text("探索中 . .", width*0.2, height * 0.65);
    } else if (dfc == 3) {
      text("探索中 . . .", width*0.2, height * 0.65);
    }
    if (frameCount%60 == 0) {
      dfc++;
      if (dfc==4)dfc=0;
    }
    fill(0, 0, 0);//黒で塗りつぶし
  }
  rect(width*0.2-50, height * 0.6+50, 200, 200);
}