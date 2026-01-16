#ifndef CONTROLS_H
#define CONTROLS_H

// Constantes de vitesse
#define VITESSE_AVANCE 700
#define VITESSE_TOURNE 500

/**
 * Contrôle moteur A en avant
 */
void moteur_A_avant(void);

/**
 * Contrôle moteur A en arrière
 */
void moteur_A_arriere(void);

/**
 * Contrôle moteur B en avant
 */
void moteur_B_avant(void);

/**
 * Contrôle moteur B en arrière
 */
void moteur_B_arriere(void);

/**
 * Fait avancer le robot
 */
void avancer(void);

/**
 * Fait tourner le robot à gauche
 */
void tourner_gauche(void);

/**
 * Fait tourner le robot à droite
 */
void tourner_droite(void);

/**
 * Arrête tous les moteurs
 */
void stop_moteurs(void);

/**
 * Tourner en rond vers la droite
 */

void tourner_en_rond(void);

#endif
