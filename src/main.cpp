#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Ringtone.h>
// Constants
const int led_biru = 11;
const int led_hijau = 12;
const int safety[] = {10, 9, 8, 7, 6};
const int r1 = 13;
const int r3 = A2;
const int OKE = 2;
const int NEXT = 4;
const int buz = 3;
const float BETA = 3950;
const float delay_button = 125;

// Global Variables
LiquidCrystal_I2C lcd(0x27, 20, 4);
float set_point = 0.00;
float TEMPERATUR_CELCIUs, temperatur_KELVIN, temperatur_FAHRENHEIT;
unsigned long WaktuAkhir = 0, loop_sensor = 10000;
bool stoploop = false;
bool output_1 = false;
bool output_2 = false;
bool output_3 = false;
int PILIH = 1;
int pilihan = 1;

// Function Declarations
void EROR();
void buzz();
void dsp(const char *m1, const char *m2, const char *m3, const char *m4);
void MENU();
void outSET();
void readTemperature();
void checkSafety();

void setup()
{
  lcd.begin(20, 4);
  lcd.backlight();
  Serial.begin(9600);

  pinMode(led_hijau, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(led_biru, OUTPUT);
  pinMode(NEXT, INPUT);
  pinMode(OKE, INPUT);
  pinMode(A1, INPUT);

  for (int w = 0; w < 5; ++w)
  {
    pinMode(safety[w], INPUT);
  }
}

void loop()
{
  if (stoploop)
  {
    return;
  }

  lcd.setCursor(7, 3);
  lcd.print(set_point);
  lcd.print("  ");

  dsp("TEMPERATUR", "SUHU:", "    ", "    ");

  checkSafety();
  readTemperature();

  if (TEMPERATUR_CELCIUs < set_point && output_1)
  {
    lcd.setCursor(18, 0);
    lcd.print("**");
    digitalWrite(r1, HIGH);
    digitalWrite(led_hijau, LOW);
    Serial.println("SUHU OVERHEAT:");
    Serial.println(TEMPERATUR_CELCIUs);
    digitalWrite(led_biru, LOW);
  }

  if (digitalRead(OKE) == HIGH)
  {
    while (digitalRead(OKE) == HIGH || digitalRead(NEXT) == HIGH)
    {
      buzz();
      break;
    }
    MENU();
  }

  if (digitalRead(NEXT) == HIGH)
  {
    PILIH += 1;
    if (PILIH > 3)
    {
      PILIH = 1;
    }
    while (digitalRead(OKE) == HIGH || digitalRead(NEXT) == HIGH)
    {
      buzz(); // Debouncing
      break;
    }
  }

  switch (PILIH)
  {
  case 1:
    Serial.println("SUHU DALAM CELCIUS:");
    Serial.println(TEMPERATUR_CELCIUs);
    lcd.setCursor(7, 1);
    lcd.print(TEMPERATUR_CELCIUs);
    lcd.print(" C  ");
    break;
  case 2:
    Serial.println("Suhu dalam KELVIN:");
    Serial.println(temperatur_KELVIN);
    lcd.setCursor(7, 1);
    lcd.print(temperatur_KELVIN);
    lcd.print(" K  ");
    break;
  case 3:
    Serial.println("Temperatur dalam FAHRENHEIT:");
    Serial.println(temperatur_FAHRENHEIT);
    lcd.setCursor(7, 1);
    lcd.print(temperatur_FAHRENHEIT);
    lcd.print(" F  ");
    break;
  }

  Serial.print("loop_sensor.................");
  Serial.println(WaktuAkhir);
}

void EROR()
{
  lcd.clear();
  digitalWrite(led_biru, HIGH);
  digitalWrite(led_hijau, LOW);
  digitalWrite(r1, LOW);
  Serial.println(".........SISTEM EROR........");
  lcd.setCursor(2, 2);
  lcd.print("SISTEM DIHENTIKAN");
}

void buzz()
{
  tone(buz, NOTE_45);
  delay(delay_button);
  noTone(buz);
  delay(delay_button);
}

void dsp(const char *m1, const char *m2, const char *m3, const char *m4)
{
  lcd.setCursor(5, 0);
  lcd.print(m1);
  lcd.setCursor(0, 1);
  lcd.print(m2);
  lcd.setCursor(0, 2);
  lcd.print(m3);
  lcd.setCursor(0, 3);
  lcd.print(m4);
}

void MENU()
{
  int i = 0, nm = 1, find = 1;
  while (true)
  {
    lcd.clear();
    dsp("PILIH MENU", "1. SUHU", "2. Relay", "3. kembali");
    lcd.setCursor(0, pilihan);
    lcd.print(">");
    lcd.setCursor(9, 1);
    lcd.print(set_point);
    lcd.print("   ");

    while (true)
    {
      if (stoploop)
      {
        return;
      }

      if (digitalRead(NEXT) == HIGH)
      {
        pilihan += 1;
        if (pilihan > 3)
        {
          pilihan = 1;
        }
        Serial.println("pilihan:");
        Serial.println(pilihan);
        buzz();
        break;
      }

      while (pilihan == 1)
      {
        if (digitalRead(OKE) == HIGH)
        {
          lcd.setCursor(9, 1);
          lcd.print(set_point);
          lcd.print("   ");

          if (digitalRead(NEXT) == HIGH)
          {
            buzz();
            set_point += 0.50;
            if (set_point >= 10.00)
            {
              set_point = -10.00;
            }
            Serial.println("set suhu..:");
            Serial.println(set_point);
          }
        }
        else
        {
          break;
        }
      }

      while (pilihan == 2)
      {
        if (digitalRead(OKE) == HIGH)
        {
          if (digitalRead(NEXT) == HIGH)
          {
            dsp("PILIH OUT", " OUT 1", " OUT 2", "OUT 3");
            i++;
            buzz();
            Serial.print("VALUE i = ");
            Serial.println(i);
            for (int asd = 1; asd < find; asd--)
            {
              lcd.setCursor(0, i);
              lcd.print(">");
            }
          }
        }
        else
        {
          break;
        }
      }

      while (pilihan == 3)
      {
        if (digitalRead(OKE) == HIGH)
        {
          if (digitalRead(NEXT) == HIGH)
          {
            buzz();
            for (int x = 0; x < 20; x++)
            {
              lcd.setCursor(x, 2);
              lcd.print(".");
              delay(250);
            }
            lcd.clear();
            return;
          }
          else
          {
            dsp("          ", "KEMBALI..??   ", "            ", "             ");
          }
        }
        else
        {
          break;
        }
      }

      checkSafety();
    }
  }
}

void outSET()
{
  int i = 1, sc = 1;

  while (true)
  {
    dsp("PILIH RELAY", " OUT 1", " OUT 2", " OUT 3  ");
    lcd.setCursor(0, i);
    lcd.print(">");

    if (sc > 3)
    {
      dsp("PILIH RELAY", " OUT 2", " OUT 3", " Kembali");
      lcd.setCursor(0, i);
      lcd.print(">");
    }

    while (true)
    {
      if (digitalRead(NEXT) == HIGH)
      {
        i++;
        sc++;
        if (i > 3)
        {
          i = 3;
          if (sc > 4)
          {
            i = 1;
            sc = 1;
          }
        }
        while (digitalRead(OKE) == HIGH || digitalRead(NEXT) == HIGH)
        {
          buzz();
          break;
        }
        Serial.print("i..:");
        Serial.println(i);
        Serial.print("sc..:");
        Serial.println(sc);
        break;
      }

      if (digitalRead(OKE) == HIGH)
      {
        switch (sc)
        {
        case 1:
          output_1 = true;
          break;
        case 2:
          output_2 = true;
          break;
        case 3:
          output_3 = true;
          break;
        case 4:
          return;
          break;
        }
      }
    }
  }
}

void readTemperature()
{
  unsigned long waktu = millis();
  if (waktu - WaktuAkhir >= loop_sensor)
  {
    int analogValue = analogRead(A0);
    TEMPERATUR_CELCIUs = 1 / (log(1 / (1023.0 / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
    temperatur_KELVIN = TEMPERATUR_CELCIUs + 273.15;
    temperatur_FAHRENHEIT = (TEMPERATUR_CELCIUs * 9 / 5) + 32;
    WaktuAkhir = waktu;
  }
}

void checkSafety()
{
  for (int w = 0; w < 5; w++)
  {
    if (digitalRead(safety[w]) == HIGH)
    {
      stoploop = true;
      if (stoploop)
      {
        EROR();
        return;
      }
    }
  }
}
