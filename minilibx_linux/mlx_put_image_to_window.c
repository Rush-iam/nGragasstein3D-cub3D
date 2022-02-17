/*
** mlx_put_image_to_window.c for MiniLibX in raytraceur
** 
** Made by Charlie Root
** Login   <ol@epitech.net>
** 
** Started on  Mon Aug 14 15:55:49 2000 Charlie Root
** Last update Sun Oct  2 09:53:00 2005 Olivier Crouzet
*/



#include	"mlx_int.h"

int	mlx_put_image_to_window(t_xvar *xvar,t_win_list *win,t_img *img,
									int x,int y)
{
	GC		gc = (img->gc) ? img->gc : win->gc;
	Pixmap 	target_pixmap;
	Picture target_picture;

	if (xvar->do_flush)
	{
		target_pixmap = win->window;
		target_picture = win->pict;
	}
	else
	{
		target_pixmap = win->window_back;
		target_picture = win->pict_back;
	}
	if (img->pict)
	{
		if (img->type == MLX_TYPE_XIMAGE)
			XPutImage(xvar->display, img->pix, gc, img->image, 0, 0, 0, 0,
					  img->width, img->height);
		else if (img->type == MLX_TYPE_SHM)
			XShmPutImage(xvar->display, img->pix, gc, img->image, 0, 0, 0, 0,
						 img->width, img->height, False);
		XRenderComposite(xvar->display, PictOpOver, img->pict, 0, target_picture,
						 0, 0, 0, 0, x, y, img->width, img->height);
	}
	else
	{
		if (img->type == MLX_TYPE_XIMAGE)
			XPutImage(xvar->display, target_pixmap, gc, img->image, 0, 0, x, y,
					  img->width, img->height);
		else if (img->type == MLX_TYPE_SHM)
			XShmPutImage(xvar->display, target_pixmap, gc, img->image, 0, 0, x, y,
						 img->width, img->height, False);
		else if (img->type == MLX_TYPE_SHM_PIXMAP)
			XCopyArea(xvar->display, img->pix, target_pixmap, gc,
					  0, 0, img->width, img->height, x, y);
	}
	if (xvar->do_flush)
		XFlush(xvar->display);
}
