#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define TRIGGER_PIN 2 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 3 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
void setup()
{
Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
lcd.init(); 
lcd.backlight();
}
void loop()
{
  delay(100); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.setCursor(9, 1);
  lcd.print(uS / US_ROUNDTRIP_CM);
  lcd.setCursor(12, 1);
  lcd.print("cm");
}
