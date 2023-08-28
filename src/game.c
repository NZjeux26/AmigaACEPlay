#include "game.h"
#include <ace/managers/key.h>
#include <ace/managers/game.h>
#include <ace/managers/system.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/blit.h> // Blitting fns
#include <ace/utils/file.h>
#include <ace/utils/font.h>
#include <ace/utils/string.h>
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
tFont *fallfontsmall; //global for font
tTextBitMap *scoretextbitmap;//global for score text
char scorebuffer[20];
g_obj player; //player object declaration
g_obj blocks[MAX_BLOCKS]; //block object declaration
short scoreSize;
short gSCORE = 0;
short g_highScore = 0; //needs to be assigned prior to initialization
static time_t startTime;
bool g_scored = false;

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
  s_pVpScore->pPalette[6] = 0xFFFF; // white

  // Draw line separating score VPort and main VPort, leave one line blank after it
  blitLine(
    s_pScoreBuffer->pBack,
    0, s_pVpScore->uwHeight - 2,
    s_pVpScore->uwWidth - 1, s_pVpScore->uwHeight - 2,
    SCORE_COLOR, 0xFFFF, 0 // Try patterns 0xAAAA, 0xEEEE, etc.
  );
  gSCORE = 0; 
  g_highScore;// add function here to get the highscore from file first.
  
  fallfontsmall = fontCreate("myacefont.fnt");//create font
  tTextBitMap *highscorebitmap = fontCreateTextBitMapFromStr(fallfontsmall, "High Score: ");
  fontDrawTextBitMap(s_pScoreBuffer->pBack, highscorebitmap, 0,10, 6, FONT_COOKIE);

  tTextBitMap *textbitmap = fontCreateTextBitMapFromStr(fallfontsmall, "Score: ");
  fontDrawTextBitMap(s_pScoreBuffer->pBack, textbitmap, 0,20, 6, FONT_COOKIE);
  
  stringDecimalFromULong(gSCORE, scorebuffer);
  scoretextbitmap = fontCreateTextBitMapFromStr(fallfontsmall, scorebuffer); //redo bitmap
  
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
short BLOCKS = 2;
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

  //move blocks down
  for (int s = 0; s < BLOCKS; s++){ 
      if(blocks[s].y > 195){  //if block moves past player 
      g_scored = true;//the player has scored for this frame.
      
      //change position
      blocks[s].x = rand() % (PLAYFIELD_WIDTH - blocks[s].w - 1);
      blocks[s].y = rand() % (PLAYFIELD_HEIGHT - 110);
      }//end of if
      
      else {//else move blocks
        blocks[s].y += blocks[s].yvel; 
      }

      if(Collision(&blocks[s], &player)){//check for collision
        highScoreCheck();
        gameExit();
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
  
  if (g_scored){  //if the player ahs scored, set to false and update the score
    g_scored = false;
    updateScore(); //moved into function for code clarity
  }
  vPortWaitForEnd(s_pVpMain);
  }
 
}

void gameGsDestroy(void) {
  systemUse();
  // This will also destroy all associated viewports and viewport managers
  viewDestroy(s_pView);
}

void swap(int *a, int *b){//for bubble sort.
    int temp = *a;
    *a = *b;
    *b = temp;
}//swap

void updateScore(void) {  //bug seems to appear where text for 10000 + seems to be erroring with: ERR: Text '10000' doesn't fit in text bitmap, text needs: 33,8, bitmap size: 32,8
    gSCORE = gSCORE + 100;  //add score
    stringDecimalFromULong(gSCORE, scorebuffer);
    blitRect(s_pScoreBuffer->pBack, 40, 20, scoretextbitmap->uwActualWidth, scoretextbitmap->uwActualHeight, 0); //erase scorebuffer
    fontFillTextBitMap(fallfontsmall, scoretextbitmap, scorebuffer);//refill
    fontDrawTextBitMap(s_pScoreBuffer->pBack, scoretextbitmap, 40,20, 6, FONT_COOKIE);  //draw
}

