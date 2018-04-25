/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/
#include <stdlib.h>
#include "SDL_initialization_functions.h"
#include "enemy.h"

Enemy_le* newEnemyNode = NULL;
Enemy_le* ptr = NULL;
Enemy_le* temp = NULL;

Enemy_le* firstEnemyNode = NULL;
Enemy_le* lastEnemyNode = NULL;

//create one enemy
EnemyStr* createEnemy()
{
  int random_x = rand()%SCREEN_WIDTH;
  int random_y = rand()%SCREEN_HEIGHT;

  EnemyStr* new_enemy = (EnemyStr*)malloc(sizeof(EnemyStr));
  if( random_x > SCREEN_WIDTH/2)
    new_enemy->x_pos = random_x + SCREEN_WIDTH/2;
  else
    new_enemy->x_pos = random_x - (SCREEN_WIDTH/2 + 5);
  if( random_y > SCREEN_HEIGHT/2 )
    new_enemy->y_pos = random_y + SCREEN_HEIGHT/2;
  else
    new_enemy->y_pos = random_y - (SCREEN_HEIGHT/2 + 5);
  new_enemy->x_vel = 0;
  new_enemy->y_vel = 0;
  new_enemy->center_x = new_enemy->x_pos + 43/2;
  new_enemy->center_y = new_enemy->y_pos + 70/2;
  new_enemy->hP = 3;
  //printf("Enemy was created.\n");
  return new_enemy;
}

//check collision with other objects
bool checkCollision(SDL_Rect object1, SDL_Rect object2)
{
  int object1_xA = object1.x;
  int object1_xB = object1.x + object1.w;
  int object1_yA = object1.y;
  int object1_yB = object1.y + object1.h;

  int object2_xA = object2.x;
  int object2_xB = object2.x + object2.w;
  int object2_yA = object2.y;
  int object2_yB = object2.y + object2.h;

  if (object1_yA + 3 > object2_yB) return false;
  if (object2_yA + 3 > object1_yB) return false;
  if (object1_xA + 3 > object2_xB) return false;
  if (object2_xA + 3 > object1_xB) return false;
  else
    return true;
}

//utworzenie listy wrogow
Enemy_le* createEnemyNode(EnemyStr* new_enemy)
{
  newEnemyNode = (Enemy_le*)malloc(sizeof(Enemy_le));
  if(newEnemyNode == NULL)
  {
    printf("\nMemory was not allocated!");
    return 0;
  }
  else
  {
    newEnemyNode->this_enemy = new_enemy;
    newEnemyNode->next_enemy = NULL;
    return newEnemyNode;
  }
}

//dodawanie na koniec listy
void appendEnemyNode(EnemyStr* new_enemy)
{
  newEnemyNode = createEnemyNode(new_enemy);
  if(firstEnemyNode == lastEnemyNode && lastEnemyNode == NULL)
  {
    firstEnemyNode = newEnemyNode;
    lastEnemyNode = firstEnemyNode;
    firstEnemyNode->next_enemy = NULL;
    lastEnemyNode->next_enemy = NULL;
  }
  else
  {
    lastEnemyNode->next_enemy = newEnemyNode;
    lastEnemyNode = newEnemyNode;
    lastEnemyNode->next_enemy = NULL;
  }
  //printf("Enemy appended to enemy list.\n");
}

//update pozycji wroga
void updateEnemyPos(EnemyStr* enemy, playerStr* player)
{
  //straight line movement
  float dstX = player->x_pos - enemy->x_pos;
  float dstY = player->y_pos - enemy->y_pos;
  float dst = sqrt(dstX*dstX + dstY*dstY);
  enemy->x_vel = (dstX/dst)*SPEED;
  enemy->y_vel = (dstY/dst)*SPEED;

  //calculate angle
  SDL_Rect enemyRect;
  SDL_QueryTexture(enemy_texture, NULL, NULL, &enemyRect.w, &enemyRect.h);
  enemy->center_x = enemy->x_pos + enemyRect.w/2;
  enemy->center_y = enemy->y_pos + enemyRect.h/2;
  enemy->rotation_angle = (int) (atan2( (player->y_pos - enemy->center_y), (player->x_pos - enemy->center_x) ) * (180/PI));

  //update enemy position
  enemy->x_pos += enemy->x_vel/120;
  enemy->y_pos += enemy->y_vel/120;

  SDL_Rect playerCollider, enemyCollider;
  SDL_QueryTexture(player_texture, NULL, NULL, &playerCollider.w, &playerCollider.h);
  SDL_QueryTexture(enemy_texture, NULL, NULL, &enemyCollider.w, &enemyCollider.h);
  playerCollider.x = (int) player->x_pos;
  playerCollider.y = (int) player->y_pos;
  enemyCollider.x = (int) enemy->x_pos;
  enemyCollider.y = (int) enemy->y_pos;
  if( checkCollision(playerCollider, enemyCollider) )
  {
    enemy->x_pos -= enemy->x_vel/380;
    enemy->y_pos -= enemy->y_vel/380;
  }

}

//Update enemies list - positions
void updateEnemiesPositions(playerStr* player)
{
  if(firstEnemyNode == lastEnemyNode && lastEnemyNode == NULL)
  {
    //printf("Enemies list empty!\n");
    return;
  }
  else if(firstEnemyNode != NULL)
  {
    for(ptr=firstEnemyNode; ptr!=NULL; ptr=ptr->next_enemy)
    {
      updateEnemyPos(ptr->this_enemy, player);
    }
    return;
  }
}

