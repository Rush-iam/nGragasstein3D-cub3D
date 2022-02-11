/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_events.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 19:20:05 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/03 22:12:20 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef X_EVENTS_H
# define X_EVENTS_H

# include <X11/X.h>
# include <X11/keysym.h>

enum	e_mouse
{
	MOUSE_LEFT = 1,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_SCROLLUP,
	MOUSE_SCROLLDOWN
};

# ifdef __APPLE__

#  define KEYCODE_OFFSET 0

enum	e_keyboard
{
	KEY_A = 0,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_H,
	KEY_G,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_SECTION,
	KEY_B,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_Y,
	KEY_T,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_6,
	KEY_5,
	KEY_EQUAL,
	KEY_9,
	KEY_7,
	KEY_MINUS,
	KEY_8,
	KEY_0,
	KEY_SQBRACKET_RIGHT,
	KEY_O,
	KEY_U,
	KEY_SQBRACKET_LEFT,
	KEY_I,
	KEY_P,
	KEY_ENTER,
	KEY_L,
	KEY_J,
	KEY_QUOTE = 39,
	KEY_SEMICOLON = 41,
	KEY_BACKSLASH,
	KEY_COMMA,
	KEY_SLASH,
	KEY_N,
	KEY_M,
	KEY_DOT,
	KEY_TAB,
	KEY_SPACE,
	KEY_TILDA = 50,
	KEY_BACKSPACE = 51,
	KEY_ESCAPE = 53,
	KEY_NUMASTERISK = 67,
	KEY_NUMPLUS = 69,
	KEY_NUMMINUS = 78,
	KEY_LEFT = 123,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_UP = 126,
	KEY_CONTROL_LEFT = 256,
	KEY_SHIFT_LEFT,
	KEY_SHIFT_RIGHT,
	KEY_COMMAND_LEFT,
	KEY_COMMAND_RIGHT,
	KEY_OPTION_LEFT,
	KEY_OPTION_RIGHT = 262,
	KEY_CONTROL_RIGHT = 269,
	KEY_CAPSUP = 272,
};

# else

#  define KEYCODE_OFFSET 256

enum e_keyboard
{
	KEY_A = XK_a,
	KEY_S = XK_s,
	KEY_D = XK_d,
	KEY_W = XK_w,
	KEY_ESCAPE = XK_Escape,
	KEY_NUMASTERISK = XK_KP_Multiply,
	KEY_NUMPLUS = XK_KP_Add,
	KEY_NUMMINUS = XK_KP_Subtract,
	KEY_LEFT = XK_Left,
	KEY_RIGHT = XK_Right
};

# endif

#endif
