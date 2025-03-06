#include "../header/philo.h"


long long get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

bool	input_vali(int argc, char **argv, t_parsing *parsing_data)
{
    parsing_data->meals_to_eat = -1;
	if (argc < 5 || argc > 6 || !ft_atoi_vali(argv[1], &parsing_data->num_philos) || \
		!ft_atoi_vali(argv[2], &parsing_data->time_to_die) || \
        !ft_atoi_vali(argv[3], &parsing_data->time_to_eat) || \
		!ft_atoi_vali(argv[4], &parsing_data->time_to_sleep) || \
		(argc == 6 && !ft_atoi_vali(argv[5], &parsing_data->meals_to_eat)))
	{
        printf("Theses are the required arguments : \n"\
				"\tphilosopher count (integer)\n"\
				"\ttime to die (milliseconds)\n\ttime to eat (milliseconds)\n"\
				"\ttime to sleep (milliseconds)\n"\
				"\tnumber of times each philosopher must eat (optional)\n"\
				"\tall input must be positive\nInvalid input!\n");
		return (false);
	}
	return (true);
}

long long get_relative_time(long long start_time)
{
    return get_time() - start_time;
}


void print_state(t_philo *philo, char *state)
{
    pthread_mutex_lock(&philo->data->print_mutex);
    pthread_mutex_lock(&philo->data->simulation_mutex);
    if (philo->data->simulation_running)
    {
        printf("%lld %d %s\n", get_relative_time(philo->data->start_time), philo->id, state);
    }
    pthread_mutex_unlock(&philo->data->simulation_mutex);
    pthread_mutex_unlock(&philo->data->print_mutex);
}

// Handle single philosopher case
void handle_single_philosopher(t_philo *philo)
{
    print_state(philo, "has taken a fork");
    usleep(philo->time_to_die * 1000);
}

// Check if a philosopher has died
bool check_philosopher_death(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->simulation_mutex);
    if (get_time() - philo->last_meal_time > philo->time_to_die) {
        philo->data->simulation_running = 0;
        pthread_mutex_unlock(&philo->data->simulation_mutex);
        pthread_mutex_lock(&philo->data->print_mutex);
        printf("%lld %d died\n", get_relative_time(philo->data->start_time), philo->id);
        pthread_mutex_unlock(&philo->data->print_mutex);
        return true;
    }
    pthread_mutex_unlock(&philo->data->simulation_mutex);
    return false;
}

// Check if all philosophers have completed their meals
bool check_meals_completion(t_philo *philos)
{
    int i = 0;
    bool all_completed = true;

    if (philos[0].meals_to_eat == -1)
        return false;

    while (i < philos[0].num_philos)
    {
        pthread_mutex_lock(&philos[i].data->simulation_mutex);
        if (philos[i].meals_to_eat != -1 && philos[i].meals_eaten < philos[i].meals_to_eat) {
            all_completed = false;
        }
        pthread_mutex_unlock(&philos[i].data->simulation_mutex);
        i++;
    }
    return all_completed;
}

// Set simulation status to stop
void stop_simulation(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->simulation_mutex);
    philo->data->simulation_running = 0;
    pthread_mutex_unlock(&philo->data->simulation_mutex);
}

// Get current simulation status
bool get_simulation_status(t_philo *philo)
{
    bool status;

    pthread_mutex_lock(&philo->data->simulation_mutex);
    status = philo->data->simulation_running;
    pthread_mutex_unlock(&philo->data->simulation_mutex);

    return status;
}

// Monitor thread routine
void *monitor(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    int i;
    bool simulation_running = true;

    while (simulation_running) {
        i = 0;
        while (i < philos[0].num_philos)
        {
            if (check_philosopher_death(&philos[i])) {
                return NULL;
            }
            i++;
        }

        if (check_meals_completion(philos)) {
            stop_simulation(&philos[0]);
            return NULL;
        }

        simulation_running = get_simulation_status(&philos[0]);
        usleep(5000);
    }
    return NULL;
}

// Check if simulation is still running
bool is_simulation_running(t_philo *philo)
{
    bool running;

    pthread_mutex_lock(&philo->data->simulation_mutex);
    running = philo->data->simulation_running;
    pthread_mutex_unlock(&philo->data->simulation_mutex);

    return running;
}

