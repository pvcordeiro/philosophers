/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:42:34 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/08 16:04:51 by paude-so         ###   ########.fr       */
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
	WAITING,
	FORK_TAKEN,
	FORK_AVAILABLE,
	FORK_USED,
	FORK_RETURNED
}	t_philo_state;

typedef enum e_philo_action
{
	TAKE_FORK,
	EAT,
	SLEEP,
	THINK,
}	t_philo_action;

typedef enum e_philo_status
{
	ALIVE,
	DEAD,
	FULL
}	t_philo_status;	

typedef struct s_philo
{
	int	num_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	num_eat;
}	t_philo;

#endif