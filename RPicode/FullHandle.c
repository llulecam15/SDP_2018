//*****************************************************************************
//File name: FullHandle. 
// Program to process gas meter data and upload to MySQL
//March 10, 2018. Code Modified to write to txt instead of uplaoding. 
//Format of incoming messages <id0 id1 id2 id3 id4 / d0 d1 d2 d3>
// To compile "gcc -o FullHandle FullHandle.c -lwiringPi -L/usr/lib/mysql -lmysqlclient"
// to run "sudo ./FullHandle"
// Luz Lule Camargo March 4, 2018
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
		  gData = atol(mbuff);
	      rawtime = time(NULL);
		  timeinfo = localtime(&rawtime);
		  strftime(strResponse, 128, "%Y-%m-%d %H:%M:%S", timeinfo);
		  long int id;
		  id = atol(IDbuff);
		  if (id == 11111){
			  ft = fopen ("SUB_G1.txt", "a");
			  if (ft == NULL){
				printf("Error! Opening file\n");
				return -1;
			  }
			  fprintf(ft, "%s	%ld%c", strResponse, gData, 10);
			  fclose(ft);
			  printf("G%ld %s	%ld%c", id, strResponse, gData, 10);			  

		}else {
			 ft = fopen ("SUB_G2.txt", "w+");
			  if (ft == NULL){
				printf("Error! Opening file\n");
				return -1;
			  }
			  fprintf(ft, "%s	%ld%c", strResponse, gData, 10);
			  fclose(ft);		  

			 printf("G%ld %s	%ld%c", id, strResponse, gData, 10);
		 }
		 
		  mIndex = 0;
		  mbuff[mIndex] = '\0';
		  Bflag = 0;
		  Eflag= 0;
	  }
        
  }
 
  close(fd);
}
