/********* CONFIGURO IL DISPLAY ************/
#define DmxSimpleActive

#include <Wire.h>

#ifdef DmxSimpleActive
  #include <DmxSimple.h>
#endif

//#include <DMXSerial.h>
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

#define numCanali 6  // implementare passaggio valore dall'app Android

String arrayValoriRicevuti[numCanali+1]; // l'ultimo è il valore con la somma
int arrayValoriDaStampare[numCanali+1];
int arrayValoriStampati[numCanali+1];

int numMinByteReceivable =  1+(numCanali)+(numCanali-1)+1+1;
int numMaxByteReceivable = 1+(numCanali*3)+(numCanali-1)+6+1; //calcolo lunghezza della somma per la checksum: 512*255= 130560, sono 6 cifre
bool pacchettoValido;
bool sumcheck;
int somma;
int sommaStampata;

void setup(){
    //Serial.begin(115200);
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

//DA FARE:
//SETTORIALIZZARE IN FUNZIONI TUTTE LE AZIONI, COSI' DA POTERLE CHIAMARE QUANDO SERVE.
//CONTROLLO: SE SI TROVA UN CARATTERE CHE NON è UN NUMERO O UN CARATTERE SPECIALE UTILE ALLORA ELIMINARE TUTTO IL PACCHETTO.

//TRE CONTROLLI DI CORRETTEZZA DATI:
// 1) CHECKSUM
// 2) CONTROLLO RICEZIONE CARATTERI NON CONSENTITI
// 3) INVIO DA ANDROID COPPIA NOME-VALORE

////////////////////////FUNZIONI//////////////////////////////////////////////////////////////////FUNZIONI////////////////////////////////////////////////////

bool riceviValori(){  //Leggi i valori ricevuti via bluetooth dalla porta seriale e memorizzali nell'array temporaneo

  bool pacchettoValido = true;
  
  if (BTSerial.available() > numMinByteReceivable /*&& BTSerial.find('&')*/) {
    while(command != '!'){ // attendo l'inizio della trasmissione eliminando i dati che non servono
      command = ((byte)BTSerial.read());
    }
    
    for(i=0;i<=numCanali;i++){    //inizializza l'array arrayValoriRicevuti e il contatore i
         arrayValoriRicevuti[i]="";
      }
    i= 0;
    
    while(BTSerial.available() > 0 /*&& DMXSerial.bufferLibero()*/){ //se ci sono più di zero byte da leggere nella variabile Serial in ingresso
      command = ((byte)BTSerial.read()); //ogni volta che viene letto un byte questo viene tolto dalla variabile, fino a che non vengono letti tutti e la variabile resta vuota

      if (((command >= 48)&&(command <= 57))|| command == '.' || command == '&'){
        if(command == '.'){ //il punto è il separatore di valori
          i++;
        }
        else if(i>numCanali){
            while(BTSerial.available() > 0)
              BTSerial.read();
            //pacchettoValido = false;
            break;
          }
        else if ((command >= '0') && (command <= '9')) { //se command risulta essere un carattere ASCII da 0 a 9 accoda alla string il carattere ashii contenuto in command
          arrayValoriRicevuti[i] += command;
        }
        else {
          BTSerial.read(); //elimina il carattere illeggibile
          //pacchettoValido = false;
          //break;
          //i++;
        }
      } else{
        pacchettoValido = false;  
        break;
      }
    }
  }else delay(2);
  
 /* if(i!=(numCanali+1))
       pacchettoValido=false;*/
  return pacchettoValido;
}
 
int salvaValori(){ //Trasformazione dei caratteri ricevuti in interi e "Salvataggio" dati dall'array temporaneo a quello da scrivere su display e su seriale
  bool sumcheck;
  int temp;
  for(i=0;i<=numCanali;i++){
    temp = arrayValoriRicevuti[i].toInt();
    if(/*(temp>=0)&&(temp<=255)&&*/(arrayValoriStampati[i] !=temp))
      arrayValoriDaStampare[i] = temp;
  }
  int somma =0;
  for(i=0;i<numCanali;i++){
    somma +=arrayValoriDaStampare[i];
  }
  return somma;
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
//noInterrupts();
    pacchettoValido = riceviValori();
//interrupts();
  delay(1);
  
  if(pacchettoValido)
    somma =  salvaValori();
  
  delay(1);
//   if(somma == arrayValoriDaStampare[numCanali])
      inviaValori(somma);
     
  delay(1);
}

