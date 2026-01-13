#ifndef SENSORS_H
#define SENSORS_H

// Définitions des capteurs
#define CAPTEUR_GAUCHE BIT1    // P1.1
#define CAPTEUR_DROITE BIT0    // P1.0
#define INFRA_RED BIT2
#define VREF 5.0
#define ADC_MAX 1023.0



// Seuil de détection d'obstacle
#define SEUIL_OBSTACLE 150

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
