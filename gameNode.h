#include "tetrisData.h"

#ifndef __GAME_NODE_
#define __GAME_NODE_

typedef struct _gameNode { 
	char field[HEIGHT][WIDTH];
	int lastY;
	int lastX;
	int lastShape;
	int lastRotation;
	int score;
	double value;
} GameNode;

#endif
