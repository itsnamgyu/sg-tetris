#include "tetris.h"
#include <string.h>
#include "rankSystem.h"

int BLOCK_DISPLAY_HEIGHT = 6;
int BLOCK_DISPLAY_Y = 2;

static struct sigaction act, oact;

void resetBlock();

void resetGame();

void loadBlocks();

void retrieveNextBlock();

void printr(char c);

void getRankRange();

void getRankByName();

void deleteRankByIndex();

void getNewRank(int score);

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
		case MENU_RANK: rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	saveRank();

	endwin();
	system("clear");
	return 0;
}

void InitTetris() {
	resetGame();

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotation,' ');
	DrawShadow(blockY, blockX, nextBlock[0], blockRotation);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline() {	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(BLOCK_DISPLAY_Y,WIDTH+10);
	printw("NEXT BLOCK");
	for (i = 0; i < BLOCK_NUM - 1; i ++) {
		DrawBox(BLOCK_DISPLAY_Y + i * BLOCK_DISPLAY_HEIGHT + 1,WIDTH+10,4,8);
	}

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(BLOCK_DISPLAY_Y + (BLOCK_NUM - 1) * BLOCK_DISPLAY_HEIGHT + 1,WIDTH+10);
	printw("SCORE");
	DrawBox(BLOCK_DISPLAY_Y + (BLOCK_NUM - 1) * BLOCK_DISPLAY_HEIGHT + 2,WIDTH+10,1,8);
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
		// TODO: fall
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
			while(CheckToMove(field, nextBlock[0], blockRotation, blockY + 1, blockX)) {
				blockY ++;
			}
			BlockDown(0); // trash value 0
			break;
		case KEY_UP:
			if ((drawFlag = CheckToMove(field,nextBlock[0],(blockRotation+1)%4,blockY,blockX)))
				blockRotation=(blockRotation+1)%4;
			break;
		case KEY_DOWN:
			if ((drawFlag = CheckToMove(field,nextBlock[0],blockRotation,blockY+1,blockX)))
				blockY++;
			break;
		case KEY_RIGHT:
			if ((drawFlag = CheckToMove(field,nextBlock[0],blockRotation,blockY,blockX+1)))
				blockX++;
			break;
		case KEY_LEFT:
			if ((drawFlag = CheckToMove(field,nextBlock[0],blockRotation,blockY,blockX-1)))
				blockX--;
			break;
		default:
			break;
	}

	if (drawFlag) DrawChange(field,command,nextBlock[0],blockRotation,blockY,blockX);

	return ret;	
}

void DrawField() {
	int i,j;
	for (j=0;j<HEIGHT;j++) {
		move(j+1,1);
		for (i=0;i<WIDTH;i++) {
			if (field[j][i]) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score) {
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

void DrawBlock(int y, int x, int blockID,int blockRotation,char tile) {
	int i,j;
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			if (block[blockID][blockRotation][i][j]==1 && i+y>=0) {
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}
	}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width) {
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
	InitTetris();
	do{
		if (timedOut==0) {
			alarm(1);
			timedOut=1;
		}

		command = GetCommand();
		if (ProcessCommand(command)==QUIT) {
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	} while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	getNewRank(score);
}

char menu() {
	printw("1. Play\n");
	printw("2. Rank\n");
	printw("3. Recommended Play\n");
	printw("4. Exit\n");
	return wgetch(stdscr);
}

//week 1
int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotation, int blockY, int blockX) {
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
	DrawField();
	DrawBlock(blockY,blockX,currentBlock,blockRotation,' ');
	DrawShadow(blockY,blockX,currentBlock, blockRotation);
	move(HEIGHT,WIDTH+10);
}
//week 1
void BlockDown(int sig) {
	int drawFlag=0;
	int i, j;
	
	if ((drawFlag = CheckToMove(field,nextBlock[0],blockRotation,blockY+1,blockX))) {
		// Go down
		blockY ++;
		DrawChange(field,KEY_DOWN,nextBlock[0],blockRotation,blockY,blockX);
	} else {
		if (blockY == -1) {
			gameOver = 1;
		}
		// Can't go down

		AddBlockToField(field, nextBlock[0], blockRotation, blockY, blockX);

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
		score += DeleteLine(field);

		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
		DrawBlock(blockY,blockX,nextBlock[0],blockRotation,' ');
		DrawShadow(blockY,blockX,nextBlock[0],blockRotation);
	}
	timedOut=0;
}
//week 1
int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotation, int blockY, int blockX) {
	int i,j;
	int touched = 0;
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			int isBlock = block[currentBlock][blockRotation][j][i];

			if (isBlock) {
				f[blockY + j][blockX + i] = 1;
			}
		}
	}
	return 10*touched; // doesn't matter
}

