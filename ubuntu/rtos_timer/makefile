# all: RTOSTmr_Task.c
# 	gcc -std=c99 -g -Wall -pthread -o test RTOSTmr_Task.c
# 	# gcc -std=gnu99 -g -Wall -pthread -o test RTOSTmr_Task.c
# FLAGS = -std=c99 -g -Wall -pthread -I
# all: main.c RTOSTmr_Task.c
# 	gcc -o run main.c RTOSTmr_Task.c



CC = gcc
STD = -std=c99
CFLAGS = -g -pthread # -Wall -Werror -Wextra
OUT = run
OBJS = main.o DoublyLinkedList.o

all: $(OBJS)
	$(CC) $(STD) $(CFLAGS) $(OBJS) -o $(OUT)

main.o : main.c
	$(CC) $(STD) $(CFLAGS) -c main.c

DoublyLinkedList.o : DoublyLinkedList.c DoublyLinkedList.h
	$(CC) $(STD) $(CFLAGS) -c DoublyLinkedList.c

clean:
	$(RM) all *.o *~
