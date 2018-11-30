#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_CHILDREN 5

typedef struct node Node; 

struct node{
	char* data; 
	Node* children[MAX_NUM_CHILDREN];
	int num_children; 
} ; 


Node* createNode(char* data){
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data; 
	
	int i; 
	for (i=0; i<MAX_NUM_CHILDREN; i++){
		newNode->children[i] = NULL; 
	}
	newNode->num_children = 0; 
	return newNode; 
}

void destroyNode(Node *node){
	// Destroy children
	Node *child; 
	int i; 
	for (i=0; i<MAX_NUM_CHILDREN; i++){
		if (node->children[i] != NULL){
			destroyNode(node->children[i]);
		}
	}

	// Destroy node
	free(node);
}

void addChild(Node *node, Node* child){
	node->children[node->num_children++] = child; 
}

Node* createTree1(){
	Node *root = createNode("Root"); 
	Node *n1 = createNode("1"); 
	Node *n2 = createNode("2");
	Node *n3 = createNode("3"); 
	Node *n4 = createNode("4");
	Node *n5 = createNode("5"); 
	Node *n6 = createNode("6");
	Node *n7 = createNode("7"); 
	Node *n8 = createNode("8");


	addChild(root, n1);
	addChild(root, n2); 
	addChild(root, n3);

	addChild(n1, n4); 

	addChild(n4, n5);
	addChild(n4, n6); 

	return root; 
}

void printNode(Node *node){
	printf("%s\n", node->data); 
}


void printTreeDfsPost(Node *root){

}



int main(){

	Node* tree1 = createTree1(); 

	printTreeDfsPost(tree1); 
	int i=sizeof("123");
	int j=strlen("zzl");
	printf("\n%d %d\n", i, j);
	printf("%c\n", 89);

}
