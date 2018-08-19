#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTTYPE DHT22
#define DHTPIN_2 2
#define DHTPIN_3 3
#define DHTPIN_4 4
#define DHTPIN_5 5

DHT dht2(DHTPIN_2, DHTTYPE);
DHT dht3(DHTPIN_3, DHTTYPE);
DHT dht4(DHTPIN_4, DHTTYPE);
DHT dht5(DHTPIN_5, DHTTYPE);

LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  Serial.begin(9600);
  Serial.println("setup()");

  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();

lcd.backlight();
  lcd.begin (20, 4);
  //lcd.backlight();

  lcd.setBacklight(HIGH);
  //lcd.home(); // go home
}

void printDhtOutput(int pin, float temperature, float humidity) {

  if (isnan(temperature) || isnan(humidity)) {
    Serial.print("Failed to read from dht");
    Serial.print(pin);
    Serial.println(" sensor!");
    return;
  }

  char temperatureOutput[6];
  dtostrf(temperature, 5, 1, temperatureOutput);
  
  lcd.setCursor(0, pin - 2);
  lcd.print(temperatureOutput);
  lcd.print((char)223);
  lcd.print("C");

  char humidityOutput[6];
  dtostrf(humidity, 5, 1, humidityOutput);

  lcd.setCursor(14, pin - 2);
  lcd.print(humidityOutput);
  lcd.print("%"); 

  // print to console
  //Serial.print("dht");
  //Serial.print(pin);
  //Serial.print(":\t");

  //Serial.print("temperature: ");
  //Serial.print(temperature);
  //Serial.print(" *C\t");

  //Serial.print("humidity: ");
  //Serial.print(humidity);
  //Serial.println(" %"); 
}

int lcd_reset_counter = 0;

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  float dht2_temperature = dht2.readTemperature();
  float dht2_humidity = dht2.readHumidity();

  float dht3_temperature = dht3.readTemperature();
  float dht3_humidity = dht3.readHumidity();

  float dht4_temperature = dht4.readTemperature();
  float dht4_humidity = dht4.readHumidity();

  float dht5_temperature = dht5.readTemperature();
  float dht5_humidity = dht5.readHumidity();

  if (lcd_reset_counter >= 7200) {
    lcd_reset_counter = 0;
    lcd.begin (20, 4);
  }

  printDhtOutput(DHTPIN_2, dht2_temperature, dht2_humidity);
  printDhtOutput(DHTPIN_3, dht3_temperature, dht3_humidity);
  printDhtOutput(DHTPIN_4, dht4_temperature, dht4_humidity);
  printDhtOutput(DHTPIN_5, dht5_temperature, dht5_humidity);

  lcd_reset_counter++;
}
