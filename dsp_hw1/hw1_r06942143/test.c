#include "hmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double p[MAX_SEQ][MAX_STATE];

void main(int argc,char *argv[])
{
    HMM model[5];
	load_models( argv[1], model, 5);
	FILE *test_data=open_or_die(argv[2],"r");  
	FILE *result=open_or_die(argv[3],"w+");
	
	char seq[256];

	while(fscanf(test_data,"%s",seq)>0)
	{
		int T = strlen(seq);
		double max_ = 0.0;
		int num_max = 0;
		for(int n=0;n<5;n++)
		{
			int N=model[n].state_num;
			//initial
			for(int i=0;i<N;i++)	
				p[0][i]=model[n].initial[i]*model[n].observation[seq[0]-'A'][i];

			//recursion
			for(int t=1;t<T;t++)
			{
				for(int j=0;j<N;j++)
				{
					p[t][j] = 0.0;
					//find max seqence
					for(int i=0;i<N;i++)
						p[t][j]=max(p[t][j],p[t-1][i]*model[n].transition[i][j]);
					p[t][j] *= model[n].observation[seq[t]-'A'][j];
				}
			}
			for(int i=0;i<N;i++){
				if(p[T-1][i]>max_){
					max_ = p[T-1][i];
					num_max=n;
				}
			}
		}
		fprintf(result,"%s %e\n",model[num_max].model_name,max_);
	}
}