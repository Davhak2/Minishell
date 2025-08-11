/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ganersis <ganersis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:30:55 by letto             #+#    #+#             */
/*   Updated: 2025/08/11 21:14:38 by ganersis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_second_var(char *str)
{
	char	*first_equals;
	char	*second_equals;
	char	*var_name;
	char	*result;

	first_equals = ft_strchr(str, '=');
	if (!first_equals)
		return (NULL);
	second_equals = ft_strchr(first_equals + 1, '=');
	if (!second_equals)
		return (NULL);
	var_name = ft_substr(str, first_equals - str + 1, second_equals
			- first_equals - 1);
	if (!var_name)
		return (NULL);
	result = ft_strjoin(var_name, second_equals);
	free(var_name);
	return (result);
}

int	is_valid_second_var(char *str)
{
	char	*var_name;
	int		result;

	if (!str)
		return (0);
	var_name = get_var_name(str);
	if (!var_name)
		return (0);
	result = is_valid_identifier(var_name);
	free(var_name);
	return (result);
}
