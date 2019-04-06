#include "hmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double p[MAX_SEQ][MAX_STATE];
double Pro[5];

int main(int argc,char *argv[])
{
    HMM hmmdata[5];
	load_models( argv[1], hmmdata, 5);
	FILE *input=open_or_die(argv[2],"r");  
	FILE *output=open_or_die(argv[3],"w+");

	//test
	int N,Len,target,i,j,n,t;
	char seq[256];
	double max_pro;
	//accurancy
	int correct,total;
	char line_model[256],tmp[256],pp[30];

	while(fscanf(input,"%s",seq)>0)
	{
		Len=strlen(seq);
		for(n=0;n<5;n++)
		{
			N=hmmdata[n].state_num;

			for(i=0;i<N;i++)	
				p[0][i]=hmmdata[n].initial[i]*hmmdata[n].observation[seq[0]-'A'][i];

			for(t=1;t<Len;t++)	//induction
			{
				for(j=0;j<N;j++)
				{
					max_pro=0.0;
					for(i=0;i<N;i++)
						if(p[t-1][i]*hmmdata[n].transition[i][j] > max_pro)
							max_pro=p[t-1][i]*hmmdata[n].transition[i][j];
					p[t][j]=max_pro*hmmdata[n].observation[seq[t]-'A'][j];
				}
			}
			max_pro=0.0;
			for(i=0;i<N;i++)
				if(p[Len-1][i] > max_pro)
					max_pro=p[Len-1][i];
			Pro[n]=max_pro;
		}
		max_pro=0.0; 
		for(n=0;n<5;n++)
			if(Pro[n]>max_pro)
			{
				max_pro=Pro[n];
				target=n;
			}	
		fprintf(output,"%s %e\n",hmmdata[target].model_name,max_pro);
	}
	return 0;
}