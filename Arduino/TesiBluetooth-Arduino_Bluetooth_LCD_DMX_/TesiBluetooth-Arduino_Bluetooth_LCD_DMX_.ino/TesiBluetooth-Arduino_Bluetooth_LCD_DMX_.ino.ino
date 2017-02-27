/********* CONFIGURO IL DISPLAY ************/
#include <Wire.h>
#include <DMXSerial.h>
//#include <DmxSimple.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h> //per comunicazione bluetooth

SoftwareSerial BTSerial(9,10); // RX | TX
//#define   CONTRAST_PIN   9
#define   BACKLIGHT_PIN  6
#define   CONTRAST       110
LiquidCrystal lcd(12, 11, 5, 4, 3, 2, BACKLIGHT_PIN, POSITIVE );
/**********CONFIGURO IL MAX485 **************/
#define DE_RE_PIN 7 //pin da collegare a RE e DE del MAX485

char command;
String string;
int i=0, j=0, temp;
String arrayValoriRicevuti[6];
int arrayValoriDaStampare[6];
int arrayValoriStampati[6];

int numCanali = 6;// implementare passaggio valore dall'app Android
int numMinByteReceivable = 5;
int numMaxByteReceivable = 9;


void setup(){
    //Serial.begin(115200);
    //pinMode(led1, OUTPUT);
    BTSerial.begin(115200);
  /********************INIZIALIZZO LCD*****************************/
    // Switch on the backlight and LCD contrast levels
    //pinMode(CONTRAST_PIN, OUTPUT);
    //analogWrite ( CONTRAST_PIN, CONTRAST );
    
    lcd.backlight();
    lcd.begin(16,2);               // initialize the lcd 
    lcd.home ();                   // go home
    lcd.print("Attendo valori:");  
    lcd.setCursor ( 0, 1 );        // go to the next line
    lcd.print ("               ");   
  /*******************INIZIALIZZO MAX485 **************************/
//    DmxSimple.usePin(13);
//    DmxSimple.maxChannel(numCanali);
    
    //Modalità di invio DMX ->  DE e RE a true
    pinMode(DE_RE_PIN, OUTPUT);
    digitalWrite(DE_RE_PIN, true); //imposto modalità di invio DMX ->  DE e RE a true

    DMXSerial.init(DMXController);

    /****************INIZIALIZZO ARRAY CHE VA A LEGGERE IL BUFFER *******/
      for(i=0;i<numCanali;i++){
         arrayValoriRicevuti[i]="0";
      }
  }


void loop(){
  if (BTSerial.available() > numMinByteReceivable) { //innanzitutto inizializza l'array arrayValoriRicevuti e il contatore i
 
    if(BTSerial.available() > numMaxByteReceivable) //elimina dati troppo lunghi
    while(BTSerial.available() > 0){
      command = ((byte)BTSerial.read());
    }
  

    while(command != '!'){
      delay(2);
      command = ((byte)BTSerial.read());
    }

    /*
    for(i=0;i<numCanali;i++){
         arrayValoriRicevuti[i]="";
      }
    i= 0;*/
    
    while(BTSerial.available() > 0/* && DMXSerial.bufferLibero()  */){ //se ci sono più di zero byte da leggere nella variabile Serial in ingresso
      
      BTSerial.read(); // elimino il "!". Ogni volta che viene letto un byte questo viene tolto dalla variabile, fino a che non vengono letti tutti e la variabile resta vuota
      
      if(command == '.'){ //il punto è il separatore di valori
        i++;
      }
      else if(i>numCanali){
          while(BTSerial.available() > 0)
          BTSerial.read();
        }
      else if ((command >= 48) && (command <= 57)) { //se command risulta essere un carattere ASCII da 0 a 9 accoda alla string il carattere ashii contenuto in command
        arrayValoriRicevuti[i] += command;
      }
      else BTSerial.read(); //elimina il carattere illeggibile
    }
  }else delay(2);
  //BTSerial.end();

  for(i=0;i<numCanali;i++){
    temp = arrayValoriRicevuti[i].toInt();
    if((temp>=0)&&(temp<=255)&&(arrayValoriStampati[i] !=temp))
      arrayValoriDaStampare[i] = temp;
  }
    int col=0;
    int row=0;
    

  //  DMXSerial.riprendi();
  interrupts();
    for(i=0;i<numCanali;i++){
      if(i<4){row=0; col=i*4;} else if(i>=4) {row=1; col=i*4-16;};
      if ((arrayValoriDaStampare[i]>=0)&&(arrayValoriDaStampare[i]<=255)){
        if(arrayValoriStampati[i] != arrayValoriDaStampare[i]){
           //   DMXSerial.riprendi();
            
            DMXSerial.write(i+1, arrayValoriDaStampare[i]);
//            DmxSimple.write(i+1, arrayValoriDaStampare[i]);
            lcd.setCursor (col, row ); 
            lcd.print ("    "); 
            lcd.setCursor ( col, row ); 
            lcd.print (arrayValoriDaStampare[i]); 
            arrayValoriStampati[i] = arrayValoriDaStampare[i]; 
            delay(2);
         //   DMXSerial.ferma();
            
          }
      }
      delay(1);
    }
    delay(6);
    noInterrupts();
  //  DMXSerial.ferma();
}
  

