AC_INIT([minishell], [1.0], [kanekiayanami271@protonmail.com])

AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_PROG_CC

CPPFLAGS="$CFLAGS -I$HOME/.brew/opt/readline/include -I/usr/local/opt/readline/include"
LDFLAGS="$LDFLAGS -L$HOME/.brew/opt/readline/lib -L/usr/local/opt/readline/lib"

AC_CHECK_HEADER([readline/readline.h], [], [
    AC_MSG_ERROR([unable to find the readline header])
    ])

AC_CHECK_LIB([readline], [rl_replace_line], [], [
    AC_MSG_ERROR([unable to find the rl_replace_line function])
    ])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT