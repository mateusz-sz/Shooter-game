/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/
#include <stdlib.h>
#include "SDL_initialization_functions.h"
#include "bullet.h"

bulletNode* newBulletNode = NULL;
bulletNode* bulletPtr = NULL;
bulletNode* bulletTemp = NULL;

bulletNode* firstBulletNode = NULL;
bulletNode* lastBulletNode = NULL;


//  Creating bullet in player's position to mouse position
//
bulletStr* createBulletP(playerStr* player)
{
  bulletStr* bullet = (bulletStr*)malloc(sizeof(bulletStr));

  bullet->x_pos = player->center_x;
  bullet->y_pos = player->center_y;

  //get direction
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  bullet->target_x = mouse_x;
  bullet->target_y = mouse_y;
  float dstX = bullet->target_x - bullet->x_pos;
  float dstY = bullet->target_y - bullet->y_pos;
  float dst = sqrt(dstX*dstX + dstY*dstY);

  //set velocity
  bullet->x_vel = (dstX/dst)*SPEED;
  bullet->y_vel = (dstY/dst)*SPEED;

  //move bullet from enemy first
  bullet->x_pos += bullet->x_vel/8;
  bullet->y_pos += bullet->y_vel/8;

  return bullet;
}

//  Creating bullet in enemy's position to player's position
//
bulletStr* createBulletE(EnemyStr* enemy, playerStr* player)
{
  bulletStr* bullet = (bulletStr*)malloc(sizeof(bulletStr));

  bullet->x_pos = enemy->center_x;
  bullet->y_pos = enemy->center_y;

  //get direction
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  bullet->target_x = player->x_pos;
  bullet->target_y = player->y_pos;
  float dstX = bullet->target_x - bullet->x_pos;
  float dstY = bullet->target_y - bullet->y_pos;
  float dst = sqrt(dstX*dstX + dstY*dstY);

  //set velocity
  bullet->x_vel = (dstX/dst)*SPEED;
  bullet->y_vel = (dstY/dst)*SPEED;

  //move bullet from enemy first
  bullet->x_pos += bullet->x_vel/10;
  bullet->y_pos += bullet->y_vel/10;

  return bullet;
}


//  Creating bullet node (list)
//
bulletNode* createBulletNode(bulletStr* new_bullet)
{
  newBulletNode = (bulletNode*)malloc(sizeof(bulletNode));
  if(newBulletNode == NULL)
  {
    printf("\nMemory was not allocated!");
    return 0;
  }
  else
  {
    newBulletNode->this_bullet = new_bullet;
    newBulletNode->next_bulletNode = NULL;
    return newBulletNode;
  }
}

//
//  Adding new_bullet to the end of list
//
void appendBulletNode(bulletStr* new_bullet)
{
  newBulletNode = createBulletNode(new_bullet);
  if(firstBulletNode == lastBulletNode && lastBulletNode == NULL)
  {
    firstBulletNode = lastBulletNode = newBulletNode;
    firstBulletNode->next_bulletNode = NULL;
    lastBulletNode->next_bulletNode = NULL;
  }
  else
  {
    lastBulletNode->next_bulletNode = newBulletNode;
    lastBulletNode = newBulletNode;
    lastBulletNode->next_bulletNode = NULL;
  }
  //printf("\nElement appended to bullet list.");
}

//  Moving single bullet
//
void moveBullet(bulletStr* bullet)
{
  bullet->x_pos += bullet->x_vel/40;
  bullet->y_pos += bullet->y_vel/40;
  return;
}

//Moving all the bullets
void moveAllBullets()
{
  if(firstBulletNode == NULL)
  {
    //printf("Lista pocisków pusta!\n");
    return;
  }
  else
  {
    for(bulletPtr=firstBulletNode; bulletPtr!=NULL; bulletPtr=bulletPtr->next_bulletNode)
    {
      moveBullet(bulletPtr->this_bullet);
    }
  }
}

//  Checking if bullet is being outside screen
//
bool outside_screen(bulletStr* bullet)
{
  if(bullet->x_pos > SCREEN_WIDTH+10 || bullet->x_pos < -10)
    return true;
  if(bullet->y_pos > SCREEN_HEIGHT+10 || bullet->y_pos < -10)
    return true;

  return false;
}



