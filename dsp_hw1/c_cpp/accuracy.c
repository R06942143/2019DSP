#include "hmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{
	int total,correct;
	char line_model[256],guess[256],pro[30];
	
	
	FILE *input = open_or_die(argv[1],"r");
	FILE *answer = open_or_die(argv[2],"r");
	FILE *acc = open_or_die(argv[3],"w+");
	
	correct=0;
	total=0;
	while(fscanf(answer,"%s",line_model)>0)
	{

		fscanf(input,"%s %s",guess,pro);
		total++;
		if(strcmp(line_model,guess)==0)
			correct++;
	}
	fprintf(acc,"accuracy = %f\n",(double)correct/total);
	return 0;
}