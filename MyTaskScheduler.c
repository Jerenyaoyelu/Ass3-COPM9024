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
//merge two binomial trees( with the same degree )
HeapNode *MergeBT(HeapNode *t1, HeapNode *t2){
	HeapNode *new, *temp;
	if(t1 == NULL){
		return NULL;
	}
	if(t1->key >= t2->key){
		t1->Nextsibling = t2->child;
		if(t2->child!=NULL)
		{// t2 has more than one node
			t2->child->Lastsibling = t1;
		}
		t2->child = t1;
		new = t2;
		temp = t1;
	}else{
		//make the t2 the child of t1
		t2->Nextsibling = t1->child;
		if(t1->child!=NULL)
		{// t1 has more than one node
			t1->child->Lastsibling = t2;
		}
		t1->child = t2;
		new = t1;
		temp = t2;
	}
	new->degree = new->degree + temp->degree + 1;
	// printf("ee %d,%d,%d\n",new->TaskName,new->degree,new->Dline);
	return new;
}

// O(1)
//add binomial tree to a binomial heap(in increased degree)
void AddBTToBH(BinomialHeap *NewHp, HeapNode *new){
	//update size first
	NewHp->size = NewHp->size + new->degree + 1;
	if(NewHp->smallestB == NULL){
		NewHp->smallestB = new;
		NewHp->tail = NewHp->smallestB;
	}else{
		// new added binomial tree has the same degree with the last binomial tree in the new BH
		if(NewHp->tail->degree == new->degree){
			// key of tail is larger than key of new
			if(NewHp->tail->key > new->key){
				//update degree first.
				new->degree = new->degree + NewHp->tail->degree+1;
				if(NewHp->tail != NewHp->smallestB){
					NewHp->tail->Lastsibling->Nextsibling = new;
				}else{
					NewHp->smallestB = new;
				}
				NewHp->tail->Nextsibling = new->child;
				new->child = NewHp->tail;
				NewHp->tail->Lastsibling = NULL;
				NewHp->tail = new;
			}else{// key of tail is smaller than key of new
				//update degree first.
				NewHp->tail->degree = NewHp->tail->degree + new->degree + +1;
				new->Nextsibling = NewHp->tail->child;
				NewHp->tail->child = new;
			}
		}else{//new added binomial tree has the larger degree than the last binomial tree in the new BH
			NewHp->tail->Nextsibling = new;
			new->Lastsibling = NewHp->tail;
			NewHp->tail = new;
		}
	}
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
		// H1 and H2 are all empty, terminates the loop
		if(H1->smallestB == NULL && H2->smallestB == NULL){
			break;
		}else if(H1->smallestB != NULL && H2->smallestB != NULL)
		{// H1 and H2 are all not empty
			//binomial trees of smallest k from H1 and H2 have the same degree
			if(H1->smallestB->degree == H2->smallestB->degree){
				//minus size of each BH
				H1->size = H1->size - H1->smallestB->degree - 1;
				H2->size = H2->size - H2->smallestB->degree - 1;
				//extract two BTs
				HeapNode *t1 = H1->smallestB, *t2 = H2->smallestB;
				H1->smallestB = t1->Nextsibling;
				//H1 has more than 1 tree before extracting
				if(H1->smallestB != NULL){
					H1->smallestB->Lastsibling = NULL;
				}else{//H1 is empty after extracting
					H1->tail = H1->smallestB;
				}
				t1->Nextsibling = NULL;
				H2->smallestB = t2->Nextsibling;
				//H2 has more than 1 tree before extracting
				if(H2->smallestB != NULL){
					H2->smallestB->Lastsibling = NULL;
				}else{//H2 is empty after extracting
					H2->tail = H2->smallestB;
				}
				t2->Nextsibling = NULL;
				//merge two BTs from two BHs
				new = MergeBT(t1,t2);
			}else {//binomial trees of smallest k from H1 and H2 have the different degree
				if(H1->smallestB->degree >= H2->smallestB->degree)
				{//binomial trees of smallest k from H2 has the smaller degree
					new = H2->smallestB;
					H2->size = H2->size - new->degree - 1;
					//move the smallestB
					H2->smallestB = new->Nextsibling;
					if(H2->smallestB != NULL){//more than one tree before extracting
						H2->smallestB->Lastsibling = NULL;
					}else{//empty after extracting
						H2->tail = H2->smallestB;
					}
				}else
				{//binomial trees of smallest k from H1 has the smaller degree
					new = H1->smallestB;
					H1->size = H1->size - new->degree - 1;
					//move the smallestB
					H1->smallestB = H1->smallestB->Nextsibling;
					if(H1->smallestB != NULL){//more than one tree before extracting
						H1->smallestB->Lastsibling = NULL;
					}else{//empty after extracting
						H1->tail = H1->smallestB;
					}
				}
				//extract new from H2
				new->Nextsibling = NULL;
			}
		}else if(H1->smallestB != NULL)
		{// H2 is empty
			new = H1->smallestB;
			H1->size = H1->size - new->degree - 1;
			//move the smallestB
			H1->smallestB = H1->smallestB->Nextsibling;
			if(H1->smallestB != NULL){//more than one tree before extracting
				H1->smallestB->Lastsibling = NULL;
			}else{//empty after extracting
				H1->tail = H1->smallestB;
			}
			new->Nextsibling = NULL;
		}else{// H1 is empty
			new = H2->smallestB;
			H2->size = H2->size - new->degree - 1;
			//move the smallestB
			H2->smallestB = H2->smallestB->Nextsibling;
			if(H2->smallestB != NULL){//more than one tree before extracting
				H2->smallestB->Lastsibling = NULL;
			}else{//empty after extracting
				H2->tail = H2->smallestB;
			}
			new->Nextsibling = NULL;
		}
		//add new BT to the new heap
		AddBTToBH(NewHp,new);
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
	// printf("%d\n",B0->smallestB->TaskName);
	BinomialHeap *newH;
	// problem
	newH = UnionBH(T,B0);
	// update binomial Heap T
	T->size = newH->size;
	T->smallestB = newH->smallestB;
	T->tail = newH->tail;
}


