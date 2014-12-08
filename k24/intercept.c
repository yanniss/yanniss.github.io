#include <sys/ioctl.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define LINESIZE 128
#define BACKSPACE 0x7f

int getchar_silent()
{
    int ch;
    struct termios oldt, newt;

    /* Retrieve old terminal settings */
    tcgetattr(STDIN_FILENO, &oldt);

    /* Disable canonical input mode, and input character echoing. */
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );

    /* Set new terminal settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    /* Read next character, and then switch to old terminal settings. */
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

int main (int argv, const char *args[])
{
    int next;

    struct winsize w;

    /* Keep reading one character at a time */
    while ((next = getchar_silent()) != EOF)
    {
        /* Print normal characters */
        if (isalpha(next) || ispunct(next) || next == ' ' || next == '\n')
        {
            putchar(next);
            continue;
        }

        /* Treat special characters differently */
        switch(next) {

        case '\t':              /* Just read a tab */

            /* Get terminal window size */
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

            /* Fill next line with asterisks */
            putchar('\n');
            for (int i = 0; i < w.ws_col; i++)
                putchar('*');
            putchar('\n');
            break;

        case BACKSPACE:         /* Just read a backspace */

            /* Move the cursor one step back, print space, and move
             * cursor back once more to imitate backward character
             * deletion.
             */

            printf("\b \b");
            break;

        case VEOF:              /* Just read EOF (Ctrl-D) */

            /* The EOF character is recognized only in canonical input
             * mode. In noncanonical input mode, all input control
             * characters such as Ctrl-D are passed to the application
             * program exactly as typed. Thus, Ctrl-D now produces
             * this character instead.
             */

            printf("\n\nExiting. Bye...");
            goto THE_END;
        default:
            continue;
        }
    }
THE_END:
    putchar('\n');
    return EXIT_SUCCESS;
}
