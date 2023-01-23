/*************************************************** 
  PCA M2000C - JOYSTICK - DCS BIOS
 ****************************************************/

#include <Joystick.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"

#ifndef _BV
  #define _BV(bit) (1<<(bit))
#endif

Joystick_ Joystick;

Adafruit_LEDBackpack matrix = Adafruit_LEDBackpack();
Adafruit_LEDBackpack matrix_bas = Adafruit_LEDBackpack();

int channels[] = {14, 15};
int channelsSize = sizeof(channels) / sizeof(channels[0]);
int inputs[] = {4, 5, 6, 7, 8, 9, 16};
int inputsSize = sizeof(inputs) / sizeof(inputs[0]);

uint8_t counter = 0;

int pannel_led = 10;

int refresh_rate = 100;

void setup() {
  DcsBios::setup();

  for (int i = 0; i < inputsSize; i++){
    pinMode(inputs[i], INPUT_PULLUP);
  }
  
  matrix.begin(0x70);  // pass in the address 70 or 74
  matrix.setBrightness(6);
  matrix_bas.begin(0x74);  // pass in the address 70 or 74
  matrix_bas.setBrightness(6);

  Joystick.begin();

  pinMode(pannel_led, OUTPUT);
  analogWrite(pannel_led, 255);
}

bool lastButtonState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

char text_haut[] = "PCA M   2000C  ";
char previous_text_haut = text_haut;

char text_bas[] = " 1  2  3  4  5 ";
char previous_text_bas = text_bas;

bool led_selected_haut[] = {0, 1, 1, 1, 1, 1};
bool previous_led_selected_haut = led_selected_haut;

bool led_selected_bas[] = {1, 1, 1, 1, 1, 1};
bool previous_led_selected_bas = led_selected_bas;

bool led_pret_bas[] = {1, 1, 1, 1, 1, 1};
bool previous_led_pret_bas = led_pret_bas;


// 0: dp
// 1: g
// 2: f
// 3: e
// 4: d
// 5: c
// 6: b
// 7: a


bool zero[] = {0, 0, 1, 1, 1, 1, 1, 1};
bool one[] = {0, 0, 0, 0, 0, 1, 1, 0};
bool two[] = {0, 1, 0, 1, 1, 0, 1, 1};
bool three[] = {0, 1, 0, 0, 1, 1, 1, 1};
bool four[] = {0, 1, 1, 0, 0, 1, 1, 0};
bool five[] = {0, 1, 1, 0, 1, 1, 0, 1};
bool six[] = {0, 1, 1, 1, 1, 1, 0, 1};
bool seven[] = {0, 0, 1, 0, 0, 1, 1, 1};
bool height[] = {0, 1, 1, 1, 1, 1, 1, 1};
bool nine[] = {0, 1, 1, 0, 1, 1, 1, 1};

bool A[] = {0, 1, 1, 1, 0, 1, 1, 1};
bool B[] = {0, 1, 1, 1, 1, 1, 0, 0};
bool C[] = {0, 0, 1, 1, 1, 0, 0, 1};
bool D[] = {0, 1, 0, 1, 1, 1, 1, 0};
bool E[] = {0, 1, 1, 1, 1, 0, 0, 1};
bool F[] = {0, 1, 1, 1, 0, 0, 0, 1};
bool G[] = {0, 0, 1, 1, 1, 1, 0, 1};
bool H[] = {0, 1, 1, 1, 0, 1, 1, 0};
bool I[] = {0, 0, 1, 1, 0, 0, 0, 0};
bool J[] = {0, 0, 0, 1, 1, 1, 1, 0};
bool K[] = {0, 1, 1, 1, 0, 1, 0, 1};
bool L[] = {0, 0, 1, 1, 1, 0, 0, 0};
bool M[] = {0, 0, 1, 1, 0, 1, 1, 1};
bool N[] = {0, 1, 0, 1, 0, 1, 0, 0};
bool O[] = {0, 0, 1, 1, 1, 1, 1, 1};
bool P[] = {0, 1, 1, 1, 0, 0, 1, 1};
bool Q[] = {0, 1, 1, 0, 0, 1, 1, 1};
bool R[] = {0, 0, 1, 1, 0, 0, 1, 1};
bool S[] = {0, 1, 1, 0, 1, 1, 0, 1};
bool T[] = {0, 1, 1, 1, 1, 0, 0, 0};
bool U[] = {0, 0, 1, 1, 1, 1, 1, 0};
bool V[] = {0, 0, 1, 0, 1, 1, 1, 0};
bool W[] = {0, 0, 1, 0, 1, 0, 1, 0};
bool X[] = {0, 1, 1, 1, 0, 1, 1, 0};
bool Y[] = {0, 1, 1, 0, 1, 1, 1, 0};
bool Z[] = {0, 1, 0, 0, 1, 0, 1, 1};
bool space[] = {0, 0, 0, 0, 0, 0, 0, 0};


