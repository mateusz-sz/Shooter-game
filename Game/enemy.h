/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/
#ifndef enemy_h
#define enemy_h
#include "structures.h"

extern Enemy_le* newEnemyNode;
extern Enemy_le* ptr;
extern Enemy_le* temp;

EnemyStr* createEnemy(void);

//
// Pozycja i alokacja
//
//update pozycji wroga
void updateEnemyPos(EnemyStr*, playerStr*);

//update pozycji wszystkich wrogow
void updateEnemiesPositions(playerStr*);

//utworzenie nowego node'a z wpisanym wrogiem
Enemy_le* createEnemyNode(EnemyStr* new_enemy);

//dodawanie na koniec listy jednego node'a
void appendEnemyNode(EnemyStr* new_enemy);

//sprawdzenie czy rect wroga koliduje z innym obiektem
bool checkCollision(SDL_Rect, SDL_Rect);

//obsluga kolizji kazdego wroga z kazdym
void handleCollisionBetweenEnemies(void);

//usuwanie z listy wroga o zerowym hp
void removeDeadEnemies(void);

//makes route for player when going through enemies
void makeRoute(playerStr*);

//
//  Strzelanie
//

//
//  Rendering
//
void renderAllEnemies(void);

#endif
