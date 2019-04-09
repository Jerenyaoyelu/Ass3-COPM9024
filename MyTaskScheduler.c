#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
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
	HeapNode *child; //point to the child with largest degree
	HeapNode *Nextsibling; // point to its next siblings
	HeapNode *Lastsibling; // point to its next siblings
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

// // get array of all roots in BH
// void GetRtA(BinomialHeap *H, HeapNode *RtA[]){
// 	HeapNode *crt = H->smallestBT;
// 	while(crt != NULL){
// 		RtA[log(crt->degree+1)+1] = crt;
// 		RtA[0] ++;
// 		crt = crt->Nextsibling;
// 	}
// }

// // merge two binomial heaps
// BinomialHeap *Merge(BinomialHeap *H1, BinomialHeap *H2){
// 	BinomialHeap *new = newHeap();
// 	// store all the roots of BT in the corresponding position of array in the order of B0,B1,B2...
// 	HeapNode *RtArray1[log(H1->size)+1];
// 	HeapNode *RtArray2[log(H2->size)+1];
// 	GetRtA(H1,RtArray1);
// 	GetRtA(H2,RtArray2);
// 	int len = max(RtArray1[0],RtArray2[0]);
// 	for(int i = 1; i <= len; i++){
// 		if(RtArray1[i] != NULL && RtArray2[i] != NULL){
// 			// if(RtArray1[i]->Lastsibling != NULL)
// 			RtArray1[i]->Lastsibling->Nextsibling = RtArray1[i]->Nextsibling;
// 			RtArray1[i]->Nextsibling->Lastsibling = RtArray1[i]->Lastsibling;
// 			RtArray1[i]->Lastsibling = NULL;
// 			RtArray1[i]->Nextsibling = RtArray2[i]->child;
// 			RtArray2[i]->child->Lastsibling = RtArray1[i];
// 			RtArray2[i]->child = RtArray1[i];
// 			RtArray2[i]->degree = RtArray1[i]->degree+1;
// 		}else if(RtArray1[i] != NULL){

// 		}
// 	}
	
// }

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

//add binomial tree to a binomial heap(in increased degree)
void AddBTToBH(BinomialHeap *NewHp, HeapNode *new){
	if(NewHp->smallestB = NULL){
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
				HeapNode *new = MergeBT(H1->smallestB,H2->smallestB);
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
	return newHeap;
}

// put the time complexity analysis for Insert() here    
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline 
  // You don't need to check if this task already exists in T 	 
  //put your code here
  HeapNode *new = newHeapNode(k,n,c,r,d);
  BinomialHeap *B0 = newHeap();
  B0->size ++;
  B0->smallestB = new;
}

// put your time complexity for RemoveMin() here
HeapNode *RemoveMin(BinomialHeap *T)
{
 // put your code here
}

int Min(BinomialHeap *T)
{
  // put your code here
}


// put your time complexity analysis for MyTaskScheduler here
int TaskScheduler(char *f1, char *f2, int m )
{
 // put your code here
}

int main() //sample main for testing 
{ int i;
  i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 4 cores */
  i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 3 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 5 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 4 cores */
  i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 2 cores */
  i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 2 cores */
 return 0; 
}
