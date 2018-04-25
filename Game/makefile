#OBJS specifies which files to compile as part of the project
OBJS = main3.c player.c enemy.c bullet.c screen_functions.c SDL_initialization_functions.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -std=c99 -g

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = shooter_1.0

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
