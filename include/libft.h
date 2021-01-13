/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/04 15:25:45 by mgalliou          #+#    #+#             */
/*   Updated: 2020/02/10 14:44:06 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <wchar.h>
# ifndef intmax_t
#  include <stdint.h>
# endif

typedef	struct			s_list
{
	void				*content;
	size_t				content_size;
	struct s_list		*next;
}						t_list;

typedef	struct			s_dlist
{
	void				*content;
	size_t				content_size;
	struct s_dlist		*next;
	struct s_dlist		*prev;
}						t_dlist;

typedef struct			s_btree
{
	struct s_btree		*left;
	struct s_btree		*right;
	void				*item;
}						t_btree;

enum					e_rb_color
{
	RB_BLACK,
	RB_RED
};

typedef struct			s_rb_node
{
	struct s_rb_node	*parent;
	struct s_rb_node	*left;
	struct s_rb_node	*right;
	void				*item;
	enum e_rb_color		color;
}						t_rb_node;

/*
**	PART I;
*/

void					*ft_memset(void *b, int c, size_t len);
void					ft_bzero(void *s, size_t n);
void					*ft_memcpy(void *dst, const void *src, size_t n);
void					*ft_memccpy(void *dst, const void *src,
								int c, size_t n);
void					*ft_memmove(void *dst, const void *src, size_t len);
void					*ft_memchr(const void *s, int c, size_t n);
int						ft_memcmp(const void *s1, const void *s2, size_t n);
size_t					ft_strlen(const char *s);
char					*ft_strdup(const char *s1);
char					*ft_strcpy(char *dst, const char *src);
char					*ft_strncpy(char *dst, const char *src, size_t len);
char					*ft_strcat(char *s1, const char *s2);
char					*ft_strncat(char *s1, const char *s2, size_t n);
size_t					ft_strlcat(char *dst, const char *src, size_t size);
char					*ft_strchr(const char *s, int c);
char					*ft_strrchr(const char *s, int c);
char					*ft_strstr(const char *big, const char *little);
char					*ft_strnstr(const char *big, const char *little,
								size_t len);
int						ft_strcmp(const char *s1, const char *s2);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_atoi(const char *str);
int						ft_isalpha(int c);
int						ft_isdigit(int c);
int						ft_isalnum(int c);
int						ft_isascii(int c);
int						ft_isprint(int c);
int						ft_toupper(int c);
int						ft_tolower(int c);

/*
**	PART II
*/

void					*ft_memalloc(size_t size);
void					ft_memdel(void **ap);
char					*ft_strnew(size_t size);
void					ft_strdel(char **as);
void					ft_strclr(char *s);
void					ft_striter(char *s, void (*f)(char *));
void					ft_striteri(char *s, void (*f)(unsigned int, char *));
char					*ft_strmap(char const *s, char (*f)(char));
char					*ft_strmapi(char const *s,
								char (*f)(unsigned int, char));
int						ft_strequ(char const *s1, char const *s2);
int						ft_strnequ(char const *s1, char const *s2, size_t n);
char					*ft_strsub(char const *s, unsigned int start,
								size_t len);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strtrim(char const *s);
char					**ft_strsplit(char const *s, char c);
char					*ft_itoa(int n);
void					ft_putchar(char c);
void					ft_putstr(char const *s);
void					ft_putendl(char const *s);
void					ft_putnbr(int n);
void					ft_putnbr_base(uintmax_t n, int base);
void					ft_putchar_fd(char c, int fd);
void					ft_putstr_fd(char const *s, int fd);
void					ft_putendl_fd(char const *s, int fd);
void					ft_putnbr_fd(int n, int fd);

/*
**	PART BONUS
*/

t_list					*ft_lstnew(void const *content, size_t content_size);
void					ft_lstdelone(t_list **alst, void (*del)(void*, size_t));
void					ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void					ft_lstadd(t_list **alst, t_list *new);
void					ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list					*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));

