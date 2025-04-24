/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 20:43:38 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/24 17:45:00 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

int	cleanup_resources(void)
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
