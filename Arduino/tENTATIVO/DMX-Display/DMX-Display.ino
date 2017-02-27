// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DmxSerialRecv.ino: Sample DMX application for retrieving 3 DMX values:
// Codice di Matteo Zanda, basato sulla libreria DMXSerial di Matthias Hertel, http://www.mathertel.de
// - - - - -
/*******************************/

#include <Wire.h>
#include <DMXSerial.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2, 6, POSITIVE );// Set the LCD I2C address
  
/********************************/

// Constants for demo program

const int Red1Pin =    4;  // PWM output pin for Red Light.
const int Green1Pin =  3;  // PWM output pin for Green Light.
const int Blue1Pin =   2;  // PWM output pin for Blue Light.
const int Red2Pin =    7;  // PWM output pin for Red Light.
const int Green2Pin =  6;  // PWM output pin for Green Light.
const int Blue2Pin =   5;  // PWM output pin for Blue Light.
const int Red3Pin =    10;  // PWM output pin for Red Light.
const int Green3Pin =  9;  // PWM output pin for Green Light.
const int Blue3Pin =   8;  // PWM output pin for Blue Light.
const int Red4Pin =    13;  // PWM output pin for Red Light.
const int Green4Pin =  12;  // PWM output pin for Green Light.
const int Blue4Pin =   11;  // PWM output pin for Blue Light.


const int btnOkPin = 30;
const int btnPlusPin = 31;
const int btnMenoPin = 32;
boolean btnOk = false;
boolean btnPlus = false;
boolean btnMeno = false;

int pagina = 0;

int eepromDMXAddress = 0; // cella 0 della EEPROM in cui memorizzare l'indirizzo DMX di partenza
int dmxAddress = 1;
int writeCountAddress = 1; // cella 1 della EEPROM in cui memorizzare il conto delle scritture
int writeCount = 0; //numero di volte che le celle 0 e 1 sono state scritte

int red1 = 0;
int green1 = 0;
int blue1 = 0;
int red2 = 0;
int green2 = 0;
int blue2 = 0;
int red3 = 0;
int green3 = 0;
int blue3 = 0;
int red4 = 0;
int green4 = 0;
int blue4 = 0;

void setup () {
  DMXSerial.init(DMXReceiver);
  //EEPROM.update(eepromDMXAddress, 1);
  
  writeCount = EEPROM.read(writeCountAddress); //legge dalla eeprom il numero di volte che le celle 0 e 1 sono state scritte

  int readDmxAddress = EEPROM.read(eepromDMXAddress); //legge dalla eeprom il valore dell'indirizzo DMX memorizzato in precedenza
  if(readDmxAddress>0 && readDmxAddress<255) 
    dmxAddress = readDmxAddress;
  else dmxAddress= 13;
  
  // enable pwm outputs
  pinMode(Red1Pin,   OUTPUT); // sets the digital pin as output
  pinMode(Green1Pin, OUTPUT);
  pinMode(Blue1Pin,  OUTPUT);
  pinMode(Red2Pin,   OUTPUT);
  pinMode(Green2Pin, OUTPUT);
  pinMode(Blue2Pin,  OUTPUT);
  pinMode(Red3Pin,   OUTPUT);
  pinMode(Green3Pin, OUTPUT);
  pinMode(Blue3Pin,  OUTPUT);
  pinMode(Red4Pin,   OUTPUT);
  pinMode(Green4Pin, OUTPUT);
  pinMode(Blue4Pin,  OUTPUT);

  pinMode(btnPlusPin, INPUT);
  pinMode(btnMenoPin, INPUT);
  /*pinMode(btnOkPin, INPUT);*/

  
  lcd.begin(16,2); 
  lcd.backlight();
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("DMX Address:    ");
  lcd.setCursor(13,1);
  lcd.print(dmxAddress);
  delay(5);

}


void loop() {
  checkButtons(); //set dmx address
  
  // Calculate how long no data backet was received
  unsigned long lastPacket = DMXSerial.noDataSince();
  
  if (lastPacket < 5000) {
    lcd.setCursor(0,0);
        lcd.print("Receiving DMX   ");
        
    // read recent DMX values and set pwm levels 
    red1 = DMXSerial.read(dmxAddress);
    analogWrite(Red1Pin, red1);
    green1 = DMXSerial.read(dmxAddress+1);
    analogWrite(Green1Pin, green1);
    blue1 = DMXSerial.read(dmxAddress+2);
    analogWrite(Blue1Pin, blue1);
    
    red2 = DMXSerial.read(dmxAddress+3);
    analogWrite(Red2Pin, red2);
    green2 = DMXSerial.read(dmxAddress+4);
    analogWrite(Green2Pin, green2);
    blue2 = DMXSerial.read(dmxAddress+5);
    analogWrite(Blue2Pin, blue2);
    
    red3 = DMXSerial.read(dmxAddress+6);
    analogWrite(Red3Pin, red3);
    green3 = DMXSerial.read(dmxAddress+7);
    analogWrite(Green3Pin, green3);
    blue3 = DMXSerial.read(dmxAddress+8);
    analogWrite(Blue3Pin, blue3);
    
    red4 = DMXSerial.read(dmxAddress+9);
    analogWrite(Red4Pin, red4);
    green4 = DMXSerial.read(dmxAddress+10);
    analogWrite(Green4Pin, green4);
    blue4 = DMXSerial.read(dmxAddress+11);
    analogWrite(Blue4Pin, blue4);
    
    delay(5);

  } else {
    lcd.setCursor(0,0);
    lcd.print("No DMX Signal   ");
    delay(150);
  } // if
}

void checkButtons(){
    btnOk = digitalRead(btnOkPin);
    btnPlus = digitalRead(btnPlusPin);
    btnMeno = digitalRead(btnMenoPin);
    
    if (btnPlus == HIGH && btnOk == HIGH && btnMeno == LOW) { //set dmx address if button pressed
      if(dmxAddress>255) dmxAddress=0;
        dmxAddress++;
        lcd.setCursor(13,1);
        lcd.print("    ");
        lcd.setCursor(13,1);
        lcd.print(dmxAddress);
        EEPROM.update(eepromDMXAddress, dmxAddress);
        writeCount++;
        EEPROM.update(writeCountAddress, writeCount);
        delay(250);
    }
    if (btnPlus == LOW && btnOk == HIGH && btnMeno == HIGH) { //set dmx address if button pressed
      if(dmxAddress<1) dmxAddress=256;
        dmxAddress--;
        lcd.setCursor(13,1);
        lcd.print("    ");
        lcd.setCursor(13,1);
        lcd.print(dmxAddress);
        EEPROM.update(eepromDMXAddress, dmxAddress);
        writeCount++;
        EEPROM.update(writeCountAddress, writeCount);
        delay(250);
    }
}


// End.
