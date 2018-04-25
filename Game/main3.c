/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/
#include <stdio.h>
#include "SDL_initialization_functions.h"
#include "structures.h"
#include "screen_functions.h"

//Key presses surfaces constants
enum gKeyPressSurfaces
{
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};
//The actions that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

int main( int argc, char* args[] )
{
  //Initialize SDL
  if( !init() )
  {
    printf("Could not initialize!\n");
  }
  else
  {
    //Load media
    if( !loadMedia() )
    {
      printf("Could not load media\n");
    }
    else
    {
      int frequency = 180;
      int frequency_counter = 0;

      //Main loop flag
      bool quit = false;
      bool paused = false;
      //Event handler
      SDL_Event e;
      //Moving player flags
      bool up = false;
      bool down = false;
      bool left = false;
      bool right = false;
      bool shot = false;
      bool bullet_flag = false;

      /// DEFINICJA MENU ///
      SDL_Rect newGameBtn_dest;
      SDL_QueryTexture(new_game_button_texture, NULL, NULL, &newGameBtn_dest.w, &newGameBtn_dest.h);
      newGameBtn_dest.x = SCREEN_WIDTH/2 - newGameBtn_dest.w/2;
      newGameBtn_dest.y = SCREEN_HEIGHT/2 - newGameBtn_dest.h/2;
      SDL_Rect quitBtn_dest = newGameBtn_dest;
      quitBtn_dest.y = newGameBtn_dest.y + quitBtn_dest.h + 20;








      //flagi
      bool game_started = false;
      bool game_over = false;

      /// PLAYER ///
        playerStr* player = createPlayer();
        SDL_Rect player_dest;
        SDL_QueryTexture( player_texture, NULL, NULL, &player_dest.w, &player_dest.h );

      /// ENEMY ///
        EnemyStr* new_enemy;
        SDL_Rect enemy_dest;
        SDL_QueryTexture( enemy_texture, NULL, NULL, &enemy_dest.w, &enemy_dest.h );
        Enemy_le* ptr_main = NULL;

        int create_enemy_count = 0;
        int maxEnemiesNumber = 0;

      /// BULLETS ///
        bulletStr* new_bullet = NULL;
        int shot_counter = 0;

      /// BACKGROUND ///
        SDL_Rect bcg_src, bcg_dest;
        bcg_src.x = 640;
        bcg_src.y = 384;
        bcg_src.h = bcg_src.w = 480-384;
        bcg_dest = bcg_src;
        bcg_dest.x = bcg_dest.y = 0;

      //MAIN LOOP
      //
      //While application is running
      while(!quit)
      {
        //Handle events on queue
        while( SDL_PollEvent(&e) )
        {
          //do action depending on key pressed - obsluga przyciskow
          switch(e.type)
          {
            case SDL_QUIT:
              quit = true;
              break;
            case SDL_KEYDOWN:
              switch (e.key.keysym.scancode)
              {
                case SDL_SCANCODE_W:
                  up = true;
                  break;
                case SDL_SCANCODE_S:
                  down = true;
                  break;
                case SDL_SCANCODE_A:
                  left = true;
                  break;
                case SDL_SCANCODE_D:
                  right = true;
                  break;
                case SDL_SCANCODE_SPACE:
                  shot = true;
                  break;
                case SDL_SCANCODE_P:
                  if(!game_over)
                  {
                    if(paused)
                      paused = false;
                    else
                      paused = true;
                  }
                  break;
                default:
                  break;
              }
              break;
            case SDL_KEYUP:
              switch(e.key.keysym.scancode)
              {
                case SDL_SCANCODE_W:
                  up = false;
                  break;
                case SDL_SCANCODE_S:
                  down = false;
                  break;
                case SDL_SCANCODE_A:
                  left = false;
                  break;
                case SDL_SCANCODE_D:
                  right = false;
                  break;
                default:
                  break;
              }
              break;
            case SDL_MOUSEBUTTONDOWN:
              switch(e.button.button)
              {
                case SDL_BUTTON_LEFT:
                  if(game_started)
                  {
                    bullet_flag = true;
                  }
                  else
                  {
                    if(mouseOver(newGameBtn_dest))
                      game_started = true;
                    else if(mouseOver(quitBtn_dest))
                      quit = true;
                  }
                  break;
                default:
                  break;
              }
              break;
            case SDL_MOUSEBUTTONUP:
              switch(e.button.button)
              {
                case SDL_BUTTON_LEFT:
                  if(game_started)
                  {
                    bullet_flag = false;
                  }
                  else
                  {
                    game_started = false;
                  }
                default:
                  break;
              }
              break;
          }

        }

        //Display start menu screen
        if(!game_started)
        {
          //if(mouseOver(newGameBtn_dest))
          //if(mouseOver(quitBtn_dest))

          SDL_RenderClear(gRenderer);
          SDL_RenderCopy(gRenderer, start_screen_texture, NULL, NULL);
          SDL_RenderCopy(gRenderer, new_game_button_texture, NULL, &newGameBtn_dest);
          SDL_RenderCopy(gRenderer, quit_button_texture, NULL, &quitBtn_dest);
          SDL_RenderPresent(gRenderer);
          SDL_Delay(1000/60);
        }

        //Display game when started
        else if(game_started)
        {
          if(!paused && !game_over)
          {
            //Update player position
            player = movePlayer(player, up, right, down, left);

            //Eventually give a shot from player
            if(bullet_flag)
            {
              new_bullet = createBulletP(player);
              appendBulletNode(new_bullet);
              bullet_flag = false;
            }

            //Eventually give a shot from enemy
            shot_counter++;
            if(shot_counter == 60)
            {
              //give a shot form all the enemies
              if(firstEnemyNode == NULL)
              {
                printf("Lista wrogów jest pusta więc nie mogą strzelać!\n");
              }
              else
              {
                for(ptr_main=firstEnemyNode; ptr_main!=NULL; ptr_main=ptr_main->next_enemy)
                {
                  new_bullet = createBulletE(ptr_main->this_enemy, player);
                  appendBulletNode(new_bullet);
                }
              }

              shot_counter = 0;
            }

            //Move all bullets
            moveAllBullets();

            //Remove bullets colliding with player and enemies
            removeCollidingBulletsP(player);
            for(Enemy_le* ptr=firstEnemyNode; ptr!=NULL; ptr=ptr->next_enemy)
            {
              removeCollidingBulletsE( ptr->this_enemy, player );
            }

            //Eventually create new enemy
            create_enemy_count++;
            if( create_enemy_count == frequency /*&&  maxEnemiesNumber < 12*/  )
            {
              new_enemy = createEnemy();
              appendEnemyNode(new_enemy);
              create_enemy_count = 0;
              //maxEnemiesNumber++;
            }
            frequency_counter++;
            if(frequency_counter == 600)
            {
              if(frequency > 22)
                frequency /= 2;
              frequency_counter = 0;
            }

            //move all enemies to player position
            updateEnemiesPositions(player);

            //Handle collision between enemies themselves
            handleCollisionBetweenEnemies();

            //Make route for player when going through enemies
            makeRoute(player);

            //Delete dead enemies
            removeDeadEnemies();
          }

          //Stop the game and do actions for game_over
          if(player->hP == 0)
          {
            game_over = true;
          }



          //////////////////////
          //                  //
          //  RENDERING AREA  //
          //                  //
          //////////////////////
          SDL_RenderClear(gRenderer);
          //Render background
          while( bcg_dest.y <= SCREEN_HEIGHT )
          {
            while( (bcg_dest.x + bcg_dest.w) <= SCREEN_WIDTH )
            {
              SDL_RenderCopy( gRenderer, background_texture, &bcg_src, &bcg_dest );
              bcg_dest.x += bcg_src.w;
              SDL_RenderCopy( gRenderer, background_texture, &bcg_src, &bcg_dest );
            }
            bcg_dest.x = 0;
            bcg_dest.y += bcg_dest.h;
          }bcg_dest.y = 0;

          //render player score
          renderPlayerScore(player->score);
          //Render bullets
          renderAllBullets();
          //Render enemies
          renderAllEnemies();
          //render player
          renderPlayer(player);

          if(paused)
            renderPauseScreen();
          if(game_over)
            renderGameOverScreen(player->score);

          //if(game_over)
            //renderGameOverScreen();

          //present renderer on the screen
          SDL_RenderPresent(gRenderer);
          SDL_Delay(1000/60);


        }


      }//MAIN LOOP END


      //Free resources and quit SDL
      cleanUp();
      destroyPlayer(player);
    }
  }
  return 0;
}
