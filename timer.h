#ifndef TIMER_H
#define TIMER_H

/**
 * Initialise le timer d'arrêt automatique (120 secondes)
 */
void init_timer_arret(void);

/**
 * Vérifie si le timer d'arrêt a expiré
 * @return 1 si arrêt demandé, 0 sinon
 */
unsigned char est_arrete(void);

#endif
