/*  DEFINITIONS des constantes */

#define VITESSE_AVANCE 400
#define VITESSE_TOURNE 200

/*FONCTIONS MOTEURS */

void moteur_A_avant(void)
{
    P2OUT |= BIT1;
    P2OUT &= ~BIT0;
}

void moteur_A_arriere(void)
{
    P2OUT |= BIT0;
    P2OUT &= ~BIT1;
}

void moteur_B_avant(void)
{
    P2OUT |= BIT5;
    P2OUT &= ~BIT3;
}

void moteur_B_arriere(void)
{
    P2OUT |= BIT3;
    P2OUT &= ~BIT5;
}

/* direction et sens que les moteurs doivent suivrent */

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

    // coupe TOUTES les directions moteurs (en mettant 0 dans le registre pour les bits correspondant)
      P2OUT &= ~(BIT0 | BIT1 | BIT3 | BIT5);
}
