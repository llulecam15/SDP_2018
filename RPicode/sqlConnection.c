// Sqlconnect.c
// Created by Luz LUle Camargo
//Connect to my sql
// compile "gcc -o sqlConnection sqlconnection.c -L/usr/lib/mysql -lmysqlclient"
//To run "sudo ./sqlConnection"
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

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
	
	/*Creat table */
	/*if (mysql_query(&mysql, "create table networktwo(IP varchar(20), primary key (IP))") != 0){
		
	printf("Failed to create\n");
	return 0;;
	}*/


	if (mysql_query(&mysql, "load data local infile 'NewF.txt' into table gas_meter_one") != 0 ){
		printf("failed to load\n");
		return 0;
	}
	printf("success Loading\n");

	mysql_close(&mysql);
return 1;
}
