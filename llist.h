#ifndef LL_H
#define LL_H
/* The code above is magic to prevent this library from being included more than once */

typedef struct LList
{
  struct LNode* head;
};

struct LNode {
  char* name;
  float latitude;
  double value;
  struct LNode* next;
};

struct LList* createLList();

void printList(struct LList *list);

int addNode(struct LList *list, char* name, double lat);

int remNode(struct LList *list, char* name);

int clearList(struct LList *list);

float get(struct LList *list, char* name);

void adjustNodeData(struct LList *list, char* name, double lat);
#endif
