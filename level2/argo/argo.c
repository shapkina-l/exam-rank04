#include "argo.h"

static int  g_error = 0;
static int  g_error_no_key = 0;

json    parse_map(FILE *stream)
{
    json    json_map = {.type = MAP, .map = {.data = NULL, .size = 0}};
    if (!accept(stream, '{'))
    {
        g_error = 1;
        return (json_map);
    }
    do
    {
        pair new_pair;
        new_pair.key = parse_string(stream);
        if (g_error)
        {
            g_error_no_key = 1;
            free(new_pair.key);
            return (json_map);
        }
        if (!accept(stream, ':'))
        {
            g_error = 1;
            return (json_map);
        }
        new_pair.value = parse_json(stream);
        if (g_error)
        {
            free(new_pair.key);
            free_json(new_pair.value);
            return (json_map);
        }
        json_map.map.size++;
        json_map.map.data = realloc(json_map.map.data, json_map.map.size * sizeof(pair));
        json_map.map.data[json_map.map.size - 1] = new_pair;
    } while (accept(stream, ','));
    if (!accept(stream, '}'))
    {
        g_error = 1;
        return (json_map);
    }
    return (json_map);
}

char *parse_string(FILE *stream)
{
    char    *res;
    char    cur_char;
    int     str_len;

    res = malloc(sizeof(char));
    res[0] = '\0';
    str_len = 0;
    if (!accept(stream, '"'))
    {
        g_error = 1;
        return (res);
    }
    while (peek(stream) != '"' && peek(stream) != EOF && !g_error)
    {
        str_len++;
        res = realloc(res, (str_len + 1) * sizeof(char));
        res[str_len] = '\0';
        cur_char = getc(stream);
        if (cur_char == '\\')
        {
            if (peek(stream) != '\\' && peek(stream) != '\"')
            {
                g_error = 1;
                return (res);
            }
            cur_char = getc(stream);
        }
        res[str_len - 1] = cur_char;
    }
    if (!accept(stream, '"'))
    {
        g_error = 1;
    }
    return (res);
}

json    parse_string_json(FILE *stream)
{
    json json_string;

    json_string.type = STRING;
    json_string.string = parse_string(stream);
    return (json_string);
}
json    parse_number(FILE *stream)
{
    json    num_json;
    int     res;
    int     ret_fscanf;

    num_json.type = INTEGER;
    num_json.integer = -42;
    ret_fscanf = fscanf(stream, "%d", &res);
    if (ret_fscanf <= 0 || ret_fscanf == EOF)
    {
        g_error = 1;
        return (num_json);
    }
    num_json.integer = res;
    return (num_json);
}
json    parse_json(FILE *stream)
{
    json    nothing;

    if (peek(stream) == '"')
        return (parse_string_json(stream));
    if (peek(stream) == '{')
        return (parse_map(stream));
    if (peek(stream) == '-' || isdigit(peek(stream)))
        return (parse_number(stream));
    nothing.type = STRING;
    nothing.string = NULL;
    g_error = 1;
    return (nothing);
}

int argo(json *dst, FILE *stream)
{
    if (!stream)
        return (-1);
    *dst = parse_json(stream);
    if (g_error_no_key)
    {
        return (-1);
    }
    if (g_error)
    {
        unexpected(stream);
        return (-1);
    }
    return (1);
}