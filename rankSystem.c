#include "rankSystem.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char * const RANK_FILE = "rank.txt";

RankNode *head = NULL;
int rankCount = 0;

// Utility Functions
RankNode *newRankNode(int score, char *name);

RankNode *newRankNode(int score, char *name) {
	RankNode* node = (RankNode*) malloc(sizeof(RankNode));
	node->score = score;
	if (name) {
		strcpy(node->name, name);
	} // Should only be NULL when making pseudo-head (term: Header Node)
	node->next = NULL;

	return node;
}

void loadRank() {
	FILE *file;
	int i;
	char name[RANK_NAME_LEN];
	int score;
	RankNode *node;
	
	head = newRankNode(0, "");
	rankCount = 0;
	node = head;

	if ((file = fopen(RANK_FILE, "r"))) {
		fscanf(file, "%d", &rankCount);
		for (i = 0; i < rankCount; ++ i) {
			fscanf(file, "%s %d\n", name, &score);

			node->next = newRankNode(score, name);
			node = node->next;
		}
	}

	fclose(file);
}

void saveRank() {
	FILE *file;
	RankNode *node;

	file = fopen(RANK_FILE, "w");
	
	fprintf(file, "%d\n", rankCount);

	for (node = head->next; node; node = node->next) {
		fprintf(file, "%s\t%d\n", node->name, node->score);
	}
	
	fclose(file);
}

void newRank(char *name, int score) {
	RankNode *newNode;
	RankNode *node;
	RankNode *lastNode;

	newNode = newRankNode(score, name);
	
	node = head->next;
	lastNode = head;
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

RankNode *getRankListByRange(int startIndex, int endIndex) {
	int i;
	RankNode *node = head->next;
	RankNode *newHead = newRankNode(0, NULL);
	RankNode *newNode;

	if (startIndex == -1) {
		startIndex = 0;
	}

	if (endIndex == -1) {
		endIndex = rankCount - 1;
	}

	if (startIndex <= endIndex && startIndex < rankCount) {
		startIndex = startIndex > 0 ? startIndex : 0;
		endIndex = endIndex < rankCount ? endIndex : rankCount;

		for (i = 0; i < startIndex; ++ i) {
			node = node->next;
		}

		newNode = newHead;
		for (; i <= endIndex; i ++) {
			newNode = newNode->next = newRankNode(node->score, node->name);
			node = node->next;
		}
	}

	return newHead;
}

RankNode *getRankListByName(char *name) {
	int i;
	RankNode *node;
	RankNode *newHead = newRankNode(0, NULL);
	RankNode *newNode;

	newNode = newHead;
	for (node = head->next; node; node = node->next) {
		if (!strcmp(name, node->name)) {
			newNode = newNode->next = newRankNode(node->score, node->name);
		}
	}

	return newHead;
}

int deleteRankAt(int index) {
	int i;

	RankNode *lastNode = head;
	RankNode *node = head->next;
	RankNode *nextNode;

	if (0 <= index && index < rankCount) {
		for (i = 0; i < index; ++ i) {
			lastNode = node;
			node = node->next;
		}

		nextNode = node->next;
		free(node);
		lastNode->next = nextNode;

		rankCount --;

		return 1;
	} else {
		return 0;
	}
}

void freeRankList(RankNode *head) {
	RankNode *node = head->next;
	RankNode *lastNode = head;

	while (node) {
		free(lastNode);

		lastNode = node;
		node = node->next;
	}
	free(lastNode);
}

