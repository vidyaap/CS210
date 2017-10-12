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





    

unsigned int stringToHex(char *hex_ptr){
  return (unsigned int) strtol(hex_ptr, NULL, 16);
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
	t = fopen(trace, "r");
    if(t == NULL) perror("Error opening file");
    else{
       char trace_line[50];
       while(fgets(trace_line, 50, t)){
	 	char *addr = getAddr(trace_line);
	 	char *cmd = getCmd(trace_line);
	 	int hex_addr = stringToHex(addr);
	 	unsigned int tag = getTag(bin_addr, s, b);
	 	unsigned int set = getSet(bin_addr, s, b);
	 
	 
	 	int i;
	 	for (i = 0; i < E; i++){
	 	  //if (strcmp(*cmd, "I") == 1){
      if(*cmd != 0x49){
	    	 if ((cache -> sets[set].lines[i].valid == 1) && (cache -> sets[set].lines[i].tag == tag)){
	       		hits++;
	       		cache -> sets[set].lines[i].age = (max_age(cache -> sets[set], E)) + 1;
	         }
	     	else if (cache -> sets[set].lines[i].valid == 0){
          misses++;
	       		if (eviction((cache -> sets[set]), E) == 1){
		 			evictions++;
	       		}
	       		else{
		 			insert(cache -> sets[set], E, tag);
	       		}
	     	}
	     
	   //if (strcmp(*cmd, "M")==0){ hits++;}
        if(*cmd == 0x4D){hits++;}
        }
      }
   	}
  }

      fclose(t);


    printSummary(hits, misses, evictions);
    return 0;
}