//week 1
int DeleteLine(char f[HEIGHT][WIDTH]) {
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

void DrawShadow(int y, int x, int blockID,int blockRotation) {
	while(CheckToMove(field, nextBlock[0], blockRotation, y + 1, x)) {
		y ++;
	}
	
	int i, j; 
	for (i = 0; i < 4; i ++) {
		for (j = 0; j < 4; j ++) {
			if (block[blockID][blockRotation][i][j]==1 && i+y>=0) {
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",'/');
				attroff(A_REVERSE);
			}
		}
	}
}

void rank() {
	int range[2];
	int index = 0;
	int i;
	int printed = 0;
	RankNode *node;

	clear();
	refresh();

	move(0, 0);
	printw("1: List ranks #a to #b\n");
	printw("2: List ranks from player\n");
	printw("3: Delete a rank\n");

	while (1) {
		switch (getch()) {
			case '1': getRankRange(); getch(); return;
			case '2': getRankByName(); getch(); return;
			case '3': deleteRankByIndex(); getch(); return;
		}
	}
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

void getRankRange(){
	int range[2];
	int i;
	int printed = 0;
	RankNode *node;
	RankNode *head;

	range[0] = -1;
	range[1] = -1;

	echo();
	printw("a: ");
	refresh();
	scanw("%d", range);

	printw("b: ");
	refresh();
	scanw("%d", range + 1);

	noecho();

	head = getRankListByRange(range[0], range[1]);
	if (head->next) {
		printw(" %-16s| %-8s\n", "name", "score");
		for (i = 0; i < 27; ++ i) {
			printw("-");
		}
		printw("\n");

		for (node = head->next; node; node = node->next) {
			printw(" %-16s| %-8d\n", node->name, node->score);
		}
	} else {
		printw("Invalid search range\n");
	}
	freeRankList(head);
}

void getRankByName() {
	char name[RANK_NAME_LEN];
	int printed = 0;
	int i;
	RankNode *node;
	RankNode *head;

	printw("Enter name of player: ");
	refresh();

	echo();
	scanw("%s", name);
	noecho();
	
	head = getRankListByName(name);
	if (head->next) {
		printw(" %-16s| %-8s\n", "name", "score");
		for (i = 0; i < 27; ++ i) {
			printw("-");
		}
		printw("\n");

		for (node = head->next; node; node = node->next) {
			printw(" %-16s| %-8d\n", node->name, node->score);
		}
	} else {
		printw("%s is not a ranker\n", name);
	}
	freeRankList(head);
}

void deleteRankByIndex() {
	int index;
	
	printw("Enter rank #: ");
	refresh();

	echo();
	scanw("%d", &index);
	noecho();
	index --;
	
	if (deleteRankAt(index)) {
		printw("Successfully deleted rank #%d\n", index + 1);
	} else {
		printw("Rank #%d does not exist\n", index + 1);
	}
}

void getNewRank(int score) {
	char name[RANK_NAME_LEN];
	clear();
	
	move(0, 0);
	printw("Enter your name: ");
	refresh();
	echo();
	scanw("%19s", name);
	noecho();

	newRank(name, score);
}

