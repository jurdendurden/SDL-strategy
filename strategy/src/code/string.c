
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"

char *string_linedel (char *, int);
char *string_lineadd (char *, char *, int);
char *numlines (char *);

/*****************************************************************************
 Name:        string_edit
 Purpose:    Clears string and puts account into editing mode.
 Called by:    none
 ****************************************************************************/
void string_edit (ACCOUNT_DATA * account, char **pString)
{
    SEND ("-========- Entering EDIT Mode -=========-\r\n", account);
    SEND ("    Type .h on a new line for help\r\n", account);
    SEND (" Terminate with a ~ or @ on a blank line.\r\n", account);
    SEND ("-=======================================-\r\n", account);

    if (*pString == NULL)
    {
        *pString = str_dup ("");
    }
    else
    {
        **pString = '\0';
    }

    account->d->pString = pString;

    return;
}



/*****************************************************************************
 Name:        string_append
 Purpose:    Puts account into append mode for given string.
 Called by:    (many)olc_act.c
 ****************************************************************************/
void string_append (ACCOUNT_DATA * account, char **pString)
{
    SEND ("-=======- Entering APPEND Mode -========-\r\n", account);
    SEND ("    Type .h on a new line for help\r\n", account);
    SEND (" Terminate with a ~ or @ on a blank line.\r\n", account);
    SEND ("-=======================================-\r\n", account);

    if (*pString == NULL)
    {
        *pString = str_dup ("");
    }
    SEND (numlines (*pString), account);

/* numlines sends the string with \r\n */
/*  if ( *(*pString + strlen( *pString ) - 1) != '\r' )
    SEND( "\r\n", account ); */

    account->d->pString = pString;

    return;
}



/*****************************************************************************
 Name:        string_replace
 Purpose:    Substitutes one string for another.
 Called by:    string_add(string.c) (aedit_builder)olc_act.c.
 ****************************************************************************/
char *string_replace (char *orig, char *old, char *new_str)
{
    char xbuf[MSL];
    int i;

    xbuf[0] = '\0';
    strcpy (xbuf, orig);
    if (strstr (orig, old) != NULL)
    {
        i = strlen (orig) - strlen (strstr (orig, old));
        xbuf[i] = '\0';
        strcat (xbuf, new_str);
        strcat (xbuf, &orig[i + strlen (old)]);
        free_string (orig);
    }

    return str_dup (xbuf);
}



/*****************************************************************************
 Name:        string_add
 Purpose:    Interpreter for string editing.
 Called by:    game_loop_xxxx(comm.c).
 ****************************************************************************/
