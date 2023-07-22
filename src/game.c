#include "game.h"
#include <ace/managers/key.h>
#include <ace/managers/game.h>
#include <ace/managers/system.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/blit.h> // Blitting fns
#include <time.h>
#include <stdlib.h>

#define BALL_WIDTH 8
#define BALL_COLOR 1
#define PADDLE_WIDTH 8
#define PADDLE_HEIGHT 32
#define PADDLE_LEFT_COLOR 2
#define PADDLE_RIGHT_COLOR 3
#define SCORE_COLOR 1
#define WALL_HEIGHT 1
#define WALL_COLOR 1
#define NSTARS 25
#define MAX_BLOCKS 10 //theoritcal maximum number of blocks
//-------------------------------------------------------------- NEW STUFF START
#define PLAYFIELD_HEIGHT (256-32)
#define PLAYFIELD_WIDTH (320) //not correct  
#define PADDLE_MAX_POS_Y (PLAYFIELD_HEIGHT - PADDLE_HEIGHT - 1)
#define PADDLE_MAX_POS_X (PLAYFIELD_WIDTH - player.w - 1)
#define PADDLE_SPEED 4

static tView *s_pView; // View containing all the viewports
static tVPort *s_pVpScore; // Viewport for score
static tSimpleBufferManager *s_pScoreBuffer;
static tVPort *s_pVpMain; // Viewport for playfield
static tSimpleBufferManager *s_pMainBuffer;

g_obj player; //player object declaration
g_obj blocks[MAX_BLOCKS]; //block object declaration
static g_star stars[NSTARS]; //star object declaration

short BLOCKS = 3;
short SCORE = 0;

void gameGsCreate(void) {
  s_pView = viewCreate(0,
    TAG_VIEW_GLOBAL_PALETTE, 1,
  TAG_END);

  // Viewport for score bar - on top of screen
  s_pVpScore = vPortCreate(0,
    TAG_VPORT_VIEW, s_pView,
    TAG_VPORT_BPP, 4,
    TAG_VPORT_HEIGHT, 32,
  TAG_END);
  s_pScoreBuffer = simpleBufferCreate(0,
    TAG_SIMPLEBUFFER_VPORT, s_pVpScore,
    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
  TAG_END);

  // Now let's do the same for main playfield
  s_pVpMain = vPortCreate(0,
    TAG_VPORT_VIEW, s_pView,
    TAG_VPORT_BPP, 4,
  TAG_END);
  s_pMainBuffer = simpleBufferCreate(0,
    TAG_SIMPLEBUFFER_VPORT, s_pVpMain,
    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
  TAG_END);
  //palette with VBB set to four i can get 16-colours
  s_pVpScore->pPalette[0] = 0x0000; // First color is also border color 
  s_pVpScore->pPalette[1] = 0x0888; // Gray
  s_pVpScore->pPalette[2] = 0x0800; // Red - not max, a bit dark
  s_pVpScore->pPalette[3] = 0x0008; // Blue - same brightness as red 
  s_pVpScore->pPalette[4] = 0x0080; // Green - same brightness as blue
  s_pVpScore->pPalette[5] = 0xFFC0; // Yellow

  // Draw line separating score VPort and main VPort, leave one line blank after it
  blitLine(
    s_pScoreBuffer->pBack,
    0, s_pVpScore->uwHeight - 2,
    s_pVpScore->uwWidth - 1, s_pVpScore->uwHeight - 2,
    SCORE_COLOR, 0xFFFF, 0 // Try patterns 0xAAAA, 0xEEEE, etc.
  );

  // int seed = time(NULL);
  // srand(seed);
  
  player; //player object
  player.x = (s_pVpMain->uwWidth - player.w) / 2;
  player.y = 190;
  player.w = 45;
  player.h = 10;
  player.colour = 5;

  for(int i = 0; i < BLOCKS; i++) {
    blocks[i].x = rand() % PLAYFIELD_WIDTH;
    blocks[i].y = rand() % (PLAYFIELD_HEIGHT - 70);
    blocks[i].w = rand() % 70;
    blocks[i].h = rand() % 70;
    blocks[i].colour = rand() % 5;
    blocks[i].yvel = (rand()%512+64)/256.0;
  }

  // Draw wall on the bottom of main VPort
  blitRect(
    s_pMainBuffer->pBack,
    0, s_pVpMain->uwHeight - WALL_HEIGHT,
    s_pVpMain->uwWidth, WALL_HEIGHT, WALL_COLOR
  );

  systemUnuse();

  // Load the view
  viewLoad(s_pView);
}

void gameGsLoop(void) {
  // This will loop every frame
  if(keyCheck(KEY_ESCAPE)) {
    gameExit();
  }
  else {
  //**UNDRAW**

  //undraw player
  blitRect( 
    s_pMainBuffer->pBack,
    player.x, player.y,
    player.w, player.h, 0
    );

  //undraw stars
  //undraw rectangles
  for (int i = 0; i < BLOCKS; i++) {
    blitRect( 
    s_pMainBuffer->pBack,
    blocks[i].x, blocks[i].y,
    blocks[i].w, blocks[i].h, 0
    );
  }

  //**Move things down**

  //move blocks down
  for (int s = 0; s < BLOCKS; s++){         //only one block is actually moving
    short y = blocks[s].y += blocks[s].yvel;
  }

  if(keyCheck(KEY_D)){  //move player right
    player.x = MIN(player.x + PADDLE_SPEED, 275);
  }
  if(keyCheck(KEY_A)){  //move player left
    player.x = MAX(player.x - PADDLE_SPEED, 0);
  }

  //**Draw things**

 // Redraw the player at new position
  blitRect(
    s_pMainBuffer->pBack, player.x, player.y,
      player.w, player.h, player.colour
  );

  for (int i = 0; i < BLOCKS; i++) {
    blitRect( 
    s_pMainBuffer->pBack,
    blocks[i].x, blocks[i].y,
    blocks[i].w, blocks[i].h, blocks[i].colour
    );
  }

  for (int s = 0; s < BLOCKS; s++){
  
    if(blocks[s].y > 200){  //if block moves past player <-------------------- Issue seems to be here
      SCORE = SCORE + 100;  //add score

      blitRect(             //remove
      s_pMainBuffer->pBack,
      blocks[s].x, blocks[s].y,
      blocks[s].w, blocks[s].h, 0
      );
                            //change position
      blocks[s].x = rand() % PLAYFIELD_WIDTH;
      blocks[s].y = rand() % (PLAYFIELD_HEIGHT - 50); //mgiht be an issue with ghosting
      
      blitRect(             //redraw
      s_pMainBuffer->pBack,
      blocks[s].x, blocks[s].y,
      blocks[s].w, blocks[s].h, blocks[s].colour
      );

    }//end of if
  }//end of for
  
  vPortWaitForEnd(s_pVpMain);
  }
}

void gameGsDestroy(void) {
  systemUse();

  // This will also destroy all associated viewports and viewport managers
  viewDestroy(s_pView);
}