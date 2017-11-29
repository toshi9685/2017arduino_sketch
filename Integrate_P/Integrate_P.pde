
import processing.serial.*;
Serial port; 
int zoneNumber = 3, mode, sonic, motor_L, motor_R, accel_X, accel_Y, accel_Z, geomag_X, geomag_Y, geomag_Z;
int CX=250, CY=250;
int green, red, blue;
int high, low;
float direction_G;

int count = 0;

float avex = 0;
int[] sensors = new int[5];
int[] sensors_p = new int[5];

void setup() {
  size(1200, 800);
  background(255);
  println(Serial.list());
  // String arduinoPort = Serial.list()[1];
  // port = new Serial(this, arduinoPort, 9600 );
  port = new Serial(this, "/dev/cu.usbserial-A90176XN", 9600 ); // シリアルポート名は各自の環境に合わせて適宜指定
}

void draw() {

  background(255);
  DrawCommonInfo();//共通情報の描画
  DrawZoneTask();//各ゾーンで描画や処理が必要な時
  DrawZoneNumber();//ゾーンナンバーを視覚化
  /*
  noStroke();  fill(255);  rect(0,0,1000,500);
   // Draw Acceleration vector
   CX = 250;
   drawVec(accel_X/100,accel_Y/100,accel_Z/100);
   // Draw Magnetic flux vector
   CX = 750;
   drawVec(geomag_X,geomag_Y,geomag_Z);
   // Draw Heading direction
   CX = 600;
   float scale = 0.5;
   line(CX-scale*geomag_Y,100+scale*geomag_X,CX+scale*geomag_Y,100-scale*geomag_X);
   line(CX+scale*geomag_Y,100-scale*geomag_X,CX+0.6*scale*geomag_Y+0.2*scale*geomag_X,100-0.6*scale*geomag_X+0.2*scale*geomag_Y);
   line(CX+scale*geomag_Y,100-scale*geomag_X,CX+0.6*scale*geomag_Y-0.2*scale*geomag_X,100-0.6*scale*geomag_X-0.2*scale*geomag_Y);
   */
  noFill();
}

void DrawCommonInfo() {
  //区画
  stroke(200, 200, 200);
  line(0, height*0.3, width*0.65, height*0.3);
  line(width*0.65, 0, width*0.65, height);
  stroke(255);
  fill(255);
  rect(0, height-50, 300, 50);
  textSize(25);
  fill(0);

  text("zoneNumber= "+(int)zoneNumber, width*0.47, height*0.1);
  text("mode= "+(int)mode, width*0.54, height*0.15);
  text("Direction ="+direction_G, width*0.65, height*0.05);
  text("RGB= ", width*0.65, height*0.1);
  text("(geomag_X,geomag_Y,geomag_Z)= ", width*0.65, height*0.2);
  text("(accel_X,accel_Y,accel_Z)= ", width*0.65, height*0.3);
  text("motorspeed(L,R)=", width*0.65, height*0.4);

  text("("+red+","+green+","+blue+")", width*0.65 +80, height*0.1);
  fill(red, green, blue);
  rect(width*0.65, height*0.1 + 5, 300, 50);
  fill(0);
  text("("+geomag_X+","+geomag_Y+","+geomag_Z+")", width*0.65, height*0.25);
  text("("+accel_X+","+accel_Y+","+accel_Z+")", width*0.65, height*0.35);
  text("("+motor_L+","+motor_R+")", width*0.65, height*0.45);
}

void DrawZoneTask() {
  //ゾーンの描画
  if(zoneNumber != 4){
    strokeWeight(10);
    stroke(0);
    fill(0);
    line(width*0.6,height*0.35,width*0.6,height*0.95);
    line(width*0.6-20,height*0.35,width*0.6-20,height*0.95);
    line(width*0.1,height*0.35,width*0.6,height*0.35);
    line(width*0.1,height*0.95,width*0.6,height*0.95);
    line(width*0.1,height*0.35,width*0.1,height*0.95);
    
    stroke(256,256,0);
    line(width*0.1+10,height*0.35,width*0.1+10,height*0.95);
  }
  //各ゾーンタスクで必要な描画、処理
  switch(zoneNumber) {
  case 1://linetrace
    stroke(84, 179, 89);
    line(width*0.6-10,height*0.35,width*0.6-10,height*0.95);
    break;
  case 2://carling
    stroke(183, 40, 24);
    line(width*0.6-10,height*0.35,width*0.6-10,height*0.95);
    break;
  case 3://hillclimbing
    stroke(12, 33, 104);
    line(width*0.6-10,height*0.35,width*0.6-10,height*0.95);
    CalcDisp();
    break;
  case 4://winning
    break;
  default:
    break;
  }
}

