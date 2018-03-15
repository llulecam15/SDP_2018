//*****************************************************************************
// Program recieves bytes and fills up a buffer. 
// To compile "gcc -o uart uart.c -lwiringPi -L/usr/lib/mysql -lmysqlclient"
// to run "sudo ./uart"
// Luz Lule Camargo Feb 17 2018
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <errno.h>
#include <time.h>
#include <mysql/mysql.h>

#define BAUDRATE B9600 // UART speed

int main (int argc, char * argv[]){
	
	//Set up the pmw
	wiringPiSetup();
	pinMode(1, PWM_OUTPUT);

  struct termios serial; // Structure to contain UART parameters

  char* dev_id = "/dev/serial0"; // UART device identifier
  //char* str = "Hello World"; // Data to be transmitted
  char rxbuffer; // Receive data buffer
 



  int period;

  printf("Opening %s\n", dev_id);
  int fd = open(dev_id, O_RDWR | O_NOCTTY | O_NDELAY);
  // O_RDWR indicates the device needs to both written and read.
  // O_NOCTTY states that the device is not used as a console.
  // O_NDELAY indicates that read and writes should be nonblocking.

  if (fd == -1){ // Open failed
    perror(dev_id);
    return -1;
  }

  // Get UART configuration
  if (tcgetattr(fd, &serial) < 0){
    perror("Getting configuration");
    return -1;
  }

  // Set UART parameters in the termios structure
  serial.c_iflag = 0;
  serial.c_oflag = 0;
  serial.c_lflag = 0;
  serial.c_cflag = BAUDRATE | CS8 | CREAD | PARENB | PARODD;
// Speed setting + 8-bit data + Enable RX + Enable Parity + Odd Parity
  
  serial.c_cc[VMIN] = 0; // 0 for Nonblocking mode
  serial.c_cc[VTIME] = 0; // 0 for Nonblocking mode
  
  // Set the parameters by writing the configuration
  tcsetattr(fd, TCSANOW, &serial);
  //Variables Needed for time stamp. 
  struct tm *timeinfo;
  time_t rawtime;
  char strResponse[128];
  //Setting up the txt file for writing. 
  FILE *ft;
  //DATA BASE CONNECTION CODE 
  char mbuff[10];
  char IDbuff[7];
  int mIndex = 0;
  int IDindex = 0;
  int Bflag = 0;
  int Eflag = 0;
  int IDflag = 0;
  long int gData;
	
	

  
  while(1){



	  //Receiving Data from the UART
	  
	  while( read(fd, &rxbuffer, 1) > 0){
		  //printf("Byte On Buff %c\n", rxbuffer);
		  //Begin message. 
		  if (rxbuffer == '<'){
			  
			  //Set flags
			  Bflag = 1;
			  IDflag = 1;
			  IDindex = 0;
			  IDbuff[IDindex] = '\0';
			  
             
		  } else if (rxbuffer ==  '>'){ //END OF MESSAGE
			 Eflag = 1;
			 break; 

		  } else if((rxbuffer == '/') && IDflag){ //end of ID
			  IDflag = 0; 
			  IDbuff[IDindex] = '\0';
		      //Init & clear index and buff message.   
			  mIndex = 0; 
			  mbuff[mIndex] = '\0';
			  
		  } else if (Bflag && !IDflag){ 
			  //Receiving message. 
			  if (mIndex < 11){
				  mbuff[mIndex] = rxbuffer;
				  mIndex++;
				  mbuff[mIndex] = '\0';
			  }
		  } else if (Bflag && IDflag){ //Receiving ID. 
			  if (IDindex < 8){
				  IDbuff[IDindex] = rxbuffer;
				  IDindex++;
				  IDbuff[IDindex] = '\0';
			  }
		  }

	  }

	  if (Bflag && Eflag){
		  //DATABASE CONNECT TO COde  
			MYSQL mysql;
  
			mysql_init(&mysql);
			if (!mysql_real_connect(&mysql,
						"128.114.9.90",
						"sdpteam",
						"goUCSC1234",
						"sdp",
						3306,
						NULL,
						0)){
				fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
				return 0;
				}	
				
		  gData = atol(mbuff);
	      rawtime = time(NULL);
		  timeinfo = localtime(&rawtime);
		  strftime(strResponse, 128, "%Y-%m-%d %H:%M:%S", timeinfo);
		  ft = fopen ("FullOneG.txt", "w");
	      if (ft == NULL){
			printf("Error! Opening file\n");
			return -1;
	      }
		  fprintf(ft, "%s	%ld%c", strResponse, gData, 10);
		  
		  fclose(ft);
		  mIndex = 0;
		  mbuff[mIndex] = '\0';
		  Bflag = 0;
		  Eflag= 0;
		  long int id;
		  id = atol(IDbuff);
		  if (id == 30503){
			if (mysql_query(&mysql, "load data local infile 'FullOneG.txt' into table meter_30503") != 0 ){
				printf("failed to load\n");
				return 0;
			}
			printf("G%ld %s	%ld%c", id, strResponse, gData, 10);
		}else {
			 if (mysql_query(&mysql, "load data local infile 'FullOneG.txt' into table meter_21523") != 0 ){
				printf("failed to load\n");
				return 0;
			 }
			 printf("G%ld %s	%ld%c", id, strResponse, gData, 10);
		 }
		 
	
		 mysql_close(&mysql);   
	  }

	       
	 /*   ft = fopen ("uartTest.txt", "a+");
	      if (ft == NULL){
		  printf("Error! Opening file\n");
		  return -1;
	  }
	  rawtime = time(NULL);
	  timeinfo = localtime(&rawtime);
	  strftime(strResponse, 128, "%Y-%m-%d %H:%M:%S", timeinfo);
	  fprintf(ft, "%s	%s%c", strResponse, rxbuffer, 10);
	  printf("%s	%s%c", strResponse, rxbuffer, 10);
	  fclose(ft);*/
	  




  // mysql_close(&mysql);	        
  }
 
  close(fd);
}
