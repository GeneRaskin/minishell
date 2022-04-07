#include <malloc/malloc.h>

int		g_malloc_count = 0;
void	*(*g_system_malloc)(malloc_zone_t *zone, size_t size);
void	(*g_system_free)(malloc_zone_t *zone, void *ptr);
void	(*g_system_free_definite_size)(malloc_zone_t *zone, void *ptr,
			size_t size);

void	*my_malloc(malloc_zone_t *zone, size_t size)
{
	g_malloc_count++;
	malloc_printf("malloc(zone=%p, size=%lu\n", zone, size);
	return (g_system_malloc(zone, size));
}

void	my_free(malloc_zone_t *zone, void *ptr)
{
	g_malloc_count--;
	malloc_printf("free(zone=%p, ptr=%p)\n", zone, ptr);
	g_system_free(zone, ptr);
}

void	my_free_definite_size(malloc_zone_t *zone, void *ptr,
							  size_t size)
{
	g_malloc_count--;
	malloc_printf("free_def_siz(zone=%p, ptr=%p, size=%lu)\n", zone, ptr, size);
	g_system_free_definite_size(zone, ptr, size);
}

void	set_zone(void)
{
	malloc_zone_t	*zone;

	zone = malloc_default_zone();
	g_system_free = zone->free;
	g_system_malloc = zone->malloc;
	g_system_free_definite_size = zone->free_definite_size;
	zone->free_definite_size = my_free_definite_size;
	zone->free = &my_free;
	zone->malloc = &my_malloc;
}
