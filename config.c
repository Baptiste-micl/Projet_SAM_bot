#include <msp430.h>
#include "config.h"
#include "sensors.h"
#include "ADC.h"


void init_clock(void)
{
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;
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
    P1DIR &= ~(CAPTEUR_GAUCHE | CAPTEUR_DROITE | INFRA_RED);

    // Activation des résistances pull-up pour capteurs de ligne
    P1REN |= (CAPTEUR_GAUCHE | CAPTEUR_DROITE);
    P1OUT |= (CAPTEUR_GAUCHE | CAPTEUR_DROITE);
}


 //recherche sortie
void init_system(void)
{
    // Arrêt du watchdog
    WDTCTL = WDTPW | WDTHOLD;

    // Horloge 1 MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;

    // ----- Moteurs sur TimerA1 -----
    // PWM sur P2.2 (TA1.1) et P2.4 (TA1.2)
    P2DIR |= BIT2 | BIT4;
    P2SEL |= BIT2 | BIT4;
    P2SEL2 &= ~(BIT2 | BIT4);

    // Broches de direction (BIT1, BIT5 en sortie GPIO)
    P2SEL  &= ~(BIT1 | BIT5);
    P2SEL2 &= ~(BIT1 | BIT5);
    P2DIR  |=  (BIT1 | BIT5);

    // TimerA1 : SMCLK, mode Up
    TA1CTL   = TASSEL_2 | MC_1;
    TA1CCTL1 = OUTMOD_7;
    TA1CCTL2 = OUTMOD_7;

    // Période PWM par défaut
    TA1CCR0 = 2250;

    // Arrêt des moteurs au démarrage
    TA1CCR1 = 0;
    TA1CCR2 = 0;

    // Init ADC une seule fois
    ADC_init();
}

