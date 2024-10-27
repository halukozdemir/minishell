#include "../minishell.h"

void ft_echo(char **args)
{
    int n_flag = 0;
    int i = 1;

    if (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
    {
        n_flag = 1;
        i++;
    }
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (!n_flag)
        printf("\n");
}

// cd with only a relative or absolute path
void ft_cd(char **args, char **new_env)
{
    if (!args[1])
    {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }
    if (chdir(args[1]) != 0)
    {
        perror("cd");
    }
    else
    {
        // Update PWD in new_env
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            int i = 0;
            while (new_env[i])
            {
                if (ft_strncmp(new_env[i], "PWD=", 4) == 0)
                {
                    free(new_env[i]);
                    new_env[i] = malloc(ft_strlen("PWD=") + ft_strlen(cwd) + 1);
                    ft_strlcpy(new_env[i], "PWD=", ft_strlen("PWD=") + 1);
                    ft_strlcat(new_env[i], cwd, ft_strlen("PWD=") + ft_strlen(cwd) + 1);
                    break;
                }
                i++;
            }
        }
    }
}

// pwd with no options
void ft_pwd(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("pwd");
    }
}

static t_env *create_env_var(const char *name, const char *value)
{
    t_env *new_var = malloc(sizeof(t_env));
    if (!new_var)
        return NULL;

    new_var->key = ft_strdup(name);
    if (!new_var->key)
    {
        free(new_var);
        return NULL;
    }

    new_var->value = value ? ft_strdup(value) : ft_strdup("");
    if (!new_var->value)
    {
        free(new_var->key);
        free(new_var);
        return NULL;
    }

    new_var->next = NULL;
    return new_var;
}

static void update_env_var(t_env *env_var, const char *value)
{
    if (env_var->value)
        free(env_var->value);
    env_var->value = value ? ft_strdup(value) : ft_strdup("");
}

static void add_env_var(t_env **env_list, t_env *new_var)
{
    if (!*env_list)
    {
        *env_list = new_var;
        return;
    }

    t_env *current = *env_list;
    while (current->next)
    {
        current = current->next;
    }
    current->next = new_var;
}

static t_env *find_env_var(t_env *env_list, const char *name)
{
    while (env_list)
    {
        if (ft_strncmp(env_list->key, name, ft_strlen(name)) == 0 && ft_strlen(env_list->key) == ft_strlen(name))
            return env_list;
        env_list = env_list->next;
    }
    return NULL;
}



void ft_export(char **args, t_env **env_list)
{
    int i = 1;
    char *equal_sign;

    while (args[i])
    {
        equal_sign = ft_strchr(args[i], '=');
        if (equal_sign)
        {
            // Split key and value
            int key_len = equal_sign - args[i];
            char key[key_len + 1];
            ft_strlcpy(key, args[i], key_len + 1);

            char *value = equal_sign + 1;
            t_env *existing_var = find_env_var(*env_list, key);
            if (existing_var)
            {
                update_env_var(existing_var, value);
            }
            else
            {
                t_env *new_var = create_env_var(key, value);
                if (new_var)
                    add_env_var(env_list, new_var);
            }
        }
        else
        {
            // If there's no '=', just add the key with an empty value
            t_env *existing_var = find_env_var(*env_list, args[i]);
            if (!existing_var)
            {
                t_env *new_var = create_env_var(args[i], "");
                if (new_var)
                    add_env_var(env_list, new_var);
            }
        }
        i++;
    }
}
// unset with no options
void ft_unset(char **args, char ***new_env)
{
    if (!args[1])
    {
        fprintf(stderr, "unset: missing argument\n");
        return;
    }
    int i = 0;
    while ((*new_env)[i])
    {
        if (ft_strncmp((*new_env)[i], args[1], ft_strlen(args[1])) == 0 && (*new_env)[i][ft_strlen(args[1])] == '=')
        {
            free((*new_env)[i]);
            while ((*new_env)[i + 1])
            {
                (*new_env)[i] = (*new_env)[i + 1];
                i++;
            }
            (*new_env)[i] = NULL;
            return;
        }
        i++;
    }
}

// env with no options or arguments
void ft_env(char **new_env)
{
    int i = 0;
    while (new_env[i])
    {
        printf("%s\n", new_env[i]);
        i++;
    }
}

// exit with no options
void ft_exit(void)
{
    printf("exit\n");
    exit(0);
}
