#include "main.h"
#include "parse.h"
#include "recycle.h"
#include <time.h>

char *strptime(const char *restrict buf, const char *restrict format,
       struct tm *restrict tm);


struct flag_stat_type {
    const struct flag_type *structure;
    bool stat;
    char * name;
};


/*****************************************************************************
 Name:        flag_stat_table
 Purpose:    This table catagorizes the tables following the lookup
         functions below into stats and flags.  Flags can be toggled
         but stats can only be assigned.  Update this table when a
         new set of flags is installed.
 ****************************************************************************/
const struct flag_stat_type flag_stat_table[] = {
/*  {    structure        stat    }, */
    {room_flags, FALSE},    
    {0, 0}
};



/*****************************************************************************
 Name:        is_stat( table )
 Purpose:    Returns TRUE if the table is a stat table and FALSE if flag.
 Called by:    flag_value and flag_string.
 Note:        This function is local and used only in bit.c.
 ****************************************************************************/
bool is_stat (const struct flag_type *flag_table)
{
    int flag;

    for (flag = 0; flag_stat_table[flag].structure; flag++)
    {
        if (flag_stat_table[flag].structure == flag_table
            && flag_stat_table[flag].stat)
            return TRUE;
    }
    return FALSE;
}

/*****************************************************************************
 Name:        flag_value( table, flag )
 Purpose:    Returns the value of the flags entered.  Multi-flags accepted.
 Called by:    olc.c and act_olc.c.
 ****************************************************************************/
int flag_value (const struct flag_type *flag_table, char *argument)
{
    char word[MIL];
    int bit;
    int marked = 0;
    bool found = FALSE;

    if (is_stat (flag_table))
        return flag_lookup (argument, flag_table);

    /*
     * Accept multiple flags.
     */
    for (;;)
    {
        argument = one_argument (argument, word);

        if (word[0] == '\0')
            break;

        if ((bit = flag_lookup (word, flag_table)) != NO_FLAG)
        {
            SET_BIT (marked, bit);
            found = TRUE;
        }
    }

    if (found)
        return marked;
    else
        return NO_FLAG;
}



/*****************************************************************************
 Name:        flag_string( table, flags/stat )
 Purpose:    Returns string with name(s) of the flags or stat entered.
 Called by:    act_olc.c, olc.c, and olc_save.c.
 ****************************************************************************/
char *flag_string (const struct flag_type *flag_table, int bits)
{
    static char buf[2][512];
    static int cnt = 0;
    long flag;

    if (++cnt > 1)
        cnt = 0;

    buf[cnt][0] = '\0';

    for (flag = 0; flag_table[flag].name != NULL; flag++)
    {
        if (!is_stat (flag_table) && IS_SET (bits, flag_table[flag].bit))
        {
            strcat (buf[cnt], " ");
            strcat (buf[cnt], flag_table[flag].name);
        }
        else if (flag_table[flag].bit == bits)
        {
            strcat (buf[cnt], " ");
            strcat (buf[cnt], flag_table[flag].name);
            break;
        }
    }
    return (buf[cnt][0] != '\0') ? (char *)buf[cnt] + 1 : (char *)"none";
}



int flag_lookup (const char *name, const struct flag_type *flag_table)
{
    int flag;

    for (flag = 0; flag_table[flag].name != NULL; flag++)
    {
        if (LOWER (name[0]) == LOWER (flag_table[flag].name[0])
            && !str_prefix (name, flag_table[flag].name))
            return flag_table[flag].bit;
    }

    return NO_FLAG;
}