void string_add (ACCOUNT_DATA * account, char *argument)
{
    char buf[MSL];

    smash_tilde (argument);

    if (*argument == '.')
    {
        char arg1[MIL];
        char arg2[MIL];
        char arg3[MIL];
        char tmparg3[MIL];

        argument = one_argument (argument, arg1);
        argument = first_arg (argument, arg2, FALSE);
        strcpy (tmparg3, argument);
        argument = first_arg (argument, arg3, FALSE);

        if (!str_cmp (arg1, ".c"))
        {
            SEND ("String cleared.\r\n", account);
            free_string (*account->d->pString);
            *account->d->pString = str_dup ("");
            return;
        }

        if (!str_cmp (arg1, ".s"))
        {
            SEND ("String so far:\r\n", account);
            SEND (numlines (*account->d->pString), account);
            return;
        }

        if (!str_cmp (arg1, ".r"))
        {
            if (arg2[0] == '\0')
            {
                SEND ("usage:  .r \"old string\" \"new string\"\r\n", account);
                return;
            }

            *account->d->pString =
                string_replace (*account->d->pString, arg2, arg3);
            sprintf (buf, "'%s' replaced with '%s'.\r\n", arg2, arg3);
            SEND (buf, account);
            return;
        }

        if (!str_cmp (arg1, ".f"))
        {
            *account->d->pString = format_string (*account->d->pString);
            SEND ("String formatted.\r\n", account);
            return;
        }

        if (!str_cmp (arg1, ".ld"))
        {
            if (!is_number(arg2))
            {
                SEND("Which line did you want to replace?\r\n",account);
                return;
            }
            *account->d->pString =
                string_linedel (*account->d->pString, atoi (arg2));
            SEND ("Line deleted.\r\n", account);
            return;
        }

        if (!str_cmp (arg1, ".li"))
        {
            if (!is_number(arg2))
            {
                SEND("Which line did you want to replace?\r\n",account);
                return;
            }            
            *account->d->pString =
                string_lineadd (*account->d->pString, tmparg3, atoi (arg2));
            SEND ("Line inserted.\r\n", account);
            return;
        }

        if (!str_cmp (arg1, ".lr"))
        {
            if (!is_number(arg2))
            {
                SEND("Which line did you want to replace?\r\n",account);
                return;
            }
            *account->d->pString =
                string_linedel (*account->d->pString, atoi (arg2));                
            *account->d->pString =
                string_lineadd (*account->d->pString, tmparg3, atoi (arg2));            
            SEND ("Line replaced.\r\n", account);
            return;
        }

        if (!str_cmp (arg1, ".h"))
        {
            SEND ("Sedit help (commands on blank line):   \r\n", account);
            SEND (".r 'old' 'new'   - replace a substring \r\n", account);
            SEND ("                   (requires '', \"\") \r\n", account);
            SEND (".h               - get help (this info)\r\n", account);
            SEND (".s               - show string so far  \r\n", account);
            SEND (".f               - (word wrap) string  \r\n", account);
            SEND (".c               - clear string so far \r\n", account);
            SEND (".ld <num>        - delete line number <num>\r\n",
                          account);
            SEND (".li <num> <str>  - insert <str> at line <num>\r\n",
                          account);
            SEND
                (".lr <num> <str>  - replace line <num> with <str>\r\n",
                 account);
            SEND ("@                - end string          \r\n", account);
            return;
        }

        SEND ("SEdit:  Invalid dot command.\r\n", account);
        return;
    }

    if (*argument == '~' || *argument == '@')
    {        
        account->d->pString = NULL;
        return;
    }

    strcpy (buf, *account->d->pString);

    /*
     * Truncate strings to MSL.
     * --------------------------------------
     * Edwin strikes again! Fixed avoid adding to a too-long
     * note. JR -- 10/15/00
     */
    if (strlen ( *account->d->pString ) + strlen (argument) >= (MSL - 4))
    {
        SEND ("String too long, last line skipped.\r\n", account);

        /* Force character out of editing mode. */
        account->d->pString = NULL;
        return;
    }

    /*
     * Ensure no tilde's inside string.
     * --------------------------------
     */
    smash_tilde (argument);

    strcat (buf, argument);
    strcat (buf, "\r\n");
    free_string (*account->d->pString);
    *account->d->pString = str_dup (buf);
    return;
}


