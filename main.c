#include <string.h>
#include "rankSystem.h"
#include "rankMenu.h"
#include "tetrisUtility.h"
#include "tetrisData.h"
#include "game.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameAI.h"

#define COLOR_BLACK   0
#define COLOR_RED     1
#define COLOR_GREEN   2
#define COLOR_YELLOW  3
#define COLOR_BLUE    4
#define COLOR_MAGENTA 5
#define COLOR_CYAN    6
#define COLOR_WHITE   7

#define MENU_PLAY '1'
#define MENU_RANK '2'
#define MENU_RECOMMEND '3'
#define MENU_EXIT '4'

char menu();

int main() {
	int exit = 0;

	srand((unsigned int)time(NULL));

	/*
	int score;
	while (1) {
		// Training weights
		bubbleWeight = - 50 - rand() % 100;
		scoreWeight = (0.75 + rand() % 100 / (double) 200);
		heightWeight = - rand() % 40;
		yWeight = -rand() % 40;
		maxTrenchDepthWeight = - 10 - rand() % 10;
		trenchSumWeight = - 10 - rand() % 10;
		maxYDeltaWeight = - 10 - rand() % 10;
		// Trained weight:
		bubbleWeight = -86;
		scoreWeight = 1.05;
		heightWeight = -6;
		yWeight = - 11;
		maxTrenchDepthWeight = - 10;
		trenchSumWeight = - 16.5;
		maxYDeltaWeight = - 13;
		printf("\t\t\tTrying: %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\n",
				bubbleWeight,
				scoreWeight,
				heightWeight,
				yWeight,
				maxTrenchDepthWeight,
				trenchSumWeight,
				maxYDeltaWeight);
		score = startGame(3);
		if (score > 1000) {
			printf("%8d: ", score);
			printf("%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\n",
					bubbleWeight,
					scoreWeight,
					heightWeight,
					yWeight,
					maxTrenchDepthWeight,
					trenchSumWeight,
					maxYDeltaWeight);
		}
	}
	*/

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	//loadRank();

	// COLOR
	start_color();
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);

	while(!exit) {
		clear();
		switch(menu()) {
			case MENU_PLAY: startGame(2); break;
			case MENU_RANK: rankMenu(); break;
			case MENU_EXIT: exit = 1; break;
			default: break;
		}
	}

	//saveRank();

	endwin();
	system("clear");
	return 0;
}

char menu() {
	printw("1. Manual Play (Press space)\n");
	printw("2. Rank\n");
	printw("4. Exit\n");
	return wgetch(stdscr);
}