/*HELP_DATA *help_lookup (char *keyword)
{
    HELP_DATA *pHelp;
    char temp[MIL], argall[MIL];

    argall[0] = '\0';

    while (keyword[0] != '\0')
    {
        keyword = one_argument (keyword, temp);
        if (argall[0] != '\0')
            strcat (argall, " ");
        strcat (argall, temp);
    }

    for (pHelp = help_first; pHelp != NULL; pHelp = pHelp->next)
        if (is_name (argall, pHelp->keyword))
            return pHelp;

    return NULL;
}

HELP_AREA *had_lookup (char *arg)
{
    HELP_AREA *temp;
    extern HELP_AREA *had_list;

    for (temp = had_list; temp; temp = temp->next)
        if (!str_cmp (arg, temp->filename))
            return temp;

    return NULL;
}


// returns a flag for wiznet 
long wiznet_lookup (const char *name)
{
    int flag;

    for (flag = 0; wiznet_table[flag].name != NULL; flag++)
    {
        if (LOWER (name[0]) == LOWER (wiznet_table[flag].name[0])
            && !str_prefix (name, wiznet_table[flag].name))
            return flag;
    }

    return -1;
}
*/

//See if a string is one of the names in a list.
bool is_name (char *str, char *namelist)
{
    char name[MIL], part[MIL];
    char *list, *string;

    /* fix crash on NULL namelist */
    if (namelist == NULL || namelist[0] == '\0')
        return FALSE;

    /* fixed to prevent is_name on "" returning TRUE */
    if (str[0] == '\0')
        return FALSE;

    string = str;
    /* we need ALL parts of string to match part of namelist */
    for (;;)
    {                            /* start parsing string */
        str = one_argument (str, part);

        if (part[0] == '\0')
            return TRUE;

        /* check to see if this is part of namelist */
        list = namelist;
        for (;;)
        {                        /* start parsing namelist */
            list = one_argument (list, name);
            if (name[0] == '\0')    /* this name was not found */
                return FALSE;

            if (!str_prefix (string, name))
                return TRUE;    /* full pattern match */

            if (!str_prefix (part, name))
                break;
        }
    }
}

bool is_exact_name (char *str, char *namelist)
{
    char name[MIL];

    if (namelist == NULL)
        return FALSE;

    for (;;)
    {
        namelist = one_argument (namelist, name);
        if (name[0] == '\0')
            return FALSE;
        if (!str_cmp (str, name))
            return TRUE;
    }
}

/*
 * See if a string is one of the names of an object.
 */

bool is_full_name( const char *str, char *namelist )
{
    char name[MIL];

    for ( ; ; )
    {
        namelist = one_argument( namelist, name );
        if ( name[0] == '\0' )
            return FALSE;
        if ( !str_cmp( str, name ) )
            return TRUE;
    }
}


bool is_name2_prefix( const char *str, char *namelist )
{
   char name[MIL];   

   for( ;; )
   {
      namelist = one_argument( namelist, name );
      if( name[0] == '\0' )
         return FALSE;
      if( !str_prefix( str, name ) )
         return TRUE;
   }
}

bool nifty_is_name_prefix( char *str, char *namelist )
{
   char name[MIL];

   if( !str || str[0] == '\0' )
      return FALSE;

   for( ;; )
   {
      str = one_argument( str, name );
      if( name[0] == '\0' )
         return TRUE;
      if( !is_name2_prefix( name, namelist ) )
         return FALSE;
   }
}


bool account_exists (char * arg)
{
    char path[MSL];
    FILE * file;
    
    sprintf( path, "%s%s", ACCOUNT_DIR, capitalize( arg ) );
    file = fopen (path, "r");
    
    if (file)
    {
        fclose(file);
        return TRUE;						
    }	
    else
        return FALSE;				
        
        
    
}


char *timestamp( char strtime[16] )
{
   int  count;
   char buf[MSL], tim[16];

   
   sprintf( buf, "%s", ctime( &current_time ));

   for( count = 0; count < 16; count++ )
      tim[count] = buf[count+11];

   tim[5] = '\0';
   sprintf( strtime, "%s", tim );
   return str_dup(strtime);
}