/*****************************************************************************
 Name:        format_string
 Purpose:    Special string formating and word-wrapping.
 Called by:    string_add(string.c) (many)olc_act.c
 ****************************************************************************/

 char *format_string( char *oldstring /*, bool fSpace */)
{
    char    xbuf[MSL];
    char    xbuf2[MSL];
    char   *rdesc;
    int     i = 0;
    int     end_of_line;
    bool    cap = TRUE;
    bool    bFormat = TRUE;

    xbuf[0] = xbuf2[0] = 0;

    i = 0;

    for (rdesc = oldstring; *rdesc; rdesc++)
    {
        if (*rdesc != '`')
        {
            if (bFormat)
            {
                if (*rdesc == '\n')
                {
                    if (*(rdesc + 1) == '\r' && *(rdesc + 2) == ' ' && *(rdesc + 3) == '\n' && xbuf[i - 1] !='\r')
                    {
                        xbuf[i] = '\n';
                        xbuf[i + 1] = '\r';
                        xbuf[i + 2] = '\n';
                        xbuf[i + 3] = '\r';
                        i += 4;
                        rdesc += 2;
                    }
                    else if (*(rdesc + 1) == '\r' && *(rdesc + 2) == ' ' && *(rdesc + 2) == '\n' && xbuf[i - 1] == '\r')
                    {
                        xbuf[i] = '\n';
                        xbuf[i + 1] = '\r';
                        i += 2;
                    }
                    else if (*(rdesc + 1) == '\r' && *(rdesc + 2) == '\n' && xbuf[i - 1] != '\r')
                    {
                        xbuf[i] = '\n';
                        xbuf[i + 1] = '\r';
                        xbuf[i + 2] = '\n';
                        xbuf[i + 3] = '\r';
                        i += 4;
                        rdesc += 1;
                    }
                    else if (*(rdesc + 1) == '\r' && *(rdesc + 2) == '\n' && xbuf[i - 1] == '\r')
                    {
                        xbuf[i] = '\n';
                        xbuf[i + 1] = '\r';
                        i += 2;
                    }
                    else if (xbuf[i - 1] != ' ' && xbuf[i - 1] != '\r')
                    {
                        xbuf[i] = ' ';
                        i++;
                    }
                }
                else if (*rdesc == '\r') ;
                else if (*rdesc == 'i' && *(rdesc + 1) == '.' && *(rdesc + 2) == 'e' && *(rdesc + 3) == '.')
                {
                    xbuf[i] = 'i';
                    xbuf[i + 1] = '.';
                    xbuf[i + 2] = 'e';
                    xbuf[i + 3] = '.';
                    i += 4;
                    rdesc += 3;
                }
                else if (*rdesc == ' ')
                {
                    if (xbuf[i - 1] != ' ')
                    {
                        xbuf[i] = ' ';
                        i++;
                    }
                }
                else if (*rdesc == ')')
                {
                    if (xbuf[i - 1] == ' ' && xbuf[i - 2] == ' '
                       && (xbuf[i - 3] == '.' || xbuf[i - 3] == '?' || xbuf[i - 3] == '!'))
                    {
                        xbuf[i - 2] = *rdesc;
                        xbuf[i - 1] = ' ';
                        xbuf[i] = ' ';
                        i++;
                    }
                    else if (xbuf[i - 1] == ' ' && (xbuf[i - 2] == ',' || xbuf[i - 2] == ';'))
                    {
                        xbuf[i - 1] = *rdesc;
                        xbuf[i] = ' ';
                        i++;
                    }
                    else
                    {
                        xbuf[i] = *rdesc;
                        i++;
                    }
                }
                else if (*rdesc == ',' || *rdesc == ';')
                {
                    if (xbuf[i - 1] == ' ')
                    {
                        xbuf[i - 1] = *rdesc;
                        xbuf[i] = ' ';
                        i++;
                    }
                    else
                    {
                        xbuf[i] = *rdesc;
                        if (*(rdesc + 1) != '\"')
                        {
                            xbuf[i + 1] = ' ';
                            i += 2;
                        }
                        else
                        {
                            xbuf[i + 1] = '\"';
                            xbuf[i + 2] = ' ';
                            i += 3;
                            rdesc++;
                        }
                    }

                }
                else if ( *rdesc == '?' || *rdesc == '!')
                {
                    if (xbuf[i - 1] == ' ' && xbuf[i - 2] == ' '
                       && (xbuf[i - 3] == '.' || xbuf[i - 3] == '?' || xbuf[i - 3] == '!'))
                    {
                        xbuf[i - 2] = *rdesc;
                        if (*(rdesc + 1) != '\"')
                        {
                            xbuf[i - 1] = ' ';
                            xbuf[i] = ' ';
                            i++;
                        }
                        else
                        {
                            xbuf[i - 1] = '\"';
                            xbuf[i] = ' ';
                            xbuf[i + 1] = ' ';
                            i += 2;
                            rdesc++;
                        }
                    }
                    else
                    {
                        xbuf[i] = *rdesc;
                        if (*(rdesc + 1) != '\"')
                        {
                            xbuf[i + 1] = ' ';
                            xbuf[i + 2] = ' ';
                            i += 3;
                        }
                        else
                        {
                            xbuf[i + 1] = '\"';
                            xbuf[i + 2] = ' ';
                            xbuf[i + 3] = ' ';
                            i += 4;
                            rdesc++;
                        }
                    }
                    cap = TRUE;
                }
                else
                {
                    xbuf[i] = *rdesc;
                    if (cap)
                    {
                        cap = FALSE;
                        xbuf[i] = UPPER (xbuf[i]);
                    }
                    i++;
                }
            }
            else
            {
                xbuf[i] = *rdesc;
                i++;
            }
        }
        else
        {
            if (*(rdesc + 1) == 'Z')
                bFormat = !bFormat;
            xbuf[i] = *rdesc;
            i++;
            rdesc++;
            xbuf[i] = *rdesc;
            i++;
        }
    }
    xbuf[i] = 0;
    strcpy (xbuf2, xbuf);

    rdesc = xbuf2;

    xbuf[0] = 0;

    for (;;)
    {
        end_of_line = 80;
        for (i = 0; i < end_of_line; i++)
        {
            if (*(rdesc + i) == '`')
            {
                end_of_line += 2;
                i++;
            }

            if (!*(rdesc + i))
                break;

            if (*(rdesc + i) == '\r')
                end_of_line = i;
        }
        if (i < end_of_line)
        {
            break;
        }
        if (*(rdesc + i - 1) != '\r')
        {
            for (i = (xbuf[0] ? (end_of_line - 1) : (end_of_line - 4)); i; i--)
            {
                if (*(rdesc + i) == ' ')
                    break;
            }
            if (i)
            {
                *(rdesc + i) = 0;
                strcat (xbuf, rdesc);
                strcat (xbuf, "\n\r");
                rdesc += i + 1;
                while (*rdesc == ' ')
                    rdesc++;
            }
            else
            {
                bug ("`5Wrap_string: `@No spaces``", 0);
                *(rdesc + (end_of_line - 2)) = 0;
                strcat (xbuf, rdesc);
                strcat (xbuf, "-\n\r");
                rdesc += end_of_line - 1;
            }
        }
        else
        {
            *(rdesc + i - 1) = 0;
            strcat (xbuf, rdesc);
            strcat (xbuf, "\r");
            rdesc += i;
            while (*rdesc == ' ')
                rdesc++;
        }
    }
    while (*(rdesc + i) && (*(rdesc + i) == ' ' ||
          *(rdesc + i) == '\n' ||
          *(rdesc + i) == '\r'))
        i--;
    *(rdesc + i + 1) = 0;
    strcat (xbuf, rdesc);
    if (xbuf[strlen (xbuf) - 2] != '\n')
        strcat (xbuf, "\n\r");

    free_string (oldstring);
    return (str_dup (xbuf));
}
 





