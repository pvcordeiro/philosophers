/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:07:14 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/26 13:05:52 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

static void	assign_left_forks(void)
{
	t_list	*philo_node;
	t_list	*fork_node;
	t_philo	*philo;

	philo_node = all()->philos;
	fork_node = all()->forks->next;
	while (philo_node)
	{
		philo = philo_node->data;
		if (fork_node)
			philo->left_fork = fork_node->data;
		else
			philo->left_fork = all()->forks->data;
		philo_node = philo_node->next;
		if (fork_node)
			fork_node = fork_node->next;
	}
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
		philo_node = philo_node->next;
		fork_node = fork_node->next;
	}
	assign_left_forks();
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
			return (free(fork), false);
		ft_list_add(&all()->forks, fork, free);
	}
	return (true);
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
		philo->full = false;
		if (pthread_mutex_init(&philo->philo_mutex, NULL) != 0)
			return (free(philo), false);
		ft_list_add(&all()->philos, philo, free);
	}
	return (true);
}
