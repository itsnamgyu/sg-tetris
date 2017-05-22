#include <ncurses.h>
#include "tetrisData.h"
#include "gameUI.h"

#define BLOCK_DISPLAY_HEIGHT	6
#define BLOCK_DISPLAY_Y			2

// Utility Functions
void printr(char c);

void resetCursor();

//
void drawField(char field[HEIGHT][WIDTH]) {
	int i,j;
	for (j=0;j<HEIGHT;j++) {
		move(j+1,1);
		for (i=0;i<WIDTH;i++) {
			if (field[j][i]) {
				printr(' ');
			}
			else printw(".");
		}
	}
}

void drawScore(int score) {
	move(BLOCK_DISPLAY_Y + (BLOCK_NUM - 1) * BLOCK_DISPLAY_HEIGHT + 3,WIDTH+11);
	printw("%8d",score);
}

void drawBlock(int y, int x, int shapeId, int rotation, char tile, int redraw) {
	int i,j;
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			move(i+y,j+x);
			if (block[shapeId][rotation][i][j]==1 && i+y>=0) {
				printr(tile);
			} else {
				if (redraw) {
					printw(" ");
				}
			}
		}
	}
	resetCursor();
}

void drawBlockOnField(int y, int x, int shapeId, int rotation, char tile) {
	int i,j;
	for (i = 0; i < 4; ++ i) {
		for (j = 0; j < 4; ++ j) {
			if (i + y >= 0) {
				move(i+y + 1,j+x + 1);
				if (block[shapeId][rotation][i][j]==1 && i+y>=0) {
					printr(tile);
				}
			}
		}
	}

	resetCursor();
}

void drawBox(int y,int x, int height, int width) {
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for (i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for (j=0;j<height;j++) {
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for (i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);

	resetCursor();
}


void printr(char c) {
	attron(A_REVERSE);
	printw("%c", c);
	attroff(A_REVERSE);
}

void drawCurrentBlock(int y, int x, int shapeId, int rotation) { 
	drawBlockOnField(y, x, shapeId, rotation, ' ');
}

void drawShadow(int y, int x, int shapeId, int rotation) { 
	attron(COLOR_PAIR(COLOR_MAGENTA));
	drawBlockOnField(y, x, shapeId, rotation, ' ');
	attroff(COLOR_PAIR(COLOR_MAGENTA));
}

void drawBlockPreview(int queue[BLOCK_NUM]) {
	int i;

	for(i = 0; i < BLOCK_NUM - 1; i ++) {
		drawBlock(BLOCK_DISPLAY_Y + i * BLOCK_DISPLAY_HEIGHT + 2, WIDTH + 13,
				queue[i + 1], 0, ' ', 1);
	}
}

void resetCursor() {
	move(22, 29);
}

void drawOutline() {
	int i, j;

	// Draw field box 
	drawBox(0,0,HEIGHT,WIDTH);

	// Draw preview box
	move(BLOCK_DISPLAY_Y, WIDTH + 10);
	printw("PREVIEW");
	for (i = 0; i < BLOCK_NUM - 1; i ++) {
		drawBox(BLOCK_DISPLAY_Y + i * BLOCK_DISPLAY_HEIGHT + 1,WIDTH+10,4,8);
	}

	// Draw score box
	move(BLOCK_DISPLAY_Y + (BLOCK_NUM - 1) * BLOCK_DISPLAY_HEIGHT + 1,WIDTH+10);
	printw("SCORE");
	drawBox(BLOCK_DISPLAY_Y + (BLOCK_NUM - 1) * BLOCK_DISPLAY_HEIGHT + 2, WIDTH+10, 1, 8);
}
