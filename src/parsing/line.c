/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 12:01:01 by xortega           #+#    #+#             */
/*   Updated: 2024/06/07 14:52:45 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_blocks(char *str)
{
	int	i;
	int	count;
	int	status_1;
	int	status_2;

	i = 0;
	status_2 = 0;
	status_1 = 0;
	count = 1;
	while (str[i])
	{
		if (str[i] == '\"' && status_2 % 2 == 0)
			status_1++;
		if (str[i] == '\'' && status_1 % 2 == 0)
			status_2++;
		if ((i > 0 && str[i] != '\"' && str[i] != '\'')
			&& ((str[i - 1] == '\"' && status_1 % 2 == 0)
				|| (str[i - 1] == '\'' && status_2 % 2 == 0)))
			count++;
		i++;
	}
	count += (status_1 / 2 + status_2 / 2);
	return (count);
}

char	*clear_line_v2(char **str)
{
	char	c;
	char	*block;

	if (!quote_case(*str))
		return (*str);
	c = '\'';
	if (!ft_strchr(*str, '\'') || (ft_strchr(*str, '"')
			&& ft_strchr(*str, '"') < ft_strchr(*str, '\'')))
		c = '"';
	block = ft_substr(*str, 0,
			((ft_strchr(*str + 1, c) - *str) + (*str[0] == c)));
	*str = line_cutter(*str, block);
	return (block);
}

char	**trim_blocks(char **blocks, int n_blocks)
{
	int		i;
	char	c;
	char	**trimmed_blocks;

	trimmed_blocks = malloc(sizeof(char *) * (n_blocks + 1));
	i = 0;
	while (i < n_blocks)
	{
		c = blocks[i][0];
		if (c == '\'' || c == '\"')
		{
			if (ft_strlen(blocks[i]) > 2)
				trimmed_blocks[i] = ft_strtrim(blocks[i], &c);
			else
				trimmed_blocks[i] = ft_strdup("");
		}
		else
			trimmed_blocks[i] = ft_strdup(blocks[i]);
		i++;
	}
	trimmed_blocks[n_blocks] = NULL;
	free_array((void **)blocks);
	return (trimmed_blocks);
}

char	**get_blocks(char **str)
{
	int		i;
	int		n_blocks;
	char	**blocks;

	n_blocks = count_blocks(*str);
	blocks = malloc(sizeof(char *) * (n_blocks + 1));
	i = 0;
	while (i < n_blocks)
	{
		blocks[i] = clear_line_v2(str);
		i++;
	}
	blocks[n_blocks] = NULL;
	return (trim_blocks(blocks, n_blocks));
}

char	*make_line(char *str)
{
	char	**blocks;
	char	*temp;
	int		n_blocks;
	int		i;

	if (!quote_case(str))
		return (str);
	blocks = get_blocks(&str);
	n_blocks = 0;
	while (blocks[n_blocks])
		n_blocks++;
	temp = ft_strdup(blocks[0]);
	i = 1;
	while (i < n_blocks)
	{
		str = ft_strjoin(temp, blocks[i]);
		free(temp);
		temp = str;
		i++;
	}
	free_array((void **)blocks);
	return (str);
}
/*
int	main(void)
{
	char	*str;

	str = ft_strdup("aaa");
	ft_printf("str_before :[%s]\n", str);
	str = make_line(str);
	ft_printf("str_after :[%s]\n", str);
	//str = ft_strdup("a\"hola\"1''\"jiji\"'que'b");
	//ft_printf("\n[%s] : [%d]\n", str, count_blocks(str));
	free(str);
	return (0);
}*/