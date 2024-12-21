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

LedControl lc=LedControl(DIN,CLK,CS,0);

void setup() {
  lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0,2);      // Set the brightness to maximum value
  lc.clearDisplay(0);         // and clear the display
}

void loop() {
  s1 = num % 10;
  s2 = num / 10;
  printByte(small_num[s1], 7);
  printByte(small_num[s2], 3);
  /*
  for(int i = 0; i < 10; i++){
    printByte(small_num[i]);
    delay(1000);
  }
  */
  delay(1000);
  num++;
  if(num == 60) num = 0;
}

void printByte(byte character [], int arr){
  for(int i = arr, j = 0; i > (arr-3); i--, j++){
    lc.setColumn(0, i, character[j]);
  }
}
