/*
Scripts by jddantes

Quotes.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define quote_start "[s#]"
#define quote_end "[e#]"
#define quote_start_size 4
#define quote_end_size 4

int main(int argc, char ** argv)
{

	int n = 1;

	if(argc>1)
	{
		sscanf(argv[1],"%d",&n);
	}

	FILE * fp = fopen("quotes","r");
	
	if(!fp)
	{
		printf("Error. Could not open file.\n");
		return 0;
	}

	int quote_id;
	while(fscanf(fp,"%d",&quote_id)!=EOF)
	{
		int print = 1;
		if(quote_id!=n)
			print = 0;


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
			Scan and print until quote_end
		*/

		int quote_end_matched = 0;
		char buffer[quote_end_size+1];
		buffer[0] = 0;
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
				if(strlen(buffer))
				{
					if(print)
						printf("%s",buffer);
					buffer[0] = 0;
				}
				else
					if(print)
						printf("%c",c);
			}
		}

		/*
			Scan and print until quote_end END
		*/
	}

	fclose(fp);

	printf("\n");

	return 0;
}