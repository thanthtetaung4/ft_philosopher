#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>

typedef struct s_fork
{
    pthread_mutex_t mutex;
    int id;
} t_fork;

typedef struct s_parsing
{
	int num_philos;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int meals_to_eat;
}	t_parsing;

typedef struct s_data t_data;

typedef struct s_philo
{
    int id;
    int num_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int meals_to_eat;
    int meals_eaten;
    long long last_meal_time;
    t_fork *left_fork;
    t_fork *right_fork;
    t_data *data;
} t_philo;

typedef struct s_data
{
    int num_philos;
    t_fork *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t simulation_mutex;
    int simulation_running;
    int meals_completed;
    long long start_time;
	pthread_t *philo_threads;
    pthread_t monitor_thread;
	t_philo *philos;
} t_data;



bool	ft_atoi_vali(const char *nptr, int *result);


#endif