/*
**	ADDITIONAL FONCTION
*/

/*
**		string fonctions
*/

size_t					ft_strclen(char const *s, char c);
size_t					ft_strlcpy(char *dst, const char *src, size_t size);
char					*ft_strndup(const char *s1, size_t n);
char					*ft_strrev(char *s);
char					*ft_strupcase(char *s);
char					*ft_strlowcase(char *s);
char					*ft_strcapitalize(char *s);
char					*ft_strcjoin(char const *s1, char const *s2, char c);
int						ft_tablen(char **tab);
void					ft_tabdel(char ***atab);
char					**ft_tabdup(char **tab);
int						ft_wordcount(char const *str, char c);
void					ft_puttab(char **sol);
void					ft_puttab_fd(char **sol, int fd);
int						ft_atoi_base(char *str, char *base);
char					*ft_itoa_base(uintmax_t n, int base);
char					*ft_ftoa(long double nbr, int preci);
int						ft_wcharlen(wint_t wint);
char					*ft_wchartoa(wint_t wchar);
int						ft_wstrlen(wchar_t *wstr);
char					*ft_wstrtoa(wchar_t *wstr);

/*
**		issers
*/

int						ft_isspace(int c);
int						ft_isblank(int c);
int						ft_isescape(int c);
int						ft_isdir(char const *path);

/*
**		list
*/

t_list					*ft_lstnewnocpy(void *content,
							size_t content_size);
void					ft_lstaddback(t_list **alst, t_list *new);
t_list					*ft_lstsort(t_list *beg, int cmp(t_list *a, t_list *b));
int						ft_lstlen(t_list *lst);

/*
**		dlist
*/

t_dlist					*ft_dlstnew(void const *content, size_t content_size);
t_dlist					*ft_dlstnewnocpy(void *content, size_t content_size);
void					ft_dlstaddback(t_dlist **adlst, t_dlist *new);
void					ft_dlstinsert(t_dlist **adlst, t_dlist *new);
void					ft_dlstremove(t_dlist **adlst, t_dlist *elem);

/*
**binary_tree
*/

t_btree					*btree_create_node(void *item);
void					btree_apply_infix(t_btree *root,
								void (*applyf)(void *));
void					btree_apply_infix_rev(t_btree *root,
								void (*applyf)(void *));
void					btree_apply_prefix(t_btree *root,
								void (*applyf)(void *));
void					btree_insert_data(t_btree **root, void *item,
								int (*cmpf)(void *, void *));
void					*btree_search_item(t_btree *root, void *data_ref,
								int (*cmpf)(void *, void *));
int						btree_level_count(t_btree *root);
void					btree_apply_by_level(t_btree *root,
								void (*applyf)(void *item,
								int current_level, int is_first_elem));
void					rb_insert(struct s_rb_node **root,
								void *data, int (*cmpf)(void *, void *));
void					rb_remove(struct s_rb_node **root, void *data,
								int (*cmpf)(void *, void *),
								void (*freef)(void *));
void					rb_delete(struct s_rb_node **root,
								void (*freef)(void *));

/*
**		environement
*/

char					*ft_getenv(const char *name);
int						ft_setenv(const char *name, const char *value,
								int overwrite);
int						ft_putenv(char *string);
int						ft_unsetenv(const char *name);

/*
**		misc
*/

void					ft_swap(void *a, void *b);
void					*ft_ptr_align(void *ptr, size_t aling);
int						ft_nbrlen(int n);
int						ft_nbrlen_base(uintmax_t n, int base);
void					*ft_memrealloc(void *ptr, size_t size);
intmax_t				ft_recursive_power(intmax_t nb, int power);
char					*ft_strrealloc(char *str, size_t size);

/*
**		terminal
*/

int						ft_getcol(void);

/*
** GET_NEXT_LINE
*/

int						get_next_line(const int fd, char **line);

/*
** FT_PRINTF
*/

int						ft_printf(const char *format, ...);

#endif
