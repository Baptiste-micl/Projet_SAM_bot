#include <msp430.h>
#include "config.h"
#include "controls.h"
#include "sensors.h"
#include "timer.h"
#include "line_follower.h"

int main(void)
{
    // Initialisation du watchdog
     WDTCTL = WDTPW | WDTHOLD;

    // Initialisation de tous les modules
    init_clock();
    init_adc();
    init_pwm();
    init_gpio_moteurs();
    init_gpio_capteurs();
    init_timer_arret();

    // Activation des interruptions globales
    __enable_interrupt();

    // Boucle principale de suivi de ligne
    suivre_ligne();

    return 0;
}
