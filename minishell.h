/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:14:00 by alex              #+#    #+#             */
/*   Updated: 2025/05/05 13:54:35 by omalovic         ###   ########.fr       */
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

typedef struct s_redirect_args
{
	char	*line;
	char	*filename;
}	t_redirect_args;

typedef struct s_var_info
{
	char	*var_value;
	char	*prefix;
	char	*suffix;
}	t_var_info;

typedef struct s_shell
{
	int		status;
	char	**env;
}	t_shell;

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
	int					*status;
	char				*cmd;
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

typedef struct s_store
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
// char	*handle_heredoc_to_file(char *delimiter, int *status, char **envp);
// char	*generate_tmp_name(void);

//minishell.c
void	close_saved_std(struct s_saved_std *std);
void	remove_chars(char **str, char ch);
int		is_nummeric(char *line);
void	handle_exit(char *line, int *status,
			struct s_saved_std *std, char **myenvp);
void	sig_handler(int sig);
int		ft_getcwd(char *line, int fd);
void	free_arr(char **arr);
int		handle_cd(char *line, char ***myenvp);
int		check_line(char *line, int i);
int		check_quastion_sign(char **line, int status);
char	*remove_first_spaces(char *line);
void	disable_ctrl_c_output(int *status);
void	setup_signal_handlers(void);
char	*my_getenv(char **myenvp, const char *name);
int		print_export(char ***myenvp);
int		print_env(char ***myenvp);
int		finish_write_cmd_path(char **buffer, char *path, char *cmd);
char	*find_cmd_in_paths(char **path_arr, char *cmd);
char	*get_command_path(char *cmd, char **myenvp);
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
void	hanlde_quotes(char *str, int i, int *flag_single, int *flag_double);

//check_var.c
int		handle_dollar(char **str, t_var_info *var_data,
			int dollar_pos, char *dollar);
int		cmp_names(char *name1, char *name2);
char	*find_var_value(char *var_name, char **myenvp);
int		get_var_name_size(char *str);
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
int		is_valid_identifier(const char *str);
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
int		check_command_quotes(char *line);

int		in_redir(char *filename, int *status);
int		out_redir(char *filename, int *status, int *i, char opt);
void	reset_stdin(void);
int		gf_name_length(char *cmd, int *i, int *start);
char	*get_filename(char *cmd);
void	handle_heredoc_child(int write_fd, const char *delimiter,
			int *status, char **envp);
int		heredoc_parent(int *status, int pipe_fd[2], pid_t pid);
int		heredoc_pipe_sign(char *filename, int *status, char **envp);
int		handle_redirection(char *line, int *status, char **envp);
void	jp_temp_s1(char *s1, char **temp, int *lens1, int lens2);
void	join_part(char **s1, char *s2);
void	redir_part(char *cmd, int *i);
char	*before_red(char *cmd, int *i);
char	*remove_redirects(char *cmd);
void	execute_pipe_commands(char *cmd, int *status, char ***myenvp);
int		check_empty_cmd(char *line);
int		check_quotes_arg(char *line);
int		has_heredoc(char *cmd);
int		save_and_redirect(struct s_saved_std *std, char *command,
			int *status, char **envp);
int		pipe_cmd_exist(t_pipe_data *data, char **clean_cmd, int i);
char	**process_command_args(char **cmd_args);
void	close_pipefd(struct s_pipe_data *data);
int		check_data_cmd(t_pipe_data *data);
void	wait_for_last_pid(pid_t last_pid, int *status);
void	close_saved_std(struct s_saved_std *std);
void	execute_command_single(char *cmd, int *status, char ***myenvp);
int		clean_command_args(char **cmd_arr);
void	handle_empty_command(char *cmd, int *status, char **envp);
char	*my_strchr_quotes(char *s, int c);
char	**copy_arr(char **arr);
int		if_exit(char *line);
void	handle_gnl(char **line, int *status);
int		init_data(t_pipe_data *data, char *cmd, int *status);
int		bva_newstr(char *str, int dollar_pos, t_var_info *var, char **myenvp);
int		replace_str_with_var(char **str, t_var_info *var_data, char *new_str);
char	*build_new_string(t_var_info *var_data);

#endif