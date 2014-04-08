/*****************************LICENSE***********************************\
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
\************************************************************************/

/*************AUTHOR*************\
|   Created By: Patrick Cossette |
|   www.DigitalDiscrepancy.com   |
\********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define  NO 0
#define YES 1

void printHelp(void);

int main (int argc, char **argv){
	char     areaCode[8];
	char       prefix[8];
	char         tmp[4];

	unsigned int listCount = 0;
	char clear[] = "clear"; //Change to "cls" on windows
	
	int   hasAreaCode = NO;
	int        verbose = 0;
	int needsPrintHelp = 0;
	char    *output = NULL;
	char    *config = NULL;
	
	int c;
	
	//Look at any options that have been passed in
	while ((c = getopt (argc, argv, "hvo:c:")) != -1){
         switch (c){
         	case 'h':
         		needsPrintHelp = 1;
         		break;
         		
         	case 'v':
         		verbose = 1;
         		break;
         		
         	case 'o':
         		output = optarg;
         		break;
         		
         	case 'c':
         		config = optarg;
         		break;
         	
         	case '?':
         		if (optopt == 'o'){
               		printf("Option -%c requires an argument.\n", optopt);
               		exit(1);
               	}
               	else if (optopt == 'c'){
               		printf("Option -%c requires an argument.\n", optopt);
               		exit(1);
               	}
             	else if (optopt){
               		printf("Unknown option `-%c'.\n", optopt);
               		exit(1);
               	}
               		
               	needsPrintHelp = 1;
         		break;
         }
    }
	
	if(needsPrintHelp){
		printHelp();
		exit(1);
	}
	
	//Load our area codes and prefixes
	FILE *configFile    = fopen(config ? config : "default.conf", "r");
	FILE *wordlist       = fopen(output ? output : "wordlist.txt", "w");
	
	if (configFile == NULL){
		printf("Error reading numbersFile.conf\n Aborting!");
		return 1;
	}
	
	if(wordlist == NULL){
		printf("Write error! Aborting");
		fclose(configFile);
		return 1;
	}
	
	if(!verbose){
		printf("\nGenerating wordlist - please be patient, this may take a while\n\n");
	}
	
	while(fscanf(configFile, "%s", tmp) != EOF){
		if (!strcmp(tmp, "+")){
			hasAreaCode = YES;
			continue;
		}
		else if(!strcmp(tmp, "end")){
			hasAreaCode = NO;
			continue;
		}
		else if (hasAreaCode){
			if(strlen(tmp) != 3){
				fclose(configFile);
				fclose(wordlist);
				printf("\nMalformed config file - aborting!\n");
				exit(1);
			}
			if(!strcmp(tmp, "all")){
				listCount += 8989101;
				for(int p = 100;p < 1000;p++){
					if (verbose){
						system(clear);
						printf("Writing %s-%03d-xxxx\n", areaCode, p);
					}
					for(int d = 0;d < 10000;d++){
						fprintf(wordlist, "%s%03d%04d\n", areaCode, p, d);
					}
				}
			}
			else{
				if (verbose){
					system(clear);
					printf("Writing %s-%s-xxxx\n", areaCode, tmp);
				}
				
				listCount += 9999;
				for(int d = 0;d < 10000;d++){
					fprintf(wordlist, "%s%s%04d\n", areaCode, tmp, d);
				}
			}
		}
		else{
			if(strlen(tmp) != 3){
				fclose(configFile);
				fclose(wordlist);
				printf("\nMalformed config file - aborting!\n");
				exit(1);
			}
			strcpy(areaCode, tmp);
		}
	}

	//Cleanup
	fclose(configFile);
	fclose(wordlist);
	
	system(clear);
	printf("\n %u words saved to %s\n\n", listCount, output ? output : "wordlist.txt");

   return 0;
}

void printHelp(void){
	printf("\n\tWordListGen by: Patrick Cossette\n\twww.DigtialDiscrepancy.com\n\n\
	Generates phone number wordlists\n\n\
	\t-h\tShow this usage screen\n\
	\t-v\tVerbose output\n\
	\t-o\tSpecify output file (defaults to wordlist.txt)\n\
	\t-c\tSpecify config file (defaults to default.conf)\n\
	\n\
	Edit numbers.conf to specify certain area codes and/or prefixes.\n\nFormat:<areacode> + <space separated prefixes or 'all'> <end>\n(each line contains 1 area code)\n\n");
}