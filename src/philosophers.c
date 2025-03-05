#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_fork {
    pthread_mutex_t mutex;
    int id;
} t_fork;

typedef struct s_data {
    int num_philos;
    t_fork *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t simulation_mutex;
    int simulation_running;
    int meals_completed;
    long long start_time;  // Added to track simulation start time
} t_data;

typedef struct s_philo {
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

// Get current timestamp in milliseconds
long long get_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

// Get relative timestamp (relative to simulation start)
long long get_relative_time(long long start_time) {
    return get_time() - start_time;
}

// Print philosopher state with timestamp
void print_state(t_philo *philo, char *state) {
    pthread_mutex_lock(&philo->data->print_mutex);
    pthread_mutex_lock(&philo->data->simulation_mutex);
    if (philo->data->simulation_running) {
        // Print relative time since simulation start
        printf("%lld %d %s\n", get_relative_time(philo->data->start_time), philo->id, state);
    }
    pthread_mutex_unlock(&philo->data->simulation_mutex);
    pthread_mutex_unlock(&philo->data->print_mutex);
}

// Handle single philosopher case
void handle_single_philosopher(t_philo *philo) {
    print_state(philo, "has taken a fork");
    usleep(philo->time_to_die * 1000);
}

// Monitor thread routine
void *monitor(void *arg) {
    t_philo *philos = (t_philo *)arg;
    int simulation_running = 1;
    int all_meals_completed;

    while (simulation_running) {
        all_meals_completed = 1;
        for (int i = 0; i < philos[0].num_philos; i++) {
            // Check for death
            pthread_mutex_lock(&philos[i].data->simulation_mutex);
            if (get_time() - philos[i].last_meal_time > philos[i].time_to_die) {
                philos[i].data->simulation_running = 0;
                pthread_mutex_unlock(&philos[i].data->simulation_mutex);
                pthread_mutex_lock(&philos[i].data->print_mutex);
                printf("%lld %d died\n", get_relative_time(philos[i].data->start_time), philos[i].id);
                pthread_mutex_unlock(&philos[i].data->print_mutex);
                return NULL;
            }
            else
                pthread_mutex_unlock(&philos[i].data->simulation_mutex);

            pthread_mutex_lock(&philos[i].data->simulation_mutex);
            // Check meals completion if required
            if (philos[i].meals_to_eat != -1) {
                if (philos[i].meals_eaten < philos[i].meals_to_eat) {
                    all_meals_completed = 0;
                }
            }
            pthread_mutex_unlock(&philos[i].data->simulation_mutex);
        }

        // Check if all required meals are completed
        if (philos[0].meals_to_eat != -1 && all_meals_completed) {
            pthread_mutex_lock(&philos[0].data->simulation_mutex);
            philos[0].data->simulation_running = 0;
            pthread_mutex_unlock(&philos[0].data->simulation_mutex);
            return NULL;
        }

        pthread_mutex_lock(&philos[0].data->simulation_mutex);
        simulation_running = philos[0].data->simulation_running;
        pthread_mutex_unlock(&philos[0].data->simulation_mutex);
        // usleep((philos->time_to_die/2) * 1000);
        usleep(5000);
        // 4 410 200 200 10

    }
    return NULL;
}

// Philosopher routine
void *philosopher(void *arg) {
    t_philo *philo = (t_philo *)arg;

    // Handle single philosopher case
    if (philo->num_philos == 1) {
        handle_single_philosopher(philo);
        return NULL;
    }

    while (1) {
        pthread_mutex_lock(&philo->data->simulation_mutex);
        if (!philo->data->simulation_running) {
            pthread_mutex_unlock(&philo->data->simulation_mutex);
            // pthread_mutex_unlock(&philo->left_fork->mutex);
            // pthread_mutex_unlock(&philo->right_fork->mutex);
            break;
        }
        pthread_mutex_unlock(&philo->data->simulation_mutex);

        // Take forks
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

        // Eat
        print_state(philo, "is eating");
        pthread_mutex_lock(&philo->data->simulation_mutex);
        philo->last_meal_time = get_time();
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->data->simulation_mutex);
        usleep(philo->time_to_eat * 1000);

        // Release forks
        pthread_mutex_unlock(&philo->left_fork->mutex);
        pthread_mutex_unlock(&philo->right_fork->mutex);

        // Sleep
        print_state(philo, "is sleeping");
        usleep(philo->time_to_sleep * 1000);

        // Think
        print_state(philo, "is thinking");
        int time_to_think = ((philo->time_to_die - (philo->time_to_eat + philo->time_to_sleep))/2);
        if (time_to_think > 0) {
            usleep(time_to_think * 1000);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 5 && argc != 6) {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return 1;
    }

    int num_philos = atoi(argv[1]);
    int time_to_die = atoi(argv[2]);
    int time_to_eat = atoi(argv[3]);
    int time_to_sleep = atoi(argv[4]);
    int meals_to_eat = (argc == 6) ? atoi(argv[5]) : -1;

    if (num_philos <= 0 || time_to_die <= 0 || time_to_eat <= 0 || time_to_sleep <= 0 || (argc == 6 && meals_to_eat <= 0)) {
        printf("Invalid arguments\n");
        return 1;
    }

    // Initialize shared data
    t_data data;
    data.num_philos = num_philos;
    data.simulation_running = 1;
    data.meals_completed = 0;
    pthread_mutex_init(&data.print_mutex, NULL);
    pthread_mutex_init(&data.simulation_mutex, NULL);

    // Record simulation start time
    data.start_time = get_time();

    // Initialize forks
    data.forks = malloc(sizeof(t_fork) * num_philos);
    for (int i = 0; i < num_philos; i++) {
        pthread_mutex_init(&data.forks[i].mutex, NULL);
        data.forks[i].id = i;
    }

    // Initialize philosophers
    t_philo *philos = malloc(sizeof(t_philo) * num_philos);
    pthread_t *philo_threads = malloc(sizeof(pthread_t) * num_philos);

    for (int i = 0; i < num_philos; i++) {
        philos[i].id = i + 1;
        philos[i].num_philos = num_philos;
        philos[i].time_to_die = time_to_die;
        philos[i].time_to_eat = time_to_eat;
        philos[i].time_to_sleep = time_to_sleep;
        philos[i].meals_to_eat = meals_to_eat;
        philos[i].meals_eaten = 0;
        philos[i].last_meal_time = get_time();  // Initialize with current time
        philos[i].left_fork = &data.forks[i];
        philos[i].right_fork = &data.forks[(i + 1) % num_philos];
        philos[i].data = &data;
    }

    // Create monitor thread
    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, monitor, philos);

    // Create philosopher threads
    for (int i = 0; i < num_philos; i++) {
        pthread_create(&philo_threads[i], NULL, philosopher, &philos[i]);
    }

    // Wait for monitor thread to finish
    pthread_join(monitor_thread, NULL);

    // Wait for philosopher threads to finish
    for (int i = 0; i < num_philos; i++) {
        pthread_join(philo_threads[i], NULL);
    }

    // Cleanup
    for (int i = 0; i < num_philos; i++) {
        pthread_mutex_destroy(&data.forks[i].mutex);
    }
    pthread_mutex_destroy(&data.print_mutex);
    pthread_mutex_destroy(&data.simulation_mutex);
    free(data.forks);
    free(philos);
    free(philo_threads);

    return 0;
}
