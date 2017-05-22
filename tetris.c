﻿#include <string.h>
#include "tetris.h"
#include "rankSystem.h"
#include "rankMenu.h"
#include "tetrisUtility.h"
#include "tetrisData.h"

#define COLOR_BLACK   0
#define COLOR_RED     1
#define COLOR_GREEN   2
#define COLOR_YELLOW  3
#define COLOR_BLUE    4
#define COLOR_MAGENTA 5
#define COLOR_CYAN    6
#define COLOR_WHITE   7

#define NOTHING	0
#define QUIT	'q'
#define FALL	' '

#define MENU_PLAY '1'
#define MENU_RANK '2'
#define MENU_RECOMMEND '3'
#define MENU_EXIT '4'

#define BLOCK_DISPLAY_HEIGHT = 6;
#define BLOCK_DISPLAY_Y = 2;

static struct sigaction act, oact;

void resetBlock();

void resetGame();

void initBlockQueue();

void popBlockQueue();

void printr(char c);

void getNewRank(int score);

void redrawCurrentBlock();

void redrawBlockPreview();

void redrawBlockPreview();

void resetCursor();

int main() {
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	loadRank();

	srand((unsigned int)time(NULL));

	// COLOR
	start_color();
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);

	while(!exit) {
		clear();
		switch(menu()) {
			case MENU_PLAY: play(); break;
			case MENU_RANK: rankMenu(); break;
			case MENU_EXIT: exit = 1; break;
			default: break;
		}
	}

	saveRank();

	endwin();
	system("clear");
	return 0;
}

int GetCommand() {
	int command;

	command = wgetch(stdscr);
	switch(command) {
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;
		case KEY_LEFT:
			break;
		case KEY_RIGHT:
			break;
		case ' ':
			command = FALL;
			break;
		case 'q':
		case 'Q':
			command = QUIT;
			break;
		default:
			command = NOTHING;
			break;
	}
	return command;
}

int ProcessCommand(int command) {
	int ret=1;
	int drawFlag=0;

	switch(command) {
		case QUIT:
			ret = QUIT;
			break;
		case FALL:
			blockY = getMinBlockY(field, nextBlock[0], blockRotation, blockY, blockX);
			BlockDown(0); // trash value 0
			break;
		case KEY_UP:
			if ((drawFlag = canPlaceBlock(field,nextBlock[0],(blockRotation+1)%4,blockY,blockX)))
				blockRotation=(blockRotation+1)%4;
			break;
		case KEY_DOWN:
			if ((drawFlag = canPlaceBlock(field,nextBlock[0],blockRotation,blockY+1,blockX)))
				blockY++;
			break;
		case KEY_RIGHT:
			if ((drawFlag = canPlaceBlock(field,nextBlock[0],blockRotation,blockY,blockX+1)))
				blockX++;
			break;
		case KEY_LEFT:
			if ((drawFlag = canPlaceBlock(field,nextBlock[0],blockRotation,blockY,blockX-1)))
				blockX--;
			break;
		default:
			break;
	}

	if (drawFlag) DrawChange(field,command,nextBlock[0],blockRotation,blockY,blockX);

	return ret;	
}

