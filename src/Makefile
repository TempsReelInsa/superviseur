CC=gcc
LINK=g++
CFLAGS=-I../xenomai/include  -I../lib_destijl/headers
OBJ=handle_error.o mutex.o msg_queue.o threads.o global.o main.o
OBJ_SRC=../lib_destijl/source/*.o
LIB= -lopencv_core -lopencv_highgui -lopencv_imgproc -lxenomai -lnative -L../xenomai/lib

all: main


main: $(OBJ)
	$(LINK) $(OBJ) $(OBJ_SRC) -o robot $(LIB)

handle_error.o: handle_error.c handle_error.h debug.h
	$(CC) -g -Wall $(CFLAGS)  -c handle_error.c -o handle_error.o

mutex.o: mutex.c mutex.h 
	$(CC) -g -Wall $(CFLAGS)  -c mutex.c -o mutex.o

threads.o: threads.c dmessage.o threads.h debug.h
	$(CC) -g -Wall $(CFLAGS)  -c threads.c -o threads.o


global.o: global.c global.h
	$(CC) -g -Wall $(CFLAGS) -c global.c -o global.o


main.o: main.c global.h threads.h
	$(CC) -g -Wall $(CFLAGS) -c main.c -o main.o

msg_queue.o: msg_queue.c msg_queue.h
	$(CC) -g -Wall $(CFLAGS) -c msg_queue.c -o msg_queue.o

dmessage.o: ../lib_destijl/source/dmessage.c ../lib_destijl/headers/dmessage.h ../lib_destijl/headers/dtools.h
	cd ../lib_destijl/source; echo "entering source"; \
	$(CC) -g -Wall $(CFLAGS)  -c *.c
	cd ..

clean:
	rm -f $(OBJ) $(OBJ_SRC) robot
