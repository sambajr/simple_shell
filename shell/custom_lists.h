#ifndef CUSTOM_LISTS_H
#define CUSTOM_LISTS_H

struct Node {
    char *data;
    struct Node *next;
};

void insertNode(struct Node **head, char *data);
void printList(struct Node *head);
void freeList(struct Node *head);

#endif
