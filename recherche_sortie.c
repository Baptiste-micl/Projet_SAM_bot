#include <msp430.h>
#include "ADC.h"

#define CAPTEUR_LIGHT         BIT3
#define CAPTEUR_LIGHT_ANALOG  3
#define SEUIL_LUMIERE         1000

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

// ---------- Initialisation globale ----------

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

// ---------- Fonctions moteurs ----------

void stop_moteurs(void)
{
    TA1CCR1 = 0;
    TA1CCR2 = 0;
    // coupe les directions
    P2OUT &= ~(BIT1 | BIT5);
}

void avancer(void)
{
    // Direction avant (à adapter si tes moteurs sont inversés)
    P2OUT &= ~BIT1;
    P2OUT |=  BIT5;

    TA1CCR0 = 2250;   // période
    TA1CCR1 = 250;   // vitesse moteur droit
    TA1CCR2 = 250;   // vitesse moteur gauche
}

void reculer(void)
{
    // Direction arrière
    P2OUT |=  BIT1;
    P2OUT &= ~BIT5;

    TA1CCR0 = 2250;
    TA1CCR1 = 1125;
    TA1CCR2 = 1125;
}

void tourner_gauche(void)
{
    // Un moteur plus lent que l'autre
    P2OUT &= ~BIT1;
    P2OUT |=  BIT5;

    TA1CCR0 = 2250;
    TA1CCR1 = 500;    // moteur droit
    TA1CCR2 = 1500;   // moteur gauche
}

void tourner_droite(void)
{
    P2OUT &= ~BIT1;
    P2OUT |=  BIT5;

    TA1CCR0 = 2250;
    TA1CCR1 = 1500;   // moteur droit
    TA1CCR2 = 500;    // moteur gauche
}

void tourner_en_rond(void)
{
    // Ex : les deux moteurs même sens mais faible vitesse
    P2OUT |=  BIT1;
    P2OUT |=  BIT5;

    TA1CCR0 = 2250;
    TA1CCR1 = 250;
    TA1CCR2 = 250;
}

// ---------- Programme principal ----------

int main(void)
{
    init_system();

    while (1)
    {
        // 1) Robot cherche la lumière en tournant
        tourner_en_rond();

        // Boucle jusqu’à ce que la lumière dépasse le seuil
        while (return_light_sensor() < SEUIL_LUMIERE) {
            // ici on peut éventuellement mettre un petit délai
            __delay_cycles(10000);
        }

        // 2) Source détectée : on s’arrête un court instant
        stop_moteurs();
        __delay_cycles(200000);  // pause visible

        // 3) Puis on avance vers la source
        avancer();

        // Option : avancer un certain temps, puis recommencer la recherche
     __delay_cycles(1000000); // avancer un moment
        //stop_moteurs();        // si tu veux s'arrêter après
        // (ou revenir au while(1) pour recommencer le cycle)
    }
}
