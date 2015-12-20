/* 
- kolla igenom alla stackpekare
- kolla metadatan för varje objekt som pekas på
- om metadata finns så ska pekaren skickas vidare till heaptraverseringen
- sålla bort pekare som inte pekar på något
- returnera alla giltliga pekare till heaptraverseringen i form av en länkad lista.
*/

/*
  Kompilera med:
  gcc -std=c11 -Wall -ggdb -o stack_traverser stack_traverser.c

  tills vidare.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include "linked_list.c"
#include "heap.h"

#define Dump_registers()			\
  jmp_buf env;					\
  if (setjmp(env)) abort();			\

extern char **environ; // bottom of the stack

int function() {
  int stack2;
  printf("stack2:  %15p\n", &stack2);
  return &stack2;
}

ll_node **build_stack_list() {
  void *top = __builtin_frame_address(1); // top of the stack
  ll_node **root = LL_initRoot();
  int counter = 0;
  while (top < environ) {
    ll_node *stackTop = LL_createAndInsertSequentially(root, top);
    //printf("stackTop: %p\n", stackTop);,
    printf("Count %d: New stackpointer %p with content %04x was added to the list.\n", counter, stackTop, stackTop->nodeContent);
    top += sizeof(void *);
    counter++;
  }
  return root;
}

ll_node **traverse_stack_list() {
  return NULL;  
}

void print_stack_list(ll_node **root) {
  puts("Printing stacklist");
  void *top = __builtin_frame_address(1);
  ll_node *iterator = *root;
  int counter = 0;

  while (iterator) {
    printf("\nCounter: %d\n%04x\n", counter, iterator->nodeContent);
    top += sizeof(void *);
    counter++;
    if (iterator->previous)
      printf("%04x has previous pointer at %04x\n", iterator->nodeContent, iterator->previous->nodeContent);
    if (iterator->next)
      printf("%04x has next pointer at %04x\n", iterator->nodeContent, iterator->next->nodeContent);
    iterator = iterator->next;
    }
  
}

void print_stack() {

  const int N = 10;
  int arr[N];
  int i = 0;
  while (i < N) {
    arr[i] = i * 100 + i * 10 + i;
    printf("arr[%d]: %d\n", i, arr[i]);
    i++;
  }

  puts("\n----------------------------------------\n");
  void *top = __builtin_frame_address(1); // top of the stack

  printf("Top of the stack: %p\nBottom of the stack %p\n", top, environ);
  
  int bottomBigger = 0;
  
  if (environ > top) {
    bottomBigger = 1;
  }
  
  while (top < environ) {
    printf("\n%p\n", top);
    top = top + sizeof(void *);
  }
  puts("\n----------------------------------------\n");  
}

int main() {
  Dump_registers();
  ll_node **root = build_stack_list();
  print_stack_list(root);
  function();
  return 0;
}
