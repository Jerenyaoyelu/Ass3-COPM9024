#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<string.h>
#ifndef max
	#define max(a,b)(((a)>(b))?(a):(b))
#endif

// This template is only a guide 
// You need to include additional fields, data structures and functions  

// data type for heap nodes
typedef struct HeapNode { 
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //key of this task
	int TaskName;  //task name 
	int Etime; //execution time of this task
	int Rtime; // release time of this task
	int Dline; // deadline of this task
	// add additional fields here
	int degree;  // count of children
	struct HeapNode *child; //point to the child with largest degree
	struct HeapNode *Nextsibling; // point to its next siblings
	struct HeapNode *Lastsibling; // point to its next siblings
} HeapNode;

//data type for a priority queue (heap) 
typedef struct BinomialHeap{ //this is heap header
	int  size;      // count of items in the heap
	// add additional fields here
	HeapNode *smallestB; //point to the root of binomial tree with the smallest degree
	// HeapNode *sk; // point to the root of BT with the smallest nodes(B0,B1,B2...)
	// HeapNode *RtArray;
	HeapNode *tail;
} BinomialHeap;

// create a new heap node to store an item (task) 
HeapNode *newHeapNode(int k, int n, int c, int r, int d, ...)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
  // ... you need to define other parameters yourself) 	 
	HeapNode *new;
	new = malloc(sizeof(HeapNode));
	assert(new != NULL);
	new->key = k;
	new->TaskName = n;
	new->Etime = c;
	new->Rtime = r; 
	new->Dline = d;
	// initialise other fields
	new->degree = 0;
	new->child = NULL;
	new->Nextsibling = NULL; 
	new->Lastsibling = NULL;
	return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert (T != NULL);
	// initialise all the fields here
	T->size = 0;
	T->smallestB = NULL;
	T->tail = NULL;
	return T;
}

void print(BinomialHeap *T);


// O(1)
//merge two smallestBs from two BHs
HeapNode *MergeBT(BinomialHeap *H1, BinomialHeap *H2){
	HeapNode *new, *temp;
	if(H1->smallestB == NULL){
		return H2->smallestB;
	}
	if(H2->smallestB == NULL){
		return H1->smallestB;
	}
	if(H1->smallestB->key >= H2->smallestB->key){
		//extract temp
		temp = H1->smallestB;
		H1->smallestB = temp->Nextsibling;
		temp->Nextsibling->Lastsibling = NULL;
		//extract new
		new = H2->smallestB;
		H2->smallestB = new->Nextsibling;
		new->Nextsibling->Lastsibling = NULL;
		//make the H1->smallestB the child of H2->smallestB
		temp->Nextsibling = new->child;
		new->child = temp;
	}else{
		//extract temp
		temp = H2->smallestB;
		H2->smallestB = temp->Nextsibling;
		temp->Nextsibling->Lastsibling = NULL;
		//extract new
		new = H1->smallestB;
		H1->smallestB = new->Nextsibling;
		new->Nextsibling->Lastsibling = NULL;
		//make the H2->smallestB the child of H1->smallestB
		temp->Nextsibling = new->child;
		new->child = temp;
	}
	new->degree = new->degree + temp->degree + 1;
	return new;
}

// O(1)
//add binomial tree to a binomial heap(in increased degree)
void AddBTToBH(BinomialHeap *NewHp, HeapNode *new){
	if(NewHp->smallestB == NULL){
		NewHp->smallestB = new;
		NewHp->tail = NewHp->smallestB;
	}else{
		// new added binomial tree has the same degree with the last binomial tree in the new BH
		if(NewHp->tail->degree == new->degree){
			// key of tail is larger than key of new
			if(NewHp->tail->key >= new->key){
				if(NewHp->tail->Lastsibling != NULL){
					NewHp->tail->Lastsibling->Nextsibling = new;
				}
				NewHp->tail->Nextsibling = new->child;
				new->child = NewHp->tail;
				NewHp->tail->Lastsibling = NULL;
				NewHp->tail = new;
			}else{// key of tail is smaller than key of new
				new->Nextsibling = NewHp->tail->child;
				NewHp->tail->child = new;
			}
		}else{//new added binomial tree has the larger degree than the last binomial tree in the new BH
			NewHp->tail->Nextsibling = new;
			new->Lastsibling = NewHp->tail;
			NewHp->tail = NewHp->tail->Nextsibling;
		}
	}
	NewHp->size = NewHp->size + new->degree + 1;
}

