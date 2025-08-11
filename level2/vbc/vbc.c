#include "vbc.h"

node *new_node(node n)
{
    node *ret = calloc(1, sizeof(node));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void destroy_tree(node *tree)
{
    if (tree)
    {
        if (tree->type != VAL)
        {
            destroy_tree(tree->left);
            destroy_tree(tree->right);
        }
        free(tree);
    }
}


int	eval_tree(node *tree)
{
	switch (tree->type)
	{
		case ADD:
			return (eval_tree(tree->left) + eval_tree(tree->right));
		case MULTI:
			return (eval_tree(tree->left) * eval_tree(tree->right));
		case VAL:
			return tree->val;
	}
	return 0;
}


void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int	expect(char **s, char c)
{
	if (accept(s, c))
		return 1;
	unexpected(**s);
		return 0;
}

node *parse_numbers_parentheses(char **s)
{
    if(accept(s, '('))
    {
        node *n = parse_addition(s);
        if(!expect(s, ')'))
        {
            destroy_tree(n);
            return (NULL);
        }
        return (n);
    }
    if (isdigit(**s))
    {
        node n = {VAL, **s - '0', NULL, NULL};
        (*s)++;
        return(new_node(n));
    }
    unexpected(**s);
    return (NULL);
}

node *parse_multiplication(char **s)
{
    node *left = parse_numbers_parentheses(s);
    while(accept(s, '*'))
    {
        node *right = parse_numbers_parentheses(s);
        if (!right)
        {
            destroy_tree(left);
            return (NULL);
        }
        node n = {MULTI, 0, left, right};
        left = new_node(n);
    }
    return (left);
}

node *parse_addition(char **s)
{
    node *left = parse_multiplication(s);
    while(accept(s, '+'))
    {
        node *right = parse_multiplication(s);
        if(!right)
        {
            destroy_tree(left);
            return (NULL);
        }
        node n = {ADD, 0, left, right};
        left = new_node(n);
    }
    return(left);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return (1);
    
    char *input = argv[1];
    node *tree = parse_addition(&input);
    if (!tree)
        return (1);
    else if(*input)
    {
        unexpected(*input);
        destroy_tree(tree);
        return(1);
    }
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
    return (0);
}