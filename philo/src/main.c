/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 20:43:38 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/21 13:57:22 by paude-so         ###   ########.fr       */
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
	return ((tv.tv_sec * 1000));
}

bool	philo_alive(t_philo *philo)
{
	return (philo->status == ALIVE || (all()->num_eat > 0 && philo->meals < all()->num_eat));
}

void	print_status(t_philo *philo, t_philo_action action)
{
	size_t	timestamp;

	timestamp = get_time() - all()->start_time;
	//TODO
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, TAKE_RIGHT_FORK);
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, TAKE_LEFT_FORK);
}

void	eat(t_philo *philo)
{
	print_status(philo, EAT);
	philo->last_meal = get_time();
	philo->meals++;
	usleep(all()->time_to_eat * 1000);
}

void	release_forks(t_philo *philo)
{
	print_status(philo, SLEEP);
	usleep(all()->time_to_sleep * 1000);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
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
	t_philo	*philo;

	while (42)
	{
		node = all()->philos;
		while (node)
		{
			philo = node->data;
			if ((get_time() - philo->last_meal) > all()->time_to_die)
			{
				print_status(philo, DIE);
				//TODO stop all threads
				return (NULL);
			}
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
	while (++i < all()->num_philo)
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
	size_t	i;

	philo_node = all()->philos;
	fork_node = all()->forks;
	i = 0;
	while (philo_node)
	{
		philo = philo_node->data;
		philo->right_fork = fork_node->data;
		fork_node = fork_node->next;
		if (!fork_node)
			fork_node = all()->forks;
		philo->left_fork = fork_node->data;
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

bool	init_all(int argc, char **argv)
{
	int i = 0;
	while (++i < argc)
	{
		if (!positive(ft_atoll(argv[i])))
			exit(ft_printf("Only positive numbers allowed\n"));
	}
	all()->num_philo = ft_atoll(argv[1]);
	all()->time_to_die = ft_atoll(argv[2]);
	all()->time_to_eat = ft_atoll(argv[3]);
	all()->time_to_sleep = ft_atoll(argv[4]);
	if (argc == 6)
		all()->num_eat = ft_atoll(argv[5]);
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
			return (ft_printf("Error creating philosopher thread\n"), false);
		node = node->next;
	}
	if (pthread_create(&monitor, NULL, death_monitor, NULL) != 0)
		return (ft_printf("Error creating monitor thread\n"), false);
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
			return (ft_printf("Error joining philosopher thread\n"), false);
		node = node->next;
	}
	if (pthread_join(all()->monitor_thread, NULL) != 0)
		return (ft_printf("Error joinining monitor thread\n"), false);
	return (true);
}

int	main(int argc, char **argv)
{
	t_list		*node;
	t_philo		*philo;

	if (argc < 5 || argc > 6)
		return (ft_printf("Usage: number_of_philosophers time_to_die time_to_eat time_to_sleep Optional: [number_of_times_each_philosopher_must_eat]\n"));
	if (!init_all(argc, argv));
		return (ft_printf("Error initializing data\n"));
	if (!create_forks())
		return (ft_printf("Error creating forks\n"));
	if (!create_philos())
		return (ft_printf("Error creating philosophers\n"));
	assign_forks();
	if (!create_threads())
		return (cleanup_resources());
	if (!join_threads())
		return (cleanup_resources());
	cleanup_resources();
	return (0);
}
