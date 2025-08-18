/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_queue_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:48:22 by chrleroy          #+#    #+#             */
/*   Updated: 2025/08/18 14:16:15 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** This file contains basic queue management utilities
*/
//
void	capacity_threshold_handler(t_key_queue *queue)
{
	int	i;
	int j;

	j = 0;
	i = queue->front;
	while (j < i)
	{
		queue->key_queue[j] = queue->key_queue[i];
		queue->key_queue[i] = -1;
		j++;
	}
	queue->front = 0;
}

//
void	enqueue(t_key_queue *queue, int key)
{
	capacity_threshold_handler(queue);
	queue->back++;
	queue->key_queue[queue->back] = key;
}

//
int	dequeue(t_key_queue *queue)
{
	int	i;
	int ret;

	i = queue->front;
	ret = queue->key_queue[i];
	queue->key_queue[i] = -1;
	queue->front++;
	return (ret);
}
