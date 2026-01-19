#include <msp430.h> 

#include "config.h"
#include "controls.h"
#include "sensors.h"
#include "exit_search.h"


//#include "timer.h"
//#include "line_follower.h"
#include "ADC.h"
 /* INCLURE TOUTE LES BIBLIOTH7QUE N2CESSAIRES
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  */


typedef enum {
    BOT_CENTRE,
    BOT_DROIT,
    BOT_GAUCHE,
    BOT_ARRET,
}EVENT_SUIVIE;


typedef enum{
    AVANCER,
    TOURNER_GAUCHE,
    TOURNER_DROITE,
    ARRET
}ETAT_SUIVIE;


EVENT_SUIVIE get_event_suivie(){
    EVENT_SUIVIE event_suivie;

    if(!lire_capteur_gauche() && !lire_capteur_droite())    event_suivie =BOT_CENTRE;

    if(lire_capteur_gauche() && !lire_capteur_droite())    event_suivie =BOT_GAUCHE;

    if(!lire_capteur_gauche() && lire_capteur_droite())    event_suivie =BOT_DROIT;

    if(lire_capteur_gauche() && lire_capteur_droite())    event_suivie =BOT_ARRET;


    return event_suivie;

}


void main()
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    init_pwm();
    init_gpio_moteurs();
    init_gpio_capteurs();
 //   init_timer_arret();
/*
 * TOUTES LES INITIALISATIONS NECESSAIRES
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
	ETAT_SUIVIE etat_suivie = AVANCER ;
	//EVENT_SUIVIE event_suivie;

	while(1){
	    EVENT_SUIVIE event_suivie = get_event_suivie();

	    switch(etat_suivie){
	    case  AVANCER:
	        switch(event_suivie) {
	        case BOT_CENTRE :
	             avancer();
	            etat_suivie = AVANCER;
	           break;
	        case BOT_DROIT :
                 tourner_droite();
                etat_suivie = TOURNER_DROITE;
               break;
	        case BOT_GAUCHE :
                 tourner_gauche();
                etat_suivie = TOURNER_GAUCHE;
               break;
            case BOT_ARRET :
                stop_moteurs();
                etat_suivie = ARRET;
               break;

	        }

	        break;

        case TOURNER_GAUCHE :
            switch(event_suivie) {
            case BOT_CENTRE :
                 avancer();
                etat_suivie = AVANCER;
               break;
            case BOT_GAUCHE :
                 tourner_gauche();
                etat_suivie = TOURNER_GAUCHE;
               break;
            case BOT_ARRET :
                stop_moteurs();
                etat_suivie = ARRET;
               break;

            }
	            break;
        case TOURNER_DROITE :
            switch(event_suivie) {
            case BOT_CENTRE :
                 avancer();
                etat_suivie = AVANCER;
               break;
            case BOT_DROIT :
                 tourner_droite();
                etat_suivie = TOURNER_DROITE;
               break;
            case BOT_ARRET :
                stop_moteurs();
                etat_suivie = ARRET;
               break;
            }
            break;
        case ARRET :
            switch(event_suivie) {
            case BOT_CENTRE :
                 avancer();
                etat_suivie = AVANCER;
               break;
            case BOT_DROIT :
                 tourner_droite();
                etat_suivie = TOURNER_DROITE;
               break;
            case BOT_GAUCHE :
                 tourner_gauche();
                etat_suivie = TOURNER_GAUCHE;
               break;
            case BOT_ARRET :
                stop_moteurs();
// RECHERCHE D'OBSTACLE
                init_system();
                exit_search();
                etat_suivie = ARRET;
               break;
            }
            break;
	    }

	}
	

}
