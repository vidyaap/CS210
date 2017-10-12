#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 *Name: Vidya Akavoor
 *Login: vidyaap
 *
 */




//struct representing one line of the cache;
//will be used to make entire cahce in main
struct MyLine{
  int valid;
  int age;
  unsigned int tag;
};


struct Set{
  struct MyLine *lines;
};


struct Cache{
  struct Set *sets;
};




char *hex2bin(char *hex){
  char bin[64];
  char *bin_ptr = &bin[0];

  while(*hex){
    switch(*hex){
      case '0': strcat(bin, "0000"); break;
      case '1': strcat(bin, "0001"); break;
      case '2': strcat(bin, "0010"); break;
      case '3': strcat(bin, "0011"); break;
      case '4': strcat(bin, "0100"); break;
      case '5': strcat(bin, "0101"); break;
      case '6': strcat(bin, "0110"); break;
      case '7': strcat(bin, "0111"); break;
      case '8': strcat(bin, "1000"); break;
      case '9': strcat(bin, "1001"); break;
      case 'a': strcat(bin, "1010"); break;
      case 'b': strcat(bin, "1011"); break;
      case 'c': strcat(bin, "1100"); break;
      case 'd': strcat(bin, "1101"); break;
      case 'e': strcat(bin, "1110"); break;
      case 'f': strcat(bin, "1111"); break;
      default: printf("\nInvalid hexadecimal digit %c", hex);
    }
    hex++;
  }
  return bin_ptr;
}
    

unsigned int StringtoBin(char *bin_ptr){
  return (unsigned int) strtol(bin_ptr, NULL, 2);
}


unsigned int getTag(unsigned int bin, int s, int b){
  return (bin << (s+b));
}


unsigned int getSet(unsigned int bin, int s, int b){
  return (bin << b)&((1<<s)-1);
}



char *getAddr(char line[]){
  char *token = strtok(line, " ,");
  token = strtok(NULL, " ,");
  return token;
}


char *getCmd(char line[]){
  char *token = strtok(line, " ,");
  return token;
}


    
int max_age(struct Set set, int E){
  int max = 0;
  int i;
  for (i = 0; i < E; i++){
    if ((set.lines[i].age) > (set.lines[max].age))
      max = i;
  }
  return max;
}



int min_age(struct Set set, int E){
  int min = 0;
  int i;
  for (i = 0; i < E; i++){
    if ((set.lines[i].age) < (set.lines[min].age))
      min = i;
  }
  return min;
}



int eviction(struct Set set, int E){
  int full;
  int i;
  for (i = 0; i < E; i++){
    if (set.lines[i].valid == 0){
      full = 0;
      break;
    }
    else
      full = 1;
  }

  if (full == 1){
    int victim = min_age(set, E);
    set.lines[victim].valid = 0;
    set.lines[victim].age = 0;
    set.lines[victim].tag = 0;
  }
  return full;
} 



void insert(struct Set set, int E, unsigned int tag){
  int max = max_age(set, E);
  int idx = 0;
  int i;
  for (i = 0; i < E; i++){
    if (set.lines[i].valid == 0){
      idx = 1;
      break;
    }
  }
  set.lines[idx].valid = 1;
  set.lines[idx].tag = tag;
  set.lines[idx].age = max;
}


char *trace;

int main(int argc, char** argv){

  int hits, misses, evictions;
  int opt, s, E, b;

  // read inputs to function with getopt()
  while(-1 != (opt=getopt(argc, argv, "s:E:b:t:"))){
    switch(opt){
      case 's':
        s = atoi(optarg);
		break;
      case 'E':
		E = atoi(optarg);
		break;
      case 'b':
        b = atoi(optarg);
		break;
      case 't':
        trace = optarg;
		break;
      default:
        printf("wrong argument\n");
		break;
      }
   }
    

  struct Cache *cache = NULL;
  cache = (struct Cache*)malloc(sizeof(struct Cache));
  cache -> sets = (struct Set*)malloc((2<<s)*sizeof(struct Set));
  int i;
  for (i = 0; i < (2<<s); i++){
    cache -> sets[i].lines = malloc(E * sizeof(struct MyLine));
  }
  
    
    //read trace file line by line
  	FILE *t = fopen(trace, "r");
	t = fopen(t, "r");
    if(t == NULL) perror("Error opening file");
    else{
       char trace_line[50];
       while(fgets(trace_line, 50, t)){
	 	char *addr = getAddr(trace_line);
	 	char *cmd = getCmd(trace_line);
	 	int bin_addr = hex2bin(addr);
	 	unsigned int tag = getTag(bin_addr, s, b);
	 	unsigned int set = getSet(bin_addr, s, b);
	 
	 
	 	int i;
	 	for (i = 0; i < E; i++){
	 	  if (strcmp(*cmd, "I") == 1){
	    	 if ((cache -> sets[set].lines[i].valid == 1) && (cache -> sets[set].lines[i].tag == tag)){
	       		hits++;
	       		cache -> sets[set].lines[i].age = (max_age(cache -> sets[set], E)) + 1;
	         }
	     	else if (cache -> sets[set].lines[i].valid == 0){
	       		if (eviction((cache -> sets[set]), E) == 1){
		 			evictions++;
	       		}
	       		else{
		 			insert(cache -> sets[set], E, tag);
	       		}
	       		misses++;
	     	}
	     }
	   if (strcmp(*cmd, "M")==0){ hits++;}
        }
   	  }

      fclose(t);
  }


    printSummary(hits, misses, evictions);
    return 0;
}