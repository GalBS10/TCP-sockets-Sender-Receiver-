all: Sender Receiver

Sender: Sender.o
	gcc -Wall -g -o Sender Sender.o

Receiver: Receiver.o
	gcc -Wall -g -o Receiver Receiver.o

Sender.o: Sender.c
	gcc -Wall -g -c Sender.c

Receiver.o: Receiver.c
	gcc -Wall -g -c Receiver.c

clean:
	rm -f *.o