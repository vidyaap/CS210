// int choice1(int x){
// 	return x/4;
// }


// int choice2(int x){
// 	return 12*x;
// }

// int choice3(int x){
// 	return (x<<31)&1;
// }

// int choice4(int x){
// 	return (x<0);
// }

// int choice5(int x){
// 	return (x+15)/8;
// }


// int choice6(int x){
// 	return (x>>30);
// }


// int fun3(int a){
// 	return a*128;
// }


// int fun12(int a){
// 	return a*65;
// }

// int fun5(int a){
// 	return a*33;
// }

// int fun13(int a, int b){
// 	if(a&b)
// 		return b;
// 	else
// 		return a;
// }


// int fun4(int a, int b){
// 	if(a&b)
// 		return a;
// 	else
// 		return b;
// }


// int fun15(int a, int b){
// 	if(a>b)
// 		return b;
// 	else
// 		return a;
// }


// int bar(int x){
// 	int i;
// 	int val = x*3;

// 	for(i=0; i<x; i++){
// 		val = 5+i+val;
// 		val = val*(3+i);
// 	}
// 	return val;
// }


typedef struct node {
	double x;
	unsigned int y;
	struct node *next;
	struct node *prev;
} node_t;


node_t n;
void func(){
	node_t *m;
	m = n.next;
	m->y *= 8;
	return;
}
