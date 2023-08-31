#include "menu.h"
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

#define MENU_HEIGHT	(256-32)
#define MENU_WIDTH	(320)

static tView *s_pView; //view for all viewports
static tVPort *s_pVpMain;//viewport for the menu
static tSimpleBufferManager *s_pMainBuffer;

tFont *menufont;    //font
tTextBitMap *menutextbitmap; //bitmap for the font

void menuGsCreate(void){
    //create view port and the display buffer for the main viewport
    s_pView = viewCreate(0, TAG_VIEW_GLOBAL_PALETTE, 1, TAG_END);

    s_pVpMain = vPortCreate(0, TAG_VPORT_VIEW, s_pView, TAG_VPORT_BPP, 4, TAG_END);
    s_pMainBuffer = simpleBufferCreate(0,TAG_SIMPLEBUFFER_VPORT, s_pVpMain, TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR, TAG_END);

    //colour palette for the menu
    s_pVpMain->pPalette[0] = 0x0000; //black
    s_pVpMain->pPalette[1] = 0xFFFF; //White

    menufont = fontCreate("myacefont.fnt");
    menutextbitmap = fontCreateTextBitMapFromStr(menufont, "TEST MENU");
    fontDrawTextBitMap(s_pMainBuffer->pBack, menutextbitmap, 50, 50, 1, FONT_COOKIE);

    systemUnuse();
    viewLoad(s_pView);
}

void menuGsLoop(void){
    if(keyCheck(KEY_ESCAPE)){
        gameExit();
    }
    if(keyCheck(KEY_L)){
        //load into the game state
    }
    vPortWaitForEnd(s_pVpMain);
}

void menuGsDestroy(void){
    systemUse();
    viewDestroy(s_pView);
}