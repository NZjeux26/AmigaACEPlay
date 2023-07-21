#ifndef _GAME_H_
#define _GAME_H_

// Function headers from game.c go here
// It's best to put here only those functions which are needed in other files.

void gameGsCreate(void);
void gameGsLoop(void);
void gameGsDestroy(void);

typedef struct g_obj {//struct for player parameters
    float x;        //position X
    float y;        //position Y
    short w;        //Rectangle width
    short h;        //Rectangle height
    float xvel;     //x velocity
    float yvel;     //y velocity
    short health;   //health
    short colour;    
} g_obj;

#endif // _GAME_H_