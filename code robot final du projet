#include <msp430.h>
#include "ADC.h"

/* ================== CONSTANTES ================== */

#define VITESSE_AVANCE 700
#define VITESSE_TOURNE 500

#define CAPTEUR_GAUCHE BIT1   // P1.1
#define CAPTEUR_DROITE BIT2   // P1.2

#define CAPTEUR_LIGHT         BIT4   // P1.4
#define CAPTEUR_LIGHT_ANALOG  4
#define SEUIL_LUMIERE         1023

#define INFRA_RED BIT0        // P1.0
#define SEUIL_OBSTACLE 200   // mm
#define VREF 5.0
#define ADC_MAX 1023.0
/* ================== VARIABLES INTERRUPTION ================== */

volatile unsigned int seconde = 0;
volatile unsigned char timer_actif = 0;
volatile unsigned char arret_force = 0;


/* ================== ETATS ROBOT ================== */

typedef enum {
    MODE_SUIVI_LIGNE,
    MODE_CENTRE_CERCLE,
    MODE_RECHERCHE_LUMIERE,
   // MODE_AVANCE_LUMIERE
    MODE_STOP_FINAL
} etat_robot_t;

/* ================== FONCTIONS MOTEURS ================== */

void moteur_A_avant(void)   { P2OUT |= BIT1; P2OUT &= ~BIT0; }
void moteur_A_arriere(void) { P2OUT |= BIT0; P2OUT &= ~BIT1; }
void moteur_B_avant(void)   { P2OUT |= BIT5; P2OUT &= ~BIT3; }
void moteur_B_arriere(void) { P2OUT |= BIT3; P2OUT &= ~BIT5; }

void avancer(void)
{
    moteur_A_arriere();
    moteur_B_avant();
    TA1CCR1 = VITESSE_AVANCE;
    TA1CCR2 = VITESSE_AVANCE;
}

void tourner_gauche(void)
{
    moteur_A_avant();
    moteur_B_avant();
    TA1CCR1 = VITESSE_TOURNE;
    TA1CCR2 = VITESSE_TOURNE;
}

void tourner_droite(void)
{
    moteur_A_arriere();
    moteur_B_arriere();
    TA1CCR1 = VITESSE_TOURNE;
    TA1CCR2 = VITESSE_TOURNE;
}

void stop_moteurs(void)
{
    TA1CCR1 = 0;
    TA1CCR2 = 0;
    P2OUT &= ~(BIT0 | BIT1 | BIT3 | BIT5);
}

void avancer_un_peu(void)
{
    avancer();
    __delay_cycles(300000);
    stop_moteurs();
}

void tourner_en_rond(void)
{
    moteur_A_avant();
    moteur_B_avant();
    TA1CCR1 = 500;
    TA1CCR2 = 500;
}

/* ================== CAPTEUR LUMIERE ================== */

unsigned int return_light_sensor(void)
{
    ADC_Demarrer_conversion(CAPTEUR_LIGHT_ANALOG);
    return ADC_Lire_resultat();
}
/* ================== CAPTEUR OBSTACLE ================== */
unsigned int return_distance_mm(void)
{
    unsigned int adc_value;
    float voltage;
    float distance_cm;

    P1DIR &= ~INFRA_RED;   // entrée capteur IR

    ADC_Demarrer_conversion(0);
    adc_value = ADC_Lire_resultat();

    voltage = (adc_value * VREF) / ADC_MAX;

    if (voltage < 0.45)
        voltage = 0.45;

    distance_cm = 27.86 / (voltage - 0.42);

    if (distance_cm < 4)  distance_cm = 4;
    if (distance_cm > 30) distance_cm = 30;

    return (unsigned int)(distance_cm * 10); // mm
}


/* ================== MAIN ================== */

