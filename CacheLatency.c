#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>

#define OneIte ptr =(char**)*ptr;
#define EightIte OneIte OneIte OneIte OneIte OneIte OneIte OneIte OneIte
#define Sixty4Ite EightIte EightIte EightIte EightIte EightIte EightIte EightIte EightIte
#define Five12Ite Sixty4Ite Sixty4Ite Sixty4Ite Sixty4Ite Sixty4Ite Sixty4Ite Sixty4Ite Sixty4Ite
#define TwoMBIte Five12Ite Five12Ite Five12Ite Five12Ite
int CachePerf(int ArrSi,int jump){
    unsigned long sec, usec;
    struct timeval tv1, tv2;
    struct timezone tz;

    char* Arr = (char*)calloc(ArrSi,sizeof(char));

    for(int i=0; i<(ArrSi/jump)-1; i++){
        *(char**)&Arr[i*jump]=(char*)&Arr[(i+1)*jump];
    }
    *(char**)&Arr[((ArrSi/jump)-1)*jump]=(char*)&Arr[(0)*64];
    
    register char **ptr=(char**)Arr;
    
    gettimeofday (&tv1, &tz);
    TwoMBIte;
    gettimeofday (&tv2, &tz);
    char **touch = ptr;
    if (tv2.tv_usec < tv1.tv_usec) {
        usec = 1000000 + tv2.tv_usec - tv1.tv_usec;
        sec = tv2.tv_sec - tv1.tv_sec - 1;
    } else {
        usec = tv2.tv_usec - tv1.tv_usec;
        sec = tv2.tv_sec - tv1.tv_sec;
    }

    printf("Array Size: %d KB, Jump %d B, latency %.4f ns\n\n", ArrSi / 1024, jump, (sec * 1000000  + usec) * 1000.0 / (512*4));
    free(Arr);
    return 0;
}
int main(){
    int jump[2] = {8,64};
    int Size = 1024; //Minimum Buffer size
    for (int i=0 ;i<2;i++){
        Size=1024;
        while(Size<20*1024*1024){
            CachePerf(Size,jump[i]);
            Size*=2;
        }
    }
}