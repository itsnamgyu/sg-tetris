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

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	loadRank();

	srand((unsigned int)time(NULL));

	// COLOR
	start_color();
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);

	while(!exit) {
		clear();
		switch(menu()) {
			case MENU_PLAY: startGame(1); break;
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

char menu() {
	printw("1. Play\n");
	printw("2. Rank\n");
	printw("3. Recommended Play\n");
	printw("4. Exit\n");
	return wgetch(stdscr);
}
