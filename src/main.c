/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 13:52:46 by taung             #+#    #+#             */
/*   Updated: 2024/12/08 14:55:03 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosopher.h"

void	init_inputs(t_input_data *data, int argc, char **argv)
{
	if (argc == 5)
	{
		data->number_of_philosophers = ft_atoi((const char*)argv[1]);
		data->time_to_die = ft_atoi((const char*)argv[2]);
		data->time_to_eat = ft_atoi((const char*)argv[3]);
		data->time_to_sleep = ft_atoi((const char*)argv[4]);
		data->number_of_times_each_philosopher_must_eat = 0;
		return;
	}
	data->number_of_philosophers = ft_atoi((const char*)argv[1]);
	data->time_to_die = ft_atoi((const char*)argv[2]);
	data->time_to_eat = ft_atoi((const char*)argv[3]);
	data->time_to_sleep = ft_atoi((const char*)argv[4]);
	data->number_of_times_each_philosopher_must_eat = ft_atoi((const char*)argv[5]);
}

int	main(int argc, char **argv)
{
	t_input_data	data;
	int				i;

	if (argc == 5 || argc == 6)
	{
		init_inputs(&data, argc, argv);
		print_data(data);
		if (argc == 5)
		{
			printf("do smth with 4 args\n");

		}
		else if (argc == 6)
		{
			printf("do smth with 5 args\n");
		}
	}
	else
		printf("\x1B[31mInput Error\nMust be 5 <= Inputs <= 6\n");
	return (0);
}