CC = gcc
CFLAGS = -c -g
LIBS = -lncurses

.SUFFIXES: .c .o

SRCS = tetris.c rank.c
OBJS = $(SRCS:.c=.o)

TARGET = test

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

clean :
	rm $(OBJS) $(TARGET)
	
# Made with help from juice500ml :)
