#include <Arduino.h>

// Définition des broches selon le câblage Wokwi
const int trigPin = 3;
const int echoPin = 2;
const int ledRougePin = 7;
const int ledVertePin = 8;

void setup() {
  // Initialisation de la liaison série pour le débogage
  Serial.begin(9600);

  // Configuration des E/S numériques
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledRougePin, OUTPUT);
  pinMode(ledVertePin, OUTPUT);
}

void loop() {
  long duration;
  float distance;

  // Émission d'une impulsion propre de 10 µs sur la broche TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lecture de la durée de l'impulsion ECHO en microsecondes
  duration = pulseIn(echoPin, HIGH);

  // Calcul de la distance en centimètres
  // Vitesse du son = 0.0343 cm/µs (aller-retour, donc divisé par 2)
  distance = duration * 0.0343 / 2.0;

  // Affichage des données télémétriques
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Logique de décision et pilotage des actionneurs
  if (distance > 0 && distance < 30.0) {
    // Objet détecté à moins de 30 cm : Alerte (LED Rouge active, LED Verte éteinte)
    digitalWrite(ledRougePin, HIGH);
    digitalWrite(ledVertePin, LOW);
  } else {
    // Zone dégagée : Statut nominal (LED Verte active, LED Rouge éteinte)
    digitalWrite(ledRougePin, LOW);
    digitalWrite(ledVertePin, HIGH);
  }

  // Fréquence d'échantillonnage de la boucle
  delay(100);
}