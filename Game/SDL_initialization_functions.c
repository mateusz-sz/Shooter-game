/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/

//Here are main functions needed to initialize SDL, load media, convert images to textures and free resources
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "SDL_initialization_functions.h"


//
//  Handle fonts
//
//TTF_Font* mainFont = NULL;
//player_score_texture = NULL;
void setTextColor()
{
  textColor.r = 255;
  textColor.g = 255;
  textColor.b = 255;
}




//Initialize SDL
bool init()
{
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    //Create window
    gWindow = SDL_CreateWindow( "Shooter", 200, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gWindow == NULL )
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
      success = false;
    }
    else
    {
      //Create renderer for the window
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
      if(gRenderer == NULL)
      {
        printf("Could not initialize Renderer SDL_Error%s\n", SDL_GetError());
        success = false;
      }

      //Get window surface
      gScreenSurface = SDL_GetWindowSurface( gWindow );
    }
    if( TTF_Init() == -1 )
    {
      success = false;
    }
  }
  return success;
}//end of init





SDL_Texture* loadTexture(const char* path)
{
  //Final texture
  SDL_Texture* newTexture = NULL;


  //Load image at specified path
  SDL_Surface* LoadedSurface = IMG_Load( path );
  if( LoadedSurface == NULL )
  {
    printf( "Unable to load image %s! SDL Error: %s\n\n", path, SDL_GetError() );
  }
  else
  {
    //Create texture from loaded surface
    newTexture = SDL_CreateTextureFromSurface( gRenderer, LoadedSurface );
    if(newTexture == NULL)
    {
      printf("Could not create texture from %s! SDL_Error:%s\n", path, SDL_GetError());
    }

    //Get rid of old LoadedSurface
    SDL_FreeSurface(LoadedSurface);
  }

  return newTexture;
}



bool loadMedia()
{
   //Loading success flag
   bool success = true;

   //Load start screen
   start_screen_texture = loadTexture("img/Screens/start_screen.png");
   //Load new game button
   new_game_button_texture = loadTexture("img/Screens/new_game.png");
   //Load quit game button
   quit_button_texture = loadTexture("img/Screens/quit_button.png");
   //Load pause texture
   pause_texture = loadTexture("img/Screens/pause.png");
   //Load darkscreen
   dark_screen_texture = loadTexture("img/Screens/darkScreen.png");

   //Load character texture
   player_texture = loadTexture( "img/characterSpecial (3).png" );
   if( player_texture == NULL )
   {
     printf( "Failed to load player texture image!\n" );
     success = false;
   }

   //Load character hp texture
   player_hp_texture = loadTexture( "img/player_hp.png" );
   if( player_hp_texture == NULL )
   {
     printf( "Failed to load player hp texture image!\n" );
     success = false;
   }

   //Load enemy texture
   enemy_texture = loadTexture( "img/characterRed (4).png" );
   if( enemy_texture == NULL )
   {
     printf( "Failed to load enemy texture image!\n" );
     success = false;
   }

   //Load background texture
   background_texture = loadTexture( "img/world.bmp" );
   if( background_texture == NULL )
   {
     printf("Failed to load background bitmap! SDL_Error: %s\n", SDL_GetError());
     success = false;
   }

   //Load bullet texture
   bullet_texture = loadTexture( "img/bullet.png" );
   if( bullet_texture == NULL )
   {
     printf("Failed to load bullet png image! SDL_Error: %s\n", SDL_GetError());
     success = false;
   }

   //Load enemy hp texture
   enemy_hp_texture = loadTexture( "img/enemy_hp.png" );
   if( enemy_hp_texture == NULL )
   {
     printf("Failed to load enemy_hp png image! SDL_Error: %s\n", SDL_GetError());
     success = false;
   }

   //Load game over texture
   game_over_texture = loadTexture( "img/Screens/game_over.png" );
   if( game_over_texture == NULL )
   {
     printf("Failed to load game over jpeg image! SDL_Error: %s\n", SDL_GetError());
     success = false;
   }

   //Load font
   mainFont = TTF_OpenFont("LED LCD 123.ttf", 35);
   if( mainFont == NULL )
   {
     printf("Failed to load ttf font! SDL_Error: %s\n", SDL_GetError());
     success = false;
   }

  return success;
}