void check_daily_login(ACCOUNT_DATA * account)
{
    //char buf[MSL];
    //time_t now = time(NULL);
    //struct tm *tm_struct = localtime(&now);

    //int hour = tm_struct->tm_hour;
    //int day = tm_struct->tm_mday;
    
    /*
    if (ch->daily_count == 0 || (day > (ch->daily_day + 1)) || (day == 1 && (ch->daily_day < 30 && ch->daily_day > 1)))
    {
        ch->daily_count = 1;
        ch->daily_level = 1;		
        ch->daily_hour = hour;
        ch->daily_day = day;
    }
    else
    {
        if ((ch->daily_hour <= hour) && (day > ch->daily_day))
        {
            ch->daily_count++;						
            ch->daily_day++;
            if (day == 1 && ch->daily_day > 29)
                ch->daily_day = 1;
            
            if (ch->daily_count > 9)
            {
                ch->daily_level++;
                ch->daily_count = 0;
            }
            
            sprintf(buf, "\r\n\r\n%s=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-={x\r\n", ch->score_color);
            SEND(buf,ch);
            
            sprintf(buf, "%s|                                                        |{x\r\n", ch->score_color);
            SEND(buf, ch);
            
            switch (ch->daily_level)
            {
                default:
                case 1:
                    ch->gold += (ch->daily_count * 20);
                    sprintf (buf, "%s|{x You receive {Y%d{x gold coins for your daily login bonus! %s|{x\r\n", ch->score_color, ch->daily_count * 20, ch->score_color);					
                    break;
                case 2:
                    ch->fish_points += (ch->daily_count * 10);
                    ch->fish_points_total += (ch->daily_count * 10);
                    sprintf (buf, "%s|{x You receive {C%d{x fish points for your daily login bonus! %s|{x\r\n", ch->score_color, ch->daily_count * 10, ch->score_color);
                    break;
                case 3:
                    ch->questpoints += (ch->daily_count * 8);
                    ch->questtotal += (ch->daily_count * 8);
                    sprintf (buf, "%s|{x You receive {M%d{x quest points for your daily login bonus! %s|{x\r\n", ch->score_color, ch->daily_count * 8, ch->score_color);
                    break;
            }
            SEND(buf, ch);
            
            sprintf(buf, "%s|                                                        |{x\r\n", ch->score_color);
            SEND(buf, ch);
            
            sprintf(buf, "%s=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-={x\r\n", ch->score_color);
            SEND(buf,ch);
        }
            
        if (ch->daily_level > 3)
            ch->daily_level = 1;			
    }
    */
    
    return;
    
    
}


char * port_directory(int port)
{
    char * path;

    switch (port)
    {
        default:
        case MAIN_PORT:
            path = MAIN_DIR;
            break;
        case TEST_PORT:
            path = TEST_DIR;
            break;
    }

    return path;
}

int get_sector(ROOM_INDEX_DATA *room)
{
    int sect;

    if (room->temp_sector != -1)
        sect = room->temp_sector;
    else
        sect = room->sector_type;

    return sect;
}



//This will add the fancy border I like to use to the length specified.
char *add_border(int length)
{
    int i = 0;
    static char border[MSL];

    for (i = 0; i < length; i++)
    {
        if (i % 2 == 0)
            border[i] = '-';
        else
            border[i] = '=';
    }
    border[i+1] = '\0';
    return border;
}


//This function returns how many different sector types are in game.
int count_sectors()
{
    int count = 0;
    int i = 0;

    for (i = 0; !IS_NULLSTR(sector_table[i].name); i++)
        count++;

    return count;
}


int comparetime(time_t time1,time_t time2)
{
    return difftime(time1,time2) > 0.0 ? 1 : -1; 
} 

char *format_date (ACCOUNT_DATA * account, long temp) 
{
    bool fNegative = FALSE;
    static char buf[MSL];
    char *time = ""; 
    
    temp = current_time - temp;
    
    if (time < 0) 
    {
        fNegative = TRUE;
        temp = temp * -1;
    } 

	temp = temp / 60;

	time = "minutes";

	if (temp >= 120) 
    {
   		temp = temp / 60;
  		time = "hours"; 
        if (temp >= 48) 
        {
       		temp = temp / 24;
       		time = "days";
	        if (temp >= 14) 
            {  
       		    temp = temp / 7;
       		    time = "weeks";
	            if (temp >= 8) 
                {
	                temp = temp / 4;
       		        time = "months";
	            }
	        }
	   	}   
    }
    if (fNegative) 
        sprintf(buf, "%ld %s from now", temp, time);
    else
        sprintf(buf, "%ld %s ago", temp, time);
    return buf; 
}
