/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:59:54 by taung             #+#    #+#             */
/*   Updated: 2025/03/06 21:59:55 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_fork
{
	pthread_mutex_t		mutex;
	int					id;
}						t_fork;

typedef struct s_parsing
{
	int					num_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					meals_to_eat;
}						t_parsing;

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_to_eat;
	int				meals_eaten;
	long long		last_meal_time;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_data			*data;
	pthread_mutex_t	meal_mutex;
}					t_philo;

typedef struct s_data
{
	int					num_philos;
	t_fork				*forks;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		simulation_mutex;
	int					simulation_running;
	int					meals_completed;
	long long			start_time;
	pthread_t			*philo_threads;
	pthread_t			monitor_thread;
	t_philo				*philos;
}						t_data;

void					init_data(t_data *data, t_parsing parsing_data);
void					init_philo(t_data *data, t_parsing parsing_data);
void					clean_up(t_data *data, t_parsing *parsing_data);
void					print_state(t_philo *philo, char *state);
bool					input_vali(int argc, char **argv,
							t_parsing *parsing_data);
bool					ft_atoi_vali(const char *nptr, int *result);
bool					check_philosopher_death(t_philo *philo);
void					handle_single_philosopher(t_philo *philo);
bool					check_meals_completion(t_philo *philos);
void					stop_simulation(t_philo *philo);
bool					get_simulation_status(t_philo *philo);
void					*philosopher(void *arg);
void					*monitor(void *arg);
bool					is_simulation_running(t_philo *philo);
void					take_forks(t_philo *philo);
void					eat_meal(t_philo *philo);
void					sleep_phase(t_philo *philo);
void					think_phase(t_philo *philo);
long long				get_relative_time(long long start_time);
void					print_state(t_philo *philo, char *state);
long long				get_time(void);

#endif
