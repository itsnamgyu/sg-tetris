#include <stdio.h>
#include "rankSystem.h"
#include "rankMenu.h"
#include <ncurses.h>

void showRanksByRangeScreen();

void showRanksByPlayerScreen();

void rankDeleteScreen();

void rankMenu() {
	clear();
	refresh();

	move(0, 0);
	printw("1: List scores from rank a to b\n");
	printw("2: List scores from player\n");
	printw("3: Delete a score\n");

	while (1) {
		switch (getch()) {
			case '1': showRanksByRangeScreen(); getch(); return;
			case '2': showRanksByPlayerScreen(); getch(); return;
			case '3': rankDeleteScreen(); getch(); return;
		}
	}
}

void showRanksByRangeScreen(){
	int range[2];
	int i;
	int printed = 0;
	RankNode *node;
	RankNode *head;

	clear();

	range[0] = -1;
	range[1] = -1;

	echo();
	printw("Enter start ranking: ");
	refresh();
	scanw("%d", range);

	printw("Enter end ranking: ");
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
		printw("No scores from rank %d to %d\n", range[0], range[1]);
	}
	freeRankList(head);
}

void showRanksByPlayerScreen() {
	char name[RANK_NAME_LEN];
	int printed = 0;
	int i;
	RankNode *node;
	RankNode *head;

	clear();

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
		printw("There are no scores from %s\n", name);
	}
	freeRankList(head);
}

void rankDeleteScreen() {
	int index;
	clear();

	printw("Enter which rank to delete: ");

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

void rankSubmitScreen(int score) {
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
