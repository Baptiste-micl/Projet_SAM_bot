#include <msp430.h>
#include "line_follower.h"
#include "controls.h"
#include "sensors.h"
#include "timer.h"

void suivre_ligne(void)
{
    //unsigned char derniere_direction = 0;  // 0 = droite, 1 = gauche

    while (1)
    {
        // Lecture des capteurs
        unsigned int distance = lire_distance();
        unsigned char gauche = lire_capteur_gauche();
        unsigned char droite = lire_capteur_droite();

        // Vérification de l'arrêt automatique (120 secondes)
        /*if (est_arrete())
        {
            stop_moteurs();
            while (1);  // Arrêt définitif
        }*/

        // Détection d'obstacle

        if (distance <= SEUIL_OBSTACLE)
        {
            stop_moteurs();
        }
        // Ligne centrée : les deux capteurs détectent la ligne
        else
         if (gauche && droite)
        {
            avancer();
        }
        // Ligne à gauche : corriger à gauche
        else if (gauche && !droite)
        {
            tourner_gauche();
            //derniere_direction = 1;  // Mémorise qu'on tournait à gauche
        }
        // Ligne à droite : corriger à droite
        else if (!gauche && droite)
        {
            tourner_droite();
            //derniere_direction = 0;  // Mémorise qu'on tournait à droite
        }
        // Ligne perdue : continuer dans la dernière direction connue
        else
        {
            /*
            if (derniere_direction == 1)
            {
                tourner_gauche();  // Continue à gauche
            }
            else
            {
                tourner_droite();  // Continue à droite
            }
            */
            stop_moteurs();

        }

    }
}
