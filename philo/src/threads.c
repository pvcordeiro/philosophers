/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:59:28 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/25 16:10:36 by paude-so         ###   ########.fr       */
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

static void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (all()->num_philo == 1)
		return (handle_one(philo), NULL);
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (philo_alive(philo))
	{
		if (philo->full)
			return (NULL);
		take_forks(philo);
		eat(philo);
		pthread_mutex_lock(&philo->philo_mutex);
		if (all()->num_eat && philo->meals == all()->num_eat)
			philo->full = true;
		pthread_mutex_unlock(&philo->philo_mutex);
		release_forks(philo);
		sleep_philo(philo);
		think(philo);
		pthread_mutex_lock(&all()->data_mutex);
		if (all()->dead_philo)
			return (pthread_mutex_unlock(&all()->data_mutex), NULL);
		pthread_mutex_unlock(&all()->data_mutex);
	}
	return (NULL);
}

static bool	is_kaput(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	if ((get_time() - philo->last_meal) > all()->time_to_die)
	{
		if (all()->num_eat && philo->full)
			return (false);
		philo->status = DEAD;
		pthread_mutex_unlock(&philo->philo_mutex);
		print_status(philo, DIE);
		pthread_mutex_lock(&all()->data_mutex);
		all()->dead_philo = true;
		pthread_mutex_unlock(&all()->data_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->philo_mutex);
	return (false);
}

static void	*death_monitor(void *arg)
{
	t_list	*node;
	t_philo	*philo;

	(void)arg;
	while (42)
	{
		node = all()->philos;
		while (node)
		{
			philo = node->data;
			if (is_kaput(philo))
				return (NULL);
			if (all_full() || philo->full)
				return (NULL);
			node = node->next;
		}
	}
	return (NULL);
}

bool	create_threads(void)
{
	t_list		*node;
	t_philo		*philo;

	all()->start_time = get_time();
	node = all()->philos;
	while (node)
	{
		philo = node->data;
		philo->last_meal = all()->start_time;
		if (pthread_create(&philo->thread, NULL, philo_routine, philo) != 0)
			return (ft_printf("Error creating philosopher thread\n"), false);
		node = node->next;
	}
	if (pthread_create(&all()->monitor_thread, NULL, death_monitor, NULL) != 0)
		return (ft_printf("Error creating monitor thread\n"), false);
	return (true);
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
