
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

  #define ARDRELAY1 8 
  #define ARDRELAY2 9 
  #define ARDRELAY3 10

  #define BTarret 24 
  #define BTreset 1    

  //les valeurs du courant mesurées
  float courantL1 = 0.0;
  float courantL2 = 0.0;
  float courantL3 = 0.0;

  const int m_calFactor1;
  const int m_calFactor2;
  const int m_calFactor3;

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

  void desactiverLigne(int numeroLigne) {
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
    Serial.println("Bouton ligne 1");
    ation(1, BP1, ligneL1Active, ligneL1blocked);
  }
  void buttonEnfoncer2(){
    Serial.println("Bouton ligne 2");
    ation(2, BP2, ligneL2Active, ligneL2blocked);
  }
  void buttonEnfoncer3(){
    Serial.println("Bouton ligne 3");
    ation(3, BP3, ligneL3Active, ligneL3blocked);
  }

  void ation(int nl, int pinbt, bool ligneactive, bool ligneblock){
    if (digitalRead(pinbt) == LOW){
      String complete = String(nl) + " ...";
      String message = "Ligne" + complete;
      Serial.println(message);

      int n = 0;
      while(digitalRead(pinbt) == LOW){
          n ++;
          Serial.print("le nombre n : ");
          Serial.println(n);
          delay(1000);
      }
      if(n > 15){
        desactiverLigne(nl);
        Serial.print("linge ");
        Serial.print(nl);
        Serial.println(" En arret");

      }else{
        Serial.println("pret pour");
        if(ligneblock){
          Serial.println("Entrez le code");
        }else if(!ligneactive){
          activerLigne(nl);
          complete = String(nl) + " Active";
          message = "Ligne" + complete;
          Serial.print(message);
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
      Serial.println("Arret Urgence");
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

  }

  void loop(){

    Serial.println("Debut");
    lireCourant();

    Serial.print(courantL1);
    Serial.println(" A");

    Serial.print(courantL2);
    Serial.println(" A");

    Serial.print(courantL3);
    Serial.println(" A");

    if (courantL1 > limite1) {
      desactiverLigne(1);
      ligneL1blocked = true;
    }
    
    if (courantL2 > limite2) {
      desactiverLigne(2);
      ligneL2blocked = true;
    }
    
    if (courantL3 > limite3) {
      desactiverLigne(3);
      ligneL3blocked = true;
    }
    
    delay(1000);

  }
