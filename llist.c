
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

struct LList* createLList() {
  /* Create an empty linked list. */
  /* DO NOT MODIFY THIS CODE */
  struct LList* list = malloc(sizeof(struct LList));
  list->head = NULL;
  return list;
}

void printList(struct LList *list){
  /* Print a list from head to tail. */
  /* DO NOT MODIFY THIS CODE */
  struct LNode *entry;
  if(list == NULL) {
    printf("List has not been initialized!\n");
    return;
  }
  entry = list->head;
  printf("List Contents:\n");
  while(entry != NULL)
  {
    printf("  %s is at %f\n", entry->name, entry->latitude);
    entry = entry->next;
  }
}

int addNode(struct LList *list, char* name, double lat) {
    /*  Add a new entry to the linked list.
     Return the index number of the new entry
     */
    int static indexCounter = 0;
    struct LNode* newNode;
    if(list == NULL){ //if the list hasnt been created output an error message
      printf("You need to create your list!");
    }
    if(list->head == NULL){ //if the head has not been intialized add a head node
        newNode = (struct LNode*) malloc(sizeof(struct LNode));
        newNode->name = malloc((sizeof(char) * strlen(name)) + 1);
        strcpy(newNode->name, name);
	newNode->latitude = lat; newNode->next = NULL;
        list->head = newNode;
        indexCounter++;
        return indexCounter;
    } else { //use an iterator node to iterate to the end of the list
        struct LNode* iterator = list->head;
        while(iterator->next != NULL){
          if(iterator->name == name){
            return -1;
          }
            iterator = iterator->next;
        }

        //Add a node to the end of the list
        newNode = (struct LNode*) malloc(sizeof(struct LNode));
        newNode->name = name; newNode->latitude = lat; newNode->next = NULL;
        iterator->next = newNode;
        indexCounter++;
        return indexCounter;
      }

}

void adjustNodeData(struct LList *list, char* name, double lat){
	struct LNode *iterator = list->head;
	while(iterator){
		if(strcmp(iterator->name, name) == 0){
			iterator->latitude = iterator->latitude + lat; //if we find the node with the same, increment the existing data
			return;	
		} 
						
		if(iterator->next){
			iterator = iterator->next;
		} else {
			return; //if the next node does not exist return out of the program
		}
	
	}
		
}

float get(struct LList *list, char* name){
	struct LNode* iterator = list->head;
	while(iterator != NULL){
		if(strcmp(iterator->name, name) == 0){
			return iterator->latitude;		
		}
		if(iterator->next != NULL){
			iterator = iterator->next;
		} else {
			break;
		}			
	}

	return 0;
}



int remNode(struct LList *list, char* name){
    /*  Removes the element named "name" from the list, if it exists.
     Returns the index of the element removed.
     */
    int counter = 1;
    //If the list has not been created or does not have a head node
    if((list == NULL) || (list->head == NULL)) {
        printf("List has not been initialized!\n");
        return -1;
    }

    struct LNode* iterator = list->head; //intialize the iterator node to the first node
    struct LNode* prev;
    if(strcmp(iterator->name, name) == 0){ //if we are deleting the first node
        list->head = iterator->next;
        free(iterator);
        return counter;
    }
    prev = list->head;
    iterator = iterator->next;
    while(strcmp(iterator->name, name)){
 	prev = iterator;
	iterator = iterator->next;
	counter++;
	if(iterator->next == NULL){
		//printf("name does not exist\n");
	}
    }
   
    prev->next = iterator->next;
    free(iterator);
    counter++;
    return counter;   
    
 } 

int clearList(struct LList *list) {
  /* Delete list and free all of the memory associate with its LNodes. */
  struct LNode* iterator = list->head; //create an iterator node
  struct LNode* next; //create a next node to keep to track of the last element
  while(iterator != NULL){
    next = iterator; //increase the position of next
    iterator = iterator->next; //move iterator to the next node
    free(next); //delete the node
  }
  free(list); //reset the actual list
  return 0;
}
