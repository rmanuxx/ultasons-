# ultasons-
detecteur de distance utilisans le capteur hc sr04 et deux led 

# Système Télémétrique à Ultrasons avec Double Signalisation (Arduino Uno)

Ce projet implémente un système d'alerte de distance embarqué basé sur un microcontrôleur **Arduino Uno**. Il utilise un capteur à ultrasons **HC-SR04** pour mesurer en temps réel la distance d'un obstacle et pilote un étage de signalisation visuelle composé de deux LED (rouge et verte) fonctionnant en logique de seuil.

---

##  Nomenclature des Composants (BOM)

| Composant | Référence Wokwi / Type | Rôle / Valeur |
| :--- | :--- | :--- |
| **Microcontrôleur** | `wokwi-arduino-uno` | Unité centrale de traitement (ATmega328P à 16 MHz) |
| **Télémètre** | `wokwi-hc-sr04` | Capteur à ultrasons 40 kHz (Émetteur/Récepteur) |
| **Voyant d'alerte** | `wokwi-led` (Rouge) | Signalisation de proximité critique ($d < 30\text{ cm}$) |
| **Voyant de statut** | `wokwi-led` (Verte) | Signalisation de zone dégagée |
| **Résistances** | `wokwi-resistor` ($\times 2$) | Limitation de courant direct ($220\ \Omega$) |
| **Interconnexion** | `wokwi-breadboard` | Plaque d'essai sans soudure pour le prototypage |

---

##  Cartographie des Broches (Pinout Mapping)

| Composant | Broche | Connexion Arduino Uno | Description du Signal |
| :--- | :--- | :--- | :--- |
| **HC-SR04** | `VCC` | `5V` | Alimentation principale ($+5\text{V}$) |
| **HC-SR04** | `GND` | `GND` | Référence de masse commune ($0\text{V}$) |
| **HC-SR04** | `TRIG` | `Pin 3` | Impulsion de déclenchement (Sortie numérique) |
| **HC-SR04** | `ECHO` | `Pin 2` | Réception de l'écho ultrasonore (Entrée numérique) |
| **LED Rouge** | Anode | `Pin 7` (via résistance $220\ \Omega$) | Commande du voyant d'alerte |
| **LED Verte** | Anode | `Pin 8` (via résistance $220\ \Omega$) | Commande du voyant nominal |

---

## 💻 Code Source Firmware (Arduino C++)

Le script suivant configure les entrées/sorties, cadence l'émission des salves ultrasonores, calcule le temps de vol de l'onde pour en déduire la distance géométrique, et actualise l'état des actionneurs optiques :

```cpp
// Définition des broches de contrôle
const int trigPin = 3;
const int echoPin = 2;
const int ledRougePin = 7;
const int ledVertePin = 8;

// Seuil de déclenchement de l'alerte en centimètres
const float seuilAlerteCm = 30.0;

void setup() {
  // Initialisation du moniteur série pour la télémétrie
  Serial.begin(9600);

  // Configuration des modes de broches
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledRougePin, OUTPUT);
  pinMode(ledVertePin, OUTPUT);
}

void loop() {
  long duration;
  float distance;

  // Séquence d'activation du capteur : Impulsion TTL propre de 10 µs sur TRIG
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lecture de la durée de l'impulsion ECHO en microsecondes
  duration = pulseIn(echoPin, HIGH);

  // Calcul de la distance : (Temps * Vitesse du son dans l'air ~0.0343 cm/µs) / 2 (aller-retour)
  distance = duration * 0.0343 / 2.0;

  // Journalisation des données sur le port série
  Serial.print("Distance mesuree : ");
  Serial.print(distance);
  Serial.println(" cm");

  // Logique de commutation des états
  if (distance > 0 && distance < seuilAlerteCm) {
    // Obstacle proche : Activation de l'alerte rouge
    digitalWrite(ledRougePin, HIGH);
    digitalWrite(ledVertePin, LOW);
  } else {
    // Zone sécurisée : Activation du témoin vert
    digitalWrite(ledRougePin, LOW);
    digitalWrite(ledVertePin, HIGH);
  }

  // Fréquence d'échantillonnage de la boucle principale
  delay(100);
}
