#include "utils.h"

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

bool	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '(' || c == ')'
		|| c == '&');
}

bool	is_word_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '-' || c == '.' || c == '/'
		|| c == '~' || c == '@' || c == '+' || c == '=' || c == ':'
		|| c == ',');
}

bool	is_quote(char c)
{
	return (c == '\'' || c == '"');
}