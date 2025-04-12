/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:42:34 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/08 18:28:52 by paude-so         ###   ########.fr       */
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

typedef enum e_philo_status
{
	ALIVE,
	DEAD,
	FULL
}	t_philo_status;	

typedef struct s_tudo
{
	t_list		*philos;
	t_list		*threads;
	ssize_t		start_time;
	ssize_t		num_philo;
	ssize_t		time_to_die;
	ssize_t		time_to_eat;
	ssize_t		time_to_sleep;
	ssize_t		num_eat;
}	t_tudo;

#endif