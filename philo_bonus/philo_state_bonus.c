/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_state_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunghwki <sunghwki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:57:27 by sunghwki          #+#    #+#             */
/*   Updated: 2024/03/24 15:22:20 by sunghwki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	stop_philo(t_philo *ph, long *start_usec, long cmp_time)
{
	long	now_usec;

	while (TRUE)
	{
		now_usec = ft_usec_now();
		if (now_usec - *start_usec >= ph->info->time_to_die)
		{
			print_msg(ph, now_usec - ph->info->start_time, DIED);
			return (FUN_FAIL);
		}
		else if (now_usec - *start_usec >= cmp_time)
			return (FUN_SUC);
		if (cmp_time - (now_usec - *start_usec) >= THOUSAND)
			usleep(500);
		else
			usleep(100);
	}
}

int	take_fork_philo(t_philo *ph, long *start_usec)
{
	pthread_t	pid;
	long		now_usec;

	pthread_create(&pid, NULL, th_eating, (void *)ph);
	pthread_detach(pid);
	while (TRUE)
	{
		now_usec = ft_usec_now();
		sem_wait(ph->sem->flag);
		if (*(ph->flag) == EAT)
		{
			sem_post(ph->sem->flag);
			print_msg(ph, now_usec - ph->info->start_time, EAT);
			*start_usec = now_usec;
			break ;
		}
		sem_post(ph->sem->flag);
		if (now_usec - *start_usec >= ph->info->time_to_die)
		{
			print_msg(ph, now_usec - ph->info->start_time, DIED);
			return (FUN_FAIL);
		}
		usleep(100);
	}
	return (FUN_SUC);
}

int	eating_philo(t_philo *ph, long *start_usec)
{
	if (take_fork_philo(ph, start_usec) == FUN_FAIL)
		return (FUN_FAIL);
	if (stop_philo(ph, start_usec, ph->info->time_to_eat) == FUN_FAIL)
		return (FUN_FAIL);
	sem_post(ph->sem->fork);
	sem_post(ph->sem->fork);
	return (FUN_SUC);
}

int	thinking_philo(t_philo *ph)
{
	long	now_usec;

	now_usec = ft_usec_now();
	print_msg(ph, now_usec - ph->info->start_time, THINK);
	return (FUN_SUC);
}

int	sleeping_philo(t_philo *ph, long *start_usec)
{
	long	now_usec;
	long	cmp_time;

	now_usec = ft_usec_now();
	cmp_time = ph->info->time_to_eat + ph->info->time_to_sleep;
	print_msg(ph, now_usec - ph->info->start_time, SLEEP);
	if (stop_philo(ph, start_usec, cmp_time) == FUN_FAIL)
		return (FUN_FAIL);
	return (FUN_SUC);
}