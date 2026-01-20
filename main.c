#include <msp430.h>
#include "ADC.h"
#include "controls.h"
#include "sensors.h"
#include "config.h"

/* ================== ETATS ROBOT ================== */

typedef enum {
    MODE_SUIVI_LIGNE,
    MODE_CENTRE_CERCLE,
    MODE_RECHERCHE_LUMIERE,
    MODE_AVANCE_LUMIERE
} etat_robot_t;

/* ================== MAIN ================== */

int main(void)
 {
    init_system();

    /* PWM */
    init_pwm();
    

    /* CAPTEURS LIGNE */
    init_gpio_capteurs();
    init_system();

    __enable_interrupt();

    etat_robot_t etat_robot = MODE_SUIVI_LIGNE;

    while (1)
    {
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
                avancer_un_peu();
                etat_robot = MODE_RECHERCHE_LUMIERE;
                break;

            case MODE_RECHERCHE_LUMIERE:
                tourner_en_rond();
                if (return_light_sensor() >= SEUIL_LUMIERE)
                {
                    stop_moteurs();
                    __delay_cycles(200000);
                    etat_robot = MODE_AVANCE_LUMIERE;
                }
                break;

            case MODE_AVANCE_LUMIERE:
                avancer();
                __delay_cycles(1000000);
                stop_moteurs();
                break;
        }
    }

}