void DrawZoneNumber(){
  strokeWeight(2);
  stroke(0);
  fill(0);
  line(width*0.1, height*0.28, width*0.5, height*0.28);
  line(width*0.1, height*0.1, width*0.3, height*0.1);
  line(width*0.1, height*0.01, width*0.1, height*0.28);
  line(width*0.3, height*0.01, width*0.3, height*0.28);
  line(width*0.3, height*0.18, width*0.5, height*0.18);
  line(width*0.4, height*0.18, width*0.4, height*0.28);
  line(width*0.5, height*0.18, width*0.5, height*0.28);
  line(width*0.1, height*0.01, width*0.3, height*0.01);
  switch(zoneNumber){
    case 1:
      DrawStar(10,20,width*0.45,height*0.23);
    break;
    case 2:
      DrawStar(10,20,width*0.35,height*0.23);
    break;
    case 3:
      DrawStar(10,20,width*0.2,height*0.23);
    break;
    case 4:
      DrawStar(10,20,width*0.2,height*0.05);
    break;
    default:
      DrawStar(10,20,width*0.55,height*0.23);
    break;
  }
}

void DrawStar(int R,int R_out,float C_x,float C_y){
  //R中心から頂点までの距離（半径）,R_out中心から棘までの距離（半径）,C_x中心座標x,C_y中心座標y
  int vertex_num = 10;//頂点数*2
  int R_in = R_out/2;//中心から谷までの距離（半径）
  //星
  noFill();
  strokeWeight(2);
  stroke(200, 0, 0);

  pushMatrix();
  translate(C_x, C_y);
  rotate(radians(-90));
  beginShape();
  for (int i = 0; i < vertex_num; i++) {
    if (i%2 == 0) {
      R = R_out;
    } else {
      R = R_in;
    }
    vertex(R*cos(radians(360*i/vertex_num)), R*sin(radians(360*i/vertex_num)));
  }
  endShape(CLOSE);
  popMatrix();
}

void CalcDisp(){
  count++;
  stroke(0, 0, 0);
  fill(0);
  ellipse(width*0.6,height*0.65,10,10);
  text("avex="+avex, width*0.65, height*0.9);
}

void line3D(float x0, float y0, float z0, float x1, float y1, float z1) {
  float X0 = CX+y0-0.5*x0, Y0 = CY + 1.7320508*x0/2-z0;
  float X1 = CX+y1-0.5*x1, Y1 = CY + 1.7320508*x1/2-z1;
  line(X0, Y0, X1, Y1);
}

void drawVec(float x, float y, float z) {
  stroke(128);
  line3D(0, 0, 0, 250, 0, 0);  
  line3D(0, 0, 0, 0, 250, 0);  
  line3D(0, 0, 0, 0, 0, 250);
  stroke(0);
  line3D(0, 0, 0, x, y, 0);
  line3D(x, y, 0, x, y, z);
  line3D(0, 0, 0, x, 0, 0);
  line3D(x, 0, 0, x, y, 0);
  line3D(x, y, 0, 0, y, 0);
  line3D(0, y, 0, 0, 0, 0);
  stroke(255, 0, 0);  
  line3D(0, 0, 0, x, y, z);
  fill(0);  
  text(x, CX-80, 490);  
  text(y, CX, 490);  
  text(z, CX+80, 490);
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

      //RGB値
      red =  p.read();
      green = p.read();
      blue =  p.read();
      //地磁気
      geomag_X = read2byte(p);
      geomag_Y = read2byte(p);
      geomag_Z = read2byte(p);
      //加速度
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
      mode =  p.read();
      //方角
      direction_G = (float)read2byte(p) / 100.0; // もともと100倍して送られていたので戻す
      if(zoneNumber == 3)avex = read2byte(p);
      p.clear(); // 念のためクリア
      p.write("A");
      //コンソールに表示
      print("  RGB = ");
      println(red, green, blue);
      print("(geomag_X,geomag_Y,geomag_Z) = ");
      println(geomag_X, geomag_Y, geomag_Z);
      print("(accel_X,accel_Y,accel_Z) = ");
      println(accel_Z, accel_Y, accel_Z);
      print("motorspeed =(L,R)");
      println(motor_L, motor_R);
      print("ゾーンナンバー = ");
      println(zoneNumber);
      print("タスクナンバー = ");
      println(mode);
      print("方角 = ");
      println(direction_G);
      print("avex = ");
      println(avex);
    }
  }
}