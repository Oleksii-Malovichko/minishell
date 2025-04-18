/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:14:00 by alex              #+#    #+#             */
/*   Updated: 2025/04/19 12:32:14 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdbool.h>

#include "lib/get_next_line/get_next_line.h"
#include "lib/libft/libft.h"

typedef struct s_var_info
{
	char	*var_value;
	int		is_alloc;
}	t_var_info;

typedef struct s_shell
{
	int		status;
	char	**env;
}	t_shell;

typedef struct s_saved_std
{
	int saved_stdin;
	int saved_stdout;
}	t_saved_std;

typedef struct s_pipe_data
{
	char				**commands;
	int					num_commands;
	int					prev_fd;
	int					pipefd[2];
	int					i;
}	t_pipe_data;

typedef enum e_token
{
	REDINPUT,
	REDOUTPUT,
	APPENDREDOUTPUT,
	REDDELIMETER,
	PIPET,
	SINGLEQUOTE,
	DOUBLEQUOTE,
	EXPAND,
	COMMAND,
	ARGUMENT,
	WORD
}	t_tokentype;

typedef struct s_pnode
{
	t_tokentype		toktype;
	char			*strpart;
	struct s_pnode	*next;
	struct s_pnode	*prev;
}	t_pnode;

typedef	struct s_store
{
	int		pipecount;
	int		pidcount;
	int		cmd_num;
	pid_t	*childs;
	int		save_stdin;
	int		save_stdout;
	int		fd_readl;
	int		fd_exin;
	int		fd_exout;
	int		pipefd[2];
}	t_store;

//check_redir.c
// char	*remove_redirects(char *cmd);
// char	*get_filename(char *cmd);
// void	handle_heredoc(const char *delimiter, int pipe_fd[2]);
// void	handle_redirection(char *cmd, int *status);

//minishell.c
void	close_saved_std(struct s_saved_std *std);
void	remove_chars(char **str, char ch);
// void	execute_pipe_commands(char *cmd, int *status);
int		is_nummeric(char *line);
void	handle_exit(char *line, int *status, struct s_saved_std *std, char **myenvp);
void	sig_handler(int sig);
int		ft_getcwd(char *line, int fd);
void	free_arr(char **arr);
int		handle_cd(char *line, char ***myenvp);
int		check_line(char *line, int i);
int		check_quastion_sign(char **line, int status);
char	*remove_first_spaces(char *line);
void	disable_ctrl_c_output(int *status);
void	setup_signal_handlers(void);
int		print_env(char ***myenvp);
int		finish_write_cmd_path(char **buffer, char *path, char *cmd);
char	*find_cmd_in_paths(char **path_arr, char *cmd);
char	*get_command_path(char *cmd);
char	**get_commands(char *cmd, char *temp);

//msh_cmd.c
int		is_builtin(char *cmd);
void	execute_builtin(char *cmd, int fd, int *status, char ***myenvp);

//check_line.c
void	write_stderr(char *str);
int		ft_isoperator(int c);
int		ft_isspace(int c);
int		skip_whites(char *line, int	*i);
int		is_empty(char *line);

//check_var.c
// int		get_var_name_size(char *str);
// void	get_var_name(char *dest, char *str);
// int		cmp_names(char *name1, char *name2);
// char	*find_var_value(char *var_name);
// void	change_str(char **str, char *name, char *value);
// void	remove_var_name(char **str, char *name);
// void	bridge_var(char **str);
int	cmp_names(char *name1, char *name2);
char	*find_var_value(char *var_name, char **myenvp);
int	get_var_name_size(char *str);
void	get_var_name(char *dest, char *str);

//msh_utils.c
// void	write_stderr(char *str);
void	*ft_realloc(void *oldptr, size_t oldsize, size_t newsize);
char	*str_realloc(char *oldstr, size_t newsize);
char	*ft_strndup(const char *s1, size_t n);
// void	free_arr(char **arr);

//bridge_var.c
void	bridge_var(char **str, char **myenvp);

//echo.c
void	wr_stillquotes(char *line, int fd, int *i, char quotes);
void	mywrite(char *line, int fd);
void	show_input(char **arr, int fd, int flag);
int		handle_echo(char *line, int fd);

//handle_export_unset.c
void	full_arr(int dlt_num, int size, char **result, char ***myenvp);
char	**my_unsetenv(char *name, char ***myenvp);
char	**create_new_arr(char *str, char **myenvp);
int		mysetenv(char *name, char *value, char ***myenvp);
int		find_var_in_env(char *name, char **myenvp);

//handle_export_unset_utils.c
int		handle_unset(char *line, char ***myenvp);
int		handle_export(char *line, char ***myenvp);

//msh_quotes.c
char	*remove_quotes(char *line);
char	*get_temp_remove_quotes(char *line, int *i, char quotes);
int		check_quotes(char *line);
char	*get_temp_remove_quotes(char *line, int *i, char quotes);

//msh_quotes_utils.c
char	*remove_quotes_first_word(char *line);
char	*copy_quoted_word(char *line, char *result, int i);
int		check_command_quotes(char *line);


int	in_redir(char *filename, int *status);
int	out_redir(char *filename, int *status, int *i, char opt);
void	reset_stdin(void);
int	gf_name_length(char *cmd, int *i, int *start);
char	*get_filename(char *cmd);
void	handle_heredoc_child(int write_fd, const char *delimiter, int *status, char **envp);
int	heredoc_parent(char *filename, int *status, int pipe_fd[2], pid_t pid);
int	heredoc_pipe_sign(char *filename, int *status, char **envp);
int	ch_redirect(char *line, int *i, char *filename, int *status, char **envp);
void	handle_redirection(char *line, int *status, char **envp);
void	jp_temp_s1(char *s1, char **temp, int *lens1, int lens2);
void	join_part(char **s1, char *s2);
void	redir_part(char *cmd, int *i);
char	*before_red(char *cmd, int *i);
char	*remove_redirects(char *cmd);
/*
The following file includes functions for checking 
state of different aspect of minishell
*/
//test_funcs.c
// void	fds_state(void);
// void	check_tty();

#endif