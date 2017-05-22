#include "tetris.h"
#include <string.h>
#include "rankSystem.h"
#include "rankMenu.h"

int BLOCK_DISPLAY_HEIGHT = 6;
int BLOCK_DISPLAY_Y = 2;

static struct sigaction act, oact;

void resetBlock();

void resetGame();

void loadBlocks();

void retrieveNextBlock();

void printr(char c);

void getNewRank(int score);

int getMinBlockY(char field[HEIGHT][WIDTH], int shapeId, int rotation, int y, int x);

int main() {
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	loadRank();

	srand((unsigned int)time(NULL));

	while(!exit) {
		clear();
		switch(menu()) {
			case MENU_PLAY: play(); break;
			case MENU_RANK: rankMenu(); break;
			case MENU_EXIT: exit = 1; break;
			default: break;
		}
	}

	saveRank();

	endwin();
	system("clear");
	return 0;
}

int GetCommand() {
	int command;

	command = wgetch(stdscr);
	switch(command) {
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;
		case KEY_LEFT:
			break;
		case KEY_RIGHT:
			break;
		case ' ':
			command = FALL;
			break;
		case 'q':
		case 'Q':
			command = QUIT;
			break;
		default:
			command = NOTHING;
			break;
	}

	return command;
}

int ProcessCommand(int command) {
	int ret=1;
	int drawFlag=0;

	switch(command) {
		case QUIT:
			ret = QUIT;
			break;
		case FALL:
			blockY = getMinBlockY(field, nextBlock[0], blockRotation, blockY, blockX);
			BlockDown(0); // trash value 0
			break;
		case KEY_UP:
			if ((drawFlag = canPlaceBlock(field,nextBlock[0],(blockRotation+1)%4,blockY,blockX)))
				blockRotation=(blockRotation+1)%4;
			break;
		case KEY_DOWN:
			if ((drawFlag = canPlaceBlock(field,nextBlock[0],blockRotation,blockY+1,blockX)))
				blockY++;
			break;
		case KEY_RIGHT:
			if ((drawFlag = canPlaceBlock(field,nextBlock[0],blockRotation,blockY,blockX+1)))
				blockX++;
			break;
		case KEY_LEFT:
			if ((drawFlag = canPlaceBlock(field,nextBlock[0],blockRotation,blockY,blockX-1)))
				blockX--;
			break;
		default:
			break;
	}

	if (drawFlag) DrawChange(field,command,nextBlock[0],blockRotation,blockY,blockX);

	return ret;	
}

void redrawField() {
	int i,j;
	for (j=0;j<HEIGHT;j++) {
		move(j+1,1);
		for (i=0;i<WIDTH;i++) {
			if (field[j][i]) {
				printr(' ');
			}
			else printw(".");
		}
	}
}


void redrawScore(int score) {
	move(BLOCK_DISPLAY_Y + (BLOCK_NUM - 1) * BLOCK_DISPLAY_HEIGHT + 3,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock) {
	int i, j;
	int blockIndex;

	for(blockIndex = 0; blockIndex < BLOCK_NUM - 1; blockIndex ++) {
		for (i = 0; i < 4; i ++) {
			move(BLOCK_DISPLAY_Y + 2 + blockIndex * BLOCK_DISPLAY_HEIGHT + i,WIDTH + 13);
			for ( j = 0; j < 4; j ++) {
				if (block[nextBlock[blockIndex + 1]][0][i][j]) {
					attron(A_REVERSE);
					printw(" ");
					attroff(A_REVERSE);
				}
				else printw(" ");
			}
		}
	}
}

void drawBlock(int y, int x, int shapeId, int rotation, char tile) {
	int i,j;
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			if (block[shapeId][rotation][i][j]==1 && i+y>=0) {
				move(i+y,j+x);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}
	}
}

void drawBlockOnField(int y, int x, int shapeId, int rotation, char tile) {
	drawBlock(y + 1, x + 1, shapeId, rotation, tile);
}

