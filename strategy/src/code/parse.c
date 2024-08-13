#include "main.h"
#include "parse.h"

/*
 * Command logging types.
 */
#define LOG_NORMAL  0
#define LOG_ALWAYS  1
#define LOG_NEVER   2

/*
 * Log-all switch.
 */
bool fLogAll = FALSE;

int nAllocString;
int nAllocPerm;



//commands table
const struct cmd_type cmd_table[] = 
{  
    //Placed here to not confuse with the immortal stat command.
    //name          function        log?,       show?   category  
    
    {"score",       do_score,       LOG_NORMAL, TRUE,   CMD_INFO},  
    {"memory",      do_memory,      LOG_NORMAL, TRUE,   CMD_INFO},
    {"commands",    do_commands,    LOG_NORMAL, TRUE,   CMD_INFO},
    {"city",        do_city,        LOG_NORMAL, TRUE,   CMD_INFO},
    {"empire",      do_empire,      LOG_NORMAL, TRUE,   CMD_INFO},
    {"who",         do_who,         LOG_NORMAL, TRUE,   CMD_INFO},
    
    
    {"build",       do_build,       LOG_NORMAL, TRUE,   CMD_CITY},
    {"upgrade",     do_upgrade,     LOG_NORMAL, TRUE,   CMD_CITY},
    {"settle",      do_settle,      LOG_NORMAL, TRUE,   CMD_CITY},
    {"switch",      do_switch,      LOG_NORMAL, TRUE,   CMD_CITY},
    {"trade",       do_trade,       LOG_NORMAL, TRUE,   CMD_CITY},


    {"chat",        do_chat,        LOG_NORMAL, TRUE,   CMD_COMM},
    {"/",           do_chat,        LOG_NORMAL, TRUE,   CMD_COMM},
    {"timestamps",  do_timestamps,  LOG_NORMAL, TRUE,   CMD_COMM},

    {"recruit",     do_recruit,     LOG_NORMAL, TRUE,   CMD_CITY},
    {"",            0,              LOG_NORMAL, FALSE,  -1}
};




/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret (ACCOUNT_DATA * account, char *argument)
{
    char command[MIL];
    char logline[MIL];
    int cmd;    
    bool found;

    //int string_count = nAllocString ;
    //int perm_count = nAllocPerm ;	
    //char buf[MSL] ;

    
    //Strip leading spaces.
    while (isspace (*argument))
        argument++;
    if (argument[0] == '\0')
        return;

    //Grab the command word.
    //Special parsing so ' can be a command,
    //also no spaces needed after punctuation.
    strcpy (logline, argument);     
    
    sprintf(last_command,"[%6ld,%6ld] [%12s] %s", account->x, account->y, account->name, logline);	    
    log_string(last_command);
    
    if (!isalpha (argument[0]) && !isdigit(argument[0]))
    {
        command[0] = argument[0];
        command[1] = '\0';
        argument++;
        while (isspace (*argument))
            argument++;
    }
    else
    {
        argument = one_argument (argument, command);
    }

    
    //Look for command in command table.     
    found = FALSE;    
    for (cmd = 0; !IS_NULLSTR(cmd_table[cmd].name); cmd++)
    {
        if (command[0] == cmd_table[cmd].name[0] && !str_prefix (command, cmd_table[cmd].name))
        {            
            found = TRUE;
            break;
        }
    }

    
    //Log and snoop.
    /*
    smash_dollar(logline);
    
    if (cmd_table[cmd].log == LOG_NEVER)
        strcpy (logline, "");
    */
    /*if ((IS_SET(account->penalty_flags, PEN_LOG)) || fLogAll || cmd_table[cmd].log == LOG_ALWAYS )
    {
        char    s[2*MIL],*ps;
        int     i;

        ps=s; 
        sprintf( log_buf, "Log %s: %s", account->name, logline );
       //Make sure that was is displayed is what is typed
        for (i=0;log_buf[i];i++)
        { 
            *ps++=log_buf[i];  
            if (log_buf[i]=='$')
                *ps++='$';
            if (log_buf[i]=='{')
                *ps++='{';
        }
        *ps=0;
        wiznet(s,account,WIZ_SECURE,0);
        log_string( log_buf );
    }*/

    /*if (ch->desc != NULL && ch->desc->snoop_by != NULL)
    {
        write_to_buffer (ch->desc->snoop_by, "% ", 2);
        write_to_buffer (ch->desc->snoop_by, logline, 0);
        write_to_buffer (ch->desc->snoop_by, "\r\n", 2);
    }*/

    if (!found)
    {        
        SEND ("That's not a valid command. (type {Ccommands{x for a list)\r\n", account);
        return;
    }
    

    //Call function and dispatch command    
    (*cmd_table[cmd].do_fun) (account, argument);    
    SEND("\r\n",account);

    /*if (string_count < nAllocString)
    {
        sprintf(buf,
        "Memcheck : Increase in alloc strings: {x%s {r[{xcommand: {r%s{x] [vnum: {r%ld{x]", ch->name, argument, ch->in_room->vnum) ;		
        wiznet(buf, NULL, NULL, WIZ_MEMORY,0,0) ;
    }

    if (perm_count < nAllocPerm)
    {
        if (ch->in_room != NULL)
        {	
            sprintf(buf, "Increase in alloc perms: {x%s {r[{xcommand: %s{r] [{xvnum: %ld{r]{x", ch->name, argument, ch->in_room->vnum) ;
            wiznet(buf, NULL, NULL, WIZ_MEMORY, 0,0) ;

        }
        else
        {
            sprintf(buf, "Increase in alloc perms: {x%s {r[{xcommand: %s{r]{x", ch->name, argument) ;
            wiznet(buf, NULL, NULL, WIZ_MEMORY, 0,0) ;
        }
    }
    */

    tail_chain ();
    return;
}

