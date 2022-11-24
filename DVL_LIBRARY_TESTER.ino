#include "T4_DVL.h"
// DVL SERIAL is defined as Serial1 aka LPUART1

#define SPEED_OF_SOUND    1501.0f   // 1400-1500 meters/sec
#define MAX_TRACKING      100.0f    // [0, 250] meters
#define SOFT_TRIG         0         // bool

void setup() {
  Serial.begin(115200);
  init_DVL_serial();
  Serial.printf("Serial 1\nTX Capacity:\t%d bytes\nRX Capacity:\t%d bytes\n", DVLSERIAL.availableForWrite(), DVLSERIAL.available());
  
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t tt = 0;
  static bool spamData = false;
  if(Serial.available()){
    tt = Serial.read();
  }
  int e = -1;
  switch(tt){
    case 'r':
      Serial.print("Getting System Config\n");
      e = request_DVL_System_Config();
      if(e){
        Serial.printf("DVL Error On Match: %d\n\n", e); 
      } else {
        print_DVL_System_Config();
        Serial.printf("Done.\n\n");
      }
    break;
    case 's':
      Serial.print("Getting Setup.\n");
      e = request_DVL_Setup_Config();
      if(e){
        Serial.printf("DVL Error On Match: %d\n\n", e); 
      } else {
        print_DVL_Setup_Config();
        Serial.printf("Done.\n\n");  
      }
    break;

    case 'c':
//int set_DVL_Setup_Config(float speed_of_sound_, float max_track_range, uint8_t baudrate, uint8_t soft_trig);
      Serial.print("Reconfiguring DVL.\n");
      e = set_DVL_Setup_Config(SPEED_OF_SOUND, MAX_TRACKING, DVL_115200_BAUD, SOFT_TRIG);
      if(e){
        Serial.printf("DVL Error On Match: %d\n\n", e);
      } else {
        Serial.printf("Done.\n\n");
      }
    break;

    case 'q':
      spamData = !spamData;
      Serial.printf("Spam Data: %u\n", spamData);
    break;
    
    default:
      if(DVLSERIAL.available() && spamData){
        DVL_DATA_UPDATE();
        print_current_vel();
        print_current_beam_range();
        print_current_power();
        print_current_BIT();
        delay(100);
      }
    break;
  }

}
