/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/
#ifndef player_h
#define player_h
#include "structures.h"

playerStr* createPlayer(void);
//destroy
void destroyPlayer(playerStr*);
//player move
playerStr* movePlayer(playerStr*, bool /*up*/, bool /*right*/, bool /*down*/, bool /*left*/);

void renderPlayer(playerStr*);

void renderPlayerScore(int);

#endif