/* function to keep argument safe in all commands -- no static strings */
void do_function (ACCOUNT_DATA * account, DO_FUN * do_fun, char *argument)
{
    char *command_string;

    /* copy the string */
    command_string = str_dup (argument);

    /* dispatch the command */
    (*do_fun) (account, command_string);

    /* free the string */
    free_string (command_string);    
}

/*
 * Return true if an argument is completely numeric.
 */
bool is_number (char *arg)
{

    if (*arg == '\0')
        return FALSE;

    if (*arg == '+' || *arg == '-')
        arg++;

    for (; *arg != '\0'; arg++)
    {
        if (!isdigit (*arg))
            return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument (char *argument, char *arg)
{
    char *pdot;
    int number;

    for (pdot = argument; *pdot != '\0'; pdot++)
    {
        if (*pdot == '.')
        {
            *pdot = '\0';
            number = atoi (argument);
            *pdot = '.';
            strcpy (arg, pdot + 1);
            return number;
        }
    }

    strcpy (arg, argument);
    return 1;
}

/* 
 * Given a string like 14*foo, return 14 and 'foo'
*/
int mult_argument (char *argument, char *arg)
{
    char *pdot;
    int number;

    for (pdot = argument; *pdot != '\0'; pdot++)
    {
        if (*pdot == '*')
        {
            *pdot = '\0';
            number = atoi (argument);
            *pdot = '*';
            strcpy (arg, pdot + 1);
            return number;
        }
    }

    strcpy (arg, argument);
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument (char *argument, char *arg_first)
{
    char cEnd;

    while (isspace (*argument))
        argument++;

    cEnd = ' ';
    if (*argument == '\'' || *argument == '"')
        cEnd = *argument++;

    while (*argument != '\0')
    {
        if (*argument == cEnd)
        {
            argument++;
            break;
        }
        *arg_first = LOWER (*argument);
        arg_first++;
        argument++;
    }
    *arg_first = '\0';

    while (isspace (*argument))
        argument++;

    return argument;
}

CMD(do_commands)
{   
    char buf[MSL];
    int cmd;
    int col = 0;
    int category = 0;
    int count = 0;    
    int alpha = 0;
    int cmds = 0;
 
    if (argument[0] == '\0')
    {		    
        for (category = 1; category < 4; category++)	
        {         
            count = 0;           
            switch(category)
            {
                default:                    			 
                case CMD_INFO:                        
                    SEND("{cInformational Commands{x\r\n\r\n",account);
                    break;
                case CMD_CITY:                        
                    SEND("\r\n\r\n{cCity Related Commands{x\r\n\r\n",account);
                    break;
                case CMD_COMM:
                    SEND("\r\n\r\n{cCommunication Commands{x\r\n\r\n",account);                                                
                    break;                                             
            }                  
            
            for (alpha = 'a'; alpha <= 'z'; alpha++)
            {
                for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
                {
                    if (cmd_table[cmd].name[0] != alpha)
                        continue;

                    if (cmd_table[cmd].cat != category)
                        continue;  

                    if (cmd_table[cmd].show)
                    {                                                
                        sprintf( buf, "%s%s ", cmd_table[cmd].name, col < 7 ? "   " : "" );
                        SEND(buf, account);

                        count++;
                        cmds++;
                        
                        buf[0] = '\0';
                        if ( ++col % 7 == 0 )
                        {
                            col = 0;
                            SEND("\r\n",account);                        
                        }                        
                    }
                }                
            }                                    
        }

        sprintf(buf, "\r\n\r\nTotal commands available to you: %d\r\n\r\n", cmds);
        SEND (buf,account);
        return;
    }
    /*else
    {
        if (!str_cmp(argument,"?"))
        {
            send_to_char("Type commands with no argument to see all commands.\r\n",ch);
            send_to_char("Optional arguments: ?, config, common, communication, info, object, movement, combat, npc\r\n",ch);
            return;
        }
        else
        {            
            if (!str_prefix(argument, "config"))
                category = CONFIG;
            else if (!str_cmp(argument, "common"))
                category = COMMON;
            else if (!str_prefix(argument, "communication"))
                category = COMM;
            else if (!str_prefix(argument, "info"))
                category = INFO;
            else if (!str_prefix(argument, "object"))
                category = OBJ;
            else if (!str_prefix(argument, "movement"))
                category = MOVE;
            else if (!str_prefix(argument, "combat"))
                category = COMBAT;
            else if (!str_cmp(argument, "imm") || !str_cmp(argument, "immortal"))
                category = IMM;
            else if (!str_cmp(argument, "npc") || !str_cmp(argument, "pet") || !str_cmp(argument, "mount") || !str_prefix(argument, "charmie"))
                category = 99;
            else if (!str_cmp(argument, "event"))
                category = 999;
            else
            {
                send_to_char("Invalid argument.\r\n",ch);
                return;
            }
                    
            if (!IS_SET(ch->comm, COMM_BLIND))
            {
                for (alpha = 'a'; alpha <= 'z'; alpha++)
                {
                    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
                    {
                        if ( cmd_table[cmd].show && ((cmd_table[cmd].cat == category) || 
                        (category == 99 && cmd_table[cmd].npc_use) || (category == 999 && cmd_table[cmd].event_use)) )
                        {                            
                            if (cmd_table[cmd].cat == IMM && (!IS_IMMORTAL(ch) || category == 99 || category == 999))
                                continue;
                            
                            if (cmd_table[cmd].name[0] != alpha)
                                continue;

                            sprintf( buf, "%-12s   ", cmd_table[cmd].name );
                            count++;
                            send_to_char( buf, ch );
                            if ( ++col % 5 == 0 )
                                send_to_char( "\r\n", ch );
                            
                        }
                    }
                }
                if (col % 5 != 0 )
                {
                   send_to_char( "\r\n\r\n", ch );
                   col = 0;
                }				
            }
            else
            {
                for (alpha = 'a'; alpha <= 'z'; alpha++)
                {
                    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
                    {
                        if ( cmd_table[cmd].show && cmd_table[cmd].cat == category )
                        {
                            if (cmd_table[cmd].cat == IMM && !IS_IMMORTAL(ch))
                                continue;
                            if (cmd_table[cmd].name[0] != alpha)
                                continue;

                            sprintf( buf, "%s%s ", cmd_table[cmd].name, col < 7 ? "," : "" );
                            count++;
                            send_to_char( buf, ch );
                            if ( ++col % 7 == 0 )
                                send_to_char( "\r\n", ch );
                            
                        }
                    }
                }
                if (col % 5 != 0 )
                {
                   send_to_char( "\r\n\r\n", ch );
                   col = 0;
                }
            }
        }
        
    }*/
    return;
}

void do_wizhelp( ACCOUNT_DATA * account, char *argument )
{
    char buf[MSL];
    int cmd;
    int col;
    col = 0;
    char letter;
   
    for ( letter = 'a'; letter <= 'z'; letter++)
    {
        for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
        { 
            if (cmd_table[cmd].show)
            {
                if (cmd_table[cmd].name[0] == letter)
                {
                    sprintf( buf, "{x%-12s{x", cmd_table[cmd].name );
                    SEND( buf, account );
                    if ( ++col % 5 == 0 )
                    SEND( "\r\n", account );
                }
            }
        }
    }
 
    if ( col % 5 != 0 )
        SEND( "\r\n", account );
    return;
}
