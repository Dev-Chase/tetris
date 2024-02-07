#ifndef DBL_LINKED_LIST_H
#define DBL_LINKED_LIST_H

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct Node {
    void *val;
    struct Node *next;
    struct Node *prev;
} DblNode;

typedef struct DblLinkedList {
    DblNode *head;
    DblNode *tail;
    size_t len;
    size_t node_val_size;
} DblLinkedList;

DblLinkedList new_dbl_linked_list(void);
void init_dbl_linked_list(DblLinkedList *list, void *val, size_t size);
void delete_dbl_linked_list(DblLinkedList *list);
void print_dbl_linked_list(DblLinkedList *list);
void insert_dbl_node(DblLinkedList *list, size_t ind, void *val);
void insert_dbl_node_at_head(DblLinkedList *list, void *val);
void append_dbl_node(DblLinkedList *list, void *val);
void del_dbl_node_i(DblLinkedList *list, uint_fast32_t ind);
DblNode *get_dbl_node(DblLinkedList *list, size_t ind);
void *get_dbl_node_val(DblLinkedList *list, size_t ind);

#ifdef __cplusplus
}
#endif

#endif
