#ifndef BUILTINS_H
# define BUILTINS_H

int	builtin_echo(char **args);
int	builtin_pwd(void);
int	builtin_cd(char **args);
int	builtin_env(void);
int	builtin_export(char **args);
int	builtin_unset(char **args);
int	builtin_exit(char **args);

#endif
