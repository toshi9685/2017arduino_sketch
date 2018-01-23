const int buzzer_Pin = 3;
const int famima[] = {740 , 587 , 440 , 587 , 659 , 880 , 880 , 880 , 659 , 740 , 659 , 440 , 587};
static int step_count = 0;

const int kana[] = {698 , 783 , 880 , 523 , 523 , 523 , 1047 , 988 , 988 , 880 , 988 , 988 , 988 , 1319 , 1047 , 988 , 880 , 880 , 880 , 880 , 988 , 1047 , 988 ,
                    988 , 880 , 880 , 880 , 784 , 784 , 698 , 784 , 698 , 784 , 880 , 523 , 523 , 523 , 1047 , 988 , 988 , 880 , 988 , 988 , 988 , 1319 , 1047 , 988 , 880 ,
                    880 , 880 , 1319 , 1175 , 1047 , 1047 , 523 , 1047 , 988 , 880 , 784 , 698 , 698
                   };
const float a[] = {1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 2.0 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 1.5 , 0.5 ,
                   1.0 , 1.0 , 1.0 , 0.5 , 0.5 , 1.0 , 1.0 , 2.0 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 1.5 , 0.5 , 1.0 , 1.0 , 2.0 , 1.0 , 1.0 ,
                   3.0 , 1.0 , 2.0 , 1.0 , 1.0   , 1.0 , 1.0 , 1.0 , 1.0 , 2.0 , 1.0 , 1.0 , 4.0
                  };
unsigned long time_Prev;


void winning_action() {
  switch ( mode_G ) {
    case 0:
      time_Prev = millis();
      motorL_G = 0;
      motorR_G = 0;
      mode_G = 1;
      break;
    case 1:

      break;
  }
  if (step_count >= sizeof(kana) / sizeof(kana[0])) {
    noTone(buzzer_Pin);
  }
  else if ( timeNow_G - time_Prev > (290 * a[step_count])) {
    step_count++;
    time_Prev = millis();
    noTone(buzzer_Pin);
  }
  else {
    tone(buzzer_Pin, kana[step_count] );
  }

}
