/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 14:45:48 by taung             #+#    #+#             */
/*   Updated: 2024/12/08 14:49:39 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../headers/philosopher.h"

void	print_data(t_input_data data)
{
	printf("==============================================\n");
	printf("Number of philosophers: %d\n", data.number_of_philosophers);
	printf("Time to die: %d\n", data.time_to_die);
	printf("Time to eat: %d\n", data.time_to_eat);
	printf("Time to sleep: %d\n", data.time_to_sleep);
	printf("Number of times each philosopher must eat: %d\n", data.number_of_times_each_philosopher_must_eat);
	printf("==============================================\n");
}
