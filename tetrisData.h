#ifndef _TETRIS_DATA_
#define _TETRIS_DATA_

#define WIDTH	20
#define HEIGHT	22
#define NUM_OF_SHAPE	7
#define NUM_OF_ROTATE	4
#define BLOCK_HEIGHT	4
#define BLOCK_WIDTH	4
#define BLOCK_NUM	20

static const int blockRotationCount[NUM_OF_SHAPE] = {
	/*
	2,
	4, 4, 4, 
	1,
	2, 2
	*/
	4,4,4,4,4,4,4
};

static const char block[NUM_OF_SHAPE][NUM_OF_ROTATE][BLOCK_HEIGHT][BLOCK_WIDTH] ={
	{/*[0][][][]					▩▩▩▩*/
		{/*[][0][][]*/
			{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}			
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}			
		},
		{/*[][3][][]*/
			{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}
		}
	},
	{/*[1][][][];					  ▩▩▩*/
		{/*[][0][][]				      ▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0 ,1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}
		}
	},
	{/*[2][][][];					  ▩▩▩*/
		{/*[][0][][]				  ▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}
		}
	},
	{/*[3][][][];					  ▩▩▩*/
		{/*[][0][][]				    ▩*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		}
	},
	{/*[4][][][];					  ▩▩*/
		{/*[][0][][]				  ▩▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		}
	},
	{/*[5][][][];					  ▩▩*/
		{/*[][0][][]				▩▩*/
			{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}			
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}			
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}			
		}
	},
	{/*[6][][][];					▩▩*/
		{/*[][0][][]				  ▩▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		}
	}
};

#endif
