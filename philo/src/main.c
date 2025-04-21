/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 20:43:38 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/21 16:19:31 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

t_all *all(void)
{
	static t_all	all;

	return (&all);
}

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

bool	philo_alive(t_philo *philo)
{
	bool	alive;
	pthread_mutex_lock(&philo->philo_mutex);
	alive = philo->status == ALIVE || (all()->num_eat > 0 && philo->meals < all()->num_eat);
	pthread_mutex_unlock(&philo->philo_mutex);
	return (alive);
}

void	print_status(t_philo *philo, t_philo_action action)
{
	size_t	timestamp;

	pthread_mutex_lock(&all()->data_mutex);
	timestamp = get_time() - all()->start_time;
	pthread_mutex_lock(&philo->philo_mutex);
	if (philo->status == DEAD && action != DIE)
	{
		pthread_mutex_unlock(&philo->philo_mutex);
		pthread_mutex_unlock(&all()->data_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->philo_mutex);
	if (action == TAKE_RIGHT_FORK || action == TAKE_LEFT_FORK)
		printf("%zu %d has taken a fork\n", timestamp, philo->id);
	if (action == EAT)
		printf("%zu %d is eating\n", timestamp, philo->id);
	if (action == SLEEP)
		printf("%zu %d is sleeping\n", timestamp, philo->id);
	if (action == THINK)
		printf("%zu %d is thinking\n", timestamp, philo->id);
	if (action == DIE)
		printf("%zu %d died\n", timestamp, philo->id);
	pthread_mutex_unlock(&all()->data_mutex);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, TAKE_RIGHT_FORK);
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, TAKE_LEFT_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, TAKE_LEFT_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, TAKE_RIGHT_FORK);
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	print_status(philo, EAT);
	philo->last_meal = get_time();
	philo->meals++;
	pthread_mutex_unlock(&philo->philo_mutex);
	usleep(all()->time_to_eat * 1000);
	
}

void	think(t_philo *philo)
{
	print_status(philo, THINK);
}

void	sleep_philo(t_philo *philo)
{
	print_status(philo, SLEEP);
	usleep(all()->time_to_sleep * 1000);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

int    cleanup_resources(void)
{
    t_list          *fork_node;
    t_list          *philo_node;
    pthread_mutex_t *fork;
    t_philo         *philo;

    fork_node = all()->forks;
    while (fork_node)
    {
        fork = fork_node->data;
        pthread_mutex_destroy(fork);
        fork_node = fork_node->next;
    }
    philo_node = all()->philos;
    while (philo_node)
    {
        philo = philo_node->data;
        pthread_mutex_destroy(&philo->philo_mutex);
        philo_node = philo_node->next;
    }
	pthread_mutex_destroy(&all()->data_mutex);
    ft_list_destroy(&all()->philos);
    ft_list_destroy(&all()->forks);
    return (1);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (all()->num_philo == 1)
    {
        think(philo);
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, TAKE_RIGHT_FORK);
        usleep(all()->time_to_die * 1000);
        return (NULL);
    }
	if (philo->id % 2 == 0)
		usleep(15000);
	while(philo_alive(philo))
	{
		think(philo);
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		sleep_philo(philo);
	}
	return (NULL);
}

void	*death_monitor(void *arg)
{
	t_list	*node;
	t_list	*p_node;
	t_philo	*philo;

	(void)arg;
	while (42)
	{
		node = all()->philos;
		while (node)
		{
			philo = node->data;
			pthread_mutex_lock(&philo->philo_mutex);
			if ((get_time() - philo->last_meal) > all()->time_to_die && philo->status == ALIVE)
			{
				philo->status = DEAD;
				pthread_mutex_unlock(&philo->philo_mutex);
				print_status(philo, DIE);
				p_node = all()->philos;
				while (p_node)
				{
					pthread_mutex_lock(&((t_philo *)p_node->data)->philo_mutex);
					((t_philo *)p_node->data)->status = DEAD;
					pthread_mutex_unlock(&((t_philo *)p_node->data)->philo_mutex);
					p_node = p_node->next;
				}
				return (NULL);
			}
			pthread_mutex_unlock(&philo->philo_mutex);
			node = node->next;
		}
		usleep(1000);
	}
	return (NULL);
}

