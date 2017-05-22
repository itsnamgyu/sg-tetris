#include "tetrisData.h"
#include "tetrisUtility.h"
#include "gameUI.h"
#include "game.h"
#include "rankMenu.h"
#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>

char field[HEIGHT][WIDTH];
int queue[BLOCK_NUM];
int rotation;
int y;
int x;

int score;
int gameOver = 0;
int timedOut;

#define BLOCK_DISPLAY_HEIGHT	6
#define BLOCK_DISPLAY_Y			2

#define NOTHING	0
#define QUIT	'q'
#define FALL	' '

static struct sigaction act, oact;

// Utility Functions
int processCommand();

void blockFall(int sig);

void resetBlock();

void resetGame();

void initBlockQueue();

void popBlockQueue();


// 
void startGame(int mode) {
	clear();
	act.sa_handler = blockFall;
	sigaction(SIGALRM,&act,&oact);
	resetGame();

	int submitRank = 1;
	int i,j;

	drawField(field);
	drawShadow(getMinBlockY(field, queue[0], rotation, y, x), x, queue[0], rotation);
	drawCurrentBlock(y, x, queue[0], rotation);
	drawBlockPreview(queue);
	drawScore(score);

	drawOutline();

	do {
		if (timedOut==0) {
			alarm(1);
			timedOut=1;
		}
		if (processCommand() == QUIT) {
			submitRank = 0;
			break;
		}
	} while(!gameOver);

	alarm(0);
	drawBox(HEIGHT/2-1, WIDTH/2-5, 1, 10);
	move(HEIGHT/2, WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();

	if (submitRank) {
		rankSubmitScreen(score);
	}
}

int processCommand() {
	int ret=1;

	switch(wgetch(stdscr)) {
		case KEY_UP:
			if (canPlaceBlock(field, queue[0], (rotation + 1) % 4, y, x))
				rotation = (rotation + 1) % 4;
			break;
		case KEY_DOWN:
			blockFall(0);
			break;
		case KEY_LEFT:
			if (canPlaceBlock(field,queue[0],rotation,y,x-1))
				x--;
			break;
		case KEY_RIGHT:
			if (canPlaceBlock(field,queue[0],rotation,y,x+1))
				x++;
			break;
		case ' ':
			y = getMinBlockY(field, queue[0], rotation, y, x);
			blockFall(0); // trash value 0
			break;
		case 'q':
		case 'Q':
			ret = QUIT;
			break;
		default:
			break;
	}

	drawField(field);
	drawShadow(getMinBlockY(field, queue[0], rotation, y, x), x, queue[0], rotation);
	drawCurrentBlock(y, x, queue[0], rotation);

	return ret;	
}

void blockFall(int sig) {
	int drawFlag=0;
	int i, j;
	
	if ((drawFlag = canPlaceBlock(field,queue[0],rotation,y + 1,x))) {
		y ++;
		drawCurrentBlock(y, x, queue[0], rotation);
	} else {
		if (y == -1) {
			gameOver = 1;
		}

		score += dropBlockAndGetScore(field, queue[0], rotation, y, x);

		popBlockQueue();
		resetBlock();

		drawBlockPreview(queue);
		drawScore(score);
		drawField(field);
		drawShadow(getMinBlockY(field, queue[0], rotation, y, x), x, queue[0], rotation);
		drawCurrentBlock(y, x, queue[0], rotation);
	}

	timedOut=0;
}

void resetBlock() {
	rotation=0;
	y= -1;
	x=WIDTH/2-2;
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
		queue[i] = rand() % NUM_OF_SHAPE;
	}
}

void popBlockQueue() {
	for (int i = 0; i < BLOCK_NUM - 1; ++ i) {
		queue[i] = queue[i + 1];
	}

	queue[BLOCK_NUM - 1] = rand() % NUM_OF_SHAPE;
}

/*
 * Modes
 * 0: Normal
 * 1: Recommended
 * 2: Auto
 */