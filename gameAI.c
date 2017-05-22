#include "gameNode.h"
#include "tetrisData.h"
#include "tetrisUtility.h"
#include "stdlib.h"

#define BRANCH 	5
#define DEPTH	5
//#define TOTAL	340 // 340
//#define TOTAL	1364 // 340
#define TOTAL	3905// 340

GameNode *dropBlockAndGetNewGameNode(char field[HEIGHT][WIDTH], int score, int shape, int y, int x, int rotation);

double getValue(GameNode *node);

int getNextBubbleCount(GameNode *node);

int getHeight(GameNode *node);

int getMaxTrenchDepth(GameNode *node);

int getTrenchSum(GameNode *node);

int getMaxYDelta(GameNode *node);

int getY(GameNode *node);

GameNode **getNextMoveNodes(char field[HEIGHT][WIDTH], int shape, int count);

GameNode *getNextMoveAsNode(char field[HEIGHT][WIDTH], int* shapes) {
	int generationSize = 1;
	int generation = 0;
	int nodeIndex = 0;
	int valueSum[TOTAL];
	int generationStart = 0;
	GameNode *nodes[TOTAL];
	GameNode **nextNodes;
	GameNode *maxNode = NULL;
	int maxValue;
	int i, j;

	for (i = 0; i < TOTAL; ++ i) {
		valueSum[i] = 0;
		nodes[i] = NULL;
	}
	
	while(generation < DEPTH) {
		for (i = 0; i < generationSize; ++ i) {
			nextNodes = getNextMoveNodes(field, shapes[generation], BRANCH);
			for (j = 0; j < BRANCH; ++ j) {
				if (!nextNodes[j]) {
					break;
				}
				nodes[nodeIndex] = nextNodes[j];
				if (generation) {
					valueSum[nodeIndex] = valueSum[(nodeIndex - generationStart) / 4 + generationStart - generationSize] + nodes[nodeIndex]->value;
				} else {
					valueSum[nodeIndex] = nodes[nodeIndex]->value;
				}
				if (generation == DEPTH - 1) {
					if (!maxNode || maxValue < nodes[nodeIndex]->score) {
						maxValue = nodes[nodeIndex]->score;
						maxNode = nodes[(nodeIndex - generationStart) / generationSize];
					}
				}
				nodeIndex ++;
			}
			free(nextNodes);
		}
		generation ++;
		generationSize *= BRANCH;
		generationStart += generationSize;
	}

	for (i = 0; i < TOTAL; ++ i) {
		if (nodes[i] && nodes[i] != maxNode) {
			free(nodes[i]);
		}
	}
	
	return maxNode;
}

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
	double bubbleWeight = -100;
	double heightWeight = - 100;
	double scoreWeight = 1;
	double yWeight = - 30;
	double maxTrenchDepthWeight = - 50;
	double trenchSumWeight = - 10;
	double maxYDeltaWeight = - 32;
	return 
		getNextBubbleCount(node) * bubbleWeight + 
		node->score * scoreWeight + 
		getMaxTrenchDepth(node) * maxTrenchDepthWeight + 
//		getMaxYDelta(node) * maxYDeltaWeight + 
		getY(node) * yWeight;
		getHeight(node) * heightWeight + 
		getTrenchSum(node) * trenchSumWeight;
	
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

GameNode **getNextMoveNodes(char field[HEIGHT][WIDTH], int shape, int count) {
	int nextNodeCount = 0;
	int rotation;
	int x, y;
	int i, j;
	GameNode *newNode;
	GameNode **nodes = (GameNode**) malloc(sizeof(GameNode*) * count);

	for (i = 0; i < count; ++ i) {
		nodes[i] = NULL;
	}
	
	for (rotation = 0; rotation < blockRotationCount[shape]; ++ rotation) {
		x = 0;
		while (canPlaceBlock(field, shape, rotation, -1, -- x));

		x ++;
		
		for(; canPlaceBlock(field, shape, rotation, -1, x); ++ x) {
			y = getMinBlockY(field, shape, rotation, -1, x);
			newNode = dropBlockAndGetNewGameNode(field, 0, shape, y, x, rotation);
			for (i = 0; i < count; i ++) {
				if (!nodes[i] || nodes[i]->value < newNode->value) {
					free(nodes[count - 1]);
					for (j = count - 1; j > i; j --) {
						nodes[j] = nodes[j - 1];
					}
					nodes[i] = newNode;
					break;
				}
			}
			if (i == count) {
				free(newNode);
			}
		}
	}

	return nodes;
}
