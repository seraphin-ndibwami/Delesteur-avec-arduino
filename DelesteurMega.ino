//les pins arduino pour lqa sortie de capteurs
#define CAPTEUR1 A0
#define CAPTEUR2 A1
#define CAPTEUR3 A2

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

void setup() {
  Serial.begin(9600);
}

void loop(){

    Serial.println("Demarrage");
    Serial.println("");
    
    courantL1 = getCurrent(CAPTEUR1);
    Serial.print(courantL1);
    Serial.println(" A");

    courantL2 = getCurrent(CAPTEUR2);
    Serial.print(courantL2);
    Serial.println(" A");

    courantL3 = getCurrent(CAPTEUR3);
    Serial.print(courantL3);
    Serial.println(" A");

    delay(1000);

}
