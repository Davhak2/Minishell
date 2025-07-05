#include "parser.h"

t_node	*simple_command(void)
{
}

t_node	*parse_parenthesis(t_token **list)
{
}

t_node	*parse_pipe(t_token **list)
{
}

t_node	*parse_and(t_token **list)
{
}

t_node	*parse_or(t_token **list)
{
}

t_node	*parse(t_token **list)
{
	return (parse_or(list));
}