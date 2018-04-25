/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/
#ifndef bullet_h
#define bullet_h
#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"

bulletNode* newBulletNode;
bulletNode* bulletPtr;
bulletNode* bulletTemp;

//bullet create
bulletStr* createBulletP(playerStr*);
bulletStr* createBulletE(EnemyStr*, playerStr*);

//bullet move
void moveBullet(bulletStr*);

//move all bullets
void moveAllBullets(void);

//check if a bullet is outside the screen
bool outside_screen(bulletStr*);

//List functions
bulletNode* createBulletNode(bulletStr*);
void appendBulletNode(bulletStr*);
void removeBulletNode(bulletStr*);
void updateBulletPositions(void);

//
// Rendering
//
void renderAllBullets(void);

#endif