//reverse a binomial heap (nextsibling points to the root with smaller k)
void reverse(BinomialHeap *T){
	HeapNode *crt, *next, *prev;
	// T has only one BT with a single node,then do nothing
	//otherwise,
	if(T->smallestB->Lastsibling != NULL){
		crt = T->smallestB->Lastsibling;
		while(1){
			prev = crt->Nextsibling;
			if(prev == T->smallestB){
				prev->Lastsibling = NULL;
				prev->Nextsibling = crt;
			}
			if(crt == T->tail)
			{// T has only two BTs
				crt->Nextsibling = NULL;
				crt->Lastsibling = prev;
				break;
			}else{
				next = crt->Lastsibling;
				crt->Lastsibling = prev;
				crt->Nextsibling = next;
			}
			if(next == T->tail){
				next->Lastsibling = crt;
				next->Nextsibling = NULL;
				break;
			}else{
				crt = next;
			}
		}
	}
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
	// take out the whole BT from the heap
	if(MinNode == T->smallestB)
	{//when the Min node is the BT with the smallest k
		T->smallestB = MinNode->Nextsibling;
		if(T->smallestB!=NULL)
		{//when the Min node is not the root of the last Tree
			T->smallestB->Lastsibling = NULL;
		}else{//when the Min node is the root of the last Tree
			T->tail = T->smallestB;
		}
	}else{
		MinNode->Lastsibling->Nextsibling = MinNode->Nextsibling;
		if(MinNode != T->tail)
		{
			MinNode->Nextsibling->Lastsibling = MinNode->Lastsibling;
		}else{//when the Min node is the BT in the tail
			T->tail = MinNode->Lastsibling;
		}
		MinNode->Lastsibling = NULL;
		
	}
	MinNode->Nextsibling = NULL;
	//update the size of T
	T->size = T->size - MinNode->degree - 1;
	// make the reminder of the taken-out BT a new heap
	BinomialHeap *reminder = newHeap();
	reminder->smallestB = MinNode->child;
	reminder->tail = MinNode->child;
	reminder->size = MinNode->degree;
	printf("reminder:\n");
	print(reminder);
	// because child points to the child with largest degree
	while(reminder->smallestB != NULL && reminder->smallestB->Nextsibling != NULL){
		reminder->smallestB = reminder->smallestB->Nextsibling;
	}
	// lastsibling has problem!!
	HeapNode *t =reminder->tail;
	while(t!= NULL){
		printf("ttt %d,%d,%d\n",t->TaskName,t->degree,t->Dline);
		t = t->Nextsibling;
	}
	//
	// reverse the heap,right now nextsibling points to the root with smaller k
	if(reminder->smallestB != NULL){
		// has problem!!!
		reverse(reminder);
	}
	printf("reversed reminder:\n");
	print(reminder);
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
		printf("from front task %d,%d,%d\n",t->TaskName,t->degree,t->Dline);
		t = t->Nextsibling;
	}
	HeapNode *t2 =T->tail;
	while(t2!= NULL){
		printf("from back task %d,%d,%d\n",t->TaskName,t->degree,t->Dline);
		t2 = t2->Lastsibling;
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
		// a newline is not the indicator of the end of file
		if(feof(fp)){
			break;
		}
		data[data[0]+1] = atoi(data_pieces);
		data[0]++;
	}
	fclose(fp);
	// insert all task nodes into PQ1(release time as the key)
	BinomialHeap *PQ1 = newHeap();
	// for(int i = 0; i<= data[0]; i = i + 1){
	// 	printf("%d\n",data[i]);
	// }
	for(int i = 1; i<= data[0]; i = i + 4){
		if(data[i] < 0 || data[i+1] <= 0 || data[i+2] < 0 || data[i+3] <= 0){
			printf("Input error when reading the attribute of task %d\n",data[i]);
			exit(0);
		}
		int k = data[i+2];
		int n = data[i];
		int c = data[i+1];
		int r = data[i+2];
		int d = data[i+3];
		Insert(PQ1,k,n,c,r,d);
	}
	printf("pq1:\n");
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
	// print(cores);
	// int timepoint[m+1];
	int output[256];
	int t = 0;
	while(1){
		//task is ready, dequeue from PQ1, enqueue into PQ2
		if(PQ1->smallestB != NULL && Min(PQ1)<=t){
			// removeMin has prblem:
			HeapNode *RedyTsk = RemoveMin(PQ1);
			printf("rm %d,%d,%d\n",RedyTsk->TaskName,RedyTsk->degree,RedyTsk->Dline);
			//insert ready task to PQ2
			Insert(PQ2,RedyTsk->Dline,RedyTsk->TaskName,RedyTsk->Etime,RedyTsk->Rtime,RedyTsk->Dline);
		}
		print(PQ1);
		//if there existing ready task
		if(PQ2->smallestB != NULL){
			//look through all cores to find the first idle core
			int timepoint = Min(cores);
			// printf("%d\n",timepoint);
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
		}
		t++;
		//if PQ1 and PQ2 are all empty, then terminates the loop
		if(PQ1->smallestB == NULL && PQ2->smallestB == NULL){
			break;
		}
	}
	// // write output
	// fp = fopen(f2,"w+");
	// rewind(fp);
	for(int i = 1; i <= output[0];i++){
		// fprintf(fp,"%d",output[i]);
		printf("%d",output[i]);
	}
	// fclose(fp);
	return 1;
}

int main() //sample main for testing 
{ int i;
  i=TaskScheduler("test.txt", "feasibleschedule1.txt", 2);
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