/*
 * Used above in string_add.  Because this function does not
 * modify case if fCase is FALSE and because it understands
 * parenthesis, it would probably make a nice replacement
 * for one_argument.
 */
/*****************************************************************************
 Name:        first_arg
 Purpose:    Pick off one argument from a string and return the rest.
         Understands quates, parenthesis (barring ) ('s) and
         percentages.
 Called by:    string_add(string.c)
 ****************************************************************************/
char *first_arg (char *argument, char *arg_first, bool fCase)
{
    char cEnd;

    while (*argument == ' ')
        argument++;

    cEnd = ' ';
    if (*argument == '\'' || *argument == '"'
        || *argument == '%' || *argument == '(')
    {
        if (*argument == '(')
        {
            cEnd = ')';
            argument++;
        }
        else
            cEnd = *argument++;
    }

    while (*argument != '\0')
    {
        if (*argument == cEnd)
        {
            argument++;
            break;
        }
        if (fCase)
            *arg_first = LOWER (*argument);
        else
            *arg_first = *argument;
        arg_first++;
        argument++;
    }
    *arg_first = '\0';

    while (*argument == ' ')
        argument++;

    return argument;
}


/*
 * Used in olc_act.c for aedit_builders.
 */ 
char *string_unpad (char *argument)
{
    char buf[MSL];
    char *s;

    s = argument;

    while (*s == ' ')
        s++;

    strcpy (buf, s);
    s = buf;

    if (*s != '\0')
    {
        while (*s != '\0')
            s++;
        s--;

        while (*s == ' ')
            s--;
        s++;
        *s = '\0';
    }

    free_string (argument);
    return str_dup (buf);
}



/*
 * Same as capitalize but changes the pointer's data.
 * Used in olc_act.c in aedit_builder.
 */
char *string_proper (char *argument)
{
    char *s;

    s = argument;

    while (*s != '\0')
    {
        if (*s != ' ')
        {
            *s = UPPER (*s);
            while (*s != ' ' && *s != '\0')
                s++;
        }
        else
        {
            s++;
        }
    }

    return argument;
}

char *string_linedel (char *string, int line)
{
    char *strtmp = string;
    char buf[MSL];
    int cnt = 1, tmp = 0;

    buf[0] = '\0';

    for (; *strtmp != '\0'; strtmp++)
    {
        if (cnt != line)
            buf[tmp++] = *strtmp;

        if (*strtmp == '\n')
        {
            if (*(strtmp + 1) == '\r')
            {
                if (cnt != line)
                    buf[tmp++] = *(++strtmp);
                else
                    ++strtmp;
            }

            cnt++;
        }
    }

    buf[tmp] = '\0';

    free_string (string);
    return str_dup (buf);
}

