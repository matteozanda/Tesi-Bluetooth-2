/********* CONFIGURO IL DISPLAY ************/
#define DmxSimpleActive

#include <Wire.h>

#ifdef DmxSimpleActive
  #include <DmxSimple.h>
#endif

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
String indirizzoString, valoreString;
int indirizzo, valore;
int i=0, j=0;

#define numCanali 6  // implementare passaggio valore dall'app Android

String arrayValoriRicevuti[numCanali+1]; // l'ultimo è il valore con la somma
int arrayValoriDaStampare[numCanali+1];
int arrayValoriStampati[numCanali+1];

int numMinByteReceivable =  1+(numCanali)+(numCanali-1)+1+1;
int numMaxByteReceivable = 1+(numCanali*3)+(numCanali-1)+6+1; //calcolo lunghezza della somma per la checksum: 512*255= 130560, sono 6 cifre
bool pacchettoValido;
bool sumcheck;
int somma, sommaStampata;

void setup(){
    BTSerial.begin(115200);
  /********************INIZIALIZZO LCD*****************************/
    lcd.backlight();
    lcd.begin(16,2);               // initialize the lcd 
    lcd.home ();                   // go home
    lcd.print("Attendo valori:");  
    lcd.setCursor ( 0, 1 );        // go to the next line
    lcd.print ("               ");   
  /*******************INIZIALIZZO MAX485 **************************/
#ifdef DmxSimpleActive
    DmxSimple.usePin(13);
    DmxSimple.maxChannel(numCanali);
#endif
    
    //Modalità di invio DMX ->  DE e RE a true
    pinMode(DE_RE_PIN, OUTPUT);
    digitalWrite(DE_RE_PIN, true); //imposto modalità di invio DMX ->  DE e RE a true

  // DMXSerial.init(DMXController);

    /****************INIZIALIZZO ARRAY CHE VA A LEGGERE IL BUFFER *******/
      for(i=0;i<numCanali;i++){
         arrayValoriRicevuti[i]="0";
      }
  }


bool riceviValori(){  //Leggi i valori ricevuti via bluetooth dalla porta seriale e memorizzali nell'array temporaneo
  bool pacchettoValido = true;
  indirizzoString = "";
  valore = "";
  i = 0;
  
  command = BTSerial.read();
  if (command == '!'){
    
    command = BTSerial.read();
    while(command > '0' && command < '9' && i<3){
      indirizzoString += command;
      i++;
      command = BTSerial.read();    
    }
    
    i = 0;
    if(command== '-'){
      while(command > '0' && command < '9' && i<3){
        valoreString += command;
        i++;
        command = BTSerial.read();    
      }
    }
    
    if(command != '&'){
      pacchettoValido = false;
    }
    
    return pacchettoValido;
  }else delay(2);
}



 
int salvaValori(){ //Trasformazione dei caratteri ricevuti in interi e "Salvataggio" dati dall'array temporaneo a quello da scrivere su display e su seriale
  indirizzo = indirizzoString.toInt();
  valore = valoreString.toInt();
  if((indirizzo>=0)&&(indirizzo<=numCanali) &&(arrayValoriStampati[indirizzo] !=valore)){
    
    arrayValoriDaStampare[indirizzo] = valore;
  }
  
  

  return 0;
}


void inviaValori(int sommaPassata){  //Stampa dei dati su display e invia a DMX
  int col=0;
  int row=0;
  
  #ifdef DmxSimpleActive
    //interrupts();
   // DmxSimple.riprendi();
  #endif
  
  for(i=0;i<=numCanali;i++){
  if(i<4){row=0; col=i*4;} else if(i>=4) {row=1; col=i*4-16;};
  
    if(arrayValoriStampati[i] != arrayValoriDaStampare[i]){
      //DMXSerial.write(i+1, arrayValoriDaStampare[i]);
      #ifdef DmxSimpleActive
          DmxSimple.write(i+1, arrayValoriDaStampare[i]);
      #endif
      
        lcd.setCursor (col, row ); 
        lcd.print ("    "); 
        lcd.setCursor ( col, row ); 
        lcd.print (arrayValoriDaStampare[i]); 
        arrayValoriStampati[i] = arrayValoriDaStampare[i]; 
      }

      if(sommaStampata != sommaPassata){
        lcd.setCursor (12, 1 ); 
        lcd.print ("    "); 
        lcd.setCursor ( 12, 1 ); 
        lcd.print (sommaPassata); 
        sommaStampata=sommaPassata;
      }
    
  }

    
  #ifdef DmxSimpleActive
    //  DmxSimple.ferma();
      //noInterrupts();
  #endif
}

////////////////////////FUNZIONI////////////////////////////////////////////////////////////////FUNZIONI//////////////////////////////////////////////////////
/*
TO-DO:
creare comunicazione bilaterale bluetooth arduino-android.
  Quando Arduino invia segnale DMX questo crea degli interrupt. Tali interrupts corrompono i pacchetti ricevuti via bluetooth dall'app Android.
  Android dovrà quindi chiedere di poter inviare il pacchetto, attendere una risposta affermativa di Arduino e solo dopo averla ricevuta inviare il pacchetto.
  Il consenso verrà concesso quando finisce l'invio del pacchetto DMX e gli interrupts del DMX sono disabilitati.
  DOMANDA: gli interrupts del dmx sono costanti e temporizzati? Riesco a sincronizzare invio e ricezione di dati?

inviare un solo pacchetto per volta e prevedere una richiesta di reinvio da parte di arduino in caso di pacchetto corrotto?

pacchetti con valori singoli composti da:
  "!" + "canale DMX" + "-" + "intensità 0/255" + "&"
  minimo 5 caratteri,
  max 9 caratteri

Al momento devo riuscire a rendere solida la struttura, anche al rischio di appesantire le comunicazioni.
  Troverò in futuro modi per snellirle, ora deve funzionare al meglio nel minor tempo possibile.
*/




void loop(){
    pacchettoValido = riceviValori();
  delay(1);
  
  if(pacchettoValido)
    somma =  salvaValori();
  
  delay(1);
//   if(somma == arrayValoriDaStampare[numCanali])
      inviaValori(somma);
     
  delay(1);
}