void drawBox(int y,int x, int height, int width) {
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for (i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for (j=0;j<height;j++) {
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for (i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play() {
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	resetGame();

	int i,j;

	// Draw field box 
	drawBox(0,0,HEIGHT,WIDTH);

	// Draw preview box
	move(BLOCK_DISPLAY_Y,WIDTH+10);
	printw("NEXT BLOCK");
	for (i = 0; i < BLOCK_NUM - 1; i ++) {
		drawBox(BLOCK_DISPLAY_Y + i * BLOCK_DISPLAY_HEIGHT + 1,WIDTH+10,4,8);
	}

	// Draw score box
	move(BLOCK_DISPLAY_Y + (BLOCK_NUM - 1) * BLOCK_DISPLAY_HEIGHT + 1,WIDTH+10);
	printw("SCORE");
	//drawBox(BLOCK_DISPLAY_Y + (BLOCK_NUM - 1) * BLOCK_DISPLAY_HEIGHT + 2,WIDTH+10,1,8);
	redrawField();
	drawBlockOnField(blockY, blockX, nextBlock[0], blockRotation, ' ');
	redrawShadow();
	DrawNextBlock(nextBlock);
	redrawScore(score);

	do{
		if (timedOut==0) {
			alarm(1);
			timedOut=1;
		}

		command = GetCommand();
		if (ProcessCommand(command)==QUIT) {
			alarm(0);
			drawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	} while(!gameOver);

	alarm(0);
	getch();
	drawBox(HEIGHT/2-1, WIDTH/2-5, 1, 10);
	move(HEIGHT/2, WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	rankSubmitScreen(score);
}

char menu() {
	printw("1. Play\n");
	printw("2. Rank\n");
	printw("3. Recommended Play\n");
	printw("4. Exit\n");
	return wgetch(stdscr);
}

//week 1
int canPlaceBlock(char f[HEIGHT][WIDTH],int currentBlock,int blockRotation, int blockY, int blockX) {
	int i,j,ret=1;
	for (i = 0; i < 4; i ++) {
		for (j = 0; j < 4; j ++) {
			int isBlock = block[currentBlock][blockRotation][j][i];
			int isField = f[blockY + j][blockX + i];

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

//week 1
void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotation, int blockY, int blockX) {
	int i,j;
	int blk = currentBlock,rot = blockRotation, y = blockY, x = blockX;
	int oldShadowY;
	switch(command) {
		case KEY_UP:
			rot=(rot+3)%4;
			break;
		case KEY_DOWN:
			y--;
			break;
		case KEY_LEFT:
			x++;
			break;
		case KEY_RIGHT:
			x--;
			break;
	} 
	// TODO
	redrawField();
	drawBlockOnField(blockY,blockX,currentBlock,blockRotation,' ');
	redrawShadow();
	move(HEIGHT,WIDTH+10);
}
//week 1
void BlockDown(int sig) {
	int drawFlag=0;
	int i, j;
	
	if ((drawFlag = canPlaceBlock(field,nextBlock[0],blockRotation,blockY+1,blockX))) {
		// Go down
		blockY ++;
		DrawChange(field,KEY_DOWN,nextBlock[0],blockRotation,blockY,blockX);
	} else {
		if (blockY == -1) {
			gameOver = 1;
		}
		// Can't go down

		addBlockToField(field, nextBlock[0], blockRotation, blockY, blockX);

		// Calculate extra score
		for (i = 0; i < 4; i ++) {
			for (j = 0; j < 4; j ++) {
				int isBlock = block[nextBlock[0]][blockRotation][j][i];
				int isBottom = (blockY + j == HEIGHT - 1);

				score += (isBlock && isBottom) * 10;
			}
		}

		// Retrieve next block, reinit block state, and update queue
		retrieveNextBlock();
		resetBlock();

		// Delete full lines and update score
		score += deleteLinesAndGetScore(field);

		DrawNextBlock(nextBlock);
		redrawScore(score);
		redrawField();
		drawBlockOnField(blockY,blockX,nextBlock[0],blockRotation,' ');
		redrawShadow();
	}
	timedOut=0;
}
//week 1
int addBlockToField(char field[HEIGHT][WIDTH], int shapeId,int rotation, int y, int x) {
	int i,j;
	int touched = 0;
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			int isBlock = block[shapeId][rotation][j][i];

			if (isBlock) {
				field[y + j][x + i] = 1;
			}
		}
	}
	return 10*touched; // doesn't matter
}

//week 1
int deleteLinesAndGetScore(char f[HEIGHT][WIDTH]) {
	int i,j,k;
	int lineIsFull;
	int count=0;
	for (j=1; j<HEIGHT; j++) {
		lineIsFull = 1;
		for (i=0; i<WIDTH; i++) {
			if (!f[j][i]) {
				lineIsFull = 0;
				break;
			}
		}
		if (lineIsFull) {
			count ++;
			for (k=j;k>0;k--) {
				for (i=0;i<WIDTH;i++) {
					f[k][i] = f[k - 1][i];
				}
			}
		}
	}
	return 100 * count * count;
}

void redrawShadow() {
	drawBlockOnField(getMinBlockY(field, nextBlock[0], blockRotation, blockY, blockX),
			blockX, nextBlock[0], blockRotation, '/');
}

void resetBlock(){
	blockRotation=0;
	blockY=-1;
	blockX=WIDTH/2-2;
}

void resetGame(){
	int i,j;

	for (j=0;j<HEIGHT;j++)
		for (i=0;i<WIDTH;i++)
			field[j][i]=0;

	score=0;	
	gameOver=0;
	timedOut=0;

	resetBlock();
	loadBlocks();
}

void loadBlocks(){
	for (int i = 0; i < BLOCK_NUM; ++ i) {
		nextBlock[i] = rand() % NUM_OF_SHAPE;
	}
}

void retrieveNextBlock(){
	for (int i = 0; i < BLOCK_NUM - 1; ++ i) {
		nextBlock[i] = nextBlock[i + 1];
	}

	nextBlock[BLOCK_NUM - 1] = rand() % NUM_OF_SHAPE;
}

void printr(char c){
	attron(A_REVERSE);
	printw("%c", c);
	attroff(A_REVERSE);
}


int getMinBlockY(char field[HEIGHT][WIDTH], int shapeId, int rotation, int y, int x){
	while (canPlaceBlock(field, shapeId, rotation, ++ y, x));
	
	return y - 1;
}