char *string_lineadd (char *string, char *newstr, int line)
{
    char *strtmp = string;
    int cnt = 1, tmp = 0;
    bool done = FALSE;
    char buf[MSL];

    buf[0] = '\0';

    for (; *strtmp != '\0' || (!done && cnt == line); strtmp++)
    {
        if (cnt == line && !done)
        {
            strcat (buf, newstr);
            strcat (buf, "\r\n");
            tmp += strlen (newstr) + 2;
            cnt++;
            done = TRUE;
        }

        buf[tmp++] = *strtmp;

        if (done && *strtmp == '\0')
            break;

        if (*strtmp == '\n')
        {
            if (*(strtmp + 1) == '\r')
                buf[tmp++] = *(++strtmp);

            cnt++;
        }

        buf[tmp] = '\0';
    }

    free_string (string);
    return str_dup (buf);
}


char *get_line (char *str, char *buf)
{
    int tmp = 0;
    bool found = FALSE;

    while (*str)
    {
        if (*str == '\n')
        {
            found = TRUE;
            break;
        }

        buf[tmp++] = *(str++);
    }

    if (found)
    {
        if (*(str + 1) == '\r')
            str += 2;
        else
            str += 1;
    }                            

    buf[tmp] = '\0';

    return str;
}


char *numlines (char *string)
{
    int cnt = 1;
    static char buf[MSL * 3];
    char buf2[MSL * 2], tmpb[MSL];

    buf[0] = '\0';

    while (*string)
    {
        string = get_line (string, tmpb);
        sprintf (buf2, "%2d. %s\r\n", cnt++, tmpb);
        strcat (buf, buf2);
    }

    return buf;
}

//This function will strip things such as "a" "an" or "the" from a string.
//Upro 3/16/2020
char * strip_articles (char * buf)
{
    if (strlen(buf) < 3)
        return buf;

    if (buf[0] == 'a')
    {
        if (buf[1] == ' ' || (buf[1] == 'n' && buf[2] == ' '))
        {
            int x = 0;
            while (buf[x] != '\0')
            {
                buf[x] = buf[x+2];
                x++;
            }
        }
    }

    if (strlen(buf) > 3)
    {
        if (buf[0] == 't' && buf[1] == 'h' && buf[2] == 'e' && buf[3] == ' ')
        {
            int x = 0;
            while (buf[x] != '\0')
            {
                buf[x] = buf[x+3];
                x++;
            }
        }
    }

    return buf;
}

char *strip_quotes (const char * str)
{
    static char strfix[MSL * 2];
    int oldstr = 0, newstr = 0;

    while ( str[oldstr] != '\0' && oldstr < MSL*2 )
    {
        if ( str[oldstr] == '"' )                    
            oldstr++;        
        else
        {
            strfix[newstr] = str[oldstr];
            newstr++;
        }
        oldstr++;
    }
    strfix[newstr] = '\0';
    return strfix;    
}

char *strip_single_quotes (const char * str)
{
    static char strfix[MSL * 2];
    int oldstr = 0, newstr = 0;

    while ( str[oldstr] != '\0' && oldstr < MSL*2 )
    {
        if ( str[oldstr] == 39 )
            oldstr++;        
        else
        {
            strfix[newstr] = str[oldstr];
            newstr++;
        }
        oldstr++;
    }
    strfix[newstr] = '\0';
    return strfix;    
}

/* 
 * Strips color codes out of a string really fast.  Also strips
 * {{, {-, {*, and anything else, for that matter. 
 */
char *strip_color( const char *str )
{
    static char strfix[MSL * 2];
    int oldstr = 0, newstr = 0;

    while ( str[oldstr] != '\0' && oldstr < MSL*2 )
    {
        if ( str[oldstr] == '{' )
        {
            if ( str[oldstr+1] != '\0' )
            oldstr++;
        }
        else
        {
            strfix[newstr] = str[oldstr];
            newstr++;
        }
        oldstr++;
    }
    strfix[newstr] = '\0';
    return strfix;
}

/* 
 * Escapes color codes
 */
char *escape_color( const char *str )
{
    static char strfix[MSL * 2];
    int oldstr = 0, newstr = 0;

    while ( str[oldstr] != '\0' && oldstr < MSL*2 )
    {
        if ( str[oldstr] == '{' )
            strfix[newstr++] = '{';
        strfix[newstr++] = str[oldstr++];
    }
    strfix[newstr] = '\0';
    return strfix;
}