// 0 is left digit : i th
// 0: 12
// 1: 13
// 2: 14
// 3: 9
// 4: 10
// 5: 11
// 6: 6
// 7: 7
// 8: 8
// 9: 3
// 10: 4
// 11: 5
// 12: 0
// 13: 1
// 14: 2
int digit_num_correspondance[] = {12, 13, 14, 9, 10, 11, 6, 7, 8, 3, 4, 5, 0, 1, 2} ;


// UPPER TEXTS
void onPcaBr1DispChange(char* newValue) {
    text_bas[0] = newValue[0];
    text_bas[1] = newValue[1];
    text_bas[2] = newValue[2];
}
DcsBios::StringBuffer<3> pcaBr1DispBuffer(0x72d6, onPcaBr1DispChange);
void onPcaBr2DispChange(char* newValue) {
    text_bas[3] = newValue[0];
    text_bas[4] = newValue[1];
    text_bas[5] = newValue[2];
}
DcsBios::StringBuffer<3> pcaBr2DispBuffer(0x72da, onPcaBr2DispChange);
void onPcaBr3DispChange(char* newValue) {
    text_bas[6] = newValue[0];
    text_bas[7] = newValue[1];
    text_bas[8] = newValue[2];
}
DcsBios::StringBuffer<3> pcaBr3DispBuffer(0x72de, onPcaBr3DispChange);
void onPcaBr4DispChange(char* newValue) {
    text_bas[9] = newValue[0];
    text_bas[10] = newValue[1];
    text_bas[11] = newValue[2];
}
DcsBios::StringBuffer<3> pcaBr4DispBuffer(0x72e2, onPcaBr4DispChange);
void onPcaBr5DispChange(char* newValue) {
    text_bas[12] = newValue[0];
    text_bas[13] = newValue[1];
    text_bas[14] = newValue[2];
}
DcsBios::StringBuffer<3> pcaBr5DispBuffer(0x72e6, onPcaBr5DispChange);



// BELLOW TEXT
void onPcaUr1DispChange(char* newValue) {
    text_haut[0] = newValue[0];
    text_haut[1] = newValue[1];
    text_haut[2] = newValue[2];
}
DcsBios::StringBuffer<3> pcaUr1DispBuffer(0x72c2, onPcaUr1DispChange);
void onPcaUr2DispChange(char* newValue) {
    text_haut[3] = newValue[0];
    text_haut[4] = newValue[1];
    text_haut[5] = newValue[2];
}
DcsBios::StringBuffer<3> pcaUr2DispBuffer(0x72c6, onPcaUr2DispChange);
void onPcaUr3DispChange(char* newValue) {
    text_haut[6] = newValue[0];
    text_haut[7] = newValue[1];
    text_haut[8] = newValue[2];
}
DcsBios::StringBuffer<3> pcaUr3DispBuffer(0x72ca, onPcaUr3DispChange);
void onPcaUr4DispChange(char* newValue) {
    text_haut[9] = newValue[0];
    text_haut[10] = newValue[1];
    text_haut[11] = newValue[2];
}
DcsBios::StringBuffer<3> pcaUr4DispBuffer(0x72ce, onPcaUr4DispChange);
void onPcaUr5DispChange(char* newValue) {
    text_haut[12] = newValue[0];
    text_haut[13] = newValue[1];
    text_haut[14] = newValue[2];
}
DcsBios::StringBuffer<3> pcaUr5DispBuffer(0x72d2, onPcaUr5DispChange);



// GUN BUTTON
void onGunModeKl1Change(unsigned int newValue) {
    led_selected_bas[0] = newValue;
}
DcsBios::IntegerBuffer gunModeKl1Buffer(0x72c0, 0x0004, 2, onGunModeKl1Change);
void onGunModeKl2Change(unsigned int newValue) {
    led_pret_bas[0] = newValue;
}
DcsBios::IntegerBuffer gunModeKl2Buffer(0x72c0, 0x0008, 3, onGunModeKl2Change);



