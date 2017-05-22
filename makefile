CC = gcc
CFLAGS = -c -g
LIBS = -lncurses

.SUFFIXES: .c .o

SRCS = tetris.c rankSystem.c rankMenu.c
OBJS = $(SRCS:.c=.o)

TARGET = test

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

clean :
	rm $(OBJS) $(TARGET) rank.txt
	
# Made with help from juice500ml :)
