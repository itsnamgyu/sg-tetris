#include "gameNode.h"

extern double bubbleWeight;
extern double scoreWeight;
extern double heightWeight;
extern double yWeight;
extern double maxTrenchDepthWeight;
extern double trenchSumWeight;
extern double maxYDeltaWeight;

GameNode *getNextMoveAsNode(char field[HEIGHT][WIDTH],int *shapes);
