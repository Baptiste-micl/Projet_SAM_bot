#ifndef CONFIG_H
#define CONFIG_H

/**
 * Initialise l'horloge à 1 MHz
 */
void init_clock(void);

/**
 * Initialise l'ADC pour le capteur d'obstacle (P1.2)
 */
void init_adc(void);

/**
 * Initialise le PWM pour les moteurs (P2.2 et P2.4)
 */
void init_pwm(void);

/**
 * Configure les GPIO pour les moteurs
 */
void init_gpio_moteurs(void);

/**
 * Configure les GPIO pour les capteurs de ligne
 */
void init_gpio_capteurs(void);

#endif
