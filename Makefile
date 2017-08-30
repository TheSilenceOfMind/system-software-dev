all: server_1 client_1 task_2

server_1: server_1.c
	gcc --pedantic -Wall -o server_1 server_1.c

client_1: client_1.c
	gcc --pedantic -Wall -o client_1 client_1.c

task_2: task_2.c
	gcc --pedantic -Wall -o task_2 task_2.c

clean:
	rm -rf server_1 client_1 task_2
	