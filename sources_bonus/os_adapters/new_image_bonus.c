#include "mlx.h"

#ifdef __APPLE__

void	*new_image(void *mlx_ptr,int width, int height)
{
	return (mlx_new_image(mlx_ptr, width, height));
}

#else

void	*new_image(void *mlx_ptr,int width, int height)
{
	return (mlx_new_image_alpha(mlx_ptr, width, height));
}

#endif