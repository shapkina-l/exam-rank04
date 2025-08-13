#ifndef ARGO_H
#define ARGO_H

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef struct json {
    enum {MAP, INTEGER, STRING} type;
    union {
        struct {
            struct pair *data;
            size_t size;
        }   map;
        int integer;
        char *string;
    };    
} json;

typedef struct pair {
    char *key;
    json    value;
} pair;

int     argo(json *dst, FILE *stream);
int     expect(FILE *stream, char c);
void    unexpected(FILE *stream);
int     accept(FILE *stream, char c);
int     peek(FILE *stream);
char    *parse_string(FILE *stream);
json    parse_json(FILE *stream);
void	free_json(json j);

#endif