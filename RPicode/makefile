all: uart
CFLAGS = -lwiringPi -L/usr/lib/mysql -lmysqlclient -o 
CC = gcc

OneTransmission: OneTransmission.c
	$(CC) $(CFLAGS) OneTransmission OneTransmission.c
	sudo ./OneTransmission

uart: uart.c
	$(CC) $(CFLAGS) uart uart.c
	sudo ./uart

sqlConnection: sqlConnection.c
	$(CC) $(CFLAGS) sqlConnection  sqlConnection.c
	sudo ./sqlConnection

textWrite: textWrite.c
	$(CC) $(CFLAGS) textWrite textWrite.c
	sudo ./textWrite
	
FullHandle: FullHandle.c
	$(CC) $(CFLAGS) FullHandle FullHandle.c
	sudo ./FullHandle

clean:
	rm *.o all
	
