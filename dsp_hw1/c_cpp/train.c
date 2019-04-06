#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"


void main(int argc, char* argv[]){
    HMM hmm_initial;

    //========argv============
    int iteration = atoi(argv[1]);
    loadHMM(&hmm_initial, argv[2]);
    FILE *f_seq = open_or_die(argv[3], "r");
    FILE *fp_out = open_or_die(argv[4], "w");

    for(int iter = 0;iter < iteration;iter++){
        
        //========variable========
        char seq[256];
        int state_num = hmm_initial.state_num, obsv_num = hmm_initial.observ_num;
        printf("%d",obsv_num);
        double seq_gamma[256][state_num], obsv_gamma[obsv_num][state_num];
        double E[state_num][state_num];

        memset(seq_gamma, 0, sizeof(seq_gamma));
        memset(obsv_gamma, 0, sizeof(obsv_gamma));
        memset(E, 0, sizeof(E));

        int N=0, T;
        while(fscanf(f_seq,"%s",seq)>0){
            T = strlen(seq)-1;
            N++;
            double alpha[T][state_num];
            double beta[T][state_num];
            
            //========================calculate alpha
            //initial alpha
            //pi*b(o)
            for(int s = 0;s < state_num;s++){
                alpha[0][s] = hmm_initial.initial[s]*hmm_initial.observation[seq[0]-'A'][s];
                //printf("%f",hmm_initial.initial[s]);
            }

            //forward
            //
            for(int t = 1;t < T;t++){
                for(int s = 0;s < state_num;s++){
                    alpha[t][s] = 0;
                    for(int s_another = 0;s_another < state_num;s_another++){
                        alpha[t][s] += alpha[t-1][s_another]*hmm_initial.transition[s_another][s];
                    }
                    alpha[t][s] *= hmm_initial.observation[seq[t]-'A'][s];
                }
            }

            //========================calculate beta
            //initial bata
            for(int s = 0;s < state_num;s++){
                beta[T-1][s] = 1;
            }
            //backward algorithm
            for(int t = T-1;t > 0;t--){
                for(int s = 0;s < state_num;s++){
                    beta[t-1][s] = 0;
                    for(int s_another = 0;s_another < state_num;s_another++)
                        beta[t-1][s] += hmm_initial.transition[s][s_another]*hmm_initial.observation[seq[t]-'A'][s_another]*beta[t][s_another];
                }
            }
            //calculate gamma and epsilon
            for(int t=0;t<T;t++){
                double sumArr[state_num], sum = 0;
                for(int s=0;s<state_num;s++){
                    sumArr[s] = alpha[t][s]*beta[t][s];
                    sum += sumArr[s];
                }

                for(int s=0;s<state_num;s++){
                    //calculate gamma
                    seq_gamma[t][s] += sumArr[s]/sum;
                    obsv_gamma[seq[t]-'A'][s] += sumArr[s]/sum;
                   
                    //calculate epsilon
                    if(t==T-1) continue;
                    for(int s_another=0;s_another<state_num;s_another++)
                        E[s][s_another] += (alpha[t][s]*hmm_initial.transition[s][s_another]*hmm_initial.observation[seq[t+1]-'A'][s_another]*beta[t+1][s_another])/sum;
                }
            }
        }
        //clearerr(f_seq);
        fseek(f_seq, 0, SEEK_SET);
        
        for(int s=0;s<state_num;s++){

            hmm_initial.initial[s] = seq_gamma[0][s]/N;

            double gamma_sum = 0;
            for(int t=0;t<T-1;t++) 
                gamma_sum += seq_gamma[t][s];
            for(int s_another = 0;s_another < state_num;s_another++)
                hmm_initial.transition[s][s_another] = E[s][s_another]/gamma_sum;
        }


        for(int obsv=0;obsv<obsv_num;obsv++){
            for(int s=0;s<state_num;s++){
                double gamma_sum = 0;
                for(int t=0;t<T;t++) 
                    gamma_sum += seq_gamma[t][s];
                hmm_initial.observation[obsv][s] = obsv_gamma[obsv][s]/gamma_sum;
                
            }
        }
    }
    dumpHMM(fp_out, &hmm_initial);
}