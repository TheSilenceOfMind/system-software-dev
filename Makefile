all: server_1 client_1 task_2 server_3 client_3

server_1: server_1.c
	gcc --pedantic -Wall -o server_1 server_1.c

client_1: client_1.c
	gcc --pedantic -Wall -o client_1 client_1.c

task_2: task_2.c
	gcc --pedantic -Wall -o task_2 task_2.c

server_3: server_3.c
	gcc --pedantic -Wall -lnsl -lsocket -lresolv -o server_3 server_3.c

client_3: client_3.c
	gcc --pedantic -Wall -lnsl -lsocket -lresolv -o client_3 client_3.c

clean:
	rm -rf server_1 client_1 task_2 client_3 server_3
	