void highScoreCheck(void) {
  short score = gSCORE;
  char charScore[30];
  systemUse();
  char filename[20] = "scoresheet.txt";
    
  if(!fileExists(filename)){  //check if the file exists, if not create and add the score
      tFile *file = fileOpen(filename, "w");
      stringDecimalFromULong(score,charScore);
      fileWriteStr(file, charScore);//add the score to the file 
      fileClose(file); 
  }
  else{//if file exsits, read the score chec against the player score, if greater write to file else do nothing
    tFile *file = fileOpen(filename, "a");
    char tempscore[10];
    short tScore;
    fileRead(file,tempscore,10);//read the score
    //function to return highest score in the file.

    //tScore = strtol(tempscore, NULL, 10);//convert to short.
    tScore = getHighScore();
    logWrite("TScore Is: %d\n", tScore);
    if(score > tScore){//if score is greater than the current HS then add it to the end.
      stringDecimalFromULong(score,charScore);
      fileWriteStr(file, "\n");
      fileWriteStr(file,charScore);
      fileClose(file);
    }
    else fileClose(file);//else do nothing
  }
}

short getHighScore(void){
  //read the last item in the file and return it as the HS if the file doesn't exist then return Zero
  char filename[20] = "scoresheet.txt";
  short highScore = 0;
  if((!fileExists(filename))) return 0;

  tFile *file = fileOpen(filename, "r");
  if(!file) return 101; //set to 1 so i know it was a cope out

  char **lines = calloc(10, sizeof(char *));
  if(!lines){
    fileClose(file);
    return 102;
  }
  memset(lines,0,sizeof(char*) * 10);

  char tline[512];
  short x = 0;
  while(fgets(tline, 512, file)){//issues getting the tokens into the array of lines
    char *token = strtok(tline, "\n");
    while(token){
      logWrite("File Reading: %s\n", token);
      short tSize = sizeof(token);//might not be needed since token is already a char but leaving for now.
      char temp[tSize] = token; 
      short tScore = strtol(temp, NULL, 10);  //convert to short
      if(tScore > highScore) highScore = tScore;  //if the read score is > than the HS then overwirte it.
      token = strtok(NULL, "\n");
      if(token == NULL) break;
    }
  }

  for(int i = 0; i < x; i++){
    logWrite("Lines Array: %i\n", lines[i]);
  }
  fileClose(file);
 
  return highScore; //return the Highest found score.
  /*In Theory the last int he file should be the highest if this works correctly. This could get resource intensive if a person has hundreds of High scores*/
}
// void highScoreCheck(void){
//     short score = gSCORE;
//     short scoreText[11]; //set to 11, 0-9 are actual scores and the 11 is used as drop space.
//     char charScore[30];
//     systemUse();
//     char filename[20] = "scoresheet.txt";
    
//     if(!fileExists(filename)){  //check if the file exists, if not create and add the score
//         tFile *file = fileOpen(filename, "w");
//         stringDecimalFromULong(score,charScore);
//         fileWriteStr(file, charScore);//add the score to the file 
//         fileClose(file);
        
//     }
//     else{
//       tFile *file = fileOpen(filename, "r");
//         for (short i = 0; i < 11; i++){
//             if(fileScanf(file, "%d", &scoreText[i]) != 1){
//               break;
//             }
//         }
//         fileClose(file);//now the scoresheet contents are in the array
//         //put the score into the last array spot, bubble sort the array and then write the top ten to the scoresheet and the top spot to the global HS var
        
//         if(score < scoreText[10]){// what happens when there is less than 10 scores?
//           //do nothing?
//         }
//         else{
//           scoreText[11] = score;
//           bubbleSort(11, scoreText);
//         }
//     }
//    systemUnuse();
// }
//this might not work since the items in the file are chars not shorts. 
void bubbleSort(short s, short t[]){// s is the size of the array and t is the array
  bool swapping = true;
  while(swapping){
    swapping = false;
    for (short i = 0; i < s - 1; i++){
      if(t[i] > t[i + 1]){
        short temp = t[i];
        t[i] = t[i + 1];
        t[i + 1] = temp;
        swapping = true;
      }
    }
  }

}
//extra function to write the highscore
