#include "tetrisData.h"

int getMinBlockY(char field[][WIDTH], int shapeId, int rotation, int y, int x);
 
int dropBlockAndGetScore(char field[][WIDTH], int shapeId, int rotation, int y, int x);

/*
 * Return whether a block can be placed on the field
 */
int canPlaceBlock(char field[][WIDTH], int shape, int rotation, int y, int x);
