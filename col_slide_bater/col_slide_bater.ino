#include <LedControl.h> 

int DIN = 13;
int CS =  15;
int CLK = 14;

int num = 18, n;

byte small_num[10][4] = {{0x00, 0x1f, 0x11, 0x1f},
                         {0x00, 0x00, 0x1f, 0x00},
                         {0x00, 0x17, 0x15, 0x1d},
                         {0x00, 0x15, 0x15, 0x1f},
                         {0x00, 0x1c, 0x04, 0x1f},
                         {0x00, 0x1d, 0x15, 0x17},
                         {0x00, 0x1f, 0x15, 0x17},
                         {0x00, 0x10, 0x10, 0x1f},
                         {0x00, 0x1f, 0x15, 0x1f},
                         {0x00, 0x1d, 0x15, 0x1f}};


byte print_buff[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

LedControl lc=LedControl(DIN,CLK,CS,0);

void setup() {
  lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0,2);      // Set the brightness to maximum value
  lc.clearDisplay(0);         // and clear the display
  n = num;
  printByte(num);
}

void loop() {
  if(num != n) n = num, printByte_slide(num);
  delay(800);
  num++;
  if(num == 60) num = 0;
}

void printByte_slide(int new_num){
  int befor_num = (!new_num) ? 59 : (new_num-1);
  byte new_buff[8];
  byte buff[8];
  for(int i = 0; i < 4; i++){
    new_buff[i] = small_num[new_num/10][i] << 1;
    new_buff[i+4] = small_num[new_num%10][i] << 1;
    print_buff[i] = small_num[befor_num/10][i];
    print_buff[i+4] = small_num[befor_num%10][i];
  }
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 8; j++) buff[j] = (new_buff[j] >> i) & 0x01;
    slide(buff, ((new_num/10 != befor_num/10) ? 0 : 4));
    for(int m = 0; m < 8; m++){
      lc.setColumn(0, m, print_buff[m]);
    }
    delay(60);
  }
}

void slide(byte charbuf [], int arr){
  for(int i = arr; i < 8; i++){
    print_buff[i] = (print_buff[i] >> 1) | (charbuf[i] << 4);
  }
}

void printByte(int init_num){
  byte buff[8];
  for(int i = 0; i < 4; i++){
    buff[i] = small_num[init_num/10][i];
    buff[i+4] = small_num[init_num%10][i];
  }
  for(int i = 0; i < 8; i++){
    lc.setColumn(0, i, buff[i]);
  }
}
