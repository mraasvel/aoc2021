/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heap_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mraasvel <mraasvel@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/19 19:25:11 by mraasvel      #+#    #+#                 */
/*   Updated: 2021/12/15 21:46:07 by mraasvel      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"
#include <stdbool.h>

int	left_child_index(int parent)
{
	return (2 * parent + 1);
}

int	right_child_index(int parent)
{
	return (2 * parent + 2);
}

int	parent_index(int child)
{
	return ((child - 1) / 2);
}

bool	has_left_child(t_heap *heap, int parent)
{
	return (left_child_index(parent) <= heap->len);
}

bool	has_right_child(t_heap *heap, int parent)
{
	return (right_child_index(parent) <= heap->len);
}

bool	has_parent(int child)
{
	return (parent_index(child) >= 0 && child != 0);
}

void	*left_child(t_heap *heap, int parent)
{
	return (heap->items[left_child_index(parent)]);
}

void	*right_child(t_heap *heap, int parent)
{
	return (heap->items[right_child_index(parent)]);
}

void	*parent(t_heap *heap, int child)
{
	return (heap->items[parent_index(child)]);
}
