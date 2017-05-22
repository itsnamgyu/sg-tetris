#include "gameNode.h"
#include "tetrisData.h"
#include "tetrisUtility.h"
#include "stdlib.h"

// Utility Functions
GameNode *dropBlockAndGetNewGameNode(char field[HEIGHT][WIDTH], int score, int shape, int y, int x, int rotation);

double getValue(GameNode *node);

int getNextBubbleCount(GameNode *node);

int getHeight(GameNode *node);

int getMaxTrenchDepth(GameNode *node);

int getTrenchSum(GameNode *node);

int getMaxYDelta(GameNode *node);

int getY(GameNode *node);

//
GameNode *getNextMoveAsNode(char field[HEIGHT][WIDTH], int shape) {
	int nextNodeCount = 0;
	int rotation;
	int x, y;
	int i;
	GameNode *maxNode = NULL;
	GameNode *newNode;
	
	for (rotation = 0; rotation < blockRotationCount[shape]; ++ rotation) {
		x = 0;
		while (canPlaceBlock(field, shape, rotation, -1, -- x));

		x ++;
		
		for(; canPlaceBlock(field, shape, rotation, -1, x); ++ x) {
			y = getMinBlockY(field, shape, rotation, -1, x);
			newNode = dropBlockAndGetNewGameNode(field, 0, shape, y, x, rotation);
			if (!maxNode || maxNode->value < newNode->value) {
				if (maxNode) {
					free(maxNode);
				}
				maxNode = newNode;
			} else {
				free(newNode);
			}
		}
	}

	return maxNode;
}

// 
GameNode *dropBlockAndGetNewGameNode(char field[HEIGHT][WIDTH], int score, int shape, int y, int x, int rotation) {
	int i, j;
	GameNode *node = (GameNode*) malloc(sizeof(GameNode));

	for (i = 0; i < HEIGHT; ++ i) {
		for (j = 0; j < WIDTH; ++ j) {
			node->field[i][j] = field[i][j];
		}
	}

	node->lastShape = shape;
	node->lastY = y;
	node->lastX = x;
	node->lastRotation = rotation;
	node->score = score + dropBlockAndGetScore(node->field, shape, rotation, y, x);

	node->value = getValue(node);

	return node;
}

double getValue(GameNode *node) {
	double bubbleWeight = -80;
	double heightWeight = - 20;
	double scoreWeight = 1;
	double yWeight = - 30;
	double maxTrenchDepthWeight = -20;
	double trenchSumWeight = -10;
	double maxYDeltaWeight = -20;
	return 
		getNextBubbleCount(node) * bubbleWeight + 
		getHeight(node) * heightWeight + 
		node->score * scoreWeight + 
		getY(node) * yWeight + 
		getMaxTrenchDepth(node) * maxTrenchDepthWeight + 
		getTrenchSum(node) * trenchSumWeight + 
		getMaxYDelta(node) * maxYDeltaWeight;
	
}

int getNextBubbleCount(GameNode *node) {
	int bubble = 0;
	int i, j;
	int columnIsValid;
	int bubblesInColumn;

	// i is x, j is y!
	for (i = 0; i < 4; ++ i) {
		columnIsValid = 0;
		bubblesInColumn = 0;
		for (j = 0; j < 4; ++ j) {
			if (block[node->lastShape][node->lastRotation][j][i]) {
				columnIsValid = 1;
				bubblesInColumn = 0;
			} else {
				if (node->field[node->lastY + j][node->lastX + i]) {
					if (columnIsValid) {
						break;
					}
				} else {
					bubblesInColumn ++;
				}
			}
		}

		if (columnIsValid) {
			while (!node->field[node->lastY + j][node->lastX + i] && node->lastY + j < HEIGHT) {
				bubblesInColumn ++;
				j ++;
			}
			bubble += bubblesInColumn;
		}
	}

	return bubble;
}

int getHeight(GameNode *node) {
	int i, j;

	for (i = 0; i < HEIGHT; ++ i) {
		for (j = 0; j < WIDTH; ++ j) {
			if (node->field[i][j]) {
				return HEIGHT - i;
			}
		}
	}
	
	return 0;
}

int getY(GameNode *node) {
	int i, j;

	for (i = 0; i < 4; ++ i) {
		for (j = 0; j < 4; ++ j) {
			if (block[node->lastShape][node->lastRotation][j][i]) {
				return HEIGHT - node->lastY - j;
			}
		}
	}
}

int getMaxTrenchDepth(GameNode *node) {
	int columnDepth;
	int maxDepth = 0;
	int i, j;
	int left;
	int right;

	for (i = 0; i < WIDTH; ++ i) {
		columnDepth = 0;
		for (j = 0; j < HEIGHT; ++ j) {
			if (node->field[j][i]) {
				break;
			} else {
				left = (i == 0 || node->field[j][i - 1]);
				right = (i == WIDTH - 1 || node->field[j][i + 1]);

				if (left && right) {
					columnDepth ++;
				}
			}
		}
		if (columnDepth > maxDepth) {
			maxDepth = columnDepth;
		}
	}

	return maxDepth;
}

int getTrenchSum(GameNode *node) {
	int trench = 0;
	int i, j;
	int left;
	int right;

	for (i = 0; i < WIDTH; ++ i) {
		for (j = 0; j < HEIGHT; ++ j) {
			if (node->field[j][i]) {
				break;
			} else {
				left = (i == 0 || node->field[j][i - 1]);
				right = (i == WIDTH - 1 || node->field[j][i + 1]);

				if (left && right) {
					trench ++;
				}
			}
		}
	}

	return trench;
}

int getMaxYDelta(GameNode *node) {
	int columnDepth;
	int maxDepth = 0;
	int minDepth = HEIGHT;
	int i, j;

	for (i = 0; i < WIDTH; ++ i) {
		columnDepth = 0;
		for (j = 0; j < HEIGHT; ++ j) {
			if (node->field[j][i]) {
				break;
			} else {
				columnDepth ++;
			}
		}
		if (columnDepth > maxDepth) {
			maxDepth = columnDepth;
		}
		if (columnDepth < minDepth) {
			minDepth = columnDepth;
		}
	}

	return maxDepth - minDepth;
}
