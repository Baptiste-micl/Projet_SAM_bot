#include <msp430.h> 
#include "ADC.h"
#include "Afficheur.h"

#define CAPTEUR_LIGHT BIT0
#define CAPTEUR_LIGHT_ANALOG 0

/**
 * main.c
 */

int return_light_sensor(){
    P1DIR &= ~(CAPTEUR_LIGHT);  // definition des entrées

    unsigned int adc_value;

 //   ADC_init();
//    Aff_Init();

    ADC_Demarrer_conversion(CAPTEUR_LIGHT_ANALOG);
    adc_value = ADC_Lire_resultat();

    return adc_value;

}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	

	unsigned int i;

    ADC_init();
    Aff_Init();
    ADC_Demarrer_conversion(CAPTEUR_LIGHT_ANALOG);


    while(1)
    {

        Aff_valeur(convert_Hex_Dec(return_light_sensor()));
        for( i=1000;i>0;i--);
    }
}
