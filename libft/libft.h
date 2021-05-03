/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngragas <ngragas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 21:24:33 by ngragas           #+#    #+#             */
/*   Updated: 2021/05/02 17:52:15 by ngragas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isprint(int c);
int			ft_isascii(int c);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));

size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);

void		*ft_memchr(const void *s, int c, size_t n);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);

size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memccpy(void *dst, const void *src, int c, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t len);

void		*ft_malloc_ptr(void **ptr, size_t size);
void		*ft_assign_ptr(void **dst_ptr, void *src_ptr);
void		*ft_free_ptr(void *ptr);
void		*ft_free_ptr_array(void **array, unsigned int count);
void		*ft_calloc(size_t count, size_t size);
void		ft_bzero(void *s, size_t n);
void		*ft_memset(void *b, int c, size_t len);

char		*ft_strdup(const char *s1);
char		*ft_substr(char const *s, unsigned int start, size_t len);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);

int			ft_atoi(const char *str);
char		*ft_itoa(int n);

void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);

t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		*ft_lstpop(t_list **lst);
void		ft_lstremove(t_list **begin, t_list *to_delete);
void		ft_lstremoveif(t_list **begin, void *data_ref, int (*cmp)(), \
							void (*free_fct)(void *));
void		ft_lstsort(t_list **lst, int (*cmp)());
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstreverse(t_list **begin_list);
void		ft_lstdelone(t_list *lst, void (*del)(void*));
void		ft_lstclear(t_list **lst, void (*del)(void*));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

int			ft_wchrto8(char *dst_utf8, wchar_t src_utf32);
size_t		ft_wstrto8(char *dst_utf8, const wchar_t *src_utf32, size_t n);

int			ft_min(int a, int b);
int			ft_max(int a, int b);
uint32_t	ft_umin(uint32_t a, uint32_t b);
uint32_t	ft_umax(uint32_t a, uint32_t b);

int			ft_file_exists(char *path);

#endif
