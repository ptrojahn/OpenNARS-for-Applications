#include "Shell.h"

void Shell_Start()
{
    YAN_INIT();
    OUTPUT = 0;
    INPUT = false;
    for(;;)
    {
        char line[1024];
        scanf("%1023[^\n]", line);
        int size = strlen(line);
        if(size==0)
        {
            YAN_Cycles(1);
        }
        else
        {
            if(!strcmp(line,"*volume=0"))
            {
                PRINT_DERIVATIONS = false;
            }
            else
            if(!strcmp(line,"*volume=100"))
            {
                PRINT_DERIVATIONS = true;
            }
            else
            if(!strcmp(line,"100000"))
            {
                puts("performing 100k inference steps:");
                YAN_Cycles(100000);
                puts("done with 100k additional inference steps.");
            }
            else
            {
                Term term = Encode_Term(line);
                Truth best_truth = {0};
                if(line[strlen(line)-1] == '?')
                {
                    int concept_i = 0;
                    for(int i=0; i<concepts.itemsAmount; i++)
                    {
                        Concept *c = concepts.items[i].address;
                        //now match the concept term, for now just ignoring the question variables
                        //TODO use unification approach as the generated RuleTable already uses.
                        
                        for(int j=0; j<COMPOUND_TERM_SIZE_MAX; j++)
                        {
                            if(c->term.atoms[j] != term.atoms[j] &&
                               atom_names[term.atoms[j]-1][0] != '?') //rudimentar question var for now
                            {
                                goto Continue;
                            }
                        }
                        if(c->belief.type != EVENT_TYPE_DELETED)
                        {
                            if(Truth_Expectation(c->belief.truth) > Truth_Expectation(best_truth))
                            {
                                best_truth = c->belief.truth;
                            }
                        }
                        Continue:;
                    }
                    fputs("Answer: ", stdout);
                    Truth_Print(&best_truth);
                }
                else
                {
                    YAN_AddInput(term, EVENT_TYPE_BELIEF, YAN_DEFAULT_TRUTH, 0, true);
                }
            }
        }
        memset(line, 0, 1024);
        getchar();
    }
}