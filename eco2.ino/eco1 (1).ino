#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHT_PIN    2
#define FLAME_PIN  A2
#define SOIL_PIN   A0
#define WATER_PIN  A1

DHT dht(DHT_PIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long startTime;

// LCD экранды жыпылықтатпай жаңарту функциясы
void lcdPrint(const char* r0, const char* r1) {
  lcd.setCursor(0, 0);
  char buf[17];
  snprintf(buf, 17, "%-16s", r0);
  lcd.print(buf);
  
  lcd.setCursor(0, 1);
  snprintf(buf, 17, "%-16s", r1);
  lcd.print(buf);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  lcd.init();
  lcd.backlight();
  startTime = millis();
}

void loop() {
  // 1 минут 30 секунд (90000 миллисекунд) өткен соң жүйені тоқтату
  unsigned long elapsedTime = millis() - startTime;
  
  if (elapsedTime >= 90000UL) {
    lcdPrint("Zhyie oshti!", "Kutiniz...");  
    delay(2000);
    lcd.noBacklight();
    while (true); 
  }

  // Қалған уақытты есептеу (Секундпен)
  unsigned long timeLeft = (90000UL - elapsedTime) / 1000;
  int minLeft = timeLeft / 60;
  int secLeft = timeLeft % 60;

  float temp  = dht.readTemperature();
  float hum   = dht.readHumidity();
  int   soil  = analogRead(SOIL_PIN);
  int   water = analogRead(WATER_PIN);
  int   flame = analogRead(FLAME_PIN);

  // Сенсордан мәлімет келмей қалса, қатені шығару
  if (isnan(temp) || isnan(hum)) {
    lcdPrint("DHT Error!", "Check sensor");
    delay(2000);
    return;
  }

  // Serial портқа JSON жіберу
  Serial.print("{");
  Serial.print("\"temp\":");  Serial.print(temp, 1);  Serial.print(",");
  Serial.print("\"hum\":");   Serial.print(hum, 1);   Serial.print(",");
  Serial.print("\"soil\":");  Serial.print(soil);     Serial.print(",");
  Serial.print("\"water\":"); Serial.print(water);    Serial.print(",");
  Serial.print("\"flame\":"); Serial.print(flame);
  Serial.println("}");

  char r0[17], r1[17];
  char tempStr[6], humStr[6];

  // Бөлшек сандарды мәтінге айналдыру 
  dtostrf(temp, 4, 1, tempStr);
  dtostrf(hum, 4, 1, humStr);

  // Экран 1 — Temp / Hum 
  snprintf(r0, 17, "Temp: %s C", tempStr); 
  snprintf(r1, 17, "Hum:  %s %%", humStr);
  lcdPrint(r0, r1);
  delay(2000);

  // Экран 2 — Soil / Water
  snprintf(r0, 17, "Soil:  %4d", soil);
  snprintf(r1, 17, "Water: %4d", water);
  lcdPrint(r0, r1);
  delay(2000);

  // Экран 3 — Flame
  const char* status = (flame > 500) ? "!! DANGER !!" : "Normal - OK";
  snprintf(r0, 17, "Flame: %4d", flame);
  lcdPrint(r0, status);
  delay(2000);

  // Экран 4 — Таймерді көрсету
  snprintf(r0, 17, "Oshuge kaldy:");
  snprintf(r1, 17, "%02d min %02d sec", minLeft, secLeft);
  lcdPrint(r0, r1);
  delay(2000);
}
