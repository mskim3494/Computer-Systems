#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

// Struct declarations
// cache line struct
typedef struct {
    int valid;
    unsigned long tag;
} c_line;

// cache set struct
typedef struct {
    c_line *l;
} c_set;

// Print usage information
void printh()
{
    printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>.\n");
    printf(" -h             Optional help flag that prints usage info.\n");
    printf(" -v             Optional verbose flag that displays trace info.\n");
    printf(" -s <s>         Number of set index bits (S=2^s is the number of sets).\n");
    printf(" -E <E>         Associativity (number of lines per set).\n");
    printf(" -b <b>         Number of block bits (B=2^b is the block size).\n");
    printf(" -t <tracefile> Name of the valgrind trace to replay.\n");
}

void reset_cache(c_set* cache, unsigned set, int high, unsigned long long tag)
{
    int i;
    for(i=high;i>=1;i--){ // reorganize set such that top line is empty
        cache[set].l[i].tag=cache[set].l[i-1].tag;
        cache[set].l[i].valid=cache[set].l[i-1].valid;
    } cache[set].l[0].tag=tag;
    cache[set].l[0].valid=1;
    return;
}


int main(int argc, char* argv[])
{
    // Variable declarations
    FILE *file; // trace file
    int i,j; // loop counters
    int s, b, E, S; // variable holders
    char *t; // trace file name holder
    int Vtag; // -v turned on?
    unsigned long long address; // 64-bit addresses
    char action; // M, L, S
    unsigned long long tag; // tag variable holder for input
    unsigned set; // set number of input
    unsigned missFlag; // flag to keep track of misses
    unsigned hitCount, missCount, evictionCount;
    int size; // holder for input
    
    // Initializing variables
    s = b = E = 0;
    hitCount = missCount = evictionCount = 0;
    file=NULL;
    Vtag = 0;
    
    if(argc<8){
        printh();
        exit(1);
    }
    
    for(i=1; i<argc; i++){
        if(!strcmp(argv[i], "-s")){
            s= atoi(argv[i+1]);
            S= pow(2,s);
        } else if (!strcmp(argv[i], "-b")){
            b= atoi(argv[i+1]);
        } else if (!strcmp(argv[i], "-E")){
            E= atoi(argv[i+1]);
        } else if (!strcmp(argv[i], "-t")){
            t= argv[i+1];
        } else if (!strcmp(argv[i], "-h")){
            printh();
        } else if (!strcmp(argv[i], "-v")){
            Vtag= 1;
        }
    }
    
    if(s==0||b==0||E==0){
        fprintf(stderr,"Missing arguments\n");
        printh();
        exit(1);
    }
    
    c_set* cache;
    // malloc enough space for the cache
    cache = (c_set*) malloc(S*sizeof(c_set));
    for(i=0; i<S; i++){
        cache[i].l= (c_line*) malloc(E*sizeof(c_line));
        for(j=0; j<E; j++){
            cache[i].l[j].valid=0; // initialize the cache
        }
    }
    
    file= fopen(t, "r");
    
    if(file==NULL){
        fprintf(stderr, "Invalid trace file\n");
        exit(1);
    }
    
    while(fscanf(file, " %c %llx,%d", &action, &address, &size)==3){
        tag= address & (-1<<(s+b));
        set= (int) (address>>b) & (S-1);
        if(action=='I'){
            continue;
        }
        if(Vtag){
            printf("%c %llx,%d ", action, address, size);
        } missFlag=1;
        // check the cache for hits
        for(i=0;(i<E) && (missFlag==1); i++){
            if((cache[set].l[i].valid !=0) && (cache[set].l[i].tag == tag)){
                missFlag=0; // if valid = 1 and tag matches, then it is a hit
                if(action=='M'){
                    hitCount += 2; // one for load, one for store
                } else if (action=='S'||action=='L'){
                    hitCount++;
                }
                // LRU : have MRU at top, LRU at bottom (like "pushing lines down")
                reset_cache(cache, set, i, tag);
                if(Vtag==1){
                    if(action=='M'){
                        printf("hit hit\n");
                    } else if(action=='S'||action=='L'){
                        printf("hit\n");
                    }
                }
            }
        } // cache miss
        if(missFlag==1){
            if(action=='M'){
                missCount++;
                hitCount++;
            } else if(action=='S'||action=='L'){
                missCount++;
            }
            if(Vtag==1){
                printf("miss ");
            }
            if(cache[set].l[E-1].valid==1){ // set is full, need eviction
                evictionCount++;
                if(Vtag==1){
                    printf("eviction ");
                }
            } reset_cache(cache, set, E-1, tag);
            if(Vtag==1){
                printf("hit\n"); // done after hit
            }
        }
    }
    
    printSummary(hitCount, missCount, evictionCount);
    
    // Clean up cache
    for(i=0; i<S; i++){
        free(cache[i].l);
    } free(cache);
    fclose(file);
    return 0;
}
