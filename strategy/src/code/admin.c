#include "main.h"
#include "parse.h"
#include "recycle.h"

/*
 * Stolen from save.c for reading in config stuff
 */
#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )			\
                if ( !str_cmp( word, literal ) )	\
        {					\
            field  = value;			\
            fMatch = TRUE;			\
            break;				\
        }

int colorstrlen args 		((const char *argument));
bool check_parse_name args	((char* name));  /* comm.c */
char    last_command[MSL];
int     compare args((const void *a, const void *b));
        
/*
 * Local functions.
 */
ROOM_INDEX_DATA *find_location args ((ACCOUNT_DATA * account, char *arg));

void do_wiznet (ACCOUNT_DATA * account, char *argument)
{
    int flag = 0;
    char buf[MSL];

    if (argument[0] == '\0')
    {
        if (IS_SET (account->wiznet, WIZ_ON))
        {
            SEND ("Signing off of Wiznet.\r\n", account);
            REMOVE_BIT (account->wiznet, WIZ_ON);
        }
        else
        {
            SEND ("Welcome to Wiznet!\r\n", account);
            SET_BIT (account->wiznet, WIZ_ON);
        }
        return;
    }

    if (!str_prefix (argument, "on"))
    {
        SEND ("Welcome to Wiznet!\r\n", account);
        SET_BIT (account->wiznet, WIZ_ON);
        return;
    }

    if (!str_prefix (argument, "off"))
    {
        SEND ("Signing off of Wiznet.\r\n", account);
        REMOVE_BIT (account->wiznet, WIZ_ON);
        return;
    }

     if (!str_prefix (argument, "show") ||  !str_prefix (argument, "status") ||  !str_prefix (argument, "list") ||  !str_prefix (argument, "options"))
        /* list of all wiznet options */
    {
        buf[0] = '\0';
        int i = 0;

        for (flag = 0; wiznet_table[flag].name != NULL; flag++)
        {
            if (IS_SET (account->wiznet, wiznet_table[flag].flag))				 
                strcat (buf, "({GON{x ) ");				
            else				
                strcat (buf, "({ROFF{x) ");
            
            
            strcat (buf, wiznet_table[flag].name);				
            strcat (buf, "     ");
            if (++i % 3 == 0)
                strcat (buf, "\r\n");   
        }

        strcat (buf, "\r\n");

        SEND ("Wiznet options available to you are:\r\n", account);
        SEND (buf, account);
        return;
    }

    //flag = wiznet_lookup (argument);

    if (flag == -1)
    {
        SEND ("No such option.\r\n", account);
        return;
    }

    if (IS_SET (account->wiznet, wiznet_table[flag].flag))
    {
        sprintf (buf, "You will no longer see %s on wiznet.\r\n",
                 wiznet_table[flag].name);
        SEND (buf, account);
        REMOVE_BIT (account->wiznet, wiznet_table[flag].flag);
        return;
    }
    else
    {
        sprintf (buf, "You will now see %s on wiznet.\r\n",
                 wiznet_table[flag].name);
        SEND (buf, account);
        SET_BIT (account->wiznet, wiznet_table[flag].flag);
        return;
    }

}

void wiznet (char *string, ACCOUNT_DATA * account, long flag, long flag_skip)
{
    DESCRIPTOR_DATA *d;

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected == CON_PLAYING
            && IS_SET (d->account->wiznet, WIZ_ON)
            && (!flag || IS_SET (d->account->wiznet, flag))
            && (!flag_skip || !IS_SET (d->account->wiznet, flag_skip))
             && d->account != account)
        {
            if (IS_SET (d->account->wiznet, WIZ_PREFIX))
                SEND ("{c--> ", d->account);
            else
                SEND ("{c", d->account);
            
            
            //act_new (string, d->account, obj, ch, TO_CHAR, POS_DEAD);
            SEND ("{x", d->account);
        }
    }

    return;
}
