#ifdef __ADAM__
#include "../platform-specific/graphics.h"
#include <video/tms99x8.h>
#include <conio.h>
#include <sys/ioctl.h>
#include <smartkeys.h>
#include "udgs.h"

//defines for tile locations
// #define POINTER_TILE 216
// #define CHIP_TILE 1
// #define UL_CORNER_TILE 144
// #define HORIZONTAL_TILE 145
// #define UR_CORNER_TILE 146
// #define LL_CORNER_TILE 147
// #define LR_CORNER_TILE 148
// #define VERTICAL_TILE 149

#define POINTER_TILE 0xbd
#define CHIP_TILE 0xba
#define UL_CORNER_TILE 0xb5
#define HORIZONTAL_TILE 0xb6
#define UR_CORNER_TILE 0xb7
#define LL_CORNER_TILE 0xb8
#define LR_CORNER_TILE 0xb9
#define VERTICAL_TILE 0x98

unsigned char pointerLocation_x = 255;
unsigned char pointerLocation_y = 255;
extern unsigned long currentTime;
bool vsync;

unsigned char selectBoxUL[] =
{
    0x7f,0xff,0xC0,0xC0,0xc0,0xC0,0xC0,0xC0,
    0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
    0xff,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

unsigned char selectBoxUR[] =
{
    0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xF8,0xFC,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,
    0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c
};

unsigned char selectBoxLL[] =
{
    0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xc0,
    0xC0,0xC0,0xC0,0xc0,0xFF,0x7F,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00
};

unsigned char selectBoxLR[] =
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,
    0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,
    0x0c,0x0c,0x0c,0x0c,0xFC,0xF8,0x00,0x00
};

unsigned char diePatterns[16][9] = 
{
    {0x80,0x81,0x82,0x81,0x86,0x81,0x83,0x81,0x84},  // 1
    {0x85,0x81,0x82,0x81,0x81,0x81,0x83,0x81,0x89},  // 2
    {0x85,0x81,0x82,0x81,0x86,0x81,0x83,0x81,0x89},  // 3
    {0x85,0x81,0x87,0x81,0x81,0x81,0x88,0x81,0x89},  // 4
    {0x85,0x81,0x87,0x81,0x86,0x81,0x88,0x81,0x89},  // 5
    {0x85,0x81,0x87,0x86,0x81,0x86,0x88,0x81,0x89},  // 6
    {0x80,0x81,0x82,0x81,0x86,0x81,0x83,0x81,0x84},  // 1
    {0x85,0x81,0x82,0x81,0x81,0x81,0x83,0x81,0x89},  // 2
    {0x85,0x81,0x82,0x81,0x86,0x81,0x83,0x81,0x89},  // 3
    {0x85,0x81,0x87,0x81,0x81,0x81,0x88,0x81,0x89},  // 4
    {0x85,0x81,0x87,0x81,0x86,0x81,0x88,0x81,0x89},  // 5
    {0x85,0x81,0x87,0x86,0x81,0x86,0x88,0x81,0x89},  // 6
    {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20},  // Empty Space
    {0x80,0x81,0x82,0x90,0x91,0x92,0x93,0x95,0x94},  // 1 Roll Left
    {0x80,0x81,0x82,0x90,0x91,0x92,0x93,0x96,0x94},  // 2 Rolls Left
    {0x80,0x81,0x82,0x90,0x91,0x92,0x93,0x97,0x94},  // Can't Roll
};  



void drawTextAlt(unsigned char x, unsigned char y, char* s)
{
    gotoxy(x,y);
    cputs(s);
    // drawText(x,y,s);
}

void clearBelowBoard() 
{
    vdp_vfill(0x1500,0,0x300); //Clear last three lines of the display
}

void drawChar(unsigned char x, unsigned char y, char c, unsigned char alt) {gotoxy(x,y); cputc(c);}

void drawIcon(unsigned char x, unsigned char y, unsigned char icon) 
{
    gotoxy(x,y); 
    cputc(icon);
}

