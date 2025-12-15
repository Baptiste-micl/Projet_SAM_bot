#include <msp430.h>
#include "ADC.h"
#include "Afficheur.h"

#define INFRA_RED BIT1
#define VREF 5.0
#define ADC_MAX 1023.0

unsigned int return_distance_mm(void)
{
    unsigned int adc_value;
    float voltage;
    float distance_cm;
    unsigned int distance_mm;

    P1DIR &= ~INFRA_RED;   // Entrée (capteur)
    ADC_init();

    ADC_Demarrer_conversion(1);
    adc_value = ADC_Lire_resultat();

    // Conversion ADC -> tension
    voltage = (adc_value * VREF) / ADC_MAX;

    // Sécurité (évite division par zéro)
    if (voltage < 0.45)
        voltage = 0.45;

    // Formule issue de la datasheet (approximation)
    distance_cm = 27.86 / (voltage - 0.42);

    // Limitation plage capteur
    if (distance_cm < 4)  distance_cm = 4;
    if (distance_cm > 30) distance_cm = 30;

    // Conversion en millimètres
    distance_mm = (unsigned int)(distance_cm * 10);

    return distance_mm;
}

void main_test_infra_rouge() {
    WDTCTL = WDTPW | WDTHOLD;

    Aff_Init();   //initialisation des methodes
    ADC_init();
        int i;
        while(1)
        {
            Aff_valeur(convert_Hex_Dec(return_distance_mm()));
            for(i=1000;i>0;i--);
        }


}