void renderDarkScreen()
{
  SDL_Rect darkScreen_dest;
  SDL_QueryTexture(dark_screen_texture, NULL, NULL, &darkScreen_dest.w, &darkScreen_dest.h);
  darkScreen_dest.x = 0;
  darkScreen_dest.y = 0;
  for(int i=0; i<SCREEN_WIDTH; i+=darkScreen_dest.w)
  {
    darkScreen_dest.x = i;
    for(int j=0; j<SCREEN_HEIGHT; j+=darkScreen_dest.h)
    {
      darkScreen_dest.y = j;
      SDL_RenderCopy(gRenderer, dark_screen_texture, NULL, &darkScreen_dest);
    }
  }
}

void renderPauseScreen()
{
  renderDarkScreen();
  SDL_Rect gamePaused;
  SDL_QueryTexture(pause_texture, NULL, NULL, &gamePaused.w, &gamePaused.h);
  gamePaused.x = SCREEN_WIDTH/2 - gamePaused.w/2;
  gamePaused.y = SCREEN_HEIGHT/2 - gamePaused.h/2;
  SDL_RenderCopy(gRenderer, pause_texture, NULL, &gamePaused);
}

void renderGameOverScreen(int score)
{
  char* textString = "Your score was: ";
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

  bool quit = false;
  renderDarkScreen();
  SDL_Color white = {255,255,255};
  SDL_Color gray = {108,108,108};

  SDL_Rect gameOverText_dest, restartGameBtn_dest;
  SDL_Surface* gameOverTextSurface = TTF_RenderText_Blended( TTF_OpenFont("LED LCD 123.ttf", 55), "game over", white );

  SDL_Surface* restartBtnSurface = TTF_RenderText_Blended( mainFont, allText, white );
  SDL_Texture* gameOverTextTexture = SDL_CreateTextureFromSurface( gRenderer, gameOverTextSurface );
  SDL_Texture* restartBtnTexture = SDL_CreateTextureFromSurface( gRenderer, restartBtnSurface );
  SDL_QueryTexture(gameOverTextTexture, NULL, NULL, &gameOverText_dest.w, &gameOverText_dest.h);
  SDL_QueryTexture(restartBtnTexture, NULL, NULL, &restartGameBtn_dest.w, &restartGameBtn_dest.h);
  gameOverText_dest.x = SCREEN_WIDTH/2 - gameOverText_dest.w/2;
  gameOverText_dest.y = SCREEN_HEIGHT/2 - gameOverText_dest.h * 2;
  restartGameBtn_dest.x = SCREEN_WIDTH/2 - restartGameBtn_dest.w/2;
  restartGameBtn_dest.y = gameOverText_dest.y + gameOverText_dest.h + 25;
char string;
  //If mouse over restart button - generate gray texture
  if(mouseOver(restartGameBtn_dest))
  {
    restartBtnSurface = TTF_RenderText_Blended( mainFont, allText, gray );
    restartBtnTexture = SDL_CreateTextureFromSurface( gRenderer, restartBtnSurface );

  }

  renderPlayerScore(12);
  SDL_RenderCopy(gRenderer, gameOverTextTexture, NULL, &gameOverText_dest);
  SDL_RenderCopy(gRenderer, restartBtnTexture, NULL, &restartGameBtn_dest);

}










void cleanUp()
{
  //Deallocate surface
  //SDL_FreeSurface( gMarioBMP );
  //gMarioBMP = NULL;

  SDL_DestroyTexture(gTexture);
  gTexture = NULL;

  SDL_DestroyTexture(player_texture);
  player_texture = NULL;

  //Destroy Window
  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gRenderer = NULL;
  gWindow = NULL;



  //Quit SDL subsystems
  SDL_Quit();
}
