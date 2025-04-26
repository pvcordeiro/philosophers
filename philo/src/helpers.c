/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:13:33 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/26 13:00:51 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

bool	all_full(void)
{
	t_list	*node;
	t_philo	*philo;

	if (!all()->num_eat)
		return (false);
	node = all()->philos;
	while (node)
	{
		philo = node->data;
		pthread_mutex_lock(&philo->philo_mutex);
		if (!philo->full)
			return (pthread_mutex_unlock(&philo->philo_mutex), false);
		pthread_mutex_unlock(&philo->philo_mutex);
		node = node->next;
	}
	return (true);
}

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(100);
	return (0);
}

void	handle_one(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, TAKE_FORK);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->status = DEAD;
	pthread_mutex_unlock(&philo->philo_mutex);
	pthread_mutex_unlock(philo->right_fork);
}

bool	philo_alive(t_philo *philo)
{
	bool	alive;

	pthread_mutex_lock(&philo->philo_mutex);
	alive = philo->status == ALIVE && !all()->dead_philo;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (alive);
}
