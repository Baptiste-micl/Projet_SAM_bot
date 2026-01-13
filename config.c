#include <msp430.h>
#include "config.h"
#include "sensors.h"

void init_clock(void)
{
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;
}

void init_adc(void)
{
    ADC10CTL1 = INCH_2;                    // Canal A2 (P1.2)
    ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON;  // Vcc/GND, 64 cycles, ADC ON
    ADC10AE0 |= BIT2;                      // Active A2 en entrée analogique
}

void init_pwm(void)
{
    // Configuration des broches P2.2 et P2.4 en sortie
    P2DIR |= (BIT2 | BIT4);
    P2SEL |= (BIT2 | BIT4);
    P2SEL2 &= ~(BIT2 | BIT4);

    // Configuration du Timer A1 pour PWM
    TA1CTL = TASSEL_2 | MC_1;     // SMCLK, mode UP
    TA1CCR0 = 2250;               // Période PWM
    TA1CCTL1 = OUTMOD_7;          // Reset/set pour TA1.1
    TA1CCTL2 = OUTMOD_7;          // Reset/set pour TA1.2

    // Initialisation à 0 (moteurs arrêtés)
    TA1CCR1 = 0;
    TA1CCR2 = 0;
}

void init_gpio_moteurs(void)
{
    // Configuration des broches de direction des moteurs en sortie
    P2DIR |= (BIT0 | BIT1 | BIT3 | BIT5);

    // Initialisation à 0
    P2OUT &= ~(BIT0 | BIT1 | BIT3 | BIT5);
}

void init_gpio_capteurs(void)
{
    // Configuration des capteurs en entrée
    P1DIR &= ~(CAPTEUR_GAUCHE | CAPTEUR_DROITE | CAPTEUR_OBS);

    // Activation des résistances pull-up pour capteurs de ligne
    P1REN |= (CAPTEUR_GAUCHE | CAPTEUR_DROITE);
    P1OUT |= (CAPTEUR_GAUCHE | CAPTEUR_DROITE);
}
