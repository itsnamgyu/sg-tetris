#include "tetrisData.h"
#include "tetrisUtility.h"
#include "gameUI.h"
#include "game.h"
#include "rankMenu.h"
#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>
#include "gameNode.h"
#include "gameAI.h"

char field[HEIGHT][WIDTH];
int queue[BLOCK_NUM];
int rotation;
int y;
int x;

int score;
int gameOver = 0;
int timedOut;
int gameMode;

GameNode *nextMoveNode;

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
/*
 * Modes
 * 0: Normal
 * 1: Recommended
 * 2: Auto
 * 3: Train
 */
int startGame(int mode) {
	gameMode = mode;

	if (gameMode == 3) {
		resetGame();
		initBlockQueue();
		while ((nextMoveNode = getNextMoveAsNode(field, queue[0]))) {
			x = nextMoveNode->lastX;
			y = nextMoveNode->lastY;
			rotation = nextMoveNode->lastRotation;
			score += dropBlockAndGetScore(field, queue[0], rotation, y, x);
			popBlockQueue();
		}

		return score;
	}

	clear();
	act.sa_handler = blockFall;
	sigaction(SIGALRM,&act,&oact);
	resetGame();

	int submitRank = 1;
	int i,j;

	drawField(field);
	if (gameMode) {
		nextMoveNode = getNextMoveAsNode(field, queue[0]);
		if (gameMode == 1) {
			drawRecommendation(nextMoveNode);
			x = nextMoveNode->lastX;
			rotation = nextMoveNode->lastRotation;
		} else if (gameMode == 2) {
			x = nextMoveNode->lastX;
			y = nextMoveNode->lastY;
			rotation = nextMoveNode->lastRotation;
			blockFall(0);
		}
	}
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

	if (submitRank && gameMode != 2) {
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
	if (gameMode) {
		if (nextMoveNode) {
			drawRecommendation(nextMoveNode);
		}
	}
	drawShadow(getMinBlockY(field, queue[0], rotation, y, x), x, queue[0], rotation);
	drawCurrentBlock(y, x, queue[0], rotation);

	return ret;	
}

void blockFall(int sig) {
	int drawFlag=0;
	int i, j;
	
	if ((drawFlag = canPlaceBlock(field,queue[0],rotation,y + 1,x))) {
		y ++;
		if (gameMode) {
			if (nextMoveNode) {
				drawRecommendation(nextMoveNode);
			}
		}
		drawShadow(getMinBlockY(field, queue[0], rotation, y, x), x, queue[0], rotation);
		drawCurrentBlock(y, x, queue[0], rotation);
	} else {
		if (y == -1) {
			gameOver = 1;
		}

		score += dropBlockAndGetScore(field, queue[0], rotation, y, x);

		popBlockQueue();
		resetBlock();

		if (gameMode) {
			nextMoveNode = getNextMoveAsNode(field, queue[0]);
			if (nextMoveNode) {
				move(30, 30);
				printw("Value: %lf", nextMoveNode->value);
			} else {
				gameOver = 1;
			}
		}

		drawBlockPreview(queue);
		drawScore(score);
		drawField(field);

		if (gameMode == 1) {
			if (nextMoveNode) {
				drawRecommendation(nextMoveNode);
				x = nextMoveNode->lastX;
				rotation = nextMoveNode->lastRotation;
			}
		}

		drawShadow(getMinBlockY(field, queue[0], rotation, y, x), x, queue[0], rotation);
		drawCurrentBlock(y, x, queue[0], rotation);

		if (gameMode == 2) {
			if (nextMoveNode) {
				x = nextMoveNode->lastX;
				y = nextMoveNode->lastY;
				rotation = nextMoveNode->lastRotation;
				blockFall(0);
			}
		}
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

