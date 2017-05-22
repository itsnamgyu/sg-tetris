#include "tetrisData.h"

int getMinBlockY(char field[HEIGHT][WIDTH], int shapeId, int rotation, int y, int x);

/*
 * Add's current block to field and calculate extra score
 * from floor bonus
 */
int addBlockToFieldAndGetScore(char field[HEIGHT][WIDTH], int shapeId, int rotation, int y, int x);

/*
 * Delete full lines and calculate extra score
 */
int deleteLinesAndGetScore(char f[HEIGHT][WIDTH]);

/*
 * Return whether a block can be placed on the field
 */
int canPlaceBlock(char field[HEIGHT][WIDTH], int shape, int rotation, int y, int x);