void drawDie(unsigned char x, unsigned char y, unsigned char s, bool isSelected, bool isHighlighted) 
{
  unsigned char j;
  // independent code does HEIGHT-4, but we want it to be one more than that to be at edge of screen
//   y++;

  // Don't draw die if invalid index passed in
  if (!s || s>16 || y==HEIGHT-3)
    return;

  if (isSelected)
    vdp_color(VDP_INK_GRAY,VDP_INK_MEDIUM_GREEN,VDP_INK_MEDIUM_GREEN);
  else
    vdp_color(VDP_INK_WHITE,VDP_INK_MEDIUM_GREEN,VDP_INK_MEDIUM_GREEN);

  gotoxy(x,y++);
//   for (i=0;i<3;i++) {
    for (j=0;j<9;j++) {
      cputc(diePatterns[s-1][j]);
      if (j == 2 || j == 5)
        gotoxy(x,y++);
    }
//   }
    vdp_color(VDP_INK_WHITE,VDP_INK_MEDIUM_GREEN,VDP_INK_MEDIUM_GREEN);

}

void drawClock(unsigned char x, unsigned char y) {}
void drawConnectionIcon(unsigned char x, unsigned char y) {gotoxy(x,y);cputc(0xbb);}
void drawBlank(unsigned char x, unsigned char y) {gotoxy(x,y);cputc(0x20);}
void drawSpace(unsigned char x, unsigned char y, unsigned char w) {gotoxy(x,y);cputc(0x20);}

void drawBoard() {
    static uint8_t y,x;

    // Vertical lines
    for (x=SCORES_X+9;x<32;x+=4) 
    {
        for (y=1;y<20;y++) {
          gotoxy(x,y);
          cputc(VERTICAL_TILE);
        }
        // hires_Mask(x,0,1,160,0x20);
    }
  
    // Main scores box
    drawBox(SCORES_X+5,0,23,19);
    drawBox(SCORES_X-1,2,5,17); 
  
    // Thin horz ines
    // hires_Mask(SCORES_X,9*8,29,1, 0xff & ROP_ORANGE); 
    // hires_Mask(SCORES_X,12*8,29,1, 0xff & ROP_ORANGE);
    
    // // Thick horz lines
    // hires_Mask(SCORES_X,2*8,29,2, 0xff & ROP_ORANGE); 
    // hires_Mask(SCORES_X,20*8-1,29,2, 0xff & ROP_ORANGE); 
    
    // Score names (16 for end game score)
    for(y = 0; y<14; y++) { 
      drawTextAlt(SCORES_X,scoreY[y],scores[y]);
    } 
    
    // Fujitzee score text
    drawFujitzee(SCORES_X,19);
  }
  
  void drawFujitzee(unsigned char x, unsigned char y) {
    char fujitzee[] = {0x1b, 0x1c, 0x1d, 0x1e, 0x1e, 0};
    drawText(x,y,&fujitzee);
  }
  

void drawDiceCursor(unsigned char x) 
{
    // 0xe2
    vdp_put_sprite_16(1, (x<<3)-3, (20<<3)-3, 1, VDP_INK_LIGHT_RED);
    vdp_put_sprite_16(2, (x+2<<3)-3, (20<<3)-3, 2, VDP_INK_LIGHT_RED);
    vdp_put_sprite_16(3, (x<<3)-3, (20+2<<3)-3, 3, VDP_INK_LIGHT_RED);
    vdp_put_sprite_16(4, (x+2<<3)-3, (20+2<<3)-3, 4, VDP_INK_LIGHT_RED);

}

void hideDiceCursor(unsigned char x) 
{
    // don't care where... it's a sprite so turn it off.
    vdp_put_sprite_16(1, 0, 0xd0, 1, VDP_INK_TRANSPARENT);
    vdp_put_sprite_16(2, (x+2<<3)-3, (20<<3)-3, 2, VDP_INK_TRANSPARENT);
    vdp_put_sprite_16(3, (x<<3)-3, (20+2<<3)-3, 3, VDP_INK_TRANSPARENT);
    vdp_put_sprite_16(4, (x+2<<3)-3, (20+2<<3)-3, 4, VDP_INK_TRANSPARENT);
}

// Call to save screen buffer for later restore. Returns false
// if screen buffer not supported
bool saveScreenBuffer() { return false; }
void restoreScreenBuffer() {}

void setHighlight(int8_t player, bool isThisPlayer, uint8_t flash ) {}

