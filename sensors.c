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

    // ADC â†’ tension
    voltage = ((float)adc_value * VREF) / ADC_MAX;

    // SÃ©curitÃ© capteur Sharp
    if (voltage < 0.5)
        return 300;   // trop loin â†’ 30 cm

    distance_cm = 27.86 / (voltage - 0.42);

    // Limites physiques
    if (distance_cm < 4)  distance_cm = 4;
    if (distance_cm > 30) distance_cm = 30;

    return (unsigned int)(distance_cm * 10);
}


unsigned char lire_capteur_gauche(void)
{
    // Logique inversÃ©e : 0 = ligne dÃ©tectÃ©e
    return !(P1IN & CAPTEUR_GAUCHE);
}

unsigned char lire_capteur_droite(void)
{
    // Logique inversÃ©e : 0 = ligne dÃ©tectÃ©e
    return !(P1IN & CAPTEUR_DROITE);
}


// ---------- Capteur de lumière ----------

unsigned int return_light_sensor(void)
{
    // Idéalement : config de la broche en entrée une seule fois au début
    P1DIR &= ~CAPTEUR_LIGHT;      // P1.3 en entrée

    unsigned int adc_value;
    ADC_Demarrer_conversion(CAPTEUR_LIGHT_ANALOG);
    adc_value = ADC_Lire_resultat();

    return adc_value;
}
