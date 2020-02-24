

#include <SoftwareSerial.h>
SoftwareSerial Serial1(10, 11); // RX, TX
uint8_t buff[40], data[30];
const uint8_t SYNC = 0x16;
uint32_t raw_pressure;
uint16_t raw_temperature;
float pressure, temperature, temperatureF;
bool p, sync_found = false;
int i = 0, j = 0, idx = 0;
void setup() {

  Serial.begin(9600);
  Serial1.begin(75);
  memset(buff, 0, sizeof(buff));
}

void loop() {

  if (Serial1.available()) {
    buff[j] = Serial1.read();
    if (buff[j] == 0x16) {
//      Serial.print("Sync Found \t Data is : ");
      sync_found = true;
//      i = 15; //Data Size
    }
    if (sync_found) {
      data[idx] = buff[j];      
//      Serial.write(buff[j]);
//      i--;
      idx++;
      if (buff[j] == 0x17) {
//        Serial.print("\tEOT Detected\n");
        sync_found = false;
        j=0;
        p=true;        
      }
    }
    j++;
  }
  if(p)//(!Serial.available())
  {
    raw_pressure = data[1];
    raw_pressure = (raw_pressure << 8) + data[2];
    raw_pressure = (raw_pressure << 8) + data[3];
    raw_temperature = data[4];
    raw_temperature = (raw_temperature << 8) + data[5];
    pressure = (float)raw_pressure / 4096.00;
    temperature = (float)raw_temperature / 100.00;
    temperatureF = (temperature*1.8)+32;
    Serial.print(pressure);
    Serial.println(" hPa(mbar)");
//    Serial.print(temperature);
//    Serial.println(" C");
    Serial.print(temperatureF);
    Serial.println(" F");
   for(i=5;i<sizeof(data);i++)
   {    
      Serial.write(data[i]);
    }
    p=false;
    memset(data, 0, sizeof(data));
    idx = 0;
    Serial.println();
    }  
}
