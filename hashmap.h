#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode {
        char* word;
        char* document_id;
        int num_occurrences;
        struct llnode* next;
};

struct hashmap {
        struct llnode** map;
        int num_buckets;
        int num_elements;
};

struct hashmap* hm_create();
 int hm_get(struct hashmap* hm, char* word, char* document_id);
 void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurances);
 void hm_remove(struct hashmap* hm, char* word);
 void hm_destroy(struct hashmap* hm);
 int hash(struct hashmap* hm, char* word, char* document_id);
 void readFile(struct hashmap* hashMap, char* fileName);
 void hm_print(struct hashmap* hm);

#endif
