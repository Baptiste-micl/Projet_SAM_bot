#include <msp430.h>
#include "line_following_sensors.h"

/* ===================== VARIABLES GLOBALES ===================== */

// Compteurs encodeurs
volatile unsigned int compteur_gauche = 0;
volatile unsigned int compteur_droit  = 0;

// États précédents encodeurs
unsigned char etat1_gauche = 1;
unsigned char etat1_droit  = 1;

// PID
volatile int erreur_integrale  = 0;
volatile int erreur_precedente = 0;

// Suivi de ligne
int erreur_ligne = 0;
int derniere_erreur_ligne = 0;

/* ===================== COMMANDES MOTEURS ===================== */

void moteur_arriere_gauche(void){
    P2OUT &= ~BIT1;
}

void moteur_avant_gauche(void){
    P2OUT |= BIT1;
}

void moteur_arriere_droit(void){
    P2OUT &= ~BIT5;
}

void moteur_avant_droit(void){
    P2OUT |= BIT5;
}

/* ===================== PROGRAMME PRINCIPAL ===================== */

int main(void)
 {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog

    /* ---------- Configuration GPIO ---------- */

    // Encodeurs
    P2DIR &= ~(BIT0 | BIT3);   // Entrées encodeurs

    // Directions moteurs + enable
    P2DIR |= (BIT1 | BIT2 | BIT4 | BIT5 | BIT6);
    P2OUT &= ~(BIT1 | BIT5 | BIT6);
    P2OUT |= BIT0;

    // PWM
    P2SEL |= BIT2 | BIT4;

    /* ---------- Timer PWM ---------- */

    TA1CCR0  = 1000 - 1;   // Période PWM
    TA1CCR1  = 150;        // Moteur gauche
    TA1CCR2  = 150;        // Moteur droit

    TA1CTL   = TASSEL_2 | MC_1;
    TA1CCTL1 = OUTMOD_7;
    TA1CCTL2 = OUTMOD_7;

    /* ---------- Sens de rotation ---------- */

    moteur_arriere_gauche();
    moteur_avant_droit();

    /* ---------- Initialisation états encodeurs ---------- */

    etat1_gauche = (P2IN & BIT0) ? 1 : 0;
    etat1_droit  = (P2IN & BIT3) ? 1 : 0;

    /* ---------- Paramètres ---------- */

    unsigned int compteur_regulation = 0;
    int vitesse_base = 150;   // 15 %

    /* ===================== BOUCLE PRINCIPALE ===================== */

    while (1)
    {
        /* ---------- LECTURE ENCODEURS ---------- */

        unsigned char etat2_gauche = (P2IN & BIT0) ? 1 : 0;
        unsigned char etat2_droit  = (P2IN & BIT3) ? 1 : 0;

        if (etat1_gauche == 1 && etat2_gauche == 0)
            compteur_gauche++;

        if (etat1_droit == 1 && etat2_droit == 0)
            compteur_droit++;

        etat1_gauche = etat2_gauche;
        etat1_droit  = etat2_droit;

        /* ---------- LECTURE CAPTEURS DE LIGNE ---------- */
        // 0 = blanc (ligne), 1 = noir (fond)

        unsigned char capteur_gauche = return_line_follower_left();
        unsigned char capteur_droit  = return_line_follower_right();

        if (capteur_gauche == 0 && capteur_droit == 0)
            erreur_ligne = 0;
        else if (capteur_gauche == 0 && capteur_droit == 1)
            erreur_ligne = -1;
        else if (capteur_gauche == 1 && capteur_droit == 0)
            erreur_ligne = +1;
        else
            erreur_ligne = derniere_erreur_ligne;

        derniere_erreur_ligne = erreur_ligne;

        /* ---------- RÉGULATION PID ---------- */

        compteur_regulation++;
        if (compteur_regulation >= 5000)
        {
            compteur_regulation = 0;

            // Erreur globale (ligne + encodeurs)
            int erreur = (erreur_ligne * 20)
                       + (compteur_gauche - compteur_droit);

            // P
            int correction_P = erreur * 3;

            // I
            erreur_integrale += erreur;
            if (erreur_integrale > 50)  erreur_integrale = 50;
            if (erreur_integrale < -50) erreur_integrale = -50;
            int correction_I = erreur_integrale / 2;

            // D
            int erreur_derivee = erreur - erreur_precedente;
            int correction_D = erreur_derivee * 2;
            erreur_precedente = erreur;

            int correction = correction_P + correction_I + correction_D;

            /* ---------- APPLICATION AUX MOTEURS ---------- */

            TA1CCR1 = vitesse_base - correction;
            TA1CCR2 = vitesse_base + correction;

            // Saturation PWM
            if (TA1CCR1 < 100) TA1CCR1 = 100;
            if (TA1CCR1 > 250) TA1CCR1 = 250;
            if (TA1CCR2 < 100) TA1CCR2 = 100;
            if (TA1CCR2 > 250) TA1CCR2 = 250;

            compteur_gauche = 0;
            compteur_droit  = 0;
        }

        __delay_cycles(50);
    }
}
