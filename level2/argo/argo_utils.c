#include "argo.h"

int peek(FILE *stream)
{
    int c = getc(stream);
    ungetc(c, stream);
    return c;
}

int accept(FILE *stream, char c)
{
    if (peek(stream) == c)
    {
        (void)getc(stream);
        return 1;
    }
    return 0;
}

void    unexpected(FILE *stream)
{
    if (peek(stream) != EOF)
        printf("unexpexted token '%c' \n", peek(stream));
    else
        printf("unexpexted end of input\n");
}

int     expect(FILE *stream, char c)
{
    if (accept(stream, c))
        return (1);
    unexpected(stream);
    return (0);
}

void	free_json(json j)
{
	switch (j.type) {
		case MAP:
			for (size_t i = 0; i < j.map.size; i++)
			{
				free(j.map.data[i].key);
				free_json(j.map.data[i].value);
			}
			free(j.map.data);
			break;
		case STRING:
			free(j.string);
			break;
		default:
			break;
	}
}