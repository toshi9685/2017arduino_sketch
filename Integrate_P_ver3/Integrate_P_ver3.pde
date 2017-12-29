
import processing.serial.*;
Serial port; 
float sonic, motor_L, motor_R, accel_X, accel_Y, accel_Z, geomag_X, geomag_Y, geomag_Z, direction,pitch,roll;
int zoneNumber,mode,hcolor;
int green, red, blue, green_p, red_p, blue_p;
int high, low;
int a, c;
int b;
float direction_G, time;
int[] sensors = new int[5];
int count;
int[] sensors_p = new int[5];
float theta, psi, phi;
int arfa, diff;
float Mu_x;

void setup() {
  //frame.setLocation(0, 500);
  size(1024, 768, P3D);
  PFont font;//①変数fontがフォント変数であると定義
  font = createFont("MS Gothic", 48, true);//ＭＳ明朝 「MS Mincho」
  textFont(font);//③フォント，サイズを指定する

  background(255);
  count = 0;
  println(Serial.list());
  //String arduinoPort = Serial.list()[1];
  //port = new Serial(this, arduinoPort, 9600 );
  //port = new Serial(this, "COM4", 9600 ); // シリアルポート名は各自の環境に合わせて適宜指定
  port = new Serial(this, "/dev/cu.usbserial-A90176XN", 9600 );
  zoneNumber = 3;
  mode = 0;
  red_p = 0; 
  green_p = 0; 
  blue_p = 0;
  arfa = 100;
  diff = 0;
  motor_L = -100;
  motor_R = 150;
  
  accel_X = -10500;
   accel_Y = -600;
   accel_Z = 13600;
   geomag_X = -70;
   geomag_Y = -93;
   geomag_Z = -12;
   
  Mu_x = 0;
  direction = 170;
  red = 155;
  green = 250;
  blue = 200;
  theta = 0;
  psi = 0.5;
  phi = 0;
  time = 30.0;
}

void draw() {

  //テスト用
  /*direction += 0.01;
  if(direction > 360)direction = 0;*/
  pitch = map(accel_X,-17000,17000,-360,360);
  roll = map(accel_Y,-17000,17000,-360,360);
  float y_p, y;

  hint(ENABLE_DEPTH_SORT); // ←追加
  get_Euler();
  Draw_3d();
  hint(DISABLE_DEPTH_SORT);
  Draw_map();
  Draw_speed();
  Draw_compass();
  Draw_color();
  stroke(150, 150, 150);
  strokeWeight(1);
  line(0, height*0.46, width, height*0.46);
  line(width * 0.55, 0, width * 0.55, height*0.46);


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

  if ( p.available() >= 27 ) { 
    if ( p.read() == 'H' ) {

      red_p = red;
      green_p = green;
      blue_p = blue;
      //RGB値
      red =  p.read();
      green = p.read();
      blue =  p.read();
      //地磁気
      geomag_X = 0.9 * geomag_X + 0.1 * read2byte(p);
      geomag_Y = 0.9 * geomag_Y + 0.1 * read2byte(p);
      geomag_Z = 0.9 * geomag_Z + 0.1 * read2byte(p);
      //加速度
      accel_X = 0.9 * accel_X + 0.1 * read2byte(p);
      accel_Y = 0.9 * accel_Y + 0.1 * read2byte(p);
      accel_Z = 0.9 * accel_Z + 0.1 * read2byte(p);
      //motorspeed
      motor_R = read2byte(p);
      motor_L = read2byte(p);
      //超音波
      sonic = p.read();
      //ゾーンナンバー
      zoneNumber = p.read();
      //ゾーンタスク
      mode =  p.read();
      //方角
      direction = read2byte(p);
      time = read2byte(p)/1000;
      if(zoneNumber == 3)hcolor = p.read();
      p.clear(); // 念のためクリア
      p.write("A");
/*
      print("  RGB = ");
      println(red, green, blue);
      print("(geomag_X,geomag_Y,geomag_Z) = ");
      println(geomag_X, geomag_Y, geomag_Z);
      print("(accel_X,accel_Y,accel_Z) = ");
      println(accel_Z, accel_Y, accel_Z);
      print("motorspeed =(L,R)");
      println(motor_L, motor_R);
      print("距離");
      println(sonic);
      print("ゾーンナンバー = ");
      println(zoneNumber);
      print("タスクナンバー = ");
      println(mode);
      print("方角 = ");
      println(direction);
      print("色(0:青1:赤) = ");
      println(hcolor);
      ++count;
      */
/*
      print("theta : ");
      println(theta * 180 / 3.14159265);
      print("psi : ");
      println(psi * 180 / 3.14159265);
      print("phi : ");
      println(phi  * 180 / 3.14159265);
      */
    }
  }
}


