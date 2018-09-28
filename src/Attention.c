#include "Attention.h"

Attention Attention_forgetTask(Attention *taskAttention)
{
    return (Attention) { .priority   = taskAttention->priority * taskAttention->durability,
                         .durability = taskAttention->durability };
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
Attention Attention_forgetConcept(Attention *conceptAttention, Usage *conceptUsage, long currentTime)
{
    Attention ret;
    double usefulness = Usage_usefulness(conceptUsage, currentTime);
    double lowerPriorityBarrier = usefulness * USEFULNESS_MAX_PRIORITY_BARRIER;
    ret.priority = MAX(lowerPriorityBarrier, conceptAttention->priority * conceptAttention->durability);
}