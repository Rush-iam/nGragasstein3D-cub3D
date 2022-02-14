
#include "cub3d_bonus.h"
#include "mlx.h"

#ifdef __APPLE__

void put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr, \
						 t_point offset)
{
	mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, offset.x, offset.y);
}

#else

void put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr, \
						 t_point offset)
{
	mlx_put_image_to_window(mlx_ptr, win_ptr, img_ptr, offset.x, offset.y);
}

#endif