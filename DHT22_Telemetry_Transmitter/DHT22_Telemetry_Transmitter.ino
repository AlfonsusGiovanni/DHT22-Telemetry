/*---START KONFIGURASI SENSOR---*/
#include "DHT.h"

#define DHTPIN A0
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE); 

float 
hmdty_val,
temp_val;
/*---END KONFIGURASI SENSOR---*/

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
new_hmdty_val, 
new_temp_val;

int
send_data[5];
/*---END KONFIGURASI NRF24---*/

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  dht.begin();
}

void loop(){
  hmdty_val = dht.readHumidity();
  temp_val = dht.readTemperature();

  if (isnan(hmdty_val) || isnan(temp_val)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  new_hmdty_val = hmdty_val * 100;
  new_temp_val = temp_val * 100;

  send_data[0] = 112;
  send_data[1] = 211;
  send_data[2] = new_hmdty_val;
  send_data[3] = new_temp_val;
  send_data[4] = 512;

  radio.write(&send_data, sizeof(send_data));
  delay(500);
}
