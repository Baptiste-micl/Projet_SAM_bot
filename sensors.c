#include <msp430.h>
#include "sensors.h"
#include "ADC.h"

unsigned int lire_distance(void)
{
    unsigned int adc_value;
    float voltage;
    float distance_cm;

    ADC_Demarrer_conversion(2);
    adc_value = ADC_Lire_resultat();

    // ADC → tension
    voltage = ((float)adc_value * VREF) / ADC_MAX;

    // Sécurité capteur Sharp
    if (voltage < 0.5)
        return 300;   // trop loin → 30 cm

    distance_cm = 27.86 / (voltage - 0.42);

    // Limites physiques
    if (distance_cm < 4)  distance_cm = 4;
    if (distance_cm > 30) distance_cm = 30;

    return (unsigned int)(distance_cm * 10);
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
