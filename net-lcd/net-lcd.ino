/*
	Lcd Themometer

	LM35 temperature sensor based thermometer with multifunction LCD display
    with two display modes: sensor values & graphical display.

	The circuit:
	    In: A0: LM35 sensor
             2: button for switching between display modes

        Out: 5: LCD D7
             6: LCD D6
             7: LCD D5
             8: LCD D4
            11: LCD E
            12: LCD RS

	Created 25/10/2013
	By Niko Salakka
    Modified 28/10/2013
    By Niko Salakka

	https://github.com/anobi/arduino
*/

#include <LiquidCrystal.h>

//inputs & outputs
LiquidCrystal lcd(12, 11, 8, 7, 6, 5);

//display mode variables
int screenMode = 0;
long time = 0;

//special characters for graphical display mode
byte bt[8] = {0x1f,0x0,0x0,0x0,0x0,0x0,0x0,0x1f}; //top and bottom border
byte br[8] = {0x1f,0x1,0x1,0x1,0x1,0x1,0x1,0x1f}; //right border

void setup(){
    lcd.begin(16, 2);
    Serial.begin(9600);

    //init display outputs
    for(int pinNumber = 4; pinNumber < 7; pinNumber++){
        pinMode(pinNumber, OUTPUT);
        digitalWrite(pinNumber, LOW);
    }

    //create special characters
    lcd.createChar(1, bt);
    lcd.createChar(2, br);
}

void loop(){
    //draw the screen with the selected display mode
    lcd.clear();
	draw_screen();
    delay(1000);
}

//display mode switching function
void dispMode(){
    if(millis () - time > debounce){
        Serial.println("click");
        if(screenMode == 0){
            screenMode = 1;
        }
        else{
            screenMode = 0;
        }
        time = millis();
    }
}

//draw display
void print_message(char[]* message){
    //draw header on lcd
    lcd.setCursor(0, 0);
    lcd.print(message);

    //draw values
    lcd.setCursor(0, 1);
    lcd.print(sensorVal);

    lcd.setCursor(7, 1);
    lcd.print(voltage);

    lcd.setCursor(12, 1);
    lcd.print(temperature);
}

//graphical display
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
