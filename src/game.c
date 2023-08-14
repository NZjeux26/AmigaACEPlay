#include "game.h"
#include <ace/managers/key.h>
#include <ace/managers/game.h>
#include <ace/managers/system.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/blit.h> // Blitting fns
#include <ace/utils/font.h>
#include <time.h>
#include <stdbool.h>
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
#define MAX_BLOCKS 10 //theoritcal maximum number of blocks
//-------------------------------------------------------------- NEW STUFF START
//AMiga Pal 320x256
#define PLAYFIELD_HEIGHT (256-32) //32 for the top viewport height
#define PLAYFIELD_WIDTH (320)
#define PADDLE_MAX_POS_Y (PLAYFIELD_HEIGHT - PADDLE_HEIGHT - 1)
#define PADDLE_MAX_POS_X (PLAYFIELD_WIDTH - player.w - 1)
#define PADDLE_SPEED 4

static tView *s_pView; // View containing all the viewports
static tVPort *s_pVpScore; // Viewport for score
static tSimpleBufferManager *s_pScoreBuffer;
static tVPort *s_pVpMain; // Viewport for playfield
static tSimpleBufferManager *s_pMainBuffer;
//tFont *fallfont;
g_obj player; //player object declaration
g_obj blocks[MAX_BLOCKS]; //block object declaration
 
short SCORE = 0;
static time_t startTime;


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
  
  tFont *fallfont = fontCreate("myacefont.fnt");//create font
  tTextBitMap *textbitmap = fontCreateTextBitMapFromStr(fallfont, "hello Amiga");
  fontDrawTextBitMap(s_pMainBuffer->pBack, textbitmap, 80,80, 4,FONT_COOKIE);

  int seed = time(NULL);
  srand(seed);
  startTime = time(NULL);

  player; //player object
  player.x = (s_pVpMain->uwWidth - player.w) / 2; //place the player in the centre of the screen
  player.y = 190;
  player.w = 45;
  player.h = 10;
  player.colour = 5;

  for(int i = 0; i < MAX_BLOCKS; i++) { //create the blocks
    blocks[i].w = rand() % 55;
    blocks[i].h = rand() % 25;
    blocks[i].x = rand() % (PLAYFIELD_WIDTH - blocks[i].w - 1);// take the screen width minus the block width -1 to ensure it is withing the playfield.
    blocks[i].y = rand() % (PLAYFIELD_HEIGHT - 90);
    blocks[i].colour = (rand() % 5) + 1;  //cannot have black as an option!
    blocks[i].yvel = (rand() % 3) + 1; //previous calculation relied on floats which A500 doesn't have nativly.
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
/*The following is a breakdown of the logic of the collides() function:

The first condition checks if the x-coordinate of the first block is less than the x-coordinate of the second block plus the width of the second block. This ensures that the first block is not to the right of the second block.
The second condition checks if the x-coordinate of the first block plus the width of the first block is greater than the x-coordinate of the second block. This ensures that the first block is not to the left of the second block.
The third condition checks if the y-coordinate of the first block is less than the y-coordinate of the second block plus the height of the second block. This ensures that the first block is not above the second block.
The fourth condition checks if the y-coordinate of the first block plus the height of the first block is greater than the y-coordinate of the second block. This ensures that the first block is not below the second block.
If all of these conditions are met, then the two blocks collide.*/
//collision detection function
bool Collision(g_obj *a, g_obj *b){
  return (a->x < b->x + b->w && a->x + a->w > b->x && a->y < b->y + b->h && a->y + a->h > b->y);
}

//need a 2nd block counter first one is the max and the second is the current amount
short BLOCKS = 3;
void gameGsLoop(void) {
  // This will loop every frame
  if(keyCheck(KEY_ESCAPE)) {
    gameExit();
  }
  else {
  
  //undraw player
  blitRect( 
    s_pMainBuffer->pBack,
    player.x, player.y,
    player.w, player.h, 0
    );

  //undraw rectangles
  for (int i = 0; i < BLOCKS; i++) {
    blitRect( 
    s_pMainBuffer->pBack,
    blocks[i].x, blocks[i].y,
    blocks[i].w, blocks[i].h, 0
    );
  }
  //**Move things down**

  //move blocks down **Third block doesn't move?
  for (int s = 0; s < BLOCKS; s++){ //Sometimes all blocks move, some compiles only one or only two move
      if(blocks[s].y > 195){  //if block moves past player 
      //SCORE = SCORE + 100;  //add score
     
      //change position
      blocks[s].x = rand() % (PLAYFIELD_WIDTH - blocks[s].w - 1);
      blocks[s].y = rand() % (PLAYFIELD_HEIGHT - 110);
      }//end of if
      
      else {//else move blocks
        blocks[s].y += blocks[s].yvel; 
      }

      if(Collision(&blocks[s], &player)){//check for collision
        //gameExit();
      }
  }

  if(keyCheck(KEY_D)){  //move player right
    player.x = MIN(player.x + PADDLE_SPEED, 275);
  }
  if(keyCheck(KEY_A)){  //move player left
    player.x = MAX(player.x - PADDLE_SPEED, 0);
  }

 //**Draw things**
  time_t currentTime = time(&currentTime);  //gets the current time and if 15 has past add another block
  if(currentTime - startTime >= 15){
    BLOCKS++;
    startTime = currentTime;
  }
 // Redraw the player at new position
  blitRect(
    s_pMainBuffer->pBack, player.x, player.y,
      player.w, player.h, player.colour
  );

  for (int y = 0; y < BLOCKS; y++) {
    blitRect( 
    s_pMainBuffer->pBack,
    blocks[y].x, blocks[y].y,
    blocks[y].w, blocks[y].h, blocks[y].colour
    );
  }

  vPortWaitForEnd(s_pVpMain);
  }
}

void gameGsDestroy(void) {
  systemUse();
  // This will also destroy all associated viewports and viewport managers
  viewDestroy(s_pView);
 // fontDestroy(fallfont);//kill font
}