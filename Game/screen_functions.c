/*
Projekt Shooter
Mateusz Szałowski - 298976
Uniwersytet wrocławski - Instytut informatyki
*/
#include <stdlib.h>
#include <stdbool.h>
#include "SDL_initialization_functions.h"
#include "screen_functions.h"

bool mouseOver(SDL_Rect rectangle)
{
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  if( (mouse_x > rectangle.x) && (mouse_x < rectangle.x+rectangle.w) && (mouse_y > rectangle.y) && (mouse_y < rectangle.y + rectangle.h) )
    return true;
  else
    return false;
}
