#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <wiringPi.h>
int main(int argc, char *argv[]){
FILE *ft;
ft = fopen("sqlTest.txt", "a");
if (ft == NULL){
	printf("Error! opening file\n");
	return -1;
}
int num;


struct tm *timeinfo;
time_t rawtime;
char strResponse[128];
for(num = 0; num < 48; num = num + 1){
delay(1000);
rawtime = time(NULL);
timeinfo = localtime(&rawtime);
strftime(strResponse, 128, "%Y-%m-%d %H:%M:%S", timeinfo);
fprintf(ft,"%s	%d%c", strResponse, num,10);
printf("%s	%d%c", strResponse, num,10);

}
fclose(ft);
return 0;
}
