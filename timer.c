#include <msp430.h>
#include "timer.h"

// Variables statiques (privées au module)
static volatile unsigned int secondes = 0;
static volatile unsigned char stop_flag = 0;

void init_timer_arret(void)
{
    // Configuration Timer A0 : SMCLK / 8, mode UP
    TA0CTL = TASSEL_2 | ID_3 | MC_1 | TACLR;

    // 1 MHz / 8 = 125 kHz → pour 1 seconde
    TA0CCR0 = 125000 - 1;

    // Active l'interruption
    TA0CCTL0 = CCIE;
}

unsigned char est_arrete(void)
{
    return stop_flag;
}

// Routine d'interruption du Timer A0
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR(void)
{
    secondes++;

    if (secondes >= 120)  // 120 secondes = 2 minutes
    {
        stop_flag = 1;
        TA0CCTL0 &= ~CCIE;  // Désactive le timer
    }
}