// O(log(n+m))
//union two binomial heap
BinomialHeap *UnionBH(BinomialHeap *H1, BinomialHeap *H2){
	BinomialHeap *NewHp = newHeap();
	HeapNode *new;
	if(H1->smallestB == NULL){
		return H2;
	}
	if(H2->smallestB == NULL){
		return H1;
	}
	while(1){
		if(H1->smallestB == NULL && H2->smallestB == NULL){
			break;
		}else if(H1->smallestB != NULL){
			new = H1->smallestB;
			H1->size = H1->size - new->degree - 1;
			//move the smallestB
			H1->smallestB = H1->smallestB->Nextsibling;
		}else if(H2->smallestB != NULL){
			new = H2->smallestB;
			H2->size = H2->size - new->degree - 1;
			//move the smallestB
			H2->smallestB = H2->smallestB->Nextsibling;
		}else{
			//binomial trees from H1 and H2 have the same degree
			if(H1->smallestB->degree == H2->smallestB->degree){
				//minus size of each BH
				H1->size = H1->size - H1->smallestB->degree - 1;
				H2->size = H2->size - H2->smallestB->degree - 1;
				//merge two BTs from two BHs
				HeapNode *new = MergeBT(H1,H2);
				//add new BT to the new heap
				AddBTToBH(NewHp,new);
			}else {
				if(H1->smallestB->degree >= H2->smallestB->degree){//binomial trees from H2 has the smaller degree
					new = H2->smallestB;
					H2->size = H2->size - new->degree - 1;
					//move the smallestB
					H2->smallestB = H2->smallestB->Nextsibling;
				}else{//binomial trees from H1 has the smaller degree
					new = H1->smallestB;
					H1->size = H1->size - new->degree - 1;
					//move the smallestB
					H1->smallestB = H1->smallestB->Nextsibling;
				}
				//extract new from H2
				new->Nextsibling->Lastsibling = NULL;
				new->Nextsibling = NULL;
				AddBTToBH(NewHp,new);
			}
		}
	}
	return NewHp;
}

// O(log(n))
// put the time complexity analysis for Insert() here    
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline 
	// You don't need to check if this task already exists in T 	 
	//put your code here
	HeapNode *new = newHeapNode(k,n,c,r,d);
	BinomialHeap *B0 = newHeap();
	B0->size ++;
	B0->smallestB = new;
	// print(B0);
	BinomialHeap *newH;
	newH = UnionBH(T,B0);
}


// O(log(n))
// to-merge two heaps are two parts from same BH T, so the sum of nodes from this two heaps are n
// so the time complexity remains O(log(n)) 
HeapNode *RemoveMin(BinomialHeap *T)
{
	HeapNode *MinNode = T->smallestB, *crt = T->smallestB;
	if(crt == NULL){
		return NULL;
	}
	// find min node
	while(crt != NULL){
		if(crt->key < MinNode->key){
			MinNode = crt;
		}
		crt = crt ->Nextsibling;
	}

	//method 2: not working
	// // take out the whole BT from the heap
	// if(MinNode == T->smallestB){
	// 	T->smallestB = MinNode->Nextsibling;
	// 	MinNode->Nextsibling->Lastsibling = NULL;
	// 	MinNode->Nextsibling = NULL;
	// }else{
	// 	MinNode->Lastsibling->Nextsibling = MinNode->Nextsibling;
	// 	MinNode->Nextsibling->Lastsibling = MinNode->Lastsibling;
	// 	MinNode->Lastsibling = NULL;
	// 	MinNode->Nextsibling = NULL;
	// }
	// // make the reminder of the taken-out BT serveral BTs
	// // and add them to the original BH one by one
	// HeapNode *temp;
	// while(MinNode->child != NULL){
	// 	temp = MinNode->child;
	// 	MinNode->child = temp->Nextsibling;
	// 	if(MinNode->child != NULL){
	// 		MinNode->child->Lastsibling = NULL;
	// 	}
	// 	temp->Nextsibling = NULL;
	// // 	// AddBTToBH(T,temp); not applicable here.
	// }

	// take out the whole BT from the heap
	if(MinNode == T->smallestB){
		T->smallestB = MinNode->Nextsibling;
		MinNode->Nextsibling->Lastsibling = NULL;
		MinNode->Nextsibling = NULL;
	}else{
		MinNode->Lastsibling->Nextsibling = MinNode->Nextsibling;
		MinNode->Nextsibling->Lastsibling = MinNode->Lastsibling;
		MinNode->Lastsibling = NULL;
		MinNode->Nextsibling = NULL;
	}
	// make the reminder of the taken-out BT a new heap
	BinomialHeap *reminder = newHeap();
	reminder->smallestB = MinNode->child;
	reminder->tail = MinNode->child;
	reminder->size = MinNode->degree;
	// because child points to the child with largest degree
	while(reminder->smallestB->Nextsibling != NULL){
		reminder->smallestB = reminder->smallestB->Nextsibling;
	}
	MinNode->child = NULL;
	// Union reminder of the taken-out BT and the original BH
	BinomialHeap *AfterRm = UnionBH(T,reminder);
	// update the original BH
	T->smallestB = AfterRm->smallestB;
	T->size = AfterRm->size;
	T->tail = AfterRm->tail;
	return MinNode;
}

// O(log(n))
int Min(BinomialHeap *T)
{
  HeapNode *MinNode = T->smallestB, *crt = T->smallestB;
	if(crt == NULL){
		return -1;
	}
	// find min node
	while(crt != NULL){
		if(crt->key < MinNode->key){
			MinNode = crt;
		}
		crt = crt ->Nextsibling;
	}
	return MinNode->key;
}

