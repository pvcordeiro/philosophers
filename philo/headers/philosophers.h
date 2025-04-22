/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:42:34 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/22 14:08:13 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# include <stdbool.h>
# include <fcntl.h>
# include <pthread.h>
# include <sys/types.h>
# include "ft_utils.h"

typedef enum e_philo_state
{
	THINKING,
	EATING,
	SLEEPING,
}	t_philo_state;

typedef enum e_philo_action
{
	TAKE_LEFT_FORK,
	TAKE_RIGHT_FORK,
	EAT,
	SLEEP,
	THINK,
	DIE
}	t_philo_action;

typedef enum e_status
{
	ALIVE,
	DEAD,
	FULL
}	t_status;	

typedef struct e_philo
{
	int					id;
	t_status			status;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		philo_mutex;
	size_t				last_meal;
	size_t				meals;
	pthread_t			thread;
}	t_philo;

typedef struct s_all
{
	t_list			*philos;
	t_list			*threads;
	t_list			*forks;
	pthread_t		monitor_thread;
	size_t			start_time;
	size_t			num_philo;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			num_eat;
	pthread_mutex_t	data_mutex;
}	t_all;

// singleton
t_all	*all(void);

// threads
bool	create_threads(void);
bool	join_threads(void);

//actions
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	think(t_philo *philo);
void	sleep_philo(t_philo *philo);
void	release_forks(t_philo *philo);

// creation
bool	create_philos(void);
void	assign_forks(void);
bool	create_forks(void);

// helpers
size_t	get_time(void);
int		ft_usleep(size_t milliseconds);
bool	init_all(int argc, char **argv);
bool	check_valid_arg(int argc, char **argv);

// printer
void	print_status(t_philo *philo, t_philo_action action);

#endif