bool	create_forks(void)
{
	size_t			i;
	pthread_mutex_t	*fork;

	i = 0;
	while (++i <= all()->num_philo)
	{
		fork = malloc(sizeof(pthread_mutex_t));
		if (!fork)
			return (false);
		if (pthread_mutex_init(fork, NULL) != 0)
		{
			free(fork);
			return (false);
		}
		ft_list_add(&all()->forks, fork, free);
	}
	return (true);
}

void	assign_forks(void)
{
	t_list	*philo_node;
	t_list	*fork_node;
	t_philo	*philo;

	philo_node = all()->philos;
	fork_node = all()->forks;
	while (philo_node)
	{
		philo = philo_node->data;
		philo->right_fork = fork_node->data;
		fork_node = fork_node->next;
		if (!fork_node)
			fork_node = all()->forks;
		philo->left_fork = fork_node->data;
		fork_node = fork_node->next;
		if (!fork_node)
			fork_node = all()->forks;
		philo_node = philo_node->next;
	}
}

bool	create_philos(void)
{
	size_t			i;
	t_philo 		*philo;

	i = 0;
	while (++i <= all()->num_philo)
	{
		philo = malloc(sizeof(t_philo));
		if (!philo)
			return (false);
		philo->id = i;
		philo->status = ALIVE;
		philo->meals = 0;
		if (pthread_mutex_init(&philo->philo_mutex, NULL) != 0)
        {
            free(philo);
            return (false);
        }
		ft_list_add(&all()->philos, philo, free);
	}
	return (true);
}

bool	positive(long long arg)
{
	if (arg < 0)
		return (false);
	return (true);
}

bool	check_positive(int argc, char **argv)
{
	int i = 0;
	while (++i < argc)
	{
		if (!positive(ft_atoll(argv[i])))
			return(false);
	}
	return (true);
}

bool	init_all(int argc, char **argv)
{
	all()->num_philo = ft_atoll(argv[1]);
	all()->time_to_die = ft_atoll(argv[2]);
	all()->time_to_eat = ft_atoll(argv[3]);
	all()->time_to_sleep = ft_atoll(argv[4]);
	if (argc == 6)
		all()->num_eat = ft_atoll(argv[5]);
	if (pthread_mutex_init(&all()->data_mutex, NULL) != 0)
        return (false);
	return (true);
}

bool	create_threads(void)
{
	t_list		*node;
	t_philo		*philo;
	pthread_t	monitor;

	all()->start_time = get_time();
	node = all()->philos;
	while (node)
	{
		philo = node->data;
		philo->last_meal = all()->start_time;
		if (pthread_create(&philo->thread, NULL, philo_routine, philo))
			return (printf("Error creating philosopher thread\n"), false);
		node = node->next;
	}
	if (pthread_create(&monitor, NULL, death_monitor, NULL) != 0)
		return (printf("Error creating monitor thread\n"), false);
	all()->monitor_thread = monitor;
	return (0);
}

bool	join_threads(void)
{
	t_list	*node;
	t_philo	*philo;

	node = all()->philos;
	while (node)
	{
		philo = node->data;
		if (pthread_join(philo->thread, NULL) != 0)
			return (printf("Error joining philosopher thread\n"), false);
		node = node->next;
	}
	if (pthread_join(all()->monitor_thread, NULL) != 0)
		return (printf("Error joinining monitor thread\n"), false);
	return (true);
}

int	main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (printf("Usage: number_of_philosophers time_to_die time_to_eat time_to_sleep Optional: [number_of_times_each_philosopher_must_eat]\n"));
	if (!check_positive(argc, argv))
		return (printf("Only positive numbers allowed\n"));
	if (!init_all(argc, argv))
		return (printf("Error initializing data\n"));
	if (!create_forks())
		return (printf("Error creating forks\n"));
	if (!create_philos())
		return (printf("Error creating philosophers\n"));
	assign_forks();
	if (!create_threads())
		return (cleanup_resources());
	if (!join_threads())
		return (cleanup_resources());
	cleanup_resources();
	return (0);
}
