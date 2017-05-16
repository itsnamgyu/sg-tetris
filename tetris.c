#include "tetris.h"
#include "string.h"

typedef struct rankNode {
	int score;
	char name[NAMELEN];
	struct rankNode *next; 
} RankNode;

RankNode *rankHead;
int rankCount;

int BLOCK_DISPLAY_HEIGHT = 6;
int BLOCK_DISPLAY_Y = 2;

static struct sigaction act, oact;

void resetBlock();

void resetGame();

void loadBlocks();

void retrieveNextBlock();

void printr(char c);

RankNode *newRankNode(int score, char *name);

void getRankRange();

void getRankByName();

void deleteRankByIndex();

int main() {
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	createRankList();

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

	writeRankFile();

	endwin();
	system("clear");
	return 0;
}

void InitTetris() {
	resetGame();

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawShadow(blockY, blockX, nextBlock[0], blockRotate);
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
			while(CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
				blockY ++;
			}
			BlockDown(0); // trash value 0
			break;
		case KEY_UP:
			if ((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
				blockRotate=(blockRotate+1)%4;
			break;
		case KEY_DOWN:
			if ((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
				blockY++;
			break;
		case KEY_RIGHT:
			if ((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
				blockX++;
			break;
		case KEY_LEFT:
			if ((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
				blockX--;
			break;
		default:
			break;
	}

	if (drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);

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

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile) {
	int i,j;
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			if (block[blockID][blockRotate][i][j]==1 && i+y>=0) {
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
	newRank(score);
}

char menu() {
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

//week 1
int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX) {
	int i,j,ret=1;
	for (i = 0; i < 4; i ++) {
		for (j = 0; j < 4; j ++) {
			int isBlock = block[currentBlock][blockRotate][j][i];
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
void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX) {
	int i,j;
	int blk = currentBlock,rot = blockRotate, y = blockY, x = blockX;
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
	DrawBlock(blockY,blockX,currentBlock,blockRotate,' ');
	DrawShadow(blockY,blockX,currentBlock, blockRotate);
	move(HEIGHT,WIDTH+10);
}
//week 1
void BlockDown(int sig) {
	int drawFlag=0;
	int i, j;
	
	if ((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX))) {
		// Go down
		blockY ++;
		DrawChange(field,KEY_DOWN,nextBlock[0],blockRotate,blockY,blockX);
	} else {
		if (blockY == -1) {
			gameOver = 1;
		}
		// Can't go down

		AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);

		// Calculate extra score
		for (i = 0; i < 4; i ++) {
			for (j = 0; j < 4; j ++) {
				int isBlock = block[nextBlock[0]][blockRotate][j][i];
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
		DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
		DrawShadow(blockY,blockX,nextBlock[0],blockRotate);
	}
	timedOut=0;
}
//week 1
int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX) {
	int i,j;
	int touched = 0;
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			int isBlock = block[currentBlock][blockRotate][j][i];

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

void DrawShadow(int y, int x, int blockID,int blockRotate) {
	while(CheckToMove(field, nextBlock[0], blockRotate, y + 1, x)) {
		y ++;
	}
	
	int i, j; 
	for (i = 0; i < 4; i ++) {
		for (j = 0; j < 4; j ++) {
			if (block[blockID][blockRotate][i][j]==1 && i+y>=0) {
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",'/');
				attroff(A_REVERSE);
			}
		}
	}
}

void createRankList() {
	FILE *file;
	int i;
	char name[NAMELEN];
	int score;
	RankNode *node;
	
	rankHead = newRankNode(0, "");
	rankCount = 0;
	node = rankHead;

	if ((file = fopen("rank.txt", "r"))) {
		fscanf(file, "%d", &rankCount);
		for (i = 0; i < rankCount; ++ i) {
			fscanf(file, "%s %d\n", name, &score);

			node->next = newRankNode(score, name);
			node = node->next;
		}
	}

	fclose(file);
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
	printw("1: list ranks from X to Y\n");
	printw("2: list ranks by a specific name\n");
	printw("3: delete a specific rank\n");

	while (1) {
		switch (getch()) {
			case '1': getRankRange(); getch(); return;
			case '2': getRankByName(); getch(); return;
			case '3': deleteRankByIndex(); getch(); return;
		}
	}
}

void writeRankFile() {
	FILE *file;
	RankNode *node;

	file = fopen("rank.txt", "w");
	
	fprintf(file, "%d\n", rankCount);

	for (node = rankHead->next; node; node = node->next) {
		fprintf(file, "%s\t%d\n", node->name, node->score);
	}
	
	fclose(file);
}

void newRank(int score) {
	char name[NAMELEN];
	RankNode *newNode;
	RankNode *node;
	RankNode *lastNode;

	clear();
	
	move(0, 0);
	printw("Enter your name: ");
	refresh();
	echo();
	scanw("%19s", name);
	noecho();

	newNode = newRankNode(score, name);
	
	node = rankHead->next;
	lastNode = rankHead;
	while (1) {
		if (!node || node->score < score) {
			lastNode->next = newNode;
			newNode->next = node;
			break;
		}

		lastNode = node;
		node = node->next;
	}

	rankCount ++;
}

void DrawRecommend(int y, int x, int blockID,int blockRotate) {
	// user code
}

int recommend(RecNode *root) {
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay() {
	// user code
}

void resetBlock(){
	blockRotate=0;
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

RankNode *newRankNode(int score, char *name){
	RankNode* node = (RankNode*) malloc(sizeof(RankNode));
	node->score = score;
	strcpy(node->name, name);
	node->next = NULL;

	return node;
}

void getRankRange(){
	int range[2];
	int i;
	int printed = 0;

	RankNode *node;
	range[0] = 1;
	range[1] = rankCount;

	echo();
	printw("X: ");
	refresh();
	scanw("%d", range);

	printw("Y: ");
	refresh();
	scanw("%d", range + 1);

	noecho();


	printw(" %-16s| %-8s\n", "name", "score");

	for (i = 0; i < 27; ++ i) {
		printw("-");
	}
	printw("\n");

	node = rankHead->next;
	for (i = 0; i < range[0] - 1; ++ i) {
		if (!node) {
			break;
		}
		node = node->next;
	}

	for (i = 0; i < range[1] - range[0] + 1; ++ i) {
		if (!node) {
			break;
		}
		printed = 1;
		printw(" %-16s| %-8d\n", node->name, node->score);
		node = node->next;
	}

	if (!printed) {
		printw("Search failure: that rank does not exist\n");
	}
}

void getRankByName() {
	char name[NAMELEN];
	int printed = 0;
	int i;
	RankNode *node;

	printw("Input name: ");
	refresh();

	echo();
	scanw("%s", name);
	noecho();
	

	printw(" %-16s| %-8s\n", "name", "score");

	for (i = 0; i < 27; ++ i) {
		printw("-");
	}

	printw("\n");
	for (node = rankHead->next; node; node = node->next) {
		if (!strcmp(name, node->name)) {
			printed = 1;
			printw(" %-16s| %-8d\n", node->name, node->score);

		}
	}

	if (!printed) {
		printw("Search failure: could not find anyone with the name, %s\n", name);
	}
}

void deleteRankByIndex() {
	int index = -1;
	int i;
	RankNode *node;
	RankNode *lastNode;
	RankNode *nextNode;

	printw("Input the rank: ");
	refresh();

	echo();
	scanw("%d", &index);
	noecho();
	index --;
	
	lastNode = rankHead;
	node = rankHead->next;
	if (0 <= index && index < rankCount) {
		for (i = 0; i < index; ++ i) {
			lastNode = node;
			node = node->next;
		}

		nextNode = node->next;
		free(node);
		lastNode->next = nextNode;

		printw("Result: the rank has been deleted\n");
	} else {
		printw("Search failure: the rank is not in the list\n");
	}
}
