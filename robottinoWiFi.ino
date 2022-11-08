#include <SoftwareSerial.h> //include una libreria

#define rxPin 10 
#define txPin 8
// Set up a new SoftwareSerial object
SoftwareSerial moduloWifiseriale =  SoftwareSerial(rxPin, txPin); // creazione oggetto

String check4answer(){ // verifica la presenza di una risposta del modulo wifi
    String str = moduloWifiseriale.readString();// lettura dati
    return str;
}


String esp01cmd(String cmd) { // invio comoandi al modulo wifi
  Serial.println("sending: " + cmd); // stampa sul monitor seriale l'invio del comando(DEBUG)
  moduloWifiseriale.println(cmd); // invio dati
  delay(10);
  String risposta = check4answer(); // verifico risposta modulo wifi
  return risposta; //restituisco risposta modulo wifi
}

void setup()  {
    // Define pin modes for TX and RX
    // pinMode(rxPin, INPUT);
    // pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    moduloWifiseriale.begin(115200); // configurazione velocità porta seriale modulo wifi
    Serial.begin(9600);
    delay(1000);
    esp01cmd("AT"); // verifico che il modulo wifi sia attivo
    delay(1000);
    esp01cmd("AT+CWMODE=2"); //selezione modalità access point
    delay(1000);
    esp01cmd("AT+CWSAP=\"robottino\",\"robottino\",1,4"); // impostiamo ssid e password dell' access point
    delay(1000);
    esp01cmd("AT+CIFSR"); //mostra indirizzo ip access point
    esp01cmd("AT+CIPMUX=1"); //permette di connettere massimo 1 dispositivo alla volta
    
    
    Serial.println("ESP-01 Configuration Completed");
}

void loop() {
    Serial.println("loop...");
    // AT+CWLIF restituisce l'indirizzo ip del client connesso al modulo wifi
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168") { // verifica la presenza di un client collegato
      Serial.println("no connections so far... still waiting");  
      delay(1000);
    }
    //dopo che viene stabilita una connessione
    Serial.println("Connection from remote device was Established!!!");
    //Socket ID: 3
    //accept packets from any IP address/devices
    //Listen to local port 4567
    //outgoing packets could go to any remote host without restrictions...
    esp01cmd("AT+CIPSTART=3,\"UDP\",\"0.0.0.0\",0,4567,2"); //starting UDP Socket Server (programma che si mette in ascolto in attesa di ricevere un pacchetto UDP)
    //una volta ricevuti i dati li trasmette all' arduino tramite UART
//    String str = ;    
//    Serial.println("received: "+esp01cmd("AT+CWLIF").substring(11,18));
    delay(3000);
    while(true) {
      String str = moduloWifiseriale.readString();
      if(str != "") {
        int startOfSTR = str.indexOf(":",10)+1;
        Serial.println("Received: "+str.substring(startOfSTR));
        //Serial.println("Received: "+str);
        //Serial.println(startOfSTR);
      }
    }

}
