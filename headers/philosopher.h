# ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <stdio.h>
# include <stdlib.h>
# include<pthread.h>

typedef struct s_input_data {
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
}	t_input_data;

typedef struct s_fork {
	int	left;
	int	right;
}	t_fork;

typedef struct s_philo
{
	int			id;
	int			num_of_times_ate;
	long long	time_to_die;
	t_fork		fork;
	pthread_t	thread;
}	t_philo;

typedef struct s_env
{
	int				n_thread;
	int				philo_dead;
	long long		t0;
	t_input_data	input;
	t_philo			*philo;
	pthread_t		checker;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write;
}	t_env;

// Input Validation
int	validation(char **inputs);

// General Utils
int		ft_atoi(const char* nptr);
long	ft_atol(const char *nptr);

// Print Utils
void	print_data(t_input_data data);
# endif