//Handle collisions between enemies themselves
void handleCollisionBetweenEnemies()
{
  SDL_Rect enemyCollider1, enemyCollider2;
  SDL_QueryTexture(enemy_texture, NULL, NULL, &enemyCollider1.w, &enemyCollider1.h);
  enemyCollider2 = enemyCollider1;

  for(ptr=firstEnemyNode; ptr!=NULL; ptr=ptr->next_enemy)
  {
    EnemyStr* enemy1 = ptr->this_enemy;
    enemyCollider1.x = (int) enemy1->x_pos;
    enemyCollider1.y = (int) enemy1->y_pos;

    for(Enemy_le* ptrI=firstEnemyNode; ptrI!=NULL; ptrI=ptrI->next_enemy)
    {
      EnemyStr* enemy2 = ptrI->this_enemy;
      enemyCollider2.x = (int) enemy2->x_pos;
      enemyCollider2.y = (int) enemy2->y_pos;
      if( checkCollision(enemyCollider1, enemyCollider2) )
      {
        int distance = 2;

        if( enemy1->x_pos < enemy2->x_pos )
        {
          enemy1->x_pos -= distance;
          enemy2->x_pos += distance;
        } else {
          enemy1->x_pos += distance;
          enemy2->x_pos -= distance;
        }

        if( enemy1->y_pos < enemy2->y_pos+1 )
        {
          enemy1->y_pos -= distance;
          enemy2->y_pos += distance;
        } else {
          enemy1->y_pos += distance;
          enemy2->y_pos -= distance;
        }
      }
    }
  }
}

//makes route for player when going through enemies
void makeRoute(playerStr* player)
{
  SDL_Rect playerCollider, currentEnemyCollider;
  SDL_QueryTexture(player_texture, NULL, NULL, &playerCollider.w, &playerCollider.h);
  SDL_QueryTexture(enemy_texture, NULL, NULL, &currentEnemyCollider.w, &currentEnemyCollider.h);
  playerCollider.x = (int) player->x_pos;
  playerCollider.y = (int) player->y_pos;

  for(ptr=firstEnemyNode; ptr!=NULL; ptr=ptr->next_enemy)
  {
    currentEnemyCollider.x = (int) (ptr->this_enemy)->x_pos;
    currentEnemyCollider.y = (int) (ptr->this_enemy)->y_pos;
    if(checkCollision(playerCollider, currentEnemyCollider))
    {
      int distance = 3;
      if( player->x_pos < (ptr->this_enemy)->x_pos )
        (ptr->this_enemy)->x_pos += distance;
      else
        (ptr->this_enemy)->x_pos -= distance;
      if( player->y_pos < (ptr->this_enemy)->y_pos )
        (ptr->this_enemy)->y_pos += distance;
      else
        (ptr->this_enemy)->y_pos -= distance;
    }
  }
}

//deletes enemies with 0 hp of the list
void removeDeadEnemies(void)
{
  if(firstEnemyNode == NULL)
  {
    //printf("Enemies list empty so cannot removeDeadEnemies()!\n");
    return;
  }
  else
  {
    if(firstEnemyNode->this_enemy->hP == 0)
    {
      if(firstEnemyNode->next_enemy != NULL)
      {
        temp = firstEnemyNode;
        firstEnemyNode = firstEnemyNode->next_enemy;
        temp->next_enemy = NULL;
        free(temp);
      }
      else
      {
        firstEnemyNode = NULL;
        lastEnemyNode = NULL;
      }
    }
    else
    {
      for(ptr=firstEnemyNode; ptr->next_enemy!=NULL; ptr=ptr->next_enemy)
      {
        if(ptr->next_enemy->this_enemy->hP == 0)
        {
          if(ptr->next_enemy != lastEnemyNode)
          {
            temp = ptr->next_enemy;
            ptr->next_enemy = ptr->next_enemy->next_enemy;
            temp->next_enemy = NULL;
            free(temp);
          }
          else
          {
            lastEnemyNode = ptr;
            temp = ptr->next_enemy;
            ptr->next_enemy = NULL;
            free(temp);
          }
        }
      }
    }

  }
}






//
//  Rendering
//
void renderAllEnemies()
{
  if(firstEnemyNode == lastEnemyNode && lastEnemyNode == NULL)
  {
    //printf("Lista jest pusta! Nie można renderować!\n");
    return;
  }
  else
  {
    SDL_Rect enemy_dest;
    SDL_QueryTexture(enemy_texture, NULL, NULL, &enemy_dest.w, &enemy_dest.h);

    //hp rectangles
    SDL_Rect enemy_hp_src, enemy_hp_dest;
    enemy_hp_src.x = 0; enemy_hp_src.y = 0;
    SDL_QueryTexture( enemy_hp_texture, NULL, NULL, &enemy_hp_src.w, &enemy_hp_src.h );
    enemy_hp_dest = enemy_hp_src;

    for(ptr=firstEnemyNode; ptr!=NULL; ptr=ptr->next_enemy)
    {
      enemy_dest.x = (int) ptr->this_enemy->x_pos;
      enemy_dest.y = (int) ptr->this_enemy->y_pos;
      SDL_RenderCopyEx( gRenderer, enemy_texture, NULL, &enemy_dest, ptr->this_enemy->rotation_angle, NULL, SDL_FLIP_NONE );

      //render hp
      enemy_hp_src.w = enemy_hp_dest.w = ptr->this_enemy->hP * 9;
      enemy_hp_dest.x = enemy_dest.x;
      enemy_hp_dest.y = enemy_dest.y - 10;
      SDL_RenderCopy( gRenderer, enemy_hp_texture, &enemy_hp_src, &enemy_hp_dest );
    }
    return;
  }
}
