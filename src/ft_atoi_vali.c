/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_vali.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:13:47 by taung             #+#    #+#             */
/*   Updated: 2025/03/06 21:13:50 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

/**
 * Converts a string to an integer and validates the conversion.
 * Returns true if the conversion is successful and the result
 * is within the integer range.
 * Returns false if the string contains non-digit characters or
 * the result is out of range.
 */
bool	ft_atoi_vali(const char *nptr, int *result)
{
	long	i;
	int		sign;

	i = 0;
	sign = 1;
	while (*nptr == ' ' || (*nptr >= '\t' && *nptr <= '\r'))
		nptr++;
	if (*nptr == '+')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr)
	{
		if (*nptr >= '0' && *nptr <= '9')
			i = i * 10 + (*nptr - '0');
		else
			return (false);
		nptr++;
	}
	if ((i * sign) > INT_MAX || (i * sign) < INT_MIN)
		return (false);
	*result = i * sign;
	return (true);
}
