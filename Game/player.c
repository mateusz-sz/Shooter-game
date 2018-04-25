/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "SDL_initialization_functions.h"
#include "player.h"

playerStr* createPlayer(void)
{
  playerStr* player = (playerStr*)malloc(sizeof(playerStr));
  player->x_pos = (SCREEN_WIDTH/2) - 5;
  player->y_pos = (SCREEN_HEIGHT/2) - 5;
  player->x_vel = 0;
  player->y_vel = 0;
  player->rotation_angle = 0;
  player->hP = 5;
  player->score = 0;
  return player;
}

void destroyPlayer(playerStr* player)
{
  free(player);
}

playerStr* movePlayer(playerStr* player, bool up, bool right, bool down, bool left)
{
  //determine player velocity
  player->x_vel = player->y_vel = 0;
  if( right && !left ) player->x_vel = SPEED;
  if( left && !right ) player->x_vel = -SPEED;
  if( down && !up ) player->y_vel = SPEED;
  if( up && !down ) player->y_vel = -SPEED;

  //update player positions
  player->x_pos += player->x_vel/60;
  player->y_pos += player->y_vel/60;

  //calculate rotation angle
  SDL_Rect playerCollider;
  SDL_QueryTexture(player_texture, NULL, NULL, &playerCollider.w, &playerCollider.h);
  playerCollider.x = player->x_pos;
  playerCollider.y = player->y_pos;
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  player->center_x = playerCollider.x + playerCollider.w/2;
  player->center_y = playerCollider.y + playerCollider.h/2;
  player->rotation_angle = (int) (atan2( (mouse_y - player->center_y), (mouse_x - player->center_x) ) * (180/PI));

  //bounds collision detection for player
  if( player->x_pos <= 0 )
    player->x_pos = 0;
  if( player->y_pos <= 0 )
    player->y_pos = 0;
  if( player->x_pos >= (SCREEN_WIDTH - playerCollider.w) )
    player->x_pos = SCREEN_WIDTH - playerCollider.w;
  if( player->y_pos >= (SCREEN_HEIGHT - playerCollider.h) )
    player->y_pos = SCREEN_HEIGHT - playerCollider.h;

  return player;
}


void renderPlayer(playerStr* player)
{
  SDL_Rect player_dest, player_hp_src, player_hp_dest;
  SDL_QueryTexture(player_texture, NULL, NULL, &player_dest.w, &player_dest.h);
  player_dest.x = (int) player->x_pos;
  player_dest.y = (int) player->y_pos;
  SDL_QueryTexture(player_hp_texture, NULL, NULL, &player_hp_src.w, &player_hp_src.h);
  player_hp_src.x = 0;
  player_hp_src.y = 0;
  player_hp_dest = player_hp_src;
  player_hp_dest.x = (int) player->x_pos - 10;
  player_hp_dest.y = (int) player->y_pos - 12;
  player_hp_dest.w = player_hp_src.w = player->hP * 2;

  SDL_RenderCopy(gRenderer, player_hp_texture, &player_hp_src, &player_hp_dest);
  SDL_RenderCopyEx( gRenderer, player_texture, NULL, &player_dest, player->rotation_angle, NULL, SDL_FLIP_NONE );
}

void renderPlayerScore(int score)
{
  char* textString = "Your score: ";
  char scoreString[12];
  sprintf(scoreString, "%d", score);

  char* allText;
  allText = malloc(strlen(scoreString)+strlen(textString)+1);
  if(allText == NULL)
  {
    printf("Failed to allocate memory for text \"Your score:\"\n");
    return;
  }

  strcpy(allText, textString);
  strcat(allText, scoreString);

  SDL_Surface* playerScoreSurface = TTF_RenderText_Solid( mainFont, allText, textColor );
  player_score_texture = SDL_CreateTextureFromSurface( gRenderer, playerScoreSurface );

  SDL_Rect playerScore_dest;
  SDL_QueryTexture(player_score_texture, NULL, NULL, &playerScore_dest.w, &playerScore_dest.h);
  playerScore_dest.x = SCREEN_WIDTH/2 - playerScore_dest.w/2;
  playerScore_dest.y = 20;
  SDL_RenderCopy(gRenderer, player_score_texture, NULL, &playerScore_dest);
}





//don't skip it
