/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/
//SDL_initialization_functions declarations
#ifndef SDL_initialization_functions_h
#define SDL_initialization_functions_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 800

//Definitions for init()
  SDL_Window* gWindow;
  SDL_Renderer* gRenderer;
  SDL_Surface* gScreenSurface;

bool init(void);
bool loadMedia(void);
void cleanUp(void);


//SDL_Texture* loadTexture(const char* path)

SDL_Texture* pause_texture;
SDL_Texture* game_over_texture;
SDL_Texture* dark_screen_texture;



//declarations for loadMedia()
SDL_Texture* start_screen_texture;
SDL_Texture* new_game_button_texture;
SDL_Texture* quit_button_texture;

SDL_Texture* player_texture;
SDL_Texture* player_hp_texture;
SDL_Texture* enemy_texture;
SDL_Texture* enemy_hp_texture;
SDL_Texture* background_texture;
SDL_Texture* bullet_texture;
SDL_Texture* gTexture;

//bool loadMedia(void);

//Fonts handling
TTF_Font* mainFont;
SDL_Texture* player_score_texture;
SDL_Rect* textRect;
SDL_Color textColor;



#endif
