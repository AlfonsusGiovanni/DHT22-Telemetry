/*---START KONFIGURASI NRF34---*/
#include "RF24.h"
#include "nRF24L01.h"
#include "SPI.h"

#define CE_PIN 10
#define CSN_PIN 9

RF24 radio(CE_PIN, CSN_PIN);

const byte 
address[5] = "00001";

int
hmdty_val,
temp_val;

float
new_hmdty_val, 
new_temp_val;

int
recieve_data[5];
/*---END KONFIGURASI NRF24---*/

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop(){
  if(radio.available()){
    radio.read(&recieve_data, sizeof(recieve_data));
    
    if(recieve_data[0] == 112 && recieve_data[1] == 211 && recieve_data[4] == 512){
      hmdty_val = recieve_data[2];
      temp_val = recieve_data[3];

      new_hmdty_val = hmdty_val / 100; 
      new_temp_val = temp_val / 100; 

      Serial.print("Humidity: ");
      Serial.print(new_hmdty_val);
      Serial.print(" ");
      Serial.print("Temperature: ");
      Serial.print(new_temp_val);
      Serial.println("C");
    }

    digitalWrite(2, HIGH);
  }

  else{
    digitalWrite(2, LOW);
  }
}
