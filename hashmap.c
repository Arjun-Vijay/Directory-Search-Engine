#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"


/*CREATE EXPLANATION
create hashmap with alloted number of num_buckets
set hashmap->map equal to the first node in the list
iterator through the new list
*/
struct hashmap *hm_create(){
  int num_buckets;
  printf("Enter your desired number of buckets: ");
  scanf("%d", &num_buckets);

  struct hashmap *hashMap = (struct hashmap*) malloc(sizeof(struct hashmap)); //create the actual hashmap
  hashMap->map = (struct llnode**) malloc(sizeof(struct llnode*) * num_buckets);
  hashMap->num_buckets = num_buckets;
  hashMap->num_elements = 0;
  int i;
  for(i = 0; i< num_buckets;i++){
   hashMap->map[i] = (struct llnode*) malloc(sizeof(struct llnode)); //create each node
   hashMap->map[i]->word = "";
   hashMap->map[i]->num_occurrences = 0;
   hashMap->map[i]->document_id = "";
   hashMap->map[i]->next = NULL;
  }
  return hashMap;
}

/*
call the hash function and get the key
use that key to identify where int the hashmap the value is
*/
int hm_get(struct hashmap* hm, char* word, char* document_id){
  int key = hash(hm, word, document_id);
  if(hm->map[key]){ //make sure the word exists within the hashmap
    struct llnode *iterator = hm->map[key];
    while(iterator != NULL) {
      if((strcmp(iterator->word, word) == 0) && (strcmp(iterator->document_id, document_id) == 0)){
        return iterator->num_occurrences;
      }
      if(iterator->next == NULL){
        return 0;
      }
        iterator = iterator->next;
    }
  }
return 0;
}



void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences){
  if(strcmp(word, " ") == 0){
    return;
  }

  int key = hash(hm, word, document_id);
  struct llnode *node1 = hm->map[key];
  struct llnode *iterator = node1;

  while(iterator != NULL) {
    if((strcmp(iterator->word, word) == 0) && (strcmp(iterator->document_id, document_id) == 0)){ //check for string and document equality
      iterator->num_occurrences = iterator->num_occurrences + 1;
         hm->num_elements++;
      return;
    } else {
      iterator = iterator->next;
    }
  }

  //Adding a new node
  struct llnode *newNode = (struct llnode*) malloc(sizeof(struct llnode)); //allocated space for new llnode

  newNode->word = (char*) malloc (sizeof(char) * strlen(word)+1);
  strcpy(newNode->word, word);

  newNode->document_id = (char*) malloc (sizeof(char) * strlen(document_id)+1);
  strcpy(newNode->document_id, document_id);

  newNode->num_occurrences = num_occurrences;
  hm->map[key] = newNode;
  newNode->next = node1;
  hm->num_elements++;
}


void hm_remove(struct hashmap* hm, char* word){

  //calculate the key
  //check to see if the key exists. if it does then delete that pointer, if it doesnt then return error
  //int key = hash(hm, word, document_id);
  int i;
  struct llnode *prev;
  for(i = 0; i < hm->num_buckets; i++){
    struct llnode *iterator = hm->map[i];

    if(strcmp(hm->map[i]->word, word) == 0){
      hm->map[i] = iterator->next;
      free(iterator);
      hm->num_elements--;
    } else {
      prev = iterator;
      iterator = iterator->next;
      while(iterator){
        if(strcmp(iterator->word, word) == 0){
            prev->next = iterator->next;
            hm->num_elements--;
        }

        if(iterator->next){
          prev = iterator;
          iterator = iterator->next;
        } else {
          break;
        }
      }
    }
  }
  return;
}


/*
//free all the llnodes
//delete the actual hashmap itself
*/
void hm_destroy(struct hashmap* hm){
  int i;
  for(i = 0; i < hm->num_buckets; i++){
    struct llnode *iterator = hm->map[i];
    while(iterator){
      struct llnode *temp;
      temp = iterator;
      iterator = iterator->next;
      free(temp);
    }
  }
  free(hm->map);
  free(hm);
}

void hm_print(struct hashmap* hm){
  int i;
  for(i = 0; i < hm->num_buckets; i++){
    struct llnode *iterator = hm->map[i];
    while(iterator){
      struct llnode *temp;
      temp = iterator;
      printf("Bucket: %d word: %s\n", i, temp->word);
      iterator = iterator->next;
    }
  }
}

/*
this will be our hash function
add up ascii codes of words and document.
computer key = sum mod numberOfBuckets
return key
*/
int hash(struct hashmap* hm, char* word, char* document_id){
  int sum = 0;
  int key;
  int i = 0;

  //sum up all the characters in the word and document_id
  while(word[i] != '\0'){
    sum = sum + word[i];
    i++;
  }
 i = 0;
  while(document_id[i] != '\0'){
    sum = sum + document_id[i];
    i++;
  }

//Calculate the key using the modulo operator
  key = sum % hm->num_buckets;

//return key
  return key;
}

void readFile(struct hashmap* hashMap, char* fileName){
  FILE *file = fopen(fileName, "r");
  char word[40];
  while((fscanf(file, "%s", word) != EOF)){
    if(!strcmp(word, " ")){

    } else {
      hm_put(hashMap, word, fileName, 1);
    }
  }
}                                                                                                                                                                                                                                                                                                                                                                                                                    
