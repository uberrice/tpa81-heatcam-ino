/********************************************
*    arduino example for TPA81 and LCD03    *
*         TPA81 controlled by I2C           *
*        LCD03 controlled by serial         *
*                                           *
*         By James Henderson 2012           *
********************************************/

#include <Wire.h>
#include <RGB_LED_Matrix.h>
//#include <SoftwareSerial.h>

#define SERVOSTEP           2
#define TEMPSTEP            2
#define ADDRESS             0x68                                   // Address of TPA81
#define SOFTREG             0x00                                   // Byte for software version
#define AMBIANT             0x01                                   // Byte for ambiant temperature
RGB_LED_Matrix matrix;
int colorchoose;
int temperature[] = {0,0,0,0,0,0,0,0};                   // Array to hold temperature data
long colors[] = {
  0x00CC3300, //darkblue 1
  0x00990099, //dviolet 2
  0x006600CC, //violet 3
  0x000000CC, //red 4
  0x00005CE6, //orange 5
  0x0000FFFF, //yellow 6 
  0x00B3FFFF, //lyellow 7
  0x00FFFFFF //white 8
};

int y = 0;
void setup(){
  pinMode(2,INPUT);
  Wire.begin();
  delay(100);                                            // Wait to make sure everything is powerd up

  //Setup RGB DISPLAY
  matrix.start();
  delay(2000);
  matrix.clear_screen(0x00000000);
  matrix.update_screen();
  //End SETUP RGB DISPLAY
}
  
void loop(){
  if(digitalRead(2) || y>0){
  for(int i = 0; i < 8; i++){                            // Loops and stores temperature data in array
  temperature[i] = getData(i+2);
  }                     
  int ambiantTemp = getData(AMBIANT);                    // Get reading of ambiant temperature and print to LCD03 screen


  //Displayanzeige----------------------------------------------
  for( int i = 0; i < 8; i++){
      if (temperature[i] < ambiantTemp) colorchoose = 0;
      else {
        colorchoose = temperature[i]-ambiantTemp;
        colorchoose = colorchoose/TEMPSTEP;
        if(colorchoose > 7) colorchoose = 7;
      }
      matrix.dot(i,y,colors[colorchoose]);     
  }
  y+=1;
  if (y>7){ 
    y=0;
    matrix.update_screen();
  }
  
  //Ende Displayanzeige------------------------------------------


  //Servo--------------------------------------------------------
  Wire.beginTransmission(ADDRESS);
    Wire.write(0x00);
    Wire.write(y*SERVOSTEP);
  Wire.endTransmission();
  //Ende Servo--------------------------------------------------- 
  delay(25*SERVOSTEP);
  } 
}

byte getData(byte reg){                                   // Function to receive one byte of data from TPA81
  Wire.beginTransmission(ADDRESS);                        // Begin communication with TPA81
    Wire.write(reg);                                      // Send reg to TPA81
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS, 1);                           // Request 1 byte
  while(Wire.available() < 1);                            // Wait for byte to arrive
  byte data = Wire.read();                                // Get byte
  return(data);                                           // return byte
}
