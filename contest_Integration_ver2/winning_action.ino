const int buzzer_Pin = 3;
const int famima[] = {740 , 587 , 440 , 587 , 659 , 880 , 880 , 880 , 659 , 740 , 659 , 440 , 587};
static int step_count = 0;

void winning_action() {
  switch ( mode_G ) {
    case 0:
      mode_G = 1;
      break;
    case 1:
      turnTo(0);
      break;
  }
}
