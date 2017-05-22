#define RANK_NAME_LEN 16

typedef struct rankNode {
	int score;
	char name[RANK_NAME_LEN];
	struct rankNode *next; 
} RankNode;

/*
 *	Read RANK_FILE and get rank info (save to rankHead as linked list)
 */
void loadRank();


/*
 *	Write rank info to RANK_FILE
 */
void saveRank();


/*
 *	Add new rank (to rankHead as linked list)
 *
 * 	Parameters
 * 	char *name	Name of player
 * 	int score	Score of player
 *
 */
void newRank(char *name, int score);


/*	
 *	Get list of ranks in range. Return null if doesn't exist.
 *
 */
RankNode *getRankListByRange(int startIndex, int endIndex);


/*	
 *	Get list of ranks (linked-list) from player 'name'. Returns head.
 *
 */
RankNode *getRankListByName(char *name);


/*	
 *	Delete rank at index. Returns success.
 *
 */
int deleteRankAt(int index);


/*	
 *	Delete list of ranks (linked-list).
 *
 */
void freeRankList(RankNode *head);
