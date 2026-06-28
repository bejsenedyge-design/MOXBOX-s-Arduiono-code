#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ── Пиндер ──────────────────────────
#define DHT_PIN      2    // DHT11 S аяғы
#define FLAME_PIN    3    // Flame сенсор D0
#define BUZZER_PIN   8    // Buzzer S аяғы
#define SOIL_PIN     A0   // Топырақ сенсоры
#define WATER_PIN    A1   // Су деңгейі сенсоры

// ── Шектер ──────────────────────────
#define TEMP_MAX     35.0  // °C жоғары болса қауіп
#define HUM_MAX      80.0  // % жоғары болса қауіп
#define SOIL_DRY     700   // Топырақ құрғақ шегі
#define WATER_LOW    200   // Су аз шегі

// ── Объектілер ───────────────────────
DHT dht(DHT_PIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Егер көрсетпесе 0x3F деп өзгерт

// ── Бузер функциясы ──────────────────
void buzzerBeep(int times, int duration) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    delay(150);
  }
}

// ── LCD-ге жазу ──────────────────────
void lcdPrint(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FLAME_PIN, INPUT);
  
  dht.begin();
  
  lcd.init();
  lcd.backlight();
  lcdPrint("  Жүйе іске   ", "   қосылуда...");
  delay(2000);
  
  Serial.begin(9600);
}

void loop() {
  // ── Мәндерді оқу ──
  float temp  = dht.readTemperature();
  float hum   = dht.readHumidity();
  int   soil  = analogRead(SOIL_PIN);
  int   water = analogRead(WATER_PIN);
  
  // 🛠️ ЖӨНДЕЛГЕН ЖЕР: HIGH = от бар деп өзгерттік!
  bool  flame = (digitalRead(FLAME_PIN) == HIGH); 

  // DHT11 қате берсе
  if (isnan(temp) || isnan(hum)) {
    lcdPrint("DHT11 QATE!   ", "Sensor tekser ");
    buzzerBeep(2, 500);
    delay(2000);
    return;
  }

  // Serial Monitor-ға шығару
  Serial.print("Temp: "); Serial.print(temp);
  Serial.print(" | Hum: "); Serial.print(hum);
  Serial.print(" | Soil: "); Serial.print(soil);
  Serial.print(" | Water: "); Serial.print(water);
  Serial.print(" | Flame: "); Serial.println(flame ? "BAR!" : "Jok");

  // ════════════════════════════
  //        ҚАУІП ЖАҒДАЙЛАР
  // ════════════════════════════

  // 🔥 ОТ АНЫҚТАЛДЫ — ең қауіпті!
  if (flame) {
    lcdPrint("!!! OT BAR !!!", "QAUP! SHYGHYN!");
    buzzerBeep(10, 200);
    delay(1000);
    return;
  }

  // 🌡️ Температура жоғары
  if (temp > TEMP_MAX) {
    lcdPrint("TEMP JOGHARY!", "T:" + String(temp,1) + "C MAX:" + String(TEMP_MAX,0));
    buzzerBeep(3, 400);
    delay(1500);
    return;
  }

  // 💧 Ылғалдылық жоғары
  if (hum > HUM_MAX) {
    lcdPrint("YLGHAL JOGHARY", "H:" + String(hum,1) + "% MAX:" + String(HUM_MAX,0));
    buzzerBeep(2, 400);
    delay(1500);
    return;
  }

  // 🌱 Топырақ құрғақ
  if (soil > SOIL_DRY) {
    lcdPrint("TOPYRAQ QURGHQ", "Su quy! " + String(soil));
    buzzerBeep(1, 300);
    delay(1500);
    return;
  }

  // ════════════════════════════
  //      ҚАЛЫПТЫ ЖАҒДАЙ
  // ════════════════════════════
  // 1-экран: Температура + Ылғал
  lcdPrint(
    "T:" + String(temp,1) + "C H:" + String(hum,1) + "%",
    "Qalypty Jaghday"
  );
  delay(2000);

  // 2-экран: Топырақ + Су деңгейі
  lcdPrint(
    "Topyraq:" + String(soil),
    "Su deng:" + String(water)
  );
  delay(2000);
}
