/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:13:38 by taung             #+#    #+#             */
/*   Updated: 2025/03/06 22:01:55 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void	main_helper(t_data *data, t_parsing *parsing_data)
{
	init_data(data, *parsing_data);
	init_philo(data, *parsing_data);
	pthread_create(&data->monitor_thread, NULL, monitor, data->philos);
}

/**
 * Main function.
 * Validates input arguments, initializes data and philosophers, and starts
 * the simulation.
 * Creates threads for the monitor and each philosopher, then waits for them
 * to finish.
 * Cleans up resources after the simulation ends.
 */
int	main(int argc, char **argv)
{
	int			i;
	t_parsing	parsing_data;
	t_data		data;

	if (input_vali(argc, argv, &parsing_data))
	{
		main_helper(&data, &parsing_data);
		i = 0;
		while (i < parsing_data.num_philos)
		{
			pthread_create(&data.philo_threads[i], NULL, philosopher,
				&data.philos[i]);
			i++;
			usleep(500);
		}
		pthread_join(data.monitor_thread, NULL);
		i = 0;
		while (i < parsing_data.num_philos)
		{
			pthread_join(data.philo_threads[i++], NULL);
		}
		clean_up(&data, &parsing_data);
	}
	return (0);
}
