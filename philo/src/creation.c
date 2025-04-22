/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:07:14 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/22 13:17:24 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

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
	size_t	i;
	t_philo	*philo;

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
