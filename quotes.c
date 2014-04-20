/*
Scripts by jddantes

Quotes.

For having a randomized quote every time you open your terminal. 
(Put this and the file named "quotes" in the same folder as your .bashrc/configuration file.)
Also works nicely with the package cowsay:
	./a.out | cowsay -f turtle
	
USAGE

	Reads quotes from a file named "quotes" in the format
		<quote number> [s#]Quote here.[e#]
	For example
		1 [s#]Rome wasn't built in a day.[e#];

	Max quote size (number of characters) is defined to be 5000.

	A second command-line argument can be specified to indicate which quote number to print.
	For example, if you want the first quote, you'd run it as (assuming the executable is a.out)
		./a.out 1

LOG

v1 
	No randomization yet.

v2 
	Randomization added.

v3
	Quote length from file checked.

v4
	Added more usage of command-line arguments.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define quote_start "[s#]"
#define quote_end "[e#]"
#define quote_start_size 4
#define quote_end_size 4

#define quote_max_size 5000

#define help_message printf("quotes: Usage: %s <option> <parameter>\n",argv[0]);\
		printf("Where option is one of the following:\n");\
		printf("-f <filename> Followed by the file to open.\n");\
		printf("-q <quote number> Followed by the quote number. Indices start at either 1 or -1. If the magnitude exceeds the number of quotes, the modulo value is used.\n");\
		return 0;

void strapp(char * dest, char * src);

int main(int argc, char ** argv)
{

	int i;
	int n = 0;
	int unordered = 0;

	/*
		Command-line arguments.
	*/
	int _f = 0;
	int _q = 0;

	if(argc != 1 && argc !=3)
	{
		help_message
	}


	if(argc == 3)
	{
		if(!strcmp(argv[1], "-f"))
		{
			_f = 1;
		}
		else if (!strcmp(argv[1], "-q"))
		{
			_q = 1;
		}
		else
		{
			printf("quotes: Unrecognized option '%s'\n",argv[1]);
			help_message
		}
	}


	FILE * fp; 

	if(_f)
		fp = fopen(argv[2],"r");
	else
		fp = fopen("quotes","r");
	


	if(!fp)
	{
		printf("Error. Could not open file.\n");
		return 0;
	}

	char ** quotes_list = 0;

	int num_quotes = 0;
	int quote_id = 0;
	while(fscanf(fp,"%d",&quote_id)!=EOF)
	{

		if(quote_id -1 != num_quotes )
		{
			unordered = num_quotes;

			//printf("%s: Quote %d out of order. Previous quote is numbered %d.\n",argv[0],num_quotes);
			//return 0;
		}

		num_quotes++;
		quotes_list = (char **)realloc(quotes_list, sizeof(char *) * num_quotes);
		quotes_list[num_quotes-1] = (char *)malloc(sizeof(char) * quote_max_size + 1);
		quotes_list[num_quotes-1][0] = 0;

		char c;

		/* 
			Scan until quote_start
		*/

		int quote_start_matched = 0;

		while(fscanf(fp,"%c",&c)==1)
		{
			if(c==quote_start[quote_start_matched])
			{	
				quote_start_matched++;
				if(quote_start_matched == quote_start_size)
					break;
			}
			else
			{
				quote_start_matched = 0;
			}
		}

		/*
			Scan quote_start END
		*/

		/*
			Scan and print (or store into quotes_list) until quote_end
		*/

		int quote_end_matched = 0;
		char buffer[quote_end_size+1];
		buffer[0] = 0;

		char char_buffer[2]="";

		while(fscanf(fp,"%c",&c)==1)
		{

			if(c==quote_end[quote_end_matched])
			{
				buffer[quote_end_matched] = c;
				buffer[quote_end_matched + 1] = 0;

				quote_end_matched++;

				if(quote_end_matched == quote_end_size)
				{
					buffer[0] = 0;
					break;
				}
			}
			else
			{
				quote_end_matched = 0;

				if(strlen(buffer))
				{
					strapp(quotes_list[num_quotes-1],buffer);
					buffer[0] = 0;
				}
				
				
				char_buffer[0]=c;
				char_buffer[1]=0;

				strapp(quotes_list[num_quotes-1],char_buffer);
				
			}
		}

		/*
			Scan and print until quote_end END
		*/
	}

	fclose(fp);

	//Print quotes_list
	/*
	printf("Printing quotes list...\n");
	for(i=0; i<num_quotes; i++)
	{
		printf("Quote %d: %s\n",i+1, quotes_list[i]);
	}
	printf("End of list.\n");
	*/

	if(_q) //Quote id specified
	{
		if(unordered)
		{
			printf("quotes: warning: The entry after quote %d is numbered incorrectly.\n",unordered);
		}

		int q;
		sscanf(argv[2],"%d",&q);

		if(!q)
		{
			printf("Indices start at either 1 or -1. If the value exceeds the number of quotes, the modulo value will be used.\n");
			return 0;
		}

		if(abs(q) >  num_quotes)
			q = q % num_quotes;

		if(q>0)
			n = q-1;
		else
			n = num_quotes + q;

		
	}
	else //Produce random quote.
	{
		srand(time(NULL));
		n = rand() % num_quotes;
	}

	printf("%s\n",quotes_list[n]);

	for(i=0; i<num_quotes; i++)
		free(quotes_list[i]);
	free(quotes_list);

	return 0;
}

void strapp(char * dest, char * src)
{
	int num_chars_left = quote_max_size - strlen(dest);
	strncpy(dest+strlen(dest),src, num_chars_left);
	*(dest+strlen(dest)+num_chars_left+1) = 0;
}