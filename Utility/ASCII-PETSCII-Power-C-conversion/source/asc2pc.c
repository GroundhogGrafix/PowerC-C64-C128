/* Author: Martin Sigley  3/15/2021 Compiled with GCC.

Purpose: This program will convert ASCII source code to PETSCII Power C source code.

Disclaimer:
This has not been fully tested yet and errors are likely. If you find an incorrect character in the output file, you can correct it by locating the entry in the pc2asc array below, use the remark statements to locate the character in question. If I find errors or errors are reported to me I will apply the fixes in updates to this file.

ALL CONVERTED FILES BEING IMPORTED IN TO DIRMASTER MUST BE APPENDED WITH THE EXTENSION .SEQ!

Usage info: Drag and drop a c source file on to this program from windows and it will auto convert or you can type the name of the source file on the command line when running the program. Alternatively, you can also just run the program and enter in both source and destination file names at the prompts. Wild cards are NOT supported at this time. All source files will be appended with .seq, which is the naming convention used by the program Dirmaster when files are extracted from disk images, this applies only to names entered on the commandline, not by the prompts. If you use the prompts you can use any file names you like for both source and destination files. Full details and errors can be found in the asc2pc.log which will be located in the directory where the converted file resides if you use the drag and drop method of execution.

*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 100
#define BUFSIZE 65536

/* 
Conversion array, for C source files to Power C PETSCII files. This array is based off of Scancode set 2, upper and lower case letters not the default upper case only letters found in Scancode set 1.
*/

char asc2pc[] =
{     
/*          0     1     2     3     4  */
/*        ---------------------------- */
/*   0 */   0,    0,    0,    0,    0,
/*   5 */   0,    0,    0,    0,   32,
/*  10 */  13,   13,    0,   13,    0,
/*  15 */   0,    0,    0,    0,    0,
/*  20 */   0,    0,    0,    0,    0,
/*  25 */   0,    0,    0,    0,    0,
/*  30 */   0,    0,   32,   33,   34,
/*  35 */  35,   36,   37,   38,   39,
/*  40 */  40,   41,   42,   43,   44,
/*  45 */  45,   46,   47,   48,   49,
/*  50 */  50,   51,   52,   53,   54,
/*  55 */  55,   56,   57,   58,   59,
/*  60 */  60,   61,   62,   63,   64,
/*  65 */ 193,  194,  195,  196,  197,
/*  70 */ 198,  199,  200,  201,  202,
/*  75 */ 203,  204,  205,  206,  207,
/*  80 */ 208,  209,  210,  211,  212,
/*  85 */ 213,  214,  215,  216,  217,
/*  90 */ 218,   91,   92,   93,   94,
/*  95 */ 164,    0,   65,   66,   67,
/* 100 */  68,   69,   70,   71,   72,
/* 105 */  73,   74,   75,   76,   77,
/* 110 */  78,   79,   80,   81,   82,
/* 115 */  83,   84,   85,   86,   87,
/* 120 */  88,   89,   90,  219,  223,
/* 125 */ 221,  175,    0
};


void convert(char *ifname, char *ofname);
void quit( void );

FILE *lf;

int main( int argc, char **argv)
{
	char ifname[MAX_NAME_LEN], ofname[MAX_NAME_LEN], kbi;
	
	puts("");

    lf = fopen("asc2pc.log", "w");
	
    while(--argc)
	{
		strcpy(ifname, argv[argc]);
		strcpy(ofname, argv[argc]);
		strcat(ofname, ".seq");
		
		fprintf(lf, "\nProcessing %s...\n", ifname);
		convert(ifname, ofname);
		fprintf(lf, "\n%s completed successfully!\nOutput file name: %s\n", ifname, ofname);
		
		if(1 == argc)
			quit();
	}
	
	puts("Enter source file name:");
	fgets(ifname, MAX_NAME_LEN - 1, stdin);
	puts("Enter destination file name.");
	fgets(ofname, MAX_NAME_LEN - 1, stdin);

	/* Delete the RETURN character 13 from string */
	ifname[strlen(ifname)-1] = '\0';
	ofname[strlen(ofname)-1] = '\0';
	
	convert(ifname, ofname);
	
	fprintf(lf, "\n%s completed successfully!\nOutput file name: %s\n", ifname, ofname);
	
	quit();
}

void convert(char *ifname, char *ofname)
{
    FILE *fp;
    size_t bytesread;
    static char overwrite = 1, kbi;
    unsigned char data[BUFSIZE];
	
	if(overwrite)
	{
		if(NULL != (fp = fopen(ofname, "r")))
		{
			fclose(fp);
			printf("Output file name: %s\nAlready exists. Overwrite (Yes/No/All) ", ofname);
			kbi = toupper(getch());
			
			switch(kbi)
			{
				case 'A':
					overwrite = 0;
					break;
				case 'Y':
					break;
				default:
					return;
					break;
			}
		}
	}		

	if(NULL == (fp = fopen(ifname, "r")))
	{
		fprintf(lf, "%s: Failed to open source file.\n", ifname);
		return;
	}
	
	if(1 > (bytesread = fread(data, sizeof(char), BUFSIZE, fp)))
	{
		fprintf(lf, "%s: Failed to read source file.", ifname);
		return;
	}

	fclose(fp);
	
	/* Convert PETSCII to ASCII */
	for(int a = 0; a < bytesread; a++)
	{
		if(data[a] < 128)
		{
			putchar(data[a]);
			fputc(data[a], lf);
			data[a] = asc2pc[data[a]];
		}
		else
		{
			data[a] = 0;
		}			
	}

	if(NULL == (fp = fopen(ofname, "w")))
	{
		fprintf(lf, "%s: Error opening file for write.", ofname);
		return;
	}

	if(fwrite(data, sizeof(char), bytesread, fp) < 1)
		fprintf(lf, "%s: Failed to write to destination file.", ofname);
	
	fclose(fp);
}

void quit( void )
{
	fclose(lf);
	exit(0);
}