#ifndef SENSORS_H
#define SENSORS_H

// Définitions des capteurs
#define CAPTEUR_GAUCHE BIT1    // P1.1
#define CAPTEUR_DROITE BIT0    // P1.0
#define CAPTEUR_OBS    BIT2    // P1.2
#define CATTEUR_RIGHT_ANALOG 0
#define CATTEUR_LEFT_ANALOG 1

#define SEUIL_MAX 1021
#define SEUIL_MIN 100


// Seuil de détection d'obstacle
#define SEUIL_OBSTACLE 800

/**
 * Lit la distance du capteur d'obstacle via l'ADC
 * @return Valeur ADC (0-1023)
 */
unsigned int lire_distance(void);

/**
 * Lit l'état du capteur gauche
 * @return 1 si ligne détectée, 0 sinon
 */
unsigned char lire_capteur_gauche(void);

/**
 * Lit l'état du capteur droit
 * @return 1 si ligne détectée, 0 sinon
 */
unsigned char lire_capteur_droite(void);

#endif
