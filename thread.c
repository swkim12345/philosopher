/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sunghwki <sunghwki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:09:37 by sunghwki          #+#    #+#             */
/*   Updated: 2024/03/11 17:01:11 by sunghwki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	_msg_philo(pthread_mutex_t *mutex, t_msg *msg)
{
	int	ret;

	ret = pthread_mutex_lock(mutex);
	if (ret)
		return (ret);
	printf("%ld %ld %s\n", msg->time, msg->ph, msg->msg);
	ret = pthread_mutex_unlock(mutex);
	return (ret);
}

void	*philo(void *input)
{
	t_thread	ph;
	t_info		info;
	t_msg		msg;
	int			die;
	//int		ret;
	//long	start_time;
	long	microsec_now;
	long	start_eating; //millisecond
	//long	start_sleeping;
	//long	start_thinking;

	ph = *(t_thread *)input;
	info = ph.info;
	die = DIE;
	
	msg.print = ph.print;
	msg.ph = ph.ph_name;
	//start_sleeping = 0;
	if (ph.ph_name % 2 == 0)
		usleep(100);
	start_eating = ft_now_microsec();
	//start_thinking = 0;
	
	//usleep(20 * (ph.info.num_philo - ph.ph_name));
	//start_time = ft_now_microsec();
	//eating
	while (1)
	{
		if (ph.ph_name % 2 == 0)
		{
		while (1)
		{
			if (*ph.left_f == FALSE)
			{
				pthread_mutex_lock(ph.left_fork);
				*ph.left_f = TRUE;
				if (*ph.right_f == FALSE)
				{
					pthread_mutex_lock(ph.right_fork);
					*ph.right_f = TRUE;
					pthread_mutex_unlock(ph.right_fork);
					pthread_mutex_unlock(ph.left_fork);
					break;
				}
				else
				{
					*ph.left_f = FALSE;
					pthread_mutex_unlock(ph.left_fork);
				}
			}
			usleep(100);
		}
		}
		else
		{
			while (1)
			{
				if (*ph.right_f == FALSE)
				{
					pthread_mutex_lock(ph.right_fork);
					*ph.right_f = TRUE;
					if (*ph.left_f == FALSE)
					{
						pthread_mutex_lock(ph.left_fork);
						*ph.left_f = TRUE;
						pthread_mutex_unlock(ph.left_fork);
						pthread_mutex_unlock(ph.right_fork);
						break;
					}
					else
					{
						*ph.right_f = FALSE;
						pthread_mutex_unlock(ph.right_fork);
					}
				}
				usleep(100);
			}
		}


	
	//pthread_mutex_lock(ph.left_fork); //그냥 생짜로 잡게 함 - 가장 퍼포먼스가 낮음.
	//*ph.left_f = TRUE;
	//pthread_mutex_lock(ph.right_fork);
	//*ph.right_f = TRUE;
	
	//while (TRUE)
	//{
	//	pthread_mutex_lock(ph.left_fork);
	//	pthread_mutex_lock(ph.right_fork);
	//	if (*ph.left_f == FALSE && *ph.right_f == FALSE)
	//	{
	//		*ph.left_f = TRUE;
	//		*ph.right_f = TRUE;
	//		break;
	//	}
	//	else
	//	{
	//		pthread_mutex_unlock(ph.left_fork);
	//		pthread_mutex_unlock(ph.right_fork);
	//	}
	//	microsec_now = ft_now_microsec();
	//	if (microsec_now - start_eating >= info.time_to_die)
	//	{
	//		msg.msg = DIE_MSG;
	//		ph.flag = &die;
	//		msg.time = (microsec_now - *ph.start_time) / THOUSAND;
	//		if (_msg_philo(ph.print, &msg))
	//			return (NULL);
	//	}
	//	usleep(100);
	//}
	//pthread_mutex_unlock(ph.left_fork);
	//pthread_mutex_unlock(ph.right_fork);
	//eating printing
	start_eating = ft_now_microsec();
	microsec_now = ft_now_microsec();
	if (microsec_now - start_eating >= info.time_to_die)
	{
		*ph.count_eat = -1; //adhoc, add flag require
		msg.msg = DIE_MSG;
		ph.flag = &die;
		msg.time = (microsec_now - *ph.start_time) / THOUSAND;
		if (_msg_philo(ph.print, &msg))
			return (NULL);
	}
	else
	{
		msg.msg = EAT_MSG;
		msg.time = (microsec_now - *ph.start_time) / THOUSAND;
		if (_msg_philo(ph.print, &msg))
			return (NULL);
	}
	//if (info.time_to_die <= info.time_to_eat) //이렇게 설계하지 말고, usleep을 짧고, 많이 하여 해결하자.
	//	usleep(info.time_to_die);
	//else
	//	usleep(info.time_to_eat);
	while (1)
	{
		microsec_now = ft_now_microsec();
		if (microsec_now - start_eating >= info.time_to_eat)
			break;
		if (info.time_to_eat - (microsec_now - start_eating) >= THOUSAND)
		{
			usleep((info.time_to_eat - (microsec_now - start_eating)) / 2);
		}
		else
			usleep(10);
	}
	//if (ph.ph_name % 2 == 0)
	//{
	//	*ph.left_f = FALSE;
	//	*ph.right_f = FALSE;
	//	pthread_mutex_unlock(ph.left_fork);
	//	pthread_mutex_unlock(ph.right_fork);
	//}
	//else
	//{	*ph.left_f = FALSE;
	//	*ph.right_f = FALSE;
	//	pthread_mutex_unlock(ph.right_fork);
	//	pthread_mutex_unlock(ph.left_fork);
	//}
	pthread_mutex_lock(ph.left_fork);
	pthread_mutex_lock(ph.right_fork);
	*ph.left_f = FALSE;
	*ph.right_f = FALSE;
	pthread_mutex_unlock(ph.left_fork);
	pthread_mutex_unlock(ph.right_fork);

	//sleeping or dying printing
	microsec_now = ft_now_microsec();
	if (microsec_now - start_eating >= info.time_to_die)
	{
		*ph.count_eat = -1;
		msg.msg = DIE_MSG;
		ph.flag = &die;
		msg.time = (microsec_now - *ph.start_time) / THOUSAND;
		if (_msg_philo(ph.print, &msg))
			return (NULL);
	}
	else
	{
		msg.msg = SLEEP_MSG;
		msg.time = (microsec_now - *ph.start_time) / THOUSAND;
		if (_msg_philo(ph.print, &msg))
			return (NULL);
	}
	//sleeping
	//start_sleeping = ft_now_microsec();
	//if (info.time_to_die - info.time_to_eat <= info.time_to_sleep)
	//	usleep(info.time_to_die - info.time_to_eat);
	//else
	//	usleep(info.time_to_sleep);

	while (1)
	{
		microsec_now = ft_now_microsec();
		if (microsec_now - start_eating >= info.time_to_sleep + info.time_to_eat)
			break;
		if (info.time_to_sleep + info.time_to_eat - (microsec_now - start_eating) >= THOUSAND)
		{
			usleep((info.time_to_sleep + info.time_to_eat - (microsec_now - start_eating)) / 2);
		}
		else
			usleep(10);
		//usleep(5);
	}
	//thinking printing
	//start_thinking = ft_now_microsec();
	microsec_now = ft_now_microsec();
	if (microsec_now - start_eating >= info.time_to_die)
	{
		
		*ph.count_eat = -1;
		msg.msg = DIE_MSG;
		ph.flag = &die;
		msg.time = (microsec_now - *ph.start_time) / THOUSAND;
		if (_msg_philo(ph.print, &msg))
			return (NULL);
	}
	else
	{
		msg.msg = THINK_MSG;
		msg.time = (microsec_now - *ph.start_time) / THOUSAND;
		if (_msg_philo(ph.print, &msg))
			return (NULL);
	}
	}
}
