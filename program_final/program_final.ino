#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <LedControl.h> 
#define DIN 13
#define CS 15
#define CLK 14

int num_s;
int num_m;
int num_h;

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

byte biger_num[10][5] = {{0x7f, 0x41, 0x41, 0x7f, 0x00},
                         {0x00, 0x00, 0x7f, 0x00, 0x00},
                         {0x4f, 0x49, 0x49, 0x79, 0x00},
                         {0x49, 0x49, 0x49, 0x7f, 0x00},
                         {0x78, 0x08, 0x08, 0x7f, 0x00},
                         {0x79, 0x49, 0x49, 0x4f, 0x00},
                         {0x7f, 0x49, 0x49, 0x4f, 0x00},
                         {0x40, 0x40, 0x40, 0x7f, 0x00},
                         {0x7f, 0x49, 0x49, 0x7f, 0x00},
                         {0x79, 0x49, 0x49, 0x7f, 0x00}};


byte print_buff[4][8] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                         {0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00},
                         {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

LedControl lc = LedControl(DIN,CLK,CS,4);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
  WiFi.begin("My-WiFi", "@@1qaz2418");
  while (WiFi.status() != WL_CONNECTED) delay(500);
  //Serial.begin(9600);
  for(int i = 0; i < 4; i++){
    lc.shutdown(i, false);    //The MAX72XX is in power-saving mode on startup
    lc.setIntensity(i, 2);    // Set the brightness to maximum value
    lc.clearDisplay(i);       // and clear the display
  }
  delay(1500);
  timeClient.begin();
  timeClient.setTimeOffset(28800);
  timeClient.update();
  num_s = timeClient.getSeconds();
  num_m = timeClient.getMinutes();
  num_h = timeClient.getHours();
  if(num_h > 12) num_h -= 12;
  init_printByte(num_s, num_m, num_h);
}

void loop() {
  char buf[20] = "";
  timeClient.update();
  if(num_s != timeClient.getSeconds()){
    num_s = timeClient.getSeconds();
    num_m = timeClient.getMinutes();
    num_h = timeClient.getHours();
    if(num_h > 12) num_h -= 12;
    printByte_slide(num_s, num_m, num_h);
    //sprintf(buf, "%2d:%2d:%2d\n", num_h, num_m, num_s);
    //Serial.println(buf);
  }
}

void printByte_slide(int sec_num, int min_num, int hour_num){
  int beforsec_num = (!sec_num) ? 59 : (sec_num-1);
  int beformin_num = (!min_num) ? 59 : (min_num-1);
  int beforhour_num = (hour_num == 1) ? 12 : (hour_num-1);
  byte new_buff[4][8];
  byte buff[4][8];
  int arr_s1, arr_s2;
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 8; j++) new_buff[i][j] = 0x00;
  }
  for(int i = 0; i < 4; i++){
    new_buff[0][i] = small_num[sec_num/10][i] << 2;
    new_buff[0][i+4] = small_num[sec_num%10][i] << 2;
    print_buff[0][i] = small_num[beforsec_num/10][i];
    print_buff[0][i+4] = small_num[beforsec_num%10][i];
  }

  if(!sec_num){
    for(int m = 4, f = 7; m >= 0; m--, f--){
      new_buff[1][f] = biger_num[min_num%10][m];
      print_buff[1][f] = biger_num[beformin_num%10][m];
    }
    arr_s1 = 1;
    arr_s2 = 3;
    
    if(min_num/10 != beformin_num/10){
      for(int m = 4, f = 2, i = 1; m >= 0; m--, f--){
        new_buff[i][f] = biger_num[min_num/10][m];
        print_buff[i][f] = biger_num[beformin_num/10][m];
        if(!f) f = 8, i++;
      }
      arr_s1 = 2;
      arr_s2 = 6;
    }

    if(!min_num){
      for(int m = 4, f = 1, i = 2; m >= 0; m--, f--){
        new_buff[i][f] = biger_num[hour_num%10][m];
        print_buff[i][f] = biger_num[beforhour_num%10][m];
        if(!f) f = 8, i++;
      }
      arr_s1 = 3;
      arr_s2 = 5;

      if(hour_num/10 != beforhour_num/10){
        for(int m = 4, f = 4; m >= 0; m--, f--){
          new_buff[3][f] = biger_num[hour_num/10][m];
          print_buff[3][f] = biger_num[beforhour_num/10][m];
        }
        arr_s1 = 3;
        arr_s2 = 0;
      }
    }
  }

  //==========================================================
  for(int i = 0; i < 7; i++){
    for(int z = 0; z < 4; z++){
      for(int j = 0; j < 8; j++){
        buff[z][j] = (new_buff[z][j] >> i) & 0x01;
      }
    }
    slide_sec(buff[0], ((sec_num/10 != beforsec_num/10) ? 0 : 4));
    if(!sec_num)slide(buff, arr_s1, arr_s2);
    for(int j = 0; j < 4; j++){
      for(int m = 0; m < 8; m++){
        lc.setColumn(j, m, print_buff[j][m]);
      }
    }
    delay(15);
  }
}

void slide_sec(byte charbuf [], int arr){
  for(int i = arr; i < 8; i++){
    print_buff[0][i] = (print_buff[0][i] >> 1) | (charbuf[i] << 4);
  }
}

void slide(byte charbuf[4][8], int arr1, int arr2){
  for(int i = 1; i <= arr1; i++){
    for(int j = 7; j >= 0; j--){
      if(i == 2 && j ==3) continue;
      print_buff[i][j] = (print_buff[i][j] >> 1) | (charbuf[i][j] << 6);
      if(i == arr1 && j == arr2) break;
    }
  }
}

void init_printByte(int init_sec, int init_min, int init_hour){
  for(int i = 0; i < 4; i++){
    print_buff[0][i] = small_num[init_sec/10][i];
    print_buff[0][i+4] = small_num[init_sec%10][i];
  }
  for(int m = 4, f = 7; m >= 0; m--, f--) print_buff[1][f] = biger_num[init_min%10][m];
  for(int m = 4, f = 2, i = 1; m >= 0; m--, f--){
    print_buff[i][f] = biger_num[init_min/10][m];
    if(!f) f = 8, i++;
  }
  for(int m = 4, f = 1, i = 2; m >= 0; m--, f--){
    print_buff[i][f] = biger_num[init_hour%10][m];
    if(!f) f = 8, i++;
  }
  for(int m = 4, f = 4; m >= 0; m--, f--) print_buff[3][f] = biger_num[init_hour/10][m];
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 8; j++){
      lc.setColumn(i, j, print_buff[i][j]);
    }
  }
}
