
import processing.serial.*;
Serial port; 
int zoneNumber, mode ,sonic,motor_L,motor_R,accel_X,accel_Y,accel_Z,geomag_X,geomag_Y,geomag_Z;
int CX=250, CY=250;
int green, red, blue;
int high, low;
float direction_G;
int[] sensors = new int[5];
int[] sensors_p = new int[5];

void setup() {
  size(1200, 800);
  background(255);
  println(Serial.list());
  // String arduinoPort = Serial.list()[1];
  // port = new Serial(this, arduinoPort, 9600 );
  //port = new Serial(this, "/dev/cu.usbserial-A90174UN", 9600 ); // シリアルポート名は各自の環境に合わせて適宜指定
  zoneNumber = 0;
}

void draw() {

  background(255);

  //区画
  stroke(200, 200, 200);
  line(0, height*0.3, width*0.65, height*0.3);
  line(width*0.65, 0, width*0.65, height);


  stroke(255);
  fill(255);
  rect(0, height-50, 300, 50);
  textSize(25);
  fill(0);
  text("zone= ", width*0.55, height*0.1);
  text("mode= ", width*0.55, height*0.2);
  text("RGB= ",width*0.65, height*0.1);
  text("(geomag_X,geomag_Y,geomag_Z)= ",width*0.65, height*0.2);
  text("(accel_X,accel_Y,accel_Z)= ",width*0.65,height*0.3);
  text("motorspeed(L,R)=",width*0.65,height*0.4);
  
  text((int)zoneNumber, width*0.55 + 100, height*0.1);
  text((int)mode, width*0.55 + 100, height*0.2);
  
  text("("+red+",",width*0.65 +80, height*0.1);
  text(green+",",width*0.65 +135, height*0.1);
  text(blue+")",width*0.65 +190, height*0.1);
  fill(red,green,blue);
  rect(width*0.65, height*0.1 + 5, 300, 50);
  fill(0);
  text("("+geomag_X+",",width*0.65, height*0.25);
  text(geomag_Y+",",width*0.65 +120, height*0.25);
  text(geomag_Z+")",width*0.65 +240, height*0.25);
  text("("+accel_X+",",width*0.65, height*0.35);
  text(accel_Y+",",width*0.65 +120, height*0.35);
  text(accel_Z+")",width*0.65 +240, height*0.35);
  text("("+motor_L+",",width*0.65, height*0.45);
  text(motor_R+")",width*0.65 +120, height*0.45);
  if(zoneNumber == 3){
    
  }
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

void line3D(float x0, float y0,float z0,float x1,float y1,float z1) {
  float X0 = CX+y0-0.5*x0, Y0 = CY + 1.7320508*x0/2-z0;
  float X1 = CX+y1-0.5*x1, Y1 = CY + 1.7320508*x1/2-z1;
  line(X0,Y0,X1,Y1);
}

void drawVec(float x, float y, float z) {
  stroke(128);
  line3D(0,0,0,250,0,0);  
  line3D(0,0,0,0,250,0);  
  line3D(0,0,0,0,0,250);
  stroke(0);
  line3D(0,0,0,x,y,0);
  line3D(x,y,0,x,y,z);
  line3D(0,0,0,x,0,0);
  line3D(x,0,0,x,y,0);
  line3D(x,y,0,0,y,0);
  line3D(0,y,0,0,0,0);
  stroke(255,0,0);  line3D(0,0,0,x,y,z);
  fill(0);  text(x,CX-80,490);  text(y,CX,490);  text(z,CX+80,490);
}

// 通信方式1
void serialEvent(Serial p) { 

  if ( p.available() >= 22 ) { 
    if ( p.read() == 'H' ) {

      //RGB値
      red =  p.read();
      green = p.read();
      blue =  p.read();
      //地磁気
      high = p.read();
      low = p.read();
      int d = (high << 8 ) + low;
      if ( 32767 < d )
        d -= 65536;
      geomag_X = d;
      high = p.read();
      low = p.read();
      d = (high << 8 ) + low;
      if ( 32767 < d )
        d -= 65536;
      geomag_Y = d;
      high = p.read();
      low = p.read();
      d = (high << 8 ) + low;
      if ( 32767 < d )
        d -= 65536;
      geomag_Z = d;
      /*
      if ( 32767 < d )
        d -= 65536;
      direction_G = (float)d / 100.0; // もともと100倍して送られていたので戻す*/
      //加速度
      high = p.read();
      low = p.read();
      int ac = (high << 8 ) + low;
      if ( 32767 < ac )
        ac -= 65536;
      accel_X = ac;
      high = p.read();
      low = p.read();
      ac = (high << 8 ) + low;
      if ( 32767 < ac )
        ac -= 65536;
      accel_Y = ac;
      high = p.read();
      low = p.read();
      ac = (high << 8 ) + low;
      if ( 32767 < ac )
        ac -= 65536;
      accel_Z = ac;
      //motorspeed
      high = p.read();
      low = p.read();
      int sp = (high << 8 ) + low;
      motor_R = sp;
      high = p.read();
      low = p.read();
      sp = (high << 8 ) + low;
      motor_L = sp;
      //超音波
      sonic = p.read();
      //ゾーンナンバー
      zoneNumber = p.read();
      //ゾーンタスク
      mode =  p.read();
      
      p.clear(); // 念のためクリア
      p.write("A");

      print("  RGB = ");
      println(red, green, blue);
      print("(geomag_X,geomag_Y,geomag_Z) = ");
      println(geomag_X, geomag_Y, geomag_Z);
      print("(accel_X,accel_Y,accel_Z) = ");
      println(accel_Z, accel_Y, accel_Z);
      print("motorspeed =(L,R)");
      println(motor_L,motor_R);
      print("ゾーンナンバー = ");
      println(zoneNumber);
      print("タスクナンバー = ");
      println(mode);
    }
  }
}