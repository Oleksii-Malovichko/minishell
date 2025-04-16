/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:14:00 by alex              #+#    #+#             */
/*   Updated: 2025/04/16 11:07:46 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/resource.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <stdbool.h>

# include "lib/get_next_line/get_next_line.h"
# include "lib/libft/libft.h"

extern int g_heredoc;
extern int g_status;

typedef struct s_saved_std
{
	int	saved_stdin;
	int	saved_stdout;
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

//builtins/echo.c
void	wr_stillquotes(char *line, int fd, int *i, char quotes);
void	mywrite(char *line, int fd);
void	show_input(char **arr, int fd, int flag);
int		only_contains_char(char *str, char ch);
int		handle_echo(char *line, int fd);

//builtins/get_pwd.c
int		ft_getcwd(char *line, int fd);

//builtins/handle_cd.c
int		cd_arguments(char **arr, char **path);
int		handle_cd(char *line);

//builtins/handle_exit.c
int		check_exit_args(char **line_arr, int *status);
void	handle_exit(char *line, int *status);

//builtins/handle_export_unset.c
int		find_var_in_env(char *name);
int		add_user_env_var(char *name);
int		is_user_env_var(char *name);

//builtins/print_env.c
int		print_env(int fd);

//check_redir.c
int		gf_name_length(char *cmd, int *i, int *start);
char	*get_filename(char *cmd);
void	handle_heredoc_child(int write_fd, const char *delimiter);
// void	handle_heredoc(const char *delimiter);

//check_redir_utils.c
int		in_redir(char *filename, int *status);
int		out_redir(char *filename, int *status, int *i, char opt);
void	reset_stdin(void);


//check_question_sign.c
void	copy_from_sign(char *result, char *line, char *status, int index);
void	change_to_exit_status(int i, char **line, char *status);
int		check_quastion_sign(char **line, char *status);

//minishell.c
void	sig_handler(int sig);
int		check_line(char *line, int i);
char	*remove_first_spaces(char *line);
void	disable_ctrl_c_output(int *status);
void	setup_signal_handlers(void);
int		is_nummeric(char *line);
int		finish_write_cmd_path(char **buffer, char *path, char *cmd);
char	*get_command_path(char *cmd);
char	**get_commands(char *cmd, char *temp);
void	wait_for_last_pid(pid_t last_pid, int *status);
char	*find_cmd_in_paths(char **path_arr, char *cmd);
char	**process_command_args(char **cmd_args, char *cmd);
void	handle_child_process(t_pipe_data data, char *cmd, int i, int *status);
void	handle_parent_process(struct s_pipe_data *data, int i, pid_t pid, int *last_pid);
void	remove_chars(char **str, char ch);
void	execute_pipe_commands(char *cmd, int *status);
void	handle_exit(char *line, int *status);
void	ft_error(char *error, int exit_status);
int		handle_cd(char *line);
int		check_quastion_sign(char **line, char *status);
void	run_ex(char **line, int *status);
int		main(void);

//msh_cmd.c
int		is_builtin_choose(char *clean_cmd);
int		is_builtin(char *cmd);
void	exec_builtin_choose(char *clean_cmd, int fd, int *status);
void	execute_builtin(char *cmd, int fd, int *status);

//check_line.c
int		ft_isoperator(int c);
int		ft_isspace(int c);
int		skip_whites(char *line, int	*i);
int		is_empty(char *line);

//bridge_var.c
int		bva_newstr(char *str, int dollar_pos, char **var_value, char **suffix);
int		bridge_var_at(char **str, int dollar_pos);
void	bridge_var(char **str);

//check_var.c
int		cmp_names(char *name1, char *name2);
char	*find_var_value(char *var_name);
int		get_var_name_size(char *str);
void	get_var_name(char *dest, char *str);
void	change_str(char **str, char *name, char *value);
void	remove_var_name(char **str, char *name);

//handle_export_unset.c
void	free_var_after_exit();
int		find_var_in_env(char *name);
int		mysetenv(char *name, char *value);
int		handle_export(char *line, int fd);
int		my_unsetenv(char *name);
int		handle_unset(char *line, int fd);

//msh_quotes.c
void	gtrq_search_quotes(char *line, int *end, char quotes);
char	*get_temp_remove_quotes(char *line, int *i, char quotes);
void	rq_copy(char *line, char *result, int len);
char	*remove_quotes(char *line);

//msh_quotes_utils.c
int		rqfw_copy(char *line, char *result, int i);
char	*remove_quotes_first_word(char *line);
int		ch_quotes_part(char *line, int *i, int *end_q);
int		check_quotes(char *line);
char	*remove_quotes_commands(char *line);
int		check_command_quotes(char *line);
char	*ft_join(char **arr);

//msh_utils.c
char	*ft_strndup(const char *s1, size_t n);
void	print_arr(char **strarr);
void	*ft_realloc(void *oldptr, size_t oldsize, size_t newsize);
char	*str_realloc(char *oldstr, size_t newsize);
void	free_arr(char **arr);
void	write_stderr(char *str);

//remove_redirects.c
void	jp_temp_s1(char *s1, char **temp, int *lens1, int lens2);
void	join_part(char **s1, char *s2);
void	redir_part(char *cmd, int *i);
char	*before_red(char *cmd, int *i);
char	*remove_redirects(char *cmd);

//handle_redirect.c
int		heredoc_parent(char *filename, int *status, int pipe_fd[2], pid_t pid);
int		heredoc_pipe_sign(char *filename, int *status);
int		ch_redirect(char *line, int *i, char *filename, int *status);
void	handle_redirection(char *line, int *status);

/*
The following file includes functions for checking 
state of different aspect of minishell
*/
// test_funcs.c
void	fds_state(void);
void	check_tty();

#endif