// Take forks based on philosopher's ID to prevent deadlock
void take_forks(t_philo *philo)
{
    if (philo->id == philo->num_philos - 1)
    {
        pthread_mutex_lock(&philo->right_fork->mutex);
        print_state(philo, "has taken a fork");
        pthread_mutex_lock(&philo->left_fork->mutex);
        print_state(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&philo->left_fork->mutex);
        print_state(philo, "has taken a fork");
        pthread_mutex_lock(&philo->right_fork->mutex);
        print_state(philo, "has taken a fork");
    }
}

// Eat and track meal status
void eat_meal(t_philo *philo)
{
    print_state(philo, "is eating");

    pthread_mutex_lock(&philo->data->simulation_mutex);
    philo->last_meal_time = get_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->data->simulation_mutex);

    usleep(philo->time_to_eat * 1000);

    // Release forks
    pthread_mutex_unlock(&philo->left_fork->mutex);
    pthread_mutex_unlock(&philo->right_fork->mutex);
}

// Sleep phase
void sleep_phase(t_philo *philo)
{
    print_state(philo, "is sleeping");
    usleep(philo->time_to_sleep * 1000);
}

// Think phase with calculated thinking time
void think_phase(t_philo *philo)
{
    print_state(philo, "is thinking");

    int time_to_think = ((philo->time_to_die -
                         (philo->time_to_eat + philo->time_to_sleep)) / 2);

    if (time_to_think > 0)
    {
        usleep(time_to_think * 1000);
    }
}

// Philosopher routine
void *philosopher(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    // Handle single philosopher case
    if (philo->num_philos == 1) {
        handle_single_philosopher(philo);
        return NULL;
    }

    while (1) {
        if (!is_simulation_running(philo)) {
            break;
        }

        take_forks(philo);
        eat_meal(philo);
        sleep_phase(philo);
        think_phase(philo);
    }
    return NULL;
}

void    init_data(t_data *data, t_parsing parsing_data)
{
    int i;

    data->num_philos = parsing_data.num_philos;
    data->simulation_running = 1;
    data->meals_completed = 0;
    pthread_mutex_init(&data->print_mutex, NULL);
    pthread_mutex_init(&data->simulation_mutex, NULL);
    data->start_time = get_time();
    data->forks = malloc(sizeof(t_fork) * parsing_data.num_philos);
    i = 0;
    while (i < parsing_data.num_philos)
    {
        pthread_mutex_init(&data->forks[i].mutex, NULL);
        data->forks[i].id = i;
        i++;
    }
}

void    init_philo(t_data *data, t_parsing parsing_data)
{
    int i;

    // Initialize philosophers
    data->philos = malloc(sizeof(t_philo) * parsing_data.num_philos);
    data->philo_threads = malloc(sizeof(pthread_t) * parsing_data.num_philos);
    i = 0;
    while (i < parsing_data.num_philos)
    {
        data->philos[i].id = i + 1;
        data->philos[i].num_philos = parsing_data.num_philos;
        data->philos[i].time_to_die = parsing_data.time_to_die;
        data->philos[i].time_to_eat = parsing_data.time_to_eat;
        data->philos[i].time_to_sleep = parsing_data.time_to_sleep;
        data->philos[i].meals_to_eat = parsing_data.meals_to_eat;
        data->philos[i].meals_eaten = 0;
        data->philos[i].last_meal_time = get_time();
        data->philos[i].left_fork = &data->forks[i];
        data->philos[i].right_fork = &data->forks[(i + 1) % parsing_data.num_philos];
        data->philos[i].data = data;
        i++;
    }
}

void    clean_up(t_data *data, t_parsing *parsing_data)
{
    int i;

    // Cleanup
    i = 0;
    while (i < parsing_data->num_philos)
    {
        pthread_mutex_destroy(&data->forks[i].mutex);
        i++;
    }
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->simulation_mutex);
    free(data->forks);
    free(data->philos);
    free(data->philo_threads);
}

int main(int argc, char **argv)
{
    int i;
    t_parsing   parsing_data;
    t_data data;

    if (input_vali(argc, argv, &parsing_data))
    {
        init_data(&data, parsing_data);
        init_philo(&data, parsing_data);
        pthread_create(&data.monitor_thread, NULL, monitor, data.philos);
        i = 0;
        while ( i < parsing_data.num_philos)
        {
            pthread_create(&data.philo_threads[i], NULL, philosopher, &data.philos[i]);
            i++;
        }
        pthread_join(data.monitor_thread, NULL);
        i = 0;
        while(i < parsing_data.num_philos)
        {
            pthread_join(data.philo_threads[i], NULL);
            i++;
        }
        clean_up(&data, &parsing_data);
    }
    return 0;
}