int main(void)
     {
    WDTCTL = WDTPW | WDTHOLD;

    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;

    /* MOTEURS */
    P2DIR |= (BIT0 | BIT1 | BIT3 | BIT5);

    /* PWM */
    P2DIR |= (BIT2 | BIT4);
    P2SEL |= (BIT2 | BIT4);
    P2SEL2 &= ~(BIT2 | BIT4);

    TA1CTL = TASSEL_2 | MC_1;
    TA1CCR0 = 2250;
    TA1CCTL1 = OUTMOD_7;
    TA1CCTL2 = OUTMOD_7;

    /* CAPTEURS LIGNE */
    P1DIR &= ~(CAPTEUR_GAUCHE | CAPTEUR_DROITE);
    P1REN |=  (CAPTEUR_GAUCHE | CAPTEUR_DROITE);
    P1OUT |=  (CAPTEUR_GAUCHE | CAPTEUR_DROITE);


///////////////////////////////////////////////////////////////////////////////////////
    /* BOUTON S2  pour l'interruption*/
        P1DIR &= ~BIT3;
        P1REN |= BIT3;
        P1OUT |= BIT3;

        P1IE  |= BIT3;
        P1IES |= BIT3;
        P1IFG &= ~BIT3;

        /* TIMER 1s */
        TA0CCR0 = 125000 - 1;
        TA0CTL  = TASSEL_2 | ID_3 | MC_0;
        TA0CCTL0 = CCIE;


//////////////////////////////////////////////////////////////////////////////////////////
    /* CAPTEUR LUMIERE */
    P1DIR &= ~CAPTEUR_LIGHT;

    ADC_init();

    __enable_interrupt();

    int flag=0;
    etat_robot_t etat_robot = MODE_SUIVI_LIGNE;

    while (1)
    {

        /* ===== Condition pour arreter le robot au bout de 60s ===== */
        if (arret_force)
                 {
                     stop_moteurs();
                     continue;
                  }

        unsigned int distance = return_distance_mm();
        unsigned char gauche = !(P1IN & CAPTEUR_GAUCHE);
        unsigned char droite = !(P1IN & CAPTEUR_DROITE);



        /* ===== PRIORITÉ OBSTACLE ===== */
        if (distance < SEUIL_OBSTACLE)
        {
            stop_moteurs();
            continue;   // on bloque la machine d’état tant que l’obstacle est là
        }

        switch (etat_robot)
        {
            case MODE_SUIVI_LIGNE:
                if (!gauche && !droite) avancer();
                else if (gauche && !droite) tourner_gauche();
                else if (!gauche && droite) tourner_droite();
                else etat_robot = MODE_CENTRE_CERCLE;
                break;

            case MODE_CENTRE_CERCLE:
                if(flag==1){
                   avancer_un_peu();
                   etat_robot = MODE_STOP_FINAL;

                                }
                else if(flag==0){
                    flag=1;
                    avancer_un_peu();
                    etat_robot = MODE_RECHERCHE_LUMIERE;
                }


                break;

            case MODE_RECHERCHE_LUMIERE:
                tourner_en_rond();
                if (return_light_sensor() >= SEUIL_LUMIERE)
                {
                    stop_moteurs();
                    __delay_cycles(2000);
                    avancer_un_peu();
                                   __delay_cycles(1000);
                                   stop_moteurs();


                                   etat_robot = MODE_SUIVI_LIGNE;
                   // etat_robot = MODE_AVANCE_LUMIERE;
                }
                break;

           // case MODE_AVANCE_LUMIERE:

                //break;
            case MODE_STOP_FINAL:
            stop_moteurs();
            while (1);   // arrêt définitif

        }
    }

}


/* ================== INTERRUPTION BOUTON ================== */

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    if (P1IFG & BIT3)
    {
        if (!timer_actif)
        {
            seconde = 0;
            timer_actif = 1;
            arret_force = 0;


            TA0CTL |= MC_1;
        }

        P1IFG &= ~BIT3;
    }
}

/* ================== INTERRUPTION TIMER ================== */

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    if (timer_actif)
    {
        seconde++;

        if (seconde >= 120)
        {
            stop_moteurs();
            arret_force = 1;
            timer_actif = 0;
            TA0CTL &= ~MC_1;

        }
    }
}
