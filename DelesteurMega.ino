  // inclusion des utilitaires
  #include <Password.h> 
  #include <Keypad.h>
  #include <LiquidCrystal_I2C.h>

  // Definition des constantes limites du courant
  const int limite1 = 10;
  const int limite2 = 15;
  const int limite3 = 20;

  bool ligneL1Active = false;
  bool ligneL1blocked = false;
  bool ligneL2Active = false;
  bool ligneL2blocked = false;
  bool ligneL3Active = false;
  bool ligneL3blocked = false;

  //les pins arduino pour lqa sortie de capteurs
  #define CAPTEUR1 A0
  #define CAPTEUR2 A1
  #define CAPTEUR3 A2

  #define BP1 2
  #define BP2 3
  #define BP3 19

  #define ARDRELAY1 22 
  #define ARDRELAY2 23 
  #define ARDRELAY3 24

  #define BTarret 18 
  #define BTreset 1    

  //les valeurs du courant mesurées
  float courantL1 = 0.0;
  float courantL2 = 0.0;
  float courantL3 = 0.0;

  const int m_calFactor1;
  const int m_calFactor2;
  const int m_calFactor3;

  /*
  * setup du clavier hexadecimale
  */

  Password password = Password("4321");

  const byte ROWS = 4; // Four rows
  const byte COLS = 4; // Four columns

  //definition de la communication
  //SoftwareSerial MCU2(2, 3)

  #define I2C_ADDR 0x20 //decommenter ceci en cas d'utilisation du PCF8574 (pour la simulation)
//#define I2C_ADDR    0x27 // decommenter ceci en cas d'utilisation du PCF8574T (pour la réalisation)
//#define I2C_ADDR    0x3F ////decommenter ceci en cas d'utilisation du PCF8574A (pour la simulation)
  #define BACKLIGHT_PIN     3
  #define En_pin  2
  #define Rw_pin  1
  #define Rs_pin  0
  #define D4_pin  4
  #define D5_pin  5
  #define D6_pin  6
  #define D7_pin  7

  LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

  void envoyerDonneesBluetooth() {

    Serial.print("L1A");
    Serial.print(courantL1);
    Serial.print("L1M");
    Serial.print("10");
    Serial.print("L1S");
    Serial.print(ligneL1Active);
    Serial.print("_");
    Serial.print("L2A");
    Serial.print(courantL2);
    Serial.print("L2M");
    Serial.print("15");
    Serial.print("L2S");
    Serial.print(ligneL2Active);
    Serial.print("_");
    Serial.print("L3A");
    Serial.print(courantL3);
    Serial.print("L3M");
    Serial.print("20");
    Serial.print("L3S");
    Serial.print(ligneL3Active);

}

