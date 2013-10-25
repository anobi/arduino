/*
* LM35 sensor based thermometer with multifunction LCD display
* with two display modes: a lot of stuff and a cool simple 15 to 30 celsius bar graph
* Feel free to use as you wish, if you wish
* Author: Niko Salakka, 2013
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int sensorPin = A0;

int switchState = 0;
int prevState = 0;

int screenMode = 0;
long time = 0;
long debounce = 200;

int sensorVal = 0;
float voltage = 0;
float temperature = 0;

//special characters 
//bt = border top & br = border right
byte celsius[8] = {0x4,0xa,0x4,0x0,0x0,0x0,0x0};
byte bt[8] = {0x1f,0x0,0x0,0x0,0x0,0x0,0x0,0x1f};
byte br[8] = {0x1f,0x1,0x1,0x1,0x1,0x1,0x1,0x1f};

void setup(){
    lcd.begin(16, 2);
    Serial.begin(9600);
    pinMode(6, INPUT); //display mode button

    for(int pinNumber = 2; pinNumber < 5; pinNumber++){
        pinMode(pinNumber, OUTPUT);
        digitalWrite(pinNumber, LOW);
    }
    lcd.createChar(0, celsius);
    lcd.createChar(1, bt);
    lcd.createChar(2, br);
}

void loop(){

    //handle the display mode button
    switchState = digitalRead(6);
    if(switchState == 1 && prevState == 0 && millis() - time > debounce){
        if(screenMode == 0){
            screenMode = 1;
        }
        else{
            screenMode = 0;
        }
        time = millis();
    }
    prevState = switchState;

    //refresh the sensor data
    sensorVal = analogRead(sensorPin);
    voltage = (sensorVal / 1024.0) * 5.0;
    temperature = (voltage - .5) * 100;

    //draw the screen
    lcd.clear();
    if(screenMode == 0){
        digiTemp();
    }
    else if(screenMode == 1){
        analogTemp();
    }
    delay(1000);
}

void digiTemp(){

    //draw header on lcd
    lcd.setCursor(0, 0);
    lcd.print("Sensor Volt Temp");

    //draw data
    lcd.setCursor(0, 1);
    lcd.print(sensorVal);

    lcd.setCursor(7, 1);
    lcd.print(voltage);

    lcd.setCursor(12, 1);
    lcd.print(temperature);
}

void analogTemp(){

    //write the digital temperature on display
    lcd.setCursor(5, 0);
    lcd.print(temperature);
    lcd.setCursor(10, 0);
    lcd.print((char)0);
    lcd.setCursor(11,0);
    lcd.print("C");

    //draw the filled part of temperature bar
    for(int i = 0; i < temperature - 15; i++){
        lcd.setCursor(i, 1);
        lcd.write(1023);
    }

    //and write the "empty" part of temperature bar
    for(int i = temperature - 15; i <= 15; i++){
        lcd.setCursor(i, 1);
        if(i == 15){
            lcd.write((char)2);
        }
        else {
            lcd.write((char)1);
        }
    }
}
