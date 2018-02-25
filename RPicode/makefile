all: uart

CFLAGS = -lwiringPi -L/usr/lib/mysql -lmysqlclient -o 
CC = gcc

uart: uart.c
	$(CC) $(CFLAGS) uart uart.c
	sudo ./uart

sqlConnection: sqlConnection.c
	$(CC) $(CFLAGS) sqlConnection  sqlConnection.c
	sudo ./sqlConnection

textWrite: textWrite.c
	$(CC) $(CFLAGS) textWrite textWrite.c
	sudo ./textWrite

clean:
	rm *.o all
	