void notification(int nl){

}

  // Define the Keymap
  char keys[ROWS][COLS] = {
    {'7','8','9','A'},
    {'4','5','6','B'},
    {'1','2','3','C'},
    {'*','0','#','D'}
  };

  byte rowPins[ROWS] = { 4,5,6,7 };// Connecter le keypad ROW0, ROW1, ROW2 and ROW3 aux broches arduino
  byte colPins[COLS] = { 10,11,12,13 };// Connecter le keypad COL0, COL1 and COL2 to these Arduino pins.

  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

  void keypadEvent(KeypadEvent eKey){

    switch (keypad.getState()){
      case PRESSED:
        // lcd.clear();//EFFACER CETTE LIGNE SI ON VEUT AFFICHER TOUTES LES TOUCHES PRESSEES
        // lcd.print("*");
        switch (eKey){
          case '#': checkPassword(); 
          break;
          case '*': reinitializePassword(); 
          break;
          default: 
            password.append(eKey);
            lcd.setCursor(1, 2);
            lcd.print(password.getGues());
        }
    }
  }

  void checkPassword(){

    if (password.evaluate()){
      lcd.setCursor(1, 2);
      lcd.print("correct ");
      activerLigne(1);
      activerLigne(2);
      activerLigne(3);
      // lcd.clear();
      password.reset(); //reinitialiser le mot de passe après entrée correct
      // delay(500);
      // lcd.clear();
          
    }else{
      lcd.setCursor(1, 2);
      lcd.print("incorrect");
      password.reset(); //reinitialisation du mot de passe après mot de passe incorrecte
      delay(500);
      lcd.setCursor(1, 2);
      lcd.print("Reesayer");
    }
  }

  void reinitializePassword(){
    lcd.setCursor(1, 2);
    lcd.print("Reinitialisation ...");
    password.reset(); //reinitialisation du mot de passe 
    delay(500);
    lcd.setCursor(1, 2);
    password.reset();
    lcd.print("entre le code ");
  }



  float getCurrent(int CAPTEUR){
      float adcValue = analogRead(CAPTEUR);
      float adcVoltage = (adcValue / 1024.0) * 5000;
      float currentValue = ((adcVoltage - 2500) / 66);
      return currentValue;
  }

  void lireCourant(){
    courantL1 = getCurrent(CAPTEUR1);
    courantL2 = getCurrent(CAPTEUR2);
    courantL3 = getCurrent(CAPTEUR3);
  }

  void activerLigne(int numeroLigne) {
    if (numeroLigne == 1) {
      digitalWrite(ARDRELAY1, HIGH);
      ligneL1Active = true;
    } else if (numeroLigne == 2) {
      digitalWrite(ARDRELAY2, HIGH);
      ligneL2Active = true;
    } else if (numeroLigne == 3) {
      digitalWrite(ARDRELAY3, HIGH);
      ligneL3Active = true;
    }
  }

  void desactiverLigne(int numeroLigne){
    if (numeroLigne == 1) {
      digitalWrite(ARDRELAY1, LOW);
      ligneL1Active = false;
    } else if (numeroLigne == 2) {
      digitalWrite(ARDRELAY2, LOW);
      ligneL2Active = false;
    } else if (numeroLigne == 3) {
      digitalWrite(ARDRELAY3, LOW);
      ligneL3Active = false;
    }
  }

  void buttonEnfoncer1(){
    ation(1, BP1, ligneL1Active, ligneL1blocked);
  }
  void buttonEnfoncer2(){
    ation(2, BP2, ligneL2Active, ligneL2blocked);
  }
  void buttonEnfoncer3(){
    ation(3, BP3, ligneL3Active, ligneL3blocked);
  }

  void ation(int nl, int pinbt, bool ligneactive, bool ligneblock){
    if (digitalRead(pinbt) == LOW){

      int n = 0;
      while(digitalRead(pinbt) == LOW){
          n ++;
          delay(1000);
        if(n > 20){
          desactiverLigne(nl);
        }
      }
      
      if (n < 20){
        if(ligneblock){
          lcd.setCursor(1, 2);
          lcd.print("Entrez le code");
        }else if(!ligneactive){
          activerLigne(nl);
        }
      }
      
    
    }
  }


  //definition de interruption
  void arretUrgence(){
    if (digitalRead(BTarret) == LOW) {
      desactiverLigne(1);
      desactiverLigne(2);
      desactiverLigne(3);
      ligneL1Active = false;
      ligneL2Active = false;
      ligneL3Active = false;
    }
  }

  void setup() {

    Serial.begin(9600);
    pinMode(BP1, INPUT_PULLUP);
    pinMode(BP2, INPUT_PULLUP);
    pinMode(BP3, INPUT_PULLUP);

    pinMode(BTarret, INPUT_PULLUP);

    pinMode(ARDRELAY1, OUTPUT);
    pinMode(ARDRELAY2, OUTPUT);
    pinMode(ARDRELAY3, OUTPUT);

    digitalWrite(ARDRELAY1, LOW);
    digitalWrite(ARDRELAY2, LOW);
    digitalWrite(ARDRELAY3, LOW);

    attachInterrupt(digitalPinToInterrupt(BP3), buttonEnfoncer3, FALLING);
    attachInterrupt(digitalPinToInterrupt(BP2), buttonEnfoncer2, FALLING);
    attachInterrupt(digitalPinToInterrupt(BP1), buttonEnfoncer1, FALLING);

    attachInterrupt(digitalPinToInterrupt(BTarret), arretUrgence, FALLING);

    keypad.addEventListener(keypadEvent);

    lcd.begin(20,4);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Demarrage ...");


  }

  void loop(){

    lireCourant();

    lcd.clear();
    lcd.setCursor(0, 0);  

    lcd.print(courantL1);

    lcd.setCursor(8, 0);  
    lcd.print(courantL2);

    lcd.setCursor(16, 0);
    lcd.print(courantL3);

    if (courantL1 > limite1) {
      desactiverLigne(1);
      ligneL1blocked = true;
      notification(1);
    }
    
    if (courantL2 > limite2) {
      desactiverLigne(2);
      ligneL2blocked = true;
      notification(1);
    }
    
    if (courantL3 > limite3) {
      desactiverLigne(3);
      ligneL3blocked = true;
      notification(1);
    }

    Serial.println();
    envoyerDonneesBluetooth();
    
    keypad.getKey();
    delay(500);

  }
