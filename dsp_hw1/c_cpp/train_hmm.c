#include<stdio.h>
#include<stdlib.h>
#include "hmm.h"
#include <math.h>
#define num_of_state 6

    double alpha[200][num_of_state]={0.0};
    double beta[200][num_of_state]={0.0};
    double gama[200][num_of_state]={0.0};
    double R_gama[15000][200][num_of_state]={0.0},r[15000][num_of_state]={0.0},r1[15000][num_of_state]={0.0};
    double delta[200][num_of_state][num_of_state]={0.0};
    double E_delta[15000][num_of_state][num_of_state]={0.0};

int main(int argc,char *argv[])
{
 	HMM hmm_initial;

    double sum,sum2;
	int i,j,k,t,h,l,sample=0,interation;
    char line_data[256];
	
	interation=atoi(argv[1]);
	loadHMM( &hmm_initial, argv[2] );
	FILE *fp1=open_or_die(argv[3],"r");
    FILE *fp2=open_or_die(argv[4],"w+");
    
	for(l=0;l<interation;l++)
	{
		while(fscanf(fp1,"%s",line_data)>0)
		{
			sample=0;
			//forwarding calculate alpha
			//initial
      		for(i=0;i<num_of_state;i++)	
      		{
				k=line_data[0]-'A';
            	alpha[0][i]=hmm_initial.initial[i]*hmm_initial.observation[k][i];
      		}
      		//induction
      		for(t=0;t<strlen(line_data)-1;t++)
      		{
				for(j=0;j<num_of_state;j++)
				{
					for(sum=0.0,i=0;i<num_of_state;i++)
						sum+=alpha[t][i]*hmm_initial.transition[i][j];
			
					k=line_data[t+1]-'A';
					alpha[t+1][j]=sum*hmm_initial.observation[k][j];
				}
	  		}
	  
	  		//backwarding calculate beta
      		//initial
	  		for(i=0;i<num_of_state;i++)
      		{
				beta[strlen(line_data)-1][i]=1;
		    }
      		//induction
      		for(t=strlen(line_data)-2;t>=0;t--)
      		{
				for(i=0;i<num_of_state;i++)
				{
					for(beta[t][i]=0.0,j=0;j<num_of_state;j++)
					{
						k=line_data[t+1]-'A';
						beta[t][i]+=hmm_initial.transition[i][j]*hmm_initial.observation[k][j]*beta[t+1][j];
					}
				}
	  		}
	  
      		//calculate gama
			for(t=0;t<strlen(line_data);t++)
	  		{
				for(sum=0.0,j=0;j<num_of_state;j++)
		  			sum+=alpha[t][j]*beta[t][j];
				for(i=0;i<num_of_state;i++)
				{
	  				gama[t][i]+=((alpha[t][i]*beta[t][i])/sum);
					R_gama[sample][i][line_data[t]-'A']+=gama[t][i]; // 10000 samples
				}
			}	
	  		//calculate delta
			for(t=0;t<strlen(line_data)-1;t++)
			{
				for(sum=0.0,i=0;i<num_of_state;i++)
					for(j=0;j<num_of_state;j++)
					{
						k=line_data[t+1]-'A';
						sum+=alpha[t][i]*hmm_initial.transition[i][j]*hmm_initial.observation[k][j]*beta[t+1][j];
					}
	  			for(i=0;i<num_of_state;i++)
					for(j=0;j<num_of_state;j++)
					{
						k=line_data[t+1]-'A';
						delta[t][i][j]=((alpha[t][i]*hmm_initial.transition[i][j]*hmm_initial.observation[k][j]*beta[t+1][j])/sum);
					}
			}
	  
			for(i=0;i<num_of_state;i++)
				for(j=0;j<num_of_state;j++)
					for(t=0;t<strlen(line_data)-1;t++)
						E_delta[sample][i][j]+=delta[t][i][j];
			
			for(i=0;i<num_of_state;i++)
				for(t=0;t<strlen(line_data)-1;t++)
					r[sample][i]+=gama[t][i];
			
			for(i=0;i<num_of_state; i++)
				for(t=0;t<strlen(line_data);t++)
					r1[sample][i]+=gama[t][i];
				
	  	sample++;
		}
	    //update
	    for(i=0;i<num_of_state;i++)
	    	hmm_initial.initial[i]=gama[0][i]/sample;  //initial
	    
		for(i=0;i<num_of_state;i++)
		{
			for(j=0;j<num_of_state;j++)
			{
				sum=sum2=0.0;
				for(t=0;t<sample;t++)
				{
					sum +=E_delta[t][i][j];
					sum2+=r[t][i];
				}
				hmm_initial.transition[i][j]=sum/sum2; //transition
			}
		}
	    
		for(j=0;j<num_of_state;j++)
			for(i=0;i<hmm_initial.observ_num;i++)
			{
				sum=sum2=0.0;
				for(t=0;t<sample;t++)
				{
					sum+=R_gama[t][j][i];
					sum2+=r1[t][j];
				}
				hmm_initial.observation[i][j]=sum/sum2;
			}
	}
	dumpHMM( fp2, &hmm_initial );
	return 0;
}





















