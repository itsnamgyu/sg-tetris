#include <stdio.h>


void drawField(char field[HEIGHT][WIDTH]);

void drawScore(int score);

void drawBlock(int y, int x, int shapeId, int rotation, char tile, int redraw);

void drawBlockOnField(int y, int x, int shapeId, int rotation, char tile);

void drawBox(int y,int x, int height, int width);

void drawCurrentBlock(int y, int x, int shapeId, int rotation);

void drawShadow(int y, int x, int shapeId, int rotation);

void drawBlockPreview(int queue[BLOCK_NUM]);

void drawOutline();
