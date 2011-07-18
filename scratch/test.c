#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>


int main(int argc, char **argv){
  int opt;
  while((opt = getopt(argc,argv,"abc:")) != -1){
    switch(opt){
    case 'a': printf("a set\n");break;
    case 'b': printf("b set\n");break;
    case 'c': printf("c: %s\n",optarg);break;
    }
  }
  while(optind < argc){
    printf("-: %s\n",argv[optind]);
    optind++;
  }
}

