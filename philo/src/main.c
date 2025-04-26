/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 20:43:38 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/26 12:59:37 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

static bool	check_valid_arg(int argc, char **argv)
{
	int	i;

	i = 0;
	while (++i < argc)
	{
		if (ft_atoll(argv[i]) <= 0)
			return (false);
	}
	return (true);
}

static bool	init_all(int argc, char **argv)
{
	all()->num_philo = ft_atoll(argv[1]);
	all()->time_to_die = ft_atoll(argv[2]);
	all()->time_to_eat = ft_atoll(argv[3]);
	all()->time_to_sleep = ft_atoll(argv[4]);
	all()->dead_philo = false;
	if (argc == 6)
		all()->num_eat = ft_atoll(argv[5]);
	if (pthread_mutex_init(&all()->data_mutex, NULL) != 0)
		return (false);
	return (true);
}

static int	destructor(void)
{
	t_list			*fork_node;
	t_list			*philo_node;
	pthread_mutex_t	*fork;
	t_philo			*philo;

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

int	main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (ft_printf("Usage: np ttd tte tts [notepme]\n"));
	if (!check_valid_arg(argc, argv))
		return (ft_printf("Invalid argument\n"));
	if (!init_all(argc, argv))
		return (ft_printf("Error initializing data\n"));
	if (!create_forks())
		return (destructor(), ft_printf("Error creating forks\n"));
	if (!create_philos())
		return (destructor(), ft_printf("Error creating philosophers\n"));
	assign_forks();
	if (!create_threads())
		return (destructor());
	if (!join_threads())
		return (destructor());
	destructor();
	return (0);
}
