const int buzzer_Pin = 3;
const int famima[] = {740 , 587 , 440 , 587 , 659 , 880 , 880 , 880 , 659 , 740 , 659 , 440 , 587};
const int kana[] = {698 , 783 , 880 , 523 , 523 , 523 , 1047 , 988 , 988 , 880 , 988 , 988 , 988 , 1319 , 1047 , 988 , 880 , 880 , 880 , 880 , 988 , 1047 , 988 ,
                    988 , 880 , 880 , 880 , 784 , 784 , 698 , 784 , 698 , 784 , 880 , 523 , 523 , 523 , 1047 , 988 , 988 , 880 , 988 , 988 , 988 , 1319 , 1047 , 988 , 880 ,
                    880 , 880 , 1319 , 1175 , 1047 , 1047 , 523 , 1047 , 988 , 880 , 784 , 698 , 698
                   };
const float a[] = {1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 2.0 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 1.5 , 0.5 ,
                   1.0 , 1.0 , 1.0 , 0.5 , 0.5 , 1.0 , 1.0 , 2.0 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 2.0 , 1.0 , 1.0 ,
                   3.0 , 1.0 , 2.0 , 1.0 , 1.0   , 1.0 , 1.0 , 1.0 , 1.0 , 2.0 , 1.0 , 1.0 , 4.0
                  };
const int nisino[] = {349 , 391 , 440 , 261 , 261 , 261 , 523 , 498 , 498 , 440 , 498 , 498 , 498 , 659 , 523 , 493 , 440 , 440 , 440 , 440 , 493 , 523 , 493};
static int step_count = 0;

unsigned long timeInit_G, timeNow_G; //  スタート時間，経過時間
unsigned long time_Prev;
void setup() {
  // put your setup code here, to run once:
  time_Prev = millis();
}

void loop() {

  timeNow_G = millis(); // 経過時間
  // put your main code here, to run repeatedly:


  if (step_count >= sizeof(kana) / sizeof(kana[0])) {
    //step_count = 0;
    noTone(buzzer_Pin);
    //delay(3000);
  }
  else if ( timeNow_G - time_Prev > (190 * a[step_count])) {
    step_count++;
    time_Prev = millis();
    noTone(buzzer_Pin);
  }
  else {
    tone(buzzer_Pin, kana[step_count] );
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