// READY LIGHT
void onWeapon1RChange(unsigned int newValue) {
    led_pret_bas[1] = newValue;
}
DcsBios::IntegerBuffer weapon1RBuffer(0x72c0, 0x0020, 5, onWeapon1RChange);
void onWeapon2RChange(unsigned int newValue) {
    led_pret_bas[2] = newValue;
}
DcsBios::IntegerBuffer weapon2RBuffer(0x72c0, 0x0080, 7, onWeapon2RChange);
void onWeapon3RChange(unsigned int newValue) {
    led_pret_bas[3] = newValue;
}
DcsBios::IntegerBuffer weapon3RBuffer(0x72c0, 0x0200, 9, onWeapon3RChange);
void onWeapon4RChange(unsigned int newValue) {
    led_pret_bas[4] = newValue;
}
DcsBios::IntegerBuffer weapon4RBuffer(0x72c0, 0x0800, 11, onWeapon4RChange);
void onWeapon5RChange(unsigned int newValue) {
    led_pret_bas[5] = newValue;
}
DcsBios::IntegerBuffer weapon5RBuffer(0x72c0, 0x2000, 13, onWeapon5RChange);


// SELECTED BAS LIGHT
void onWeapon1SChange(unsigned int newValue) {
    led_selected_bas[1] = newValue;
}
DcsBios::IntegerBuffer weapon1SBuffer(0x72c0, 0x0010, 4, onWeapon1SChange);
void onWeapon2SChange(unsigned int newValue) {
    led_selected_bas[2] = newValue;
}
DcsBios::IntegerBuffer weapon2SBuffer(0x72c0, 0x0040, 6, onWeapon2SChange);
void onWeapon3SChange(unsigned int newValue) {
    led_selected_bas[3] = newValue;
}
DcsBios::IntegerBuffer weapon3SBuffer(0x72c0, 0x0100, 8, onWeapon3SChange);
void onWeapon4SChange(unsigned int newValue) {
    led_selected_bas[4] = newValue;
}
DcsBios::IntegerBuffer weapon4SBuffer(0x72c0, 0x0400, 10, onWeapon4SChange);
void onWeapon5SChange(unsigned int newValue) {
    led_selected_bas[5] = newValue;
}
DcsBios::IntegerBuffer weapon5SBuffer(0x72c0, 0x1000, 12, onWeapon5SChange);


// SELECTED HAUT LIGHT
void onPcaButton1SChange(unsigned int newValue) {
    led_selected_haut[1] = newValue;
}
DcsBios::IntegerBuffer pcaButton1SBuffer(0x72be, 0x2000, 13, onPcaButton1SChange);
void onPcaButton2SChange(unsigned int newValue) {
    led_selected_haut[2] = newValue;
}
DcsBios::IntegerBuffer pcaButton2SBuffer(0x72be, 0x4000, 14, onPcaButton2SChange);
void onPcaButton3SChange(unsigned int newValue) {
    led_selected_haut[3] = newValue;
}
DcsBios::IntegerBuffer pcaButton3SBuffer(0x72be, 0x8000, 15, onPcaButton3SChange);
void onPcaButton4SChange(unsigned int newValue) {
    led_selected_haut[4] = newValue;
}
DcsBios::IntegerBuffer pcaButton4SBuffer(0x72c0, 0x0001, 0, onPcaButton4SChange);
void onPcaButton5SChange(unsigned int newValue) {
    led_selected_haut[5] = newValue;
}
DcsBios::IntegerBuffer pcaButton5SBuffer(0x72c0, 0x0002, 1, onPcaButton5SChange);


// CONSOLE PANEL LIGHT
void onDashPanelLgtKnobChange(unsigned int newValue) {
    analogWrite(pannel_led, newValue/256);
}
DcsBios::IntegerBuffer dashPanelLgtKnobBuffer(0x729c, 0xffff, 0, onDashPanelLgtKnobChange);
void onCautAdvLgtChange(unsigned int newValue) {
    matrix.setBrightness(newValue/4096);
    matrix_bas.setBrightness(newValue/4096);
}
DcsBios::IntegerBuffer cautAdvLgtBuffer(0x72a2, 0xffff, 0, onCautAdvLgtChange);


