// Reloj de pared fabricado con matriz de neopixel, cuando se produce un sonido
// se activa el display y muestra la hora, fecha, temperatura y humedad

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "RTClib.h"
#include <DHT.h>
 

#define DHTPIN 2 // Definimos el pin digital donde se conecta el sensor de temperatura
#define DHTTYPE DHT11 // Dependiendo del tipo de sensor
DHT dht(DHTPIN, DHTTYPE); // Inicializamos el sensor DHT11

RTC_DS3231 rtc; // Iniciamos el modulo Real time clock

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define PIN 3 // Pin de datos Neopixel

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(12, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

uint16_t colors[] = {
  matrix.Color(255, 0, 0),    //rojo 0
  matrix.Color(0, 255, 0),    //verde 1
  matrix.Color(0, 0, 255),    //azul 2
  matrix.Color(100, 255, 255),//cian 3
  matrix.Color(250, 100, 100),//rosa 4
  matrix.Color(255, 255, 40)  //amarillo 5
  };

char daysOfTheWeek[7][9] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
char monthOfTheYear[12][10] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

const int knockSensor = A3; // the piezo is connected to analog pin 3
int sensorReading = 0;// variable to store the value read from the sensor pin

void setup() 
 {
  dht.begin();
  Serial.begin(9600);
  delay(3000);
  // Comprobamos si tenemos el RTC conectado
  if (! rtc.begin()) {
  Serial.println("No hay un módulo RTC");
  while (1);
  }
 
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2018, 4, 22, 17, 45, 00));
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(50);
  matrix.setTextColor(colors[0]);
}

int x    = matrix.width();
int color = 0;
int velocidad = 70; // aumentar el numero para menor velocidad de scroll

void muestra_hora()
{
  for (int x = 12; x>-60; x=x-1)
  {
  DateTime now = rtc.now();
  matrix.setCursor(x,0);
  matrix.fillScreen(0);
  matrix.print(now.hour());
  matrix.print(":");
  
  if (now.minute()<10) matrix.print("0");
  matrix.print(now.minute());
  
  matrix.print(":");
  
  if (now.second()<10) matrix.print("0");
  matrix.print(now.second());
  matrix.setTextColor(colors[1]);
  matrix.show();
  delay(velocidad);
  }
} 

void muestra_fecha()
{
  for (int x = 12; x>-115; x=x-1)
  {
  DateTime now = rtc.now();
  matrix.setCursor(x,0);
  matrix.fillScreen(0); 
    
  matrix.print(now.day());
  matrix.print(" ");
  matrix.print(monthOfTheYear[(now.month()-1)]);  
  matrix.print(" ");
  matrix.print(now.year());
  matrix.setTextColor(colors[2]);
  matrix.show();
  delay(velocidad);
  }
}

void muestra_dia()
 {
   for (int x = 12; x>-50; x=x-1)
  {
  DateTime now = rtc.now();
  matrix.setCursor(x,0);
  matrix.fillScreen(0); 
  
  matrix.print(daysOfTheWeek[now.dayOfTheWeek()]);  
  //matrix.print(now.day());
  matrix.setTextColor(colors[2]);
  matrix.show();
  delay(velocidad);
  }
 }
 
void muestra_temperatura()
 {
   // Leemos la humedad relativa
  int h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  int t = dht.readTemperature();
   
   for (int x = 12; x>-65; x=x-1)
  {
  
  matrix.setCursor(x,0);
  matrix.fillScreen(0); 
  matrix.print(t);matrix.print("C  ");
  matrix.print(h);matrix.print("%  ");
  //matrix.print(hic);matrix.print("*C ");
  //Serial.print(hic);
  if (t<=20) matrix.setTextColor(colors[3]); //cian
  if (t>20) matrix.setTextColor(colors[1]); //verde
  if (t>=30) matrix.setTextColor(colors[0]); //rojo
  
  matrix.show();
  delay(velocidad);
  }
 }
 
void loop() {  
  sensorReading = analogRead(knockSensor);
  Serial.println(sensorReading);
  if (sensorReading >= 545) 
  {
  muestra_hora();
  muestra_dia();
  muestra_fecha();
  muestra_temperatura();
  }  
}

  