//  Remove specific bullet from list
//
void removeBulletNode(bulletStr* bullet_to_delete)
{
  if(firstBulletNode == lastBulletNode && lastBulletNode == NULL)
  {
    printf("The list of bullets is empty so you cannot delete a bullet from it!\n");
    return;
  }
  else
  {
    if(firstBulletNode->this_bullet == bullet_to_delete)
    {
      if(firstBulletNode->next_bulletNode != NULL)
      {
        bulletTemp = firstBulletNode;
        firstBulletNode = firstBulletNode->next_bulletNode;
        bulletTemp->next_bulletNode = NULL;
        free(bulletTemp);
      }
      else
      {
        firstBulletNode = NULL;
        lastBulletNode = NULL;
      }
      //printf("First bullet node was deleted.\n");
    }
    else
    {
      for(bulletPtr=firstBulletNode; bulletPtr->next_bulletNode!=NULL; bulletPtr=bulletPtr->next_bulletNode)
      {
        if(bulletPtr->next_bulletNode->this_bullet == bullet_to_delete)
        {
          if(bulletPtr->next_bulletNode != lastBulletNode)
          {
            bulletTemp = bulletPtr->next_bulletNode;
            bulletPtr->next_bulletNode = bulletPtr->next_bulletNode->next_bulletNode;
            bulletTemp->next_bulletNode = NULL;
            free(bulletTemp);
          }
          else
          {
            lastBulletNode = bulletPtr;
            bulletTemp = bulletPtr->next_bulletNode;
            bulletPtr->next_bulletNode = NULL;
            free(bulletTemp);
          }
          //printf("Bullet in the middle of the list was removed.\n");
          break;
        }
      }
    }
  }
}


//
//  Removing bullets that are colliding with player form the bullet list
//
void removeCollidingBulletsP(playerStr* player)
{
  SDL_Rect playerCollider;
  SDL_QueryTexture(player_texture, NULL, NULL, &playerCollider.w, &playerCollider.h);
  playerCollider.x = (int) player->x_pos;
  playerCollider.y = (int) player->y_pos;

  SDL_Rect bulletCollider;
  SDL_QueryTexture(bullet_texture, NULL, NULL, &bulletCollider.w, &bulletCollider.h);

  for(bulletPtr=firstBulletNode; bulletPtr!=NULL; bulletPtr=bulletPtr->next_bulletNode)
  {
    bulletCollider.x = (int) bulletPtr->this_bullet->x_pos;
    bulletCollider.y = (int) bulletPtr->this_bullet->y_pos;
    if( checkCollision(playerCollider, bulletCollider) )
    {
      if(player->hP > 0)
        player->hP--;
      removeBulletNode(bulletPtr->this_bullet);
    }
    else if(outside_screen(bulletPtr->this_bullet))
    {
      removeBulletNode(bulletPtr->this_bullet);
    }
  }
  return;
}

//
//  Removing all bullets (from bullets list) that are colliding with specific enemy
//
void removeCollidingBulletsE(EnemyStr* enemy, playerStr* player)
{
  SDL_Rect enemyCollider;
  SDL_QueryTexture(enemy_texture, NULL, NULL, &enemyCollider.w, &enemyCollider.h);
  enemyCollider.x = (int) enemy->x_pos;
  enemyCollider.y = (int) enemy->y_pos;

  SDL_Rect bulletCollider;
  SDL_QueryTexture(bullet_texture, NULL, NULL, &bulletCollider.w, &bulletCollider.h);

  for(bulletNode* ptr=firstBulletNode; ptr!=NULL; ptr=ptr->next_bulletNode)
  {
    bulletCollider.x = (int) ptr->this_bullet->x_pos;
    bulletCollider.y = (int) ptr->this_bullet->y_pos;
    if( checkCollision(enemyCollider, bulletCollider) )
    {
      if(enemy->hP > 0)
      {
        enemy->hP--;
        player->score++;
      }
      removeBulletNode(ptr->this_bullet);
    }
  }
  return;
}

//DONE IN MAIN BC HERE WE'RE NOT ALLOWED TO USE ENEMIES LIST
//  Removing bullets (from bullets list) that are colliding with any of the enemies
//






//
// Rendering
//
void renderAllBullets()
{
  SDL_Rect bullet_dest;
  SDL_QueryTexture(bullet_texture, NULL, NULL, &bullet_dest.w, &bullet_dest.h);
  bullet_dest.w /= 2;
  bullet_dest.h /= 2;
  for(bulletPtr=firstBulletNode; bulletPtr!=NULL; bulletPtr=bulletPtr->next_bulletNode)
  {
    bullet_dest.x = (int) bulletPtr->this_bullet->x_pos;
    bullet_dest.y = (int) bulletPtr->this_bullet->y_pos;
    SDL_RenderCopy(gRenderer, bullet_texture, NULL, &bullet_dest);
  }
}