void resetGraphics() {}

void setColorMode() {}



void enableDoubleBuffer()
{
}

void disableDoubleBuffer()
{
}

void drawBuffer()
{
    cputs(tempBuffer);
}

// Call to clear the screen to an empty table
void resetScreen()
{
    clrscr();
    drawBoard();
}

void clearStatusBar()
{
}

void drawStatusTextAt(unsigned char x, char* s)
{
}

void drawStatusText(char* s)
{
}

void drawStatusTimer()
{
    gotoxy(0,23);
    cputs("timer goes here");
}

void drawText(unsigned char x, unsigned char y, char* s)
{
    gotoxy(x,y);
    cputs(s);
    // smartkeys_puts(x*8,y*8,s);

}

void drawLine(unsigned char x, unsigned char y, unsigned char w)
{
    unsigned char i;
    gotoxy(x,y);
    for (i=0; i<w; i++)
        cputc(HORIZONTAL_TILE);
}

void drawBox(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
    unsigned char i;
    gotoxy(x,y++);
    //Draw the top
    cputc(UL_CORNER_TILE);
    for (i = 1; i <= w; i++)
        cputc(HORIZONTAL_TILE);
    cputc(UR_CORNER_TILE);

    //Draw the sides
    for (i = 0; i < h; i++)
    {
        gotoxy(x,y);
        cputc(VERTICAL_TILE);
        vdp_vfill((y<<8) + ((x+1)<<3),0,(w-1)<<3); //Clear last three lines of the display
        gotoxy(x+w+1,y++);
        cputc(VERTICAL_TILE);
    }
    gotoxy(x,y);
    //Draw the bottom
    cputc(LL_CORNER_TILE);
    for (i = 1; i <= w; i++)
        cputc(HORIZONTAL_TILE);
    cputc(LR_CORNER_TILE);


    //   printf("\x20\x20\x20\x20\x20\x9A\x9B\x9C\x20\x20\x20\x20\x20       FIVE CARD STUD");
    
}

void drawBorder()
{
}


unsigned char cycleNextColor()
{
}

void counterInt(void)
{
    vsync = true;
    currentTime++;
}

void initGraphics()
{
    //set up the vdp vram with tile data, both tile and color... change to mode 1 first thing.
    //change background to medium green...
    // int mode = 1;
    // console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
	smartkeys_set_mode();
    void *param = &udgs;
    console_ioctl(IOCTL_GENCON_SET_UDGS,&param);
    vdp_color(VDP_INK_WHITE,VDP_INK_MEDIUM_GREEN,VDP_INK_MEDIUM_GREEN);
    // double size sprite mode, load the dir cursor sprites
    vdp_set_reg(1,0xe2);
    vdp_set_sprite_16(1, selectBoxUL);
    vdp_set_sprite_16(2, selectBoxUR);
    vdp_set_sprite_16(3, selectBoxLL);
    vdp_set_sprite_16(4, selectBoxLR);

    clrscr();

    // drawDie(0,0,13,false, false);
    // drawDie(0,4,14,false, false);
    // drawDie(0,8,15,false, false);
    // drawDie(0,12,16,false, false);
    // drawDie(0,16,5,false, false);
    // drawDie(3,20,6,false, false);
    // drawDiceCursor(3);
    // sleep(5);
    // drawDie(7,20,5,false, false);
    // drawDiceCursor(7);
    // while(1);
    // smartkeys_puts(64,64,"Here is an example of SmartKeys Output.");
    
    // cputs("\xa0\xa1 \xa2\xa3\xa4"); // BET

	// cputs(" NORMAL TEXT");

	// gotoxy(2,2);
	// textcolor(RED);
	// textbackground(WHITE);
	// cputs("\x92");
	
	// textcolor(BLACK);
	// cputs("\x8F");



    // vdp_vwrite(tiles_ch,0,sizeof(tiles_ch));
    // vdp_vwrite(tiles_cc,0x2000,sizeof(tiles_cc));
    clrscr();
    //admittedly a weird place to put this, but we are using 9918 graphics library
    // functions for this...
    vsync = false;
    add_raster_int(counterInt);
}

void waitvsync()
{
    vsync = false;
    while (!vsync);
}

#endif /* __ADAM__ */
