// Sqlconnect.c
// Created by Luz LUle Camargo
//Connect to my sql
// compile "gcc -o sqlConnection sqlconnection.c -L/usr/lib/mysql -lmysqlclient"
//To run "sudo ./sqlConnection"
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

int main(int argc, char *argv[]){


	/*Creat table */
	/*if (mysql_query(&mysql, "create table networktwo(IP varchar(20), primary key (IP))") != 0){
		
	printf("Failed to create\n");
	return 0;;
	}*/


  //Variables Needed for time stamp. 
  struct tm *timeinfo;
  time_t rawtime;
  char strResponse[128];
  int id;
  long int gData;
  
		while ( id < 2){
			
		MYSQL mysql;
		mysql_init(&mysql);
		if (!mysql_real_connect(&mysql,  // Struct 
					"128.114.9.90", //Host or IP address
						"sdpteam",  //user
					"goUCSC1234",  //Password
					"sdp", //database
						3306, //port
					NULL, //
						0)){
		fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
		return 0;
		}
	
  
	      rawtime = time(NULL);
          timeinfo = localtime(&rawtime);
		  strftime(strResponse, 128, "%Y-%m-%d %H:%M:%S", timeinfo);
		  
		  
		  if (id == 0){
			if (mysql_query(&mysql, "INSERT INTO student_union_building(TimeStamp, Gas_Meter_One) VALUES('2018-03-04 14:23:30' , 123)") != 0 ){
				printf("failed to load_ Here\n");
				return 0;
			}
			printf("G30503 %s	%ld%c", strResponse, gData, 10);
		}else {
			 if (mysql_query(&mysql, "INSERT INTO student_union_building(TimeStamp, Gas_Meter_Two) VALUES('2018-03-04 14:23:30' , 687)") != 0 ){
				printf("failed to load\n");
				return 0;
			 }
			 printf("G21523 %s	%ld%c", strResponse, gData, 10);
		 }
		 
	
     id++;
	mysql_close(&mysql); 
    }
return 1;
}
