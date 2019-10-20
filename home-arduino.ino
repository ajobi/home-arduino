// liquid crystal library from: https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/
// DHT library used: "DHT-sensor-library"

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

// address could be also 0x3f depending on display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

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

void printDhtOutput(const int pin, const float temperature, const float humidity) {  
  const int row = pin - 2;

  if (isnan(temperature) || isnan(humidity)) {
    lcd.setCursor(0, row);
    lcd.print(" Senzor je odpojeny ");
    
    Serial.print("Failed to read from dht");
    Serial.print(pin);
    Serial.println(" sensor!");
    
    return;
  }

  // print temperature
  char temperatureOutput[6];
  dtostrf(temperature, 5, 1, temperatureOutput);
  lcd.setCursor(0, row);
  lcd.print(temperatureOutput);
  lcd.print((char)223);
  lcd.print("C");

  // clear the whitespace
  lcd.print("       ");

  // print humidity
  char humidityOutput[6];
  dtostrf(humidity, 5, 1, humidityOutput);
  lcd.setCursor(14, row);
  lcd.print(humidityOutput);
  lcd.print("%");

  // print to console
  Serial.print("dht");
  Serial.print(pin);
  Serial.print(":\t");
  
  Serial.print("temperature: ");
  Serial.print(temperature);
  Serial.print(" *C\t");

  Serial.print("humidity: ");
  Serial.print(humidity);
  Serial.println(" %"); 
}

const bool TESTING = false;  // change to enable or disable testing
int test_counter = 0;

// REFRESH INTERVAL AND ARDUINO RESET SETTINGS:
int lcd_reset_counter = 0;
int refresh_interval = 2000;      // how often the data are being read from sensors
int reset_counter_limit = 1900;   // after how many intervals the arduino is being reset
void(* resetFunc) (void) = 0;     // arduino built-in reset function declared at address 0

void loop() {
  // Wait a few seconds between measurements.
  delay(refresh_interval);

  float dht2_temperature = dht2.readTemperature();
  float dht2_humidity = dht2.readHumidity();

  float dht3_temperature = dht3.readTemperature();
  float dht3_humidity = dht3.readHumidity();

  float dht4_temperature = dht4.readTemperature();
  float dht4_humidity = dht4.readHumidity();

  float dht5_temperature = dht5.readTemperature();
  float dht5_humidity = dht5.readHumidity();

  // restart display periodically
  if (lcd_reset_counter >= reset_counter_limit) {
    lcd_reset_counter = 0;
    resetFunc();
  }

  // TESTING FOR COMMON SCENARIOS
  if (TESTING == true) {
    refresh_interval = 500;
    
    // temperature scenarios
    if (test_counter == 1) {
      dht3_temperature = 0;
    }
    if (test_counter == 2) {
      dht3_temperature = 9; 
    }
    if (test_counter == 3) {
      dht3_temperature = 99; 
    }
    if (test_counter == 4) {
      dht3_temperature = 9; 
    }
    if (test_counter == 5) {
      dht3_temperature = -9;
    }
    if (test_counter == 6) {
      dht3_temperature = -99;
    }
    if (test_counter == 7) {
      dht3_temperature = -9;
    }
    if (test_counter == 8) {
      dht3_temperature = NAN;
    }
    
    // humidity scenarios
    if (test_counter == 9) {
      dht3_humidity = 0;
    }
    if (test_counter == 10) {
      dht3_humidity = 50;
    }
    if (test_counter == 11) {
      dht3_humidity = 100;
    }
    if (test_counter == 12) {
      dht3_humidity = 50;
    }
    if (test_counter == 13) {
      dht3_humidity = 0;
    }
    if (test_counter == 14) {
      dht3_humidity = NAN;
    }
    test_counter++;
  }

  printDhtOutput(DHTPIN_2, dht2_temperature, dht2_humidity);
  printDhtOutput(DHTPIN_3, dht3_temperature, dht3_humidity);
  printDhtOutput(DHTPIN_4, dht4_temperature, dht4_humidity);
  printDhtOutput(DHTPIN_5, dht5_temperature, dht5_humidity);

  lcd_reset_counter++;
}
