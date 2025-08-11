#ifndef VBC_H
#define VBC_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

typedef struct node {
    enum {ADD, MULTI, VAL} type;
    int val;
    struct node *left;
    struct node *right;
} node;

node *parse_addition(char **s);

#endif