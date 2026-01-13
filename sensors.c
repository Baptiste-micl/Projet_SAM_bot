#include <msp430.h>
#include "sensors.h"

unsigned int lire_distance(void)
{
    ADC10CTL0 |= ENC + ADC10SC;         // Active et démarre la conversion
    while (ADC10CTL1 & ADC10BUSY);      // Attend la fin de la conversion
    return ADC10MEM;                    // Retourne le résultat
}

unsigned char lire_capteur_gauche(void)
{
    // Logique inversée : 0 = ligne détectée
    return !(P1IN & CAPTEUR_GAUCHE);
}

unsigned char lire_capteur_droite(void)
{
    // Logique inversée : 0 = ligne détectée
    return !(P1IN & CAPTEUR_DROITE);
}
