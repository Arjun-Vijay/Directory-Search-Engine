#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashmap.h"
#include "llist.h"
#include "glob.h"

struct hashmap *stop_word2(struct hashmap* hm, float num_documents, struct LList* idf_scores){
  float document_frequency = 0;
  float idf_score = 0;
  int i;
  struct llnode *temp;
  temp = (struct llnode*) malloc(sizeof(struct llnode));

  for(i = 0; i < hm->num_buckets; i++){
    struct llnode *evaluated_node = hm->map[i];
    while(evaluated_node){
      temp = evaluated_node;
      char* evaluated_word = evaluated_node->word; //the current word we are testing

      int j;
      for(j = 0; j < hm->num_buckets; j++){
        struct llnode *iterator = hm->map[j];
        while(iterator){
          if(strcmp(evaluated_word, iterator->word) == 0){
              document_frequency++; //count all occurance of the word we are testing
          }

          if(iterator->next){
            iterator=iterator->next;
          } else {
            break; //We have now obtained the DF score for a given word. Now we must break to calculate it's IDF score
          }
        }
      }

      /*
      at this point in the code we know how many documents each word appears in
      Now we must use this to calculate the IDF score for each word and place the valid IDF into a linked list
      */
      idf_score = log10((num_documents/document_frequency));
      if(idf_score == 0.0){
        temp = evaluated_node;
        hm_remove(hm, evaluated_word); //if the IDF score for the given word is equal to 0 remove all occurances of that word from the hashmap
      } else {
	addNode(idf_scores, evaluated_word, idf_score); //if the IDF score for a given word is not equal to 0 add to list of IDF scores 	
      }

     //reset variables for calculation of next word
      document_frequency = 0;
      if(temp->next){
        evaluated_node = temp->next;
      } else {
        break;
      }
    }

  }
  free(temp);
  return hm;
}

void read_query(int num_docs, char* documents[], struct hashmap *hashMap, struct LList* idf_scores ){
  float idf;
  float tf;
  float tfidf_score = 0;
  int i;
  
 //create a linked list to store similiarity scores for each word typed
  struct LList *similarity_scores = createLList();
  for(i = 0; i< num_docs; i++){
	addNode(similarity_scores, documents[i], tfidf_score);	//Each node represents a document. Thus add one node for each document present	
  }

  char aword[20]; //create memory to store user word 
  char *word = aword; //pointer to memory
  while(1){
     printf("Enter a query word(Enter '.' to stop):  ");
     scanf("%s", word);
     if(strcmp(word, ".") == 0){ //If the user enters a period exit the query process
        break;
     }  
     /*
     The next block of code calculates TF-IDF score for a given document and then adds the caluclated score to the 
     already existing score. This allows us to maintain a running total of all TF-IDF scores
     */ 
     int j;
     for(j = 0; j< num_docs; j++){
        idf = get(idf_scores, word);
        tf = hm_get(hashMap, word, documents[j]);
        tfidf_score = idf * tf;
        adjustNodeData(similarity_scores, documents[j], tfidf_score); //Increment a documents' existing TF-IDF score by the one just calculated.   
       }
  }
  
  rank(similarity_scores);
  clearList(similarity_scores);  
}

/*
I rank my documents by iterating through my similarity_scores list and printing out the name of the document with the highest score
I then remove that node from the list and repeat the process until all documents have been printed
*/ 
void rank(struct LList* similarity_scores){
	struct LNode* highest_value;
	struct LNode* iterator;
	iterator = similarity_scores->head;
	highest_value = iterator;
	while(iterator){
		if(iterator->next){
		   iterator = iterator->next;
	           if(highest_value->latitude < iterator->latitude){
			highest_value = iterator; //set highest_value to equal to the node with the highest TF-IDF score	
		   }
		}else {
		   //printList(similarity_scores);  uncomment to check validity of TF-IDF scores
		   //print the highest scoring and node and the delete that node 
		   printf("%s has a TF-IDF score of: %f\n", highest_value->name, highest_value->latitude);
		   remNode(similarity_scores, highest_value->name);

		   //reset the value of the iterater and highest_value nodes
		   iterator = similarity_scores->head;
		   highest_value = iterator;	
		}
		
	}
}

struct hashmap *training(int num_docs, struct LList* idf_scores, char* documents[], struct hashmap *hashMap){
  //read all the documents
  int i;
  for(i = 0; i < num_docs; i++){
    readFile(hashMap, documents[i]);
  }
   //readFile(hashMap, documents[0]);
   //readFile(hashMap, documents[1]);
   //readFile(hashMap, documents[2]);

  //remove the stop words
  hashMap = stop_word2(hashMap, num_docs, idf_scores);

  //return the hashmap
  return hashMap;

}

int main(int argc, char** argv){

 int i;// use as iterator for arguments
 glob_t result; //structure holding the filepath, size, and other related data
 int retval; //return value
 char* pathName[20];
 //start the for loop at 1 because argv[0] is program name
 for(i = 1; i < argc; i++){
	if(i > 1){
	  glob(argv[i], GLOB_APPEND, 0, &result); //if we have more than one file append the others
	} else {
	  glob(argv[i], 0, 0, &result); //if we only have file do not append anything
	}
 }

//create an array of documents to hold all of pathnames generated by glob
 char* documents[result.gl_pathc];
 for(i = 0; i <result.gl_pathc;i++){
	printf("%s:\n", result.gl_pathv[i]);
	documents[i] = result.gl_pathv[i];
 }

 //create an array that holds all of the documents. To test without glob comment out above lines and uncomment three lines below 
 char user_choice;
 // documents[0] = "D1.txt";
 // documents[1] = "D2.txt";
 // documents[2] = "D3.txt";
  
  //create the nessecary data structures and train the hashmap 
  struct LList* idf_scores = createLList();
  struct hashmap *hashMap= hm_create(); 
  hashMap = training(result.gl_pathc, idf_scores, documents, hashMap);

  //Query loop
  while(1) {
   printf("Enter 'S' to search or 'X' to quit: ");
   scanf(" %c", &user_choice);
   if(user_choice == 'S'){
  	read_query(result.gl_pathc,documents, hashMap,idf_scores);
   } else if (user_choice == 'X'){
	clearList(idf_scores);
	hm_destroy(hashMap);
	globfree(&result);
	break;
   } else {
	printf("Enter a valid key\n");
   } 
 }
}
