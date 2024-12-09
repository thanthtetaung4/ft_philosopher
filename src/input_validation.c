/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 22:15:03 by taung             #+#    #+#             */
/*   Updated: 2024/12/09 23:18:31 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosopher.h"

int	is_number(char *num)
{
	int	i;

	i = 0;
	if (num[0] == '-' || num[0] == '+')
		i++;
	if (!(num[i] >= '0' && num[i] <= '9'))
		return (0);
	while (num[i])
	{
		if (!(num[i] >= '0' && num[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	is_limit(char *num)
{
	long	numb;

	numb = ft_atol((const char *)num);
	if (numb >= 1 && numb <= 2147483647)
		return (1);
	return (0);
}

int	validation_helper(char **inputs, int (*validation_func)(char *))
{
	int		i;
	int		j;
	char	**numbers;

	i = 1;
	while (inputs[i])
	{
		if (!validation_func(inputs[i]))
			return (0);
		i++;
	}
	return (1);
}
int	validation(char **inputs)
{
	if (!validation_helper(inputs, &is_number))
	{
		printf("\x1B[31mError\nInputs must be numbers\n");
		return (0);
	}
	if (!validation_helper(inputs, &is_limit))
	{
		printf("\x1B[31mError\n0 < Input < 2147483648\n");
		return (0);
	}
	return (1);
}