"""
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hmm.h"
#define num_of_state 6




void main(int argc,char *argv[]){
    int iteration = atoi(argv[1]);
    HMM hmm_initial;
	loadHMM( &hmm_initial, argv[2] );
    char seq_model = argv[3];
    char model    = argv[4];
    

    
    FILE *f_seq = open_or_die(seq_model,"r");
    FILE *f_model = open_or_die(mode,"w+")

    double alpha[200][num_of_state]={0.0};
    double beta[200][num_of_state]={0.0};
    double gama[200][num_of_state]={0.0};
    double R_gama[15000][200][num_of_state]={0.0},r[15000][num_of_state]={0.0},r1[15000][num_of_state]={0.0};
    double delta[200][num_of_state][num_of_state]={0.0};
    double E_delta[15000][num_of_state][num_of_state]={0.0};

    for(i=0;i<iteration;i++)
    {
    		while(fscanf(fp1,"%s",seqence)>0)
		{
			sample=0;
			//forwarding calculate alpha
			//initial
      		for(i=0;i<num_of_state;i++)	
      		{
				k=seqence[0]-'A';
            	alpha[0][i]=hmm_initial.initial[i]*hmm_initial.observation[k][i];
      		}
      		//induction
      		for(t=0;t<strlen(seqence)-1;t++)
      		{
				for(j=0;j<num_of_state;j++)
				{
					for(sum=0.0,i=0;i<num_of_state;i++)
						sum+=alpha[t][i]*hmm_initial.transition[i][j];
			
					k=seqence[t+1]-'A';
					alpha[t+1][j]=sum*hmm_initial.observation[k][j];
				}
	  		}
	  
	  		//backwarding calculate beta
      		//initial
	  		for(i=0;i<num_of_state;i++)
      		{
				beta[strlen(seqence)-1][i]=1;
		    }
      		//induction
      		for(t=strlen(seqence)-2;t>=0;t--)
      		{
				for(i=0;i<num_of_state;i++)
				{
					for(beta[t][i]=0.0,j=0;j<num_of_state;j++)
					{
						k=seqence[t+1]-'A';
						beta[t][i]+=hmm_initial.transition[i][j]*hmm_initial.observation[k][j]*beta[t+1][j];
					}
				}
	  		}
	  
      		//calculate gama
			for(t=0;t<strlen(seqence);t++)
	  		{
				for(sum=0.0,j=0;j<num_of_state;j++)
		  			sum+=alpha[t][j]*beta[t][j];
				for(i=0;i<num_of_state;i++)
				{
	  				gama[t][i]+=((alpha[t][i]*beta[t][i])/sum);
					R_gama[sample][i][seqence[t]-'A']+=gama[t][i]; // 10000 samples
				}
			}	
	  		//calculate delta
			for(t=0;t<strlen(seqence)-1;t++)
			{
				for(sum=0.0,i=0;i<num_of_state;i++)
					for(j=0;j<num_of_state;j++)
					{
						k=seqence[t+1]-'A';
						sum+=alpha[t][i]*hmm_initial.transition[i][j]*hmm_initial.observation[k][j]*beta[t+1][j];
					}
	  			for(i=0;i<num_of_state;i++)
					for(j=0;j<num_of_state;j++)
					{
						k=seqence[t+1]-'A';
						delta[t][i][j]=((alpha[t][i]*hmm_initial.transition[i][j]*hmm_initial.observation[k][j]*beta[t+1][j])/sum);
					}
			}
	  
			for(i=0;i<num_of_state;i++)
				for(j=0;j<num_of_state;j++)
					for(t=0;t<strlen(seqence)-1;t++)
						E_delta[sample][i][j]+=delta[t][i][j];
			
			for(i=0;i<num_of_state;i++)
				for(t=0;t<strlen(seqence)-1;t++)
					r[sample][i]+=gama[t][i];
			
			for(i=0;i<num_of_state; i++)
				for(t=0;t<strlen(seqence);t++)
					r1[sample][i]+=gama[t][i];
				
	  	sample++;
		}
	    //update
	    for(i=0;i<num_of_state;i++)
	    	hmm_initial.initial[i]=gama[0][i]/sample;  //initial
	    
		for(i=0;i<num_of_state;i++)
		{
			for(j=0;j<num_of_state;j++)
			{
				sum=sum2=0.0;
				for(t=0;t<sample;t++)
				{
					sum +=E_delta[t][i][j];
					sum2+=r[t][i];
				}
				hmm_initial.transition[i][j]=sum/sum2; //transition
			}
		}
	    
		for(j=0;j<num_of_state;j++)
			for(i=0;i<hmm_initial.observ_num;i++)
			{
				sum=sum2=0.0;
				for(t=0;t<sample;t++)
				{
					sum+=R_gama[t][j][i];
					sum2+=r1[t][j];
				}
				hmm_initial.observation[i][j]=sum/sum2;
			}
	}
	dumpHMM( fp2, &hmm_initial );
	return 0;
}
        
    //ToDo calculate alpha
        //Initialication
    


        //Induction


        //Termination


    //ToDo calculate beta



    //ToDo calculate gamma



    //ToDo calculate epsilon



    //ToDo Accumulate gamma and epsilon





    //ToDo re-estimate model parameter 





    }
}





int main(int argc,char *argv[])
{
    
	for(l=0;l<interation;l++)
	{
		while(fscanf(fp1,"%s",line_data)>0)
		{
			sample=0;
			//forwarding calculate alpha
			//initial
      		for(i=0;i<num_of_state;i++)	
      		{
				k=line_data[0]-'A';
            	alpha[0][i]=hmm_initial.initial[i]*hmm_initial.observation[k][i];
      		}
      		//induction
      		for(t=0;t<strlen(line_data)-1;t++)
      		{
				for(j=0;j<num_of_state;j++)
				{
					for(sum=0.0,i=0;i<num_of_state;i++)
						sum+=alpha[t][i]*hmm_initial.transition[i][j];
			
					k=line_data[t+1]-'A';
					alpha[t+1][j]=sum*hmm_initial.observation[k][j];
				}
	  		}
	  
	  		//backwarding calculate beta
      		//initial
	  		for(i=0;i<num_of_state;i++)
      		{
				beta[strlen(line_data)-1][i]=1;
		    }
      		//induction
      		for(t=strlen(line_data)-2;t>=0;t--)
      		{
				for(i=0;i<num_of_state;i++)
				{
					for(beta[t][i]=0.0,j=0;j<num_of_state;j++)
					{
						k=line_data[t+1]-'A';
						beta[t][i]+=hmm_initial.transition[i][j]*hmm_initial.observation[k][j]*beta[t+1][j];
					}
				}
	  		}
	  
      		//calculate gama
			for(t=0;t<strlen(line_data);t++)
	  		{
				for(sum=0.0,j=0;j<num_of_state;j++)
		  			sum+=alpha[t][j]*beta[t][j];
				for(i=0;i<num_of_state;i++)
				{
	  				gama[t][i]+=((alpha[t][i]*beta[t][i])/sum);
					R_gama[sample][i][line_data[t]-'A']+=gama[t][i]; // 10000 samples
				}
			}	
	  		//calculate delta
			for(t=0;t<strlen(line_data)-1;t++)
			{
				for(sum=0.0,i=0;i<num_of_state;i++)
					for(j=0;j<num_of_state;j++)
					{
						k=line_data[t+1]-'A';
						sum+=alpha[t][i]*hmm_initial.transition[i][j]*hmm_initial.observation[k][j]*beta[t+1][j];
					}
	  			for(i=0;i<num_of_state;i++)
					for(j=0;j<num_of_state;j++)
					{
						k=line_data[t+1]-'A';
						delta[t][i][j]=((alpha[t][i]*hmm_initial.transition[i][j]*hmm_initial.observation[k][j]*beta[t+1][j])/sum);
					}
			}
	  
			for(i=0;i<num_of_state;i++)
				for(j=0;j<num_of_state;j++)
					for(t=0;t<strlen(line_data)-1;t++)
						E_delta[sample][i][j]+=delta[t][i][j];
			
			for(i=0;i<num_of_state;i++)
				for(t=0;t<strlen(line_data)-1;t++)
					r[sample][i]+=gama[t][i];
			
			for(i=0;i<num_of_state; i++)
				for(t=0;t<strlen(line_data);t++)
					r1[sample][i]+=gama[t][i];
				
	  	sample++;
		}
	    //update
	    for(i=0;i<num_of_state;i++)
	    	hmm_initial.initial[i]=gama[0][i]/sample;  //initial
	    
		for(i=0;i<num_of_state;i++)
		{
			for(j=0;j<num_of_state;j++)
			{
				sum=sum2=0.0;
				for(t=0;t<sample;t++)
				{
					sum +=E_delta[t][i][j];
					sum2+=r[t][i];
				}
				hmm_initial.transition[i][j]=sum/sum2; //transition
			}
		}
	    
		for(j=0;j<num_of_state;j++)
			for(i=0;i<hmm_initial.observ_num;i++)
			{
				sum=sum2=0.0;
				for(t=0;t<sample;t++)
				{
					sum+=R_gama[t][j][i];
					sum2+=r1[t][j];
				}
				hmm_initial.observation[i][j]=sum/sum2;
			}
	}
	dumpHMM( fp2, &hmm_initial );
	return 0;
}
"""