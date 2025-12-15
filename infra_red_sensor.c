#include <msp430.h>
#include "Afficheur.h"
#include  "ADC.h"



int return_distance(){
    P1DIR &=~ BIT1; //(infra)(configurer en entrée)
    P1DIR|=BIT0; // configurer en sortie

    Aff_Init();   //initialisation des methodes
    ADC_init();  //initialisation des methodes

    unsigned int infra;             //variable pour la temperature
    volatile unsigned int i;        // volatile to prevent optimization

    ADC_Demarrer_conversion(1);
    infra=ADC_Lire_resultat();
    Aff_valeur(convert_Hex_Dec(infra));

    return convert_Hex_Dec(infra);
}


void main_test_infra_red(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer

    //definition des entrees et sorties

    P1DIR &=~ BIT1; //(infra)(configurer en entrée)
    P1DIR|=BIT0; // configurer en sortie

    Aff_Init();   //initialisation des methodes
    ADC_init();  //initialisation des methodes

    unsigned int infra;             //variable pour la temperature
    volatile unsigned int i;        // volatile to prevent optimization

    while(1)
    {
        ADC_Demarrer_conversion(1);
          infra=ADC_Lire_resultat();
          Aff_valeur(convert_Hex_Dec(infra));
        P1OUT ^= 0x01;              // eteindre P1.0
        for(i=10000; i>0; i--);     // delay
    }
}
