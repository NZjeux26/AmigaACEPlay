#ifndef _GAME_H_
#define _GAME_H_

// Function headers from game.c go here
// It's best to put here only those functions which are needed in other files.

void gameGsCreate(void);
void gameGsLoop(void);
void gameGsDestroy(void);
void updateScore(void);
void highScoreCheck(void);
short getHighScore(void);
void swap(int *a, int *b);
void bubbleSort(short s, short a[]); //array size and the array  

typedef struct g_obj {//struct for player parameters
    short x;        //position X
    short y;        //position Y
    short w;        //Rectangle width
    short h;        //Rectangle height
    short xvel;     //x velocity
    short yvel;     //y velocity
    short health;   //health
    short colour;    
} g_obj;

typedef struct g_star{
    short x;        //position X
    short y;        //position Y
    short colour;
    short velocity;
} g_star;

#endif // _GAME_H_