/* Removes \n and \r from strings.  Useful for printing time data */
char *smash_crlf( const char *str )
{
    static char strfix[MSL * 2];
    int oldstr = 0, newstr = 0;

    while ( str[oldstr] != '\0' )
    {
        if ( str[oldstr] == '\n' || str[oldstr] == '\r' )
            oldstr++;
        else
        {
            strfix[newstr] = str[oldstr];
            newstr++;
        }
        oldstr++;
    }
    strfix[newstr] = '\0';
    return strfix;
}


/*
 * Removes the tildes from a string.
 * Used for account-entered strings that go into disk files.
 */
void smash_tilde (char *str)
{
    for (; *str != '\0'; str++)
    {
        if (*str == '~')
            *str = '-';
    }

    return;
}
/*
 * Removes dollar signs to keep snerts from crashing us.
 * Posted to ROM list by Kyndig. JR -- 10/15/00
 */
void smash_dollar( char *str )
{
    for( ; *str != '\0'; str++)
    {
        if( *str == '$' )
            *str = 'S';
    }
    return;
}


char * smash_period(char *str)
{
    static char strfix[MSL * 2];
    int oldstr = 0, newstr = 0;

    while ( str[oldstr] != '\0' )
    {
        if ( str[oldstr] == '.')
            oldstr++;
        else
        {
            strfix[newstr] = str[oldstr];
            newstr++;
        }
        oldstr++;
    }
    strfix[newstr] = '\0';
    return strfix;
}

void strip_char (char *str, char character)
{
    char *pr = str, *pw = str;
    
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != character);
    }
    *pw = '\0';
}

/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool str_cmp (const char *astr, const char *bstr)
{
    //char buf[MSL];
    
    if (astr == NULL)
    {
        bug ("Str_cmp: null astr.", 0);		
        //do_function (NULL, &do_echo, (char *)(*bstr));		
        log_string (bstr);
        return TRUE;
    }

    if (bstr == NULL)
    {
        bug ("Str_cmp: null bstr.", 0);
        //buf = str_dup(bstr);
        //do_function (NULL, &do_echo, (char *)(*astr));
        return TRUE;
    }

    for (; *astr || *bstr; astr++, bstr++)
    {
        if (LOWER (*astr) != LOWER (*bstr))
            return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix (const char *astr, const char *bstr)
{
    if (astr == NULL)
    {
        bug ("Strn_cmp: null astr.", 0);
        return TRUE;
    }

    if (bstr == NULL)
    {
        bug ("Strn_cmp: null bstr.", 0);
        return TRUE;
    }

    for (; *astr; astr++, bstr++)
    {
        if (LOWER (*astr) != LOWER (*bstr))
            return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix (const char *astr, const char *bstr)
{
    int sstr1;
    int sstr2;
    int ichar;
    char c0;

    if ((c0 = LOWER (astr[0])) == '\0')
        return FALSE;

    sstr1 = strlen (astr);
    sstr2 = strlen (bstr);

    for (ichar = 0; ichar <= sstr2 - sstr1; ichar++)
    {
        if (c0 == LOWER (bstr[ichar]) && !str_prefix (astr, bstr + ichar))
            return FALSE;
    }

    return TRUE;
}



/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix (const char *astr, const char *bstr)
{
    int sstr1;
    int sstr2;

    sstr1 = strlen (astr);
    sstr2 = strlen (bstr);
    if (sstr1 <= sstr2 && !str_cmp (astr, bstr + sstr2 - sstr1))
        return FALSE;
    else
        return TRUE;
}



/*
 * Returns an initial-capped string.
 */
char *capitalize (const char *str)
{
    static char strcap[MSL];
    int i;

    for (i = 0; str[i] != '\0'; i++)
        strcap[i] = LOWER (str[i]);
    strcap[i] = '\0';
    strcap[0] = UPPER (strcap[0]);
    return strcap;
}

char *to_lower (const char * str)
{
    static char strcap[MSL];
    int i;

    for (i = 0; str[i] != '\0'; i++)
        strcap[i] = LOWER(str[i]);
    strcap[i] = '\0';    
    strcap[0] = LOWER (strcap[0]);
    return strcap;
}