// for testing
void print(BinomialHeap *T){
	HeapNode *t =T->smallestB;
	while(t!= NULL){
		printf("task %d\n",t->TaskName);
		t = t->Nextsibling;
	}
}

// O(nlog(n))
int TaskScheduler(char *f1, char *f2, int m )
{
	// read input
	FILE *fp;
	int *data = (int *)malloc(255 * sizeof(int));
	char data_pieces[255];
	fp = fopen(f1,"r");
	if(fp == NULL){
		printf("%s does not exist",f1);
		exit(0);
	}
	while(1){
		fscanf(fp,"%s",data_pieces);
		if(feof(fp)){
			break;
		}
		data[data[0]+1] = atoi(data_pieces);
		data[0]++;
	}
	fclose(fp);
	// insert all task nodes into PQ1(release time as the key)
	BinomialHeap *PQ1 = newHeap();
	for(int i = 1; i<= data[0]; i = i + 4){
		if(data[i] < 0 || data[i+1] <= 0 || data[i+2] < 0 || data[i+3] <= 0){
			printf("Input error when reading the attribute of task %d",i);
			exit(0);
		}
		int k = data[i+2];
		int n = data[i];
		int c = data[i+1];
		int r = data[i+2];
		int d = data[i+3];
		// problem:
		Insert(PQ1,k,n,c,r,d);
	}
	print(PQ1);
	// scheduling
	BinomialHeap *PQ2 = newHeap();
	// using PQ to maintain all the cores.
	// the completion time of current task in this core will be the key
	// initially, all keys are 0;
	BinomialHeap *cores = newHeap();
	for(int i = 1; i <= m;i++){
		Insert(cores,0,i,0,0,0);
	}
	// int timepoint[m+1];
	int output[256];
	int t = 0;
	while(1){
		//task is ready, dequeue from PQ1, enqueue into PQ2
		if(PQ1->smallestB != NULL && Min(PQ1)<=t){
			HeapNode *RedyTsk = RemoveMin(PQ1);
			//insert ready task to PQ2
			Insert(PQ2,RedyTsk->Dline,RedyTsk->TaskName,RedyTsk->Etime,RedyTsk->Rtime,RedyTsk->Dline);
		}
		//if there existing ready task
		if(PQ2->smallestB != NULL){
			//look through all cores to find the first idle core
			int timepoint = Min(cores);
			if(timepoint <= t){
				HeapNode *ScdulingT = RemoveMin(PQ2);
				HeapNode *ScdulingC = RemoveMin(cores);
				// should be the current time plus the execution time.
				ScdulingC->key = t + ScdulingT->Etime;
				//insert core back
				Insert(cores,ScdulingC->key,ScdulingC->TaskName,0,0,0);
				if(ScdulingC->key <= ScdulingT->Dline){
					output[output[0]+1] = ScdulingT->TaskName;
					output[output[0]+2] = ScdulingC->TaskName;
					output[output[0]+3] = t;
					output[0] += 3;
				}else{
					return 0;
				}
			}
			// for(int i = 1; i<=m;i++){
			// 	if(timepoint[i]<=t){
			// 		HeapNode *ScdulingT = RemoveMin(PQ2);
			// 		//schedule task in core i, increase the corresponding timepoint.
			// 		timepoint[i] = ScdulingT->Etime + timepoint[i];
			// 		if(timepoint[i] <=  ScdulingT->Dline){
			// 			output[output[0]+1] = ScdulingT->TaskName;
			// 			output[output[0]+2] = i;
			// 			output[output[0]+3] = t;
			// 			output[0] += 3;
			// 			break;
			// 		}else{
			// 			return 0;
			// 		}
			// 	}
			// }
		}
		t++;
		//if PQ1 and PQ2 are all empty, then terminates the loop
		if(PQ1->smallestB == NULL && PQ2->smallestB == NULL){
			break;
		}
	}
	// write output
	fp = fopen(f2,"w+");
	rewind(fp);
	for(int i = 1; i <= output[0];i++){
		fprintf(fp,"%d",output[i]);
	}
	fclose(fp);
	return 1;
}

int main() //sample main for testing 
{ int i;
  i=TaskScheduler("test.txt", "feasibleschedule1.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
//   /* There is a feasible schedule on 4 cores */
//   i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
//   if (i==0) printf("No feasible schedule!\n");
//   /* There is no feasible schedule on 3 cores */
//   i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
//   if (i==0) printf("No feasible schedule!\n");
//   /* There is a feasible schedule on 5 cores */
//   i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
//   if (i==0) printf("No feasible schedule!\n");
//   /* There is no feasible schedule on 4 cores */
//   i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
//   if (i==0) printf("No feasible schedule!\n");
//   /* There is no feasible schedule on 2 cores */
//   i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
//   if (i==0) printf("No feasible schedule!\n");
//   /* There is a feasible schedule on 2 cores */
 return 0; 
}
