/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:19:20 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/22 13:32:46 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

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
