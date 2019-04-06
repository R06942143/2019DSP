#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"


int main(int argc, char* argv[]){
    HMM hmm_initial;

    //========argv============
    int iteration = atoi(argv[1]);
    printf("%d",iteration)
    loadHMM(&hmm_initial, argv[2]);
    FILE *f_seq = open_or_die(argv[3], "r");
    FILE *fp_out = open_or_die(argv[4], "w");


    //========variable========
    char seq[256];
    int state_num = hmm_initial.state_num, obsv_num = hmm_initial.observ_num;
    double gamma[256][state_num], obsv_gamma[obsv_num][state_num];
    double epsilon[state_num][state_num];

    memset(gamma, 0, sizeof(gamma));
    memset(obsv_gamma, 0, sizeof(obsv_gamma));
    memset(epsilon, 0, sizeof(epsilon));



    for(int i=0;i<iteration;i++){
        int N=0, T;
        while(fscanf(f_seq,"%s",seq)>0){
            T = strlen(seq)-1;
            N++;

            //calculate alpha
            //initial alpha
            double alpha[T][state_num];
            for(int s=0;s<state_num;s++){
                alpha[0][s] = hmm_initial.initial[s]*hmm_initial.observation[seq[0]-'A'][s];
            }

            //forward
            for(int t=1;t<T;t++){
                for(int s=0;s<state_num;s++){
                    alpha[t][s] = 0;
                    for(int ps=0;ps<state_num;ps++){
                        alpha[t][s] += alpha[t-1][ps]*hmm_initial.transition[ps][s];
                    }
                    alpha[t][s] *= hmm_initial.observation[seq[t]-'A'][s];
                }
            }

            //calculate beta
            //initial bata
            double beta[T][state_num];
            for(int s=0;s<state_num;s++){
                beta[T-1][s] = 1;
            }
            //backward algorithm
            for(int t=T-2;t>=0;t--){
                for(int s=0;s<state_num;s++){
                    beta[t][s] = 0;
                    for(int ns=0;ns<state_num;ns++)
                        beta[t][s] += hmm_initial.transition[s][ns]*hmm_initial.observation[seq[t+1]-'A'][ns]*beta[t+1][ns];
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
                    gamma[t][s] += sumArr[s]/sum;
                    obsv_gamma[seq[t]-'A'][s] += sumArr[s]/sum;
                   
                    //calculate epsilon
                    if(t==T-1) continue;
                    for(int ns=0;ns<state_num;ns++)
                        epsilon[s][ns] += (alpha[t][s]*hmm_initial.transition[s][ns]*hmm_initial.observation[seq[t+1]-'A'][ns]*beta[t+1][ns])/sum;
                }
            }
        }


        for(int s=0;s<state_num;s++){
            //update pi
            hmm_initial.initial[s] = gamma[0][s]/N;

            //update transition(A)
            double gamma_sum = 0;
            for(int t=0;t<T-1;t++) 
                gamma_sum += gamma[t][s];
            for(int ns=0;ns<state_num;ns++)
                hmm_initial.transition[s][ns] = epsilon[s][ns]/gamma_sum;
        }

        //update observation(B)
        for(int obsv=0;obsv<obsv_num;obsv++){
            for(int s=0;s<state_num;s++){
                double gamma_sum = 0;
                for(int t=0;t<T;t++) gamma_sum += gamma[t][s];
                    hmm_initial.observation[obsv][s] = obsv_gamma[obsv][s]/gamma_sum;
            }
        }
    }


    dumpHMM(fp_out, &hmm_initial);
    fclose(fp_out);

    return 0;
}