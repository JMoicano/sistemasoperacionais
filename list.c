//https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct node {
   pid_t data;
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

pid_t getData(struct node* n){
   return n->data;
}

//insert link at the first location
void insertFirst(pid_t data) {
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->data = data;
	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}

//is list empty
int isEmpty() {
   return head == NULL;
}

int length() {
   int length = 0;
   struct node *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

//delete first item
struct node* deleteFirst() {

   //save reference to first link
   struct node *tempLink = head;
   
   //mark next to first link as first 
   head = head->next;
   
   //return the deleted link
   return tempLink;
}

//find a link with given key
struct node* find(pid_t data) {

   //start from the first link
   struct node* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->data != data) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}

//delete a link with given key
struct node* delete(pid_t data) {

   //start from the first link
   struct node* current = head;
   struct node* previous = NULL;
	
   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->data != data) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}

void libera(struct node* n){
   if(n != NULL){
      free(n);
   }
   n = NULL;
}