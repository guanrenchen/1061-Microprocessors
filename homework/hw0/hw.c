#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    char ch;
    struct Node* next;
}Node;

typedef struct Stack{
    char data[256];
    int pos;
}Stack;

char remove_from_llist(Node **llist, int n){
    Node *cur=*llist, *prev=NULL;
    char ch;

    if(n==0 && cur!=NULL){
        ch = (*llist)->ch;
        *llist = (*llist)->next;
        return ch;
    }

    while(cur->next!=NULL && n>0){
        prev = cur;
        cur = cur->next;
        --n;
    }

    if(n>0){
        printf("Index out of bound\n");
        return '\0';
    }else{
        prev->next = cur->next;
        ch = cur->ch;
        free(cur);
        return ch;
    }
}

void push_to_stack(Stack *stack, char ch){
    if (stack->pos>=255){
        printf("StackError: Stack is full\n");
        return;
    }
    stack->data[++stack->pos] = ch;
}

char pop_from_stack(Stack *stack){
    if (stack->pos<0){
        printf("StackError: Stack is empty\n");
        return '\0';
    }
    return stack->data[stack->pos--];
}

void print_stack(Stack *stack){
    int i;
    for(i=0; i<=stack->pos; ++i)
        printf("%c", stack->data[i]);
    printf("\n");
}

void print_llist(Node *llist){
    Node *cur;
    for(cur=llist; cur!=NULL; cur=cur->next)
        printf("%c", cur->ch);
    printf("\n");
}

int main(int argc, char* argv[]){
    char ch = (char)('a'-48);
    
    Stack *stack = (Stack*) malloc(sizeof(Stack));
    stack->pos=-1;

    Node *llist = (Node*) malloc(sizeof(Node));
    llist->ch = ch;
    
    Node *cur = llist;
    int i;
    for(i=0; i<50; ++i){
        cur->next = (Node*) malloc(sizeof(Node));
        cur = cur->next;
        cur->ch = ++ch;
    }
    
    printf("\nInitial State : (50 nodes in linked list, empty stack)\n");
    printf("Linked list:\t");  print_llist(llist); 
    printf("Stack:\t\t");      print_stack(stack);
    
    printf("\nRemove 1st node from linked list and push it to stack\n");
    push_to_stack(stack, remove_from_llist(&llist, 0));
    printf("Linked list:\t");  print_llist(llist); 
    printf("Stack:\t\t");      print_stack(stack);
    
    
    printf("\nRemove 10th node from linked list and push it to stack\n");
    push_to_stack(stack, remove_from_llist(&llist, 9));
    printf("Linked list:\t");  print_llist(llist); 
    printf("Stack:\t\t");      print_stack(stack);
    
    printf("\nPop from stack\n");
    pop_from_stack(stack);
    printf("Linked list:\t");  print_llist(llist); 
    printf("Stack:\t\t");      print_stack(stack);

    return 0;
}
