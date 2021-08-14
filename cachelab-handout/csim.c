#include "cachelab.h"
#include<getopt.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
typedef struct cache_line
{
    /* data */
    int tag;
    /*data if clear*/
    int dirty;
    int group;
    /*block addr*/
    char *blockAddr;
    /*time dispatch*/
    int time;
}line;


int main(int argc,char* argv[])
{
    int group_bits;
    int group_num;
    int block_bits;
    int line_num;
    line **cache;
    int i,current_time=0,tag_bits;
    char *file_path;
    char ch;

    while((ch=getopt(argc,argv,"s:E:b:t"))!=-1){
        switch (ch)
        {
            /*group number*/
        case 's':
            group_bits=atoi(optarg);
            group_num=pow(2,group_bits);
            break;
        case 'E':
            line_num=atoi(optarg);
            break;
        case 'b':
            block_bits=atoi(optarg);
            break;
        case 't':
            file_path=optarg;
            break;
        default:
            break;
        }
    }
    FILE* trace_file=fopen(file_path,"r");
    /*
        create cache
        init cache
    */
   cache=(line**)malloc(sizeof(line*)*group_num);
    for(int i=0;i<group_num;i++)
    {
        cache[i]=(line*)malloc(sizeof(line)*line_num);
        for(int j=0;j<line_num;j++){
            cache[i][j].blockAddr=(char*)malloc(block_bits+1);
            cache[i][j].dirty=0;
            cache[i][j].time=0;
            cache[i][j].tag=-1;
            cache[i][j].group=-1;
        }
    }

    tag_bits=32-group_bits-block_bits;
    while()

    printSummary(0, 0, 0);
    return 0;
}
