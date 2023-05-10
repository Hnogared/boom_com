#include "../Includes/defuser_wizard.h"

int chrstr(char c, char *str)
{
    int i;

    i = -1;
    while (str[++i]){
        if (str[i] == c)
            return (1);
    }
    return (0);
}