void redrawField() {
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


void redrawScore(int score) {
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

void play() {
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	resetGame();

	int i,j;

	// Draw field box 
	drawBox(0,0,HEIGHT,WIDTH);

	// Draw preview box
	move(BLOCK_DISPLAY_Y + i * BLOCK_DISPLAY_HEIGHT, WIDTH + 10);
	printw("PREVIEW");
	for (i = 0; i < BLOCK_NUM - 1; i ++) {
		drawBox(BLOCK_DISPLAY_Y + i * BLOCK_DISPLAY_HEIGHT + 1,WIDTH+10,4,8);
	}

	// Draw score box
	move(BLOCK_DISPLAY_Y + (BLOCK_NUM - 1) * BLOCK_DISPLAY_HEIGHT + 1,WIDTH+10);
	printw("SCORE");
	drawBox(BLOCK_DISPLAY_Y + (BLOCK_NUM - 1) * BLOCK_DISPLAY_HEIGHT + 2,WIDTH+10,1,8);

	redrawField();
	redrawShadow();
	redrawCurrentBlock();
	redrawBlockPreview();
	redrawScore(score);

	for (i = 0; i < BLOCK_NUM - 1; i ++) {
		drawBox(BLOCK_DISPLAY_Y + i * BLOCK_DISPLAY_HEIGHT + 1,WIDTH+10,4,8);
	}

	do{
		if (timedOut==0) {
			alarm(1);
			timedOut=1;
		}

		command = GetCommand();
		if (ProcessCommand(command)==QUIT) {
			alarm(0);
			drawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	} while(!gameOver);

	alarm(0);
	getch();
	drawBox(HEIGHT/2-1, WIDTH/2-5, 1, 10);
	move(HEIGHT/2, WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	rankSubmitScreen(score);
}

char menu() {
	printw("1. Play\n");
	printw("2. Rank\n");
	printw("3. Recommended Play\n");
	printw("4. Exit\n");
	return wgetch(stdscr);
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotation, int blockY, int blockX) {
	int i,j;
	int blk = currentBlock,rot = blockRotation, y = blockY, x = blockX;
	int oldShadowY;
	switch(command) {
		case KEY_UP:
			rot=(rot+3)%4;
			break;
		case KEY_DOWN:
			y--;
			break;
		case KEY_LEFT:
			x++;
			break;
		case KEY_RIGHT:
			x--;
			break;
	} 
	// TODO
	redrawField();
	redrawShadow();
	redrawCurrentBlock();
}

void BlockDown(int sig) {
	int drawFlag=0;
	int i, j;
	
	if ((drawFlag = canPlaceBlock(field,nextBlock[0],blockRotation,blockY+1,blockX))) {
		// Go down
		blockY ++;
		DrawChange(field,KEY_DOWN,nextBlock[0],blockRotation,blockY,blockX);
	} else {
		if (blockY == -1) {
			gameOver = 1;
		}

		score += addBlockToFieldAndGetScore(field, nextBlock[0], blockRotation, blockY, blockX);
		score += deleteLinesAndGetScore(field);

		// Retrieve next block, reinit block state, and update queue
		popBlockQueue();
		resetBlock();

		redrawBlockPreview();
		redrawScore(score);
		redrawField();
		redrawShadow();
		redrawCurrentBlock();
	}
	timedOut=0;
}
void redrawShadow() {
	attron(COLOR_PAIR(COLOR_MAGENTA));
	drawBlockOnField(getMinBlockY(field, nextBlock[0], blockRotation, blockY, blockX),
			blockX, nextBlock[0], blockRotation, ' ');
	attroff(COLOR_PAIR(COLOR_MAGENTA));
}

void resetBlock() {
	blockRotation=0;
	blockY= -1;
	blockX=WIDTH/2-2;
}

void resetGame() {
	int i,j;

	for (j=0;j<HEIGHT;j++)
		for (i=0;i<WIDTH;i++)
			field[j][i]=0;

	score=0;	
	gameOver=0;
	timedOut=0;

	initBlockQueue();
	resetBlock();
}

void initBlockQueue() {
	for (int i = 0; i < BLOCK_NUM; ++ i) {
		nextBlock[i] = rand() % NUM_OF_SHAPE;
	}
}

void popBlockQueue() {
	for (int i = 0; i < BLOCK_NUM - 1; ++ i) {
		nextBlock[i] = nextBlock[i + 1];
	}

	nextBlock[BLOCK_NUM - 1] = rand() % NUM_OF_SHAPE;
}

void printr(char c) {
	attron(A_REVERSE);
	printw("%c", c);
	attroff(A_REVERSE);
}

void redrawCurrentBlock() { 
	drawBlockOnField(blockY, blockX, nextBlock[0], blockRotation,' ');
}

void redrawBlockPreview() {
	int i, j;
	int blockIndex;

	for(blockIndex = 0; blockIndex < BLOCK_NUM - 1; blockIndex ++) {
		drawBlock(BLOCK_DISPLAY_Y + blockIndex * BLOCK_DISPLAY_HEIGHT + i, WIDTH + 13,
				nextBlock[blockIndex + 1], 0, ' ', 1);
	}
}

void resetCursor() {
	move(22, 29);
}
