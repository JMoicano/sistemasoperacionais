#ifndef __LIST_H__
#define __LIST_H__

typedef struct node* List;

pid_t getData(struct node* n);

void insertFirst(pid_t data);

int isEmpty();

int length();

struct node* find(pid_t data);

struct node* delete(pid_t data);

struct node* deleteFirst();

#endif