#ifndef HEAP_H
# define HEAP_H

# include <stddef.h>

typedef struct s_heap
{
	void	**items;
	int		len;
	int		size;
	int		(*cmp)(void*, void*);
}	t_heap;

/* Heap */

void	heap_insert(t_heap *heap, void *item);
void	*heap_pop(t_heap *heap);
t_heap	*heap_init(int (*cmp)(void *, void *));
void	heap_delete(t_heap *heap, void (*del)(void *));

/* Utils */

int		left_child_index(int parent);
int		right_child_index(int parent);
int		parent_index(int child);
bool	has_left_child(t_heap *heap, int parent);
bool	has_right_child(t_heap *heap, int parent);
bool	has_parent(int child);
void	*left_child(t_heap *heap, int parent);
void	*right_child(t_heap *heap, int parent);
void	*parent(t_heap *heap, int child);

/* Wrapper */

void	*ft_malloc(size_t size);
void	*oom_guard(void *malloc_return);
void	exit_perror(const char *str);

#endif
