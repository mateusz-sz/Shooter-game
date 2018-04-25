/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/
#ifndef structures_h
#define structures_h

#define SPEED 300
#define PI 3.14159265358979323846

//strukura player
typedef struct {
  float x_pos;
  float y_pos;
  float x_vel;
  float y_vel;
  int center_x;
  int center_y;
  int rotation_angle;
  unsigned int hP;
  int score;
} playerStr;

//struktura enemy
typedef struct
{
  //int numer;
  float x_pos;
  float y_pos;
  float x_vel;
  float y_vel;
  float center_x;
  float center_y;
  int rotation_angle;
  unsigned int hP;
} EnemyStr;

//struktura list of enemies
typedef struct ENEMY_LE Enemy_le;
extern struct ENEMY_LE
{
  EnemyStr* this_enemy;
  struct ENEMY_LE* next_enemy;
};

//struktura bullet
typedef struct {
  float x_pos;
  float y_pos;
  float x_vel;
  float y_vel;
  float target_x;
  float target_y;
} bulletStr;

//struktura list of bullets
typedef struct BULLET_NODE bulletNode;
extern struct BULLET_NODE
{
  bulletStr* this_bullet;
  struct BULLET_NODE* next_bulletNode;
};

extern bulletNode* firstBulletNode;
extern bulletNode* lastBulletNode;

extern Enemy_le* firstEnemyNode;
extern Enemy_le* lastEnemyNode;


#endif