void loop() {
  DcsBios::loop();

  // button reading
  for (int channel = 0; channel < channelsSize; channel++){
    // set channel selected to 0
    for (int i = 0; i < channelsSize; i++){
      if (i == channel) {
        pinMode(channels[i], OUTPUT);
        digitalWrite(channels[i], 0);
      } else {
        pinMode(channels[i], INPUT);
      }
    }
    // delay(10);

    for (int i = 0; i < inputsSize; i++){
      if (digitalRead(inputs[i]) != lastButtonState[inputsSize*channel+i]) {
        lastButtonState[inputsSize*channel+i] = !lastButtonState[inputsSize*channel+i];
        Joystick.setButton(inputsSize*channel+i, !lastButtonState[inputsSize*channel+i]);
      }
    }
  }
  

  // deal with the upper multiplexer
  if (previous_text_haut != text_haut or led_selected_haut != previous_led_selected_haut) {
    // i is the led indicator in the 7 segment aff
    for (uint8_t i=0; i<8; i++) {
      
      // digits that are enabled in binary (2 = 010 enable the second digit)
      int digits = 0;
  
      // for pret led
      if (i == 0) {
        // selected
        for  (int j = 1; j < 6; j++) {
          if (led_selected_haut[j]) {
            digits = digits + power( 2, digit_num_correspondance[3*(j-1)] );
          }
        }
      // for other digits leds
      } else {
        for (int letter_i = 0; letter_i < strlen(text_haut); letter_i++) {
          if  (text_haut[letter_i] == '0' and zero[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == '1' and one[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == '2' and two[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == '3' and three[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == '4' and four[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == '5' and five[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == '6' and six[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == '7' and seven[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == '8' and height[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == '9' and nine[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'A' and A[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'B' and B[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'C' and C[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'D' and D[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'E' and E[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'F' and F[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'G' and G[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'H' and H[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'I' and I[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'J' and J[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'K' and K[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'L' and L[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'M' and M[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'N' and N[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'O' and O[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'P' and P[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'Q' and Q[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'R' and R[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'S' and S[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'T' and T[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'U' and U[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'V' and V[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'W' and W[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'X' and X[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'Y' and Y[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == 'Z' and Z[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_haut[letter_i] == ' ' and space[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          }
        }
      }
      
      matrix.displaybuffer[i] = digits;
      matrix.writeDisplay();
    }
  }
  

  // deal with the bottom multiplexer
  if (previous_text_bas != text_bas or led_selected_bas != previous_led_selected_bas or led_pret_bas != previous_led_pret_bas) {
    // i is the led indicator in the 7 segment aff
    for (uint8_t i=0; i<8; i++) {
      
      // digits that are enabled in binary (2 = 010 enable the second digit)
      int digits = 0;
  
      // for pret led
      if (i == 0) {
        // selected
        for  (int j = 1; j < 6; j++) {
          if (led_selected_bas[j]) {
            digits = digits + power( 2, digit_num_correspondance[3*(j-1)] );
          }
        }
        // pret
        for  (int j = 1; j < 6; j++) {
          if (led_pret_bas[j]) {
            digits = digits + power( 2, digit_num_correspondance[3*(j-1)+2] );
          }
        }
        // canon
        if (led_selected_bas[0]) {
          digits = digits + power( 2, digit_num_correspondance[1] );
        }
        if (led_pret_bas[0]) {
          digits = digits + power( 2, digit_num_correspondance[4] );
        }
      // for other digits leds
      } else {
        for (int letter_i = 0; letter_i < strlen(text_bas); letter_i++) {
          if  (text_bas[letter_i] == '0' and zero[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == '1' and one[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == '2' and two[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == '3' and three[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == '4' and four[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == '5' and five[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == '6' and six[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == '7' and seven[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == '8' and height[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == '9' and nine[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'A' and A[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'B' and B[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'C' and C[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'D' and D[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'E' and E[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'F' and F[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'G' and G[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'H' and H[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'I' and I[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'J' and J[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'K' and K[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'L' and L[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'M' and M[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'N' and N[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'O' and O[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'P' and P[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'Q' and Q[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'R' and R[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'S' and S[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'T' and T[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'U' and U[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'V' and V[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'W' and W[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'X' and X[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'Y' and Y[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == 'Z' and Z[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          } else if (text_bas[letter_i] == ' ' and space[i]) {
            digits = digits + power( 2, digit_num_correspondance[letter_i] );
          }
        }
      }
      
      matrix_bas.displaybuffer[i] = digits;
      matrix_bas.writeDisplay();
    }
  }  
  
  //delay(refresh_rate);
}


int power(int base, int exp)
{
    int result = 1;
    while(exp) { result *= base; exp--; }
    return result;
}
