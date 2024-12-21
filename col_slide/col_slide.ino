#include <LedControl.h> 

int DIN = 13;
int CS =  15;
int CLK = 14;

int num = 0;
int s1 = 0, s2 = 0;

byte small_num[10][3] = {{0x1f, 0x11, 0x1f},
                         {0x00, 0x1f, 0x00},
                         {0x1d, 0x15, 0x17},
                         {0x1f, 0x15, 0x15},
                         {0x1f, 0x04, 0x1c},
                         {0x17, 0x15, 0x1d},
                         {0x17, 0x15, 0x1f},
                         {0x1f, 0x10, 0x10},
                         {0x1f, 0x15, 0x1f},
                         {0x1f, 0x15, 0x1d}};

byte small_buff[2][3] {{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}};

LedControl lc=LedControl(DIN,CLK,CS,0);

void setup() {
  lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0,2);      // Set the brightness to maximum value
  lc.clearDisplay(0);         // and clear the display
  printByte(small_num[0], small_num[9], 7);
  printByte(small_num[0], small_num[9], 3);
}

void loop() {
  if(s1 != (num%10)){
    s1 = num % 10;
    int s1_1 = (!s1) ? 9 : (s1-1);
    printByte(small_num[s1], small_num[s1_1], 7);
  }

  if(s2 != (num/10)){
    s2 = num / 10;
    int s2_1 = (!s2) ? 9 : (s2-1);
    printByte(small_num[s2], small_num[s2_1], 3);
  }
  /*
  do{
    s1 = num % 10;
    printByte(small_num[s1], 7);
  }while(s1 != (num%10));

  do{
    s2 = num / 10;
    printByte(small_num[s2], 3);
  }while(s2 != (num/10));
  */
  delay(800);
  num++;
  if(num == 60) num = 0;
}

void printByte(byte new_character [], byte befor_character [], int arr){
  byte buff[3], character_buff[3];
  for(int i = 0; i < 3; i++) character_buff[i] = new_character[i] << 1, small_buff[1][i] = befor_character[i];
  for(int m = 0; m < 6; m++){
    for(int k = 0; k < 3; k++) buff[k] = (character_buff[k] >> m) & 0x01;
    slide(buff);
    for(int i = arr, j = 0; i > (arr-3); i--, j++){
      lc.setColumn(0, i, small_buff[0][j]);
      small_buff[1][j] = small_buff[0][j];
    }
    delay(60);
  }
}

void slide(byte charbuf []){
  for(int i = 0; i < 3; i++){
    small_buff[0][i] = (small_buff[1][i] >> 1) | (charbuf[i] << 4);
  }
}
