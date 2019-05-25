#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <vector>
#include "Ngram.h"
#include "VocabMap.h"

#ifndef Matrix
#define Matrix 256
#endif

#ifndef OOV
#define OOV -100
#endif

const VocabIndex emptyContext[] = {Vocab_None};

LogP Viterbi(Ngram& lm, VocabMap& map, VocabString* words, unsigned count);
VocabIndex getIndex(VocabString w);

static int bigram;
static Vocab voc; 
static Vocab Zhu, Big5; 

VocabIndex getIndex(VocabString w)
{
	VocabIndex wid = voc.getIndex(w);
	return (wid == Vocab_None)? voc.getIndex(Vocab_Unknown): wid; 
}

LogP Viterbi(Ngram& lm, VocabMap& map, VocabString* words, unsigned count)
{
	LogP Pro[Matrix][1024] = {0.0};
	VocabIndex BackTrack[Matrix][1024];
	VocabIndex IntToIndex[Matrix][1024];
	int size[Matrix] = {0};

	/* Viterbi Algorithm initial */

	Prob p;
	VocabIndex w;
	VocabMapIter iter(map, Zhu.getIndex(words[0]));
	for(int i = 0; iter.next(w, p); size[0]++, i++){
		VocabIndex index = getIndex(Big5.getWord(w));
		LogP curP = lm.wordProb(index, emptyContext);
		if(curP == LogP_Zero) 
            curP = OOV;
		Pro[0][i] = curP;
		IntToIndex[0][i] = w;
		BackTrack[0][i] = 0;
	}



	for(int t = 1; t < count; t++){
		Prob p;
		VocabIndex w;
		VocabMapIter iter(map, Zhu.getIndex(words[t]));
		for(int j = 0; iter.next(w, p); size[t]++, j++){
			VocabIndex index = getIndex(Big5.getWord(w));
			LogP maxP = -1.0/0.0, pre;
			for(int i = 0; i < size[t-1]; i++){
				VocabIndex context[] = {getIndex(Big5.getWord(IntToIndex[t-1][i])), Vocab_None};
				LogP curP = lm.wordProb(index, context);
				LogP unigramP = lm.wordProb(index, emptyContext);
				
				if(curP == LogP_Zero && unigramP == LogP_Zero)
					curP = OOV;

				curP += Pro[t-1][i];		
		
				if(curP > maxP){
					maxP = curP;
					BackTrack[t][j] = i;
				}
			}
			Pro[t][j] = maxP;
			IntToIndex[t][j] = w;
		}
	}

	/* maximize probability */
	LogP maxP = LogP_Zero;
	int end = -1;
	for(int i = 0; i < size[count-1]; i++){
		LogP curP = Pro[count-1][i];
		if(curP > maxP){
			maxP = curP;
			end = i;
		}
	}

	/* BackTrack from end */
	int path[Matrix];
	path[count-1] = end;
	for(int t = count-2; t >= 0; t--)
		path[t] = BackTrack[t+1][path[t+1]];

	for(int t = 0; t < count; t++)
		printf("%s%s", Big5.getWord(IntToIndex[t][path[t]]), (t == count-1)? "\n": " ");

	return maxP;
}


int main(int argc, char *argv[])
{
    if (argc != 9) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "./mydisambig -text [file] -map [map] -lm [LM] -order [orderNum]\n");
        exit(1);
    }
    const char* text_filename = argv[2];
    const char* map_filename = argv[4];
    const char* lm_filename = argv[6];
	bigram = atoi(argv[8]);

	Ngram lm(voc, bigram);	
	VocabMap map(Zhu, Big5);
	File lmFile(lm_filename, "r" );
	lm.read(lmFile);
	lmFile.close();
	/* read Language Model and ZhuYin-to-Big5 map */



	File mapfile(map_filename, "r");
	map.read(mapfile);
	mapfile.close();
	
	/* recognize testdata (argv[2])*/
	File testdata(text_filename, "r");

    char* line = NULL;
	while(line = testdata.getline()){
		VocabString WordsInLine[maxWordsPerLine];
		unsigned count = Vocab::parseWords(line, &WordsInLine[1], maxWordsPerLine);
		WordsInLine[0] = "<s>";
		WordsInLine[count+1] = "</s>";
		LogP MaxP = Viterbi(lm, map, WordsInLine, count+2);
	}
	
	testdata.close();

	return 0;
}
