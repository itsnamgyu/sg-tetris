#include "tetrisUtility.h"
#include "tetrisData.h"

int getMinBlockY(char field[][WIDTH], int shapeId, int rotation, int y, int x) {
	while (canPlaceBlock(field, shapeId, rotation, ++ y, x));
	
	return y - 1;
}

int dropBlockAndGetScore(char field[][WIDTH], int shapeId, int rotation, int y, int x) {
	int i,j,k;
	int lineIsFull;
	int count=0;
	int score = 0;

	// Place blocks and get placement score
	for (i = 0; i < 4; i ++) {
		for (j = 0; j < 4; j ++) {
			int isBlock = block[shapeId][rotation][j][i];

			if (isBlock) {
				field[y + j][x + i] = 1;
				if (y + j == HEIGHT - 1) {
					score += 10;
				}
			}
		}
	}

	// Delete filled lines and get score
	for (j = 1; j < HEIGHT; j ++) {
		lineIsFull = 1;
		for (i = 0; i < WIDTH; i ++) {
			if (!field[j][i]) {
				lineIsFull = 0;
				break;
			}
		}
		if (lineIsFull) {
			count ++;
			for (k = j; k > 0; k --) {
				for (i = 0; i < WIDTH ; i ++) {
					field[k][i] = field[k - 1][i];
				}
			}
		}
	}

	score += 100 * count * count;

	return score;
}

int canPlaceBlock(char field[][WIDTH],int currentBlock,int blockRotation, int blockY, int blockX) {
	int i, j;
	for (i = 0; i < 4; i ++) {
		for (j = 0; j < 4; j ++) {
			int isBlock = block[currentBlock][blockRotation][j][i];
			int isField = field[blockY + j][blockX + i];

			if (isBlock && isField) {
				return 0;
			}

			if (isBlock) {
				int below = (blockY + j == HEIGHT);
				int left = (blockX + i == -1);
				int right = (blockX + i == WIDTH);

				if (below || left || right) {
					return 0;
				}
			}
		}
	}

	return 1;
}


