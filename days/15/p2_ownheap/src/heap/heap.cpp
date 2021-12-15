/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heap.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mraasvel <mraasvel@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/19 17:02:17 by mraasvel      #+#    #+#                 */
/*   Updated: 2021/12/15 21:48:03 by mraasvel      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

t_heap	*heap_init(int (*cmp)(void *, void *))
{
	t_heap	*new_heap;

	new_heap = (t_heap*)ft_malloc(1 * sizeof(t_heap));
	new_heap->len = 0;
	new_heap->size = 10;
	new_heap->items = (void**)ft_malloc(new_heap->size * sizeof(void *));
	new_heap->cmp = cmp;
	return (new_heap);
}

void	heap_delete(t_heap *heap, void (*del)(void *))
{
	while (heap->len > 0)
	{
		heap->len -= 1;
		del(heap->items[heap->len]);
	}
	free(heap->items);
	free(heap);
}

void	heap_swap(t_heap *heap, int a, int b)
{
	void	*temp;

	temp = heap->items[a];
	heap->items[a] = heap->items[b];
	heap->items[b] = temp;
}

void	heap_ensure_size(t_heap *heap)
{
	void	**temp;

	if (heap->len == heap->size)
	{
		heap->size = heap->size * 2;
		temp = (void**)ft_malloc(heap->size * sizeof(void *));
		memcpy(temp, heap->items, heap->len * sizeof(void *));
		free(heap->items);
		heap->items = temp;
	}
}

bool	heap_isempty(t_heap *heap)
{
	return (heap->len == 0);
}

void	*heap_peak(t_heap *heap)
{
	return (heap->items[0]);
}

void	heapify_down(t_heap *heap)
{
	int	index;
	int	smaller_child_index;

	index = 0;
	while (has_left_child(heap, index))
	{
		smaller_child_index = left_child_index(index);
		if (has_right_child(heap, index)
			&& heap->cmp(right_child(heap, index), left_child(heap, index)))
		{
			smaller_child_index = right_child_index(index);
		}
		if (heap->cmp(heap->items[index], heap->items[smaller_child_index]))
			return ;
		else
		{
			heap_swap(heap, index, smaller_child_index);
			index = smaller_child_index;
		}
	}
}

void	heapify_up(t_heap *heap)
{
	int	index;

	index = heap->len - 1;
	while (has_parent(index)
		&& heap->cmp(heap->items[index], parent(heap, index)))
	{
		heap_swap(heap, index, parent_index(index));
		index = parent_index(index);
	}
}

/* Check if heap is empty or not before calling */
void	*heap_pop(t_heap *heap)
{
	void	*item;

	item = heap->items[0];
	heap->items[0] = heap->items[heap->len - 1];
	heap->len -= 1;
	if (heap->len != 0)
		heapify_down(heap);
	return (item);
}

void	heap_insert(t_heap *heap, void *item)
{
	heap_ensure_size(heap);
	heap->items[heap->len] = item;
	heap->len += 1;
	heapify_up(heap);
}