void get_Euler() {//オイラー角
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

  cos_theta = Normaliz_az;
  sin_theta = Math.signum(Normaliz_ay) * sqrt(1 - pow(Normaliz_az, 2));
  theta = atan2(sin_theta, cos_theta);

  if (abs(sin_theta) < 0.000005) {
    psi = atan2(-Normaliz_my, Normaliz_mx);
  } else {
    sin_psi = Normaliz_ax / sin_theta;
    cos_psi = Normaliz_ay / sin_theta;
    psi = atan2(sin_psi, cos_psi);
  }

  if (abs(accel_X) < 100 && abs(accel_Y) < 100) {
    m0_x = geomag_X;
    m0_y = geomag_Y;
    m0_z = geomag_Z;
    Size_m0 = sqrt(pow(m0_x, 2) + pow(m0_y, 2) + pow(m0_z, 2));
    Mu_x = (sqrt(pow(m0_x, 2) + pow(m0_y, 2))) / Size_m0;
    println("Mu_x : " + Mu_x);
  }

  cos_phi = (1.0 / Mu_x) * 
    ((cos(psi) * (Normaliz_mx - (Mu_x * Normaliz_ax))) 
    + (-sin(psi) * (Normaliz_my - (Mu_x * Normaliz_ay))));
  sin_phi = (1.0 / Mu_x) * 
    (((-cos(theta) * sin(psi)) * (Normaliz_mx - (Mu_x * Normaliz_ax))) 
    +  ((-cos(theta) * cos(psi)) * (Normaliz_my - (Mu_x * Normaliz_ay)))
    +  ((sin(theta))             * (Normaliz_my - (Mu_x * Normaliz_ay))));

  //phi = atan2(sin_phi, cos_phi);


  print("theta : ");
  println(theta * 180 / 3.14159265);
  print("psi : ");
  println(psi * 180 / 3.14159265);
  print("phi : ");
  println(phi * 180 / 3.14159265);
  
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
  scale(0.92, 0.92);
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
    rect(zone_pos[zoneNumber][0]  * 0.92, zone_pos[zoneNumber][1]  * 0.92, zone_pos[zoneNumber][2] * 0.92, zone_pos[zoneNumber][3] * 0.92);
    if (arfa >= 100) {
      diff = -2;
    } else if (arfa <= 10) {
      diff = 2;
    }
    arfa += diff;
  }

  stroke(255);
  fill(255);
  rect(0, height * 0.45 -60, 300, 50);
  
  fill(0);
  textSize(34);
  text("経過時間: " , 10 , height * 0.28);
  text(time , 165 , height * 0.28);
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
  translate(width * 0.55, 0);
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
  text("motor_L = ", 30, 145);
  text(motor_L, 150, 145);
  text("motor_R = ", 260, 145);
  text(motor_R, 380, 145);

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

  /*
  rotateX(pitch);
  rotateY(direction);
  rotateZ(roll);
  */
  
  rotateX(-theta);
  rotateY(psi);
  rotateZ(direction);
  


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
  translate(width * 0.65, height * 0.30);
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
  fill(255, 0, 0);
  rect(0, 130, 15, red_length);
  fill(0, 255, 0);
  rect(30, 130, 15, green_length);
  fill(0, 0, 255);
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