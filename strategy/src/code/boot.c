#include "main.h"
#include "recycle.h"
#include "parse.h"

int getpid ();
time_t time (time_t * tloc);


ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];


extern int flag_lookup

args ((const char *name, const struct flag_type * flag_table));


////////////// GLOBAL WORLD DATA VARIABLES //////////////////
int crash_protect = 0;

bool newlock = FALSE;
bool wizlock = FALSE;

int empire_count;
int city_count;
//////////////////////////////////////////////////////////////


//NOTE_DATA *note_free;
extern DESCRIPTOR_DATA *descriptor_free;

int FilesOpen;
FILE_DATA  *file_list;

char bug_buf[2 * MIL];
ACCOUNT_DATA * account_list;
char *help_greeting;
char log_buf[2 * MIL];
NOTE_DATA *		note_list;
TIME_INFO_DATA time_info;
WEATHER_DATA weather_info;

//Globals for strategy game
EMPIRE_DATA * empire_list;
CITY_DATA * city_list;
BUILDING_DATA * building_list;
TROOP_DATA * troop_list;
ACCOUNT_DATA * account_list;

EMPIRE_DATA * empire_last;
CITY_DATA * city_last;
BUILDING_DATA * building_last;
TROOP_DATA * troop_last;

EMPIRE_DATA * empire_first;
CITY_DATA * city_first;
BUILDING_DATA * building_first;
TROOP_DATA * troop_first;

char *string_hash[MAX_KEY_HASH];

FILE * bug_file;

char *string_space;
char *top_string;
char str_empty[1];

int top_help;
long top_room;
long top_vnum_room;
int top_map_qd;
int top_map_visit;
int command_count;
int top_account;
int top_empire;
int top_city;


//Memory management.
//Increase MAX_STRING if you have too.
//Tune the others only if you understand what you're doing.
#define            MAX_STRING    1413120
#define            MAX_PERM_BLOCK    131072
#define            MAX_MEM_LIST    11

void *rgFreeList[MAX_MEM_LIST];
const int rgSizeList[MAX_MEM_LIST] = {
    16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768 - 64
};

extern int nAllocString;
int sAllocString;
extern int nAllocPerm;
int sAllocPerm;


bool fBootDb;  //did we boot properly?


void init_mm args ((void));
void load_helps args ((FILE * fp, char *fname));
void load_rooms args ((FILE * fp));
void load_bans args ((void));

//Loads up the game map saved for this server. If nothing exists, creates a new world.
void load_game_world()
{

    return;
}


//loads initial game information, world file, cities, buildings, etc
void boot_db ()
{
    //initialize global string space
    {
        if ((string_space = (char *)calloc (1, MAX_STRING)) == NULL)
        {
            bug ("Boot_db: can't alloc %d string space.", MAX_STRING);
            exit (1);
        }
        top_string = string_space;
        fBootDb = TRUE;
    }

    //initialize random number generator.    
    init_mm();
    
    
    //set time and weather.    
    {
        long lhour, lday, lmonth;

        lhour = (current_time - 650336715) / (PULSE_SECOND * 60);
        time_info.hour = lhour % 24;
		
        lday = lhour / 24;
        time_info.day = lday % 35;
        lmonth = lday / 35;
        time_info.month = lmonth % 17;
        time_info.year = lmonth / 17;

        if (time_info.hour < 5)
            weather_info.sunlight = SUN_DARK;
        else if (time_info.hour < 6)
            weather_info.sunlight = SUN_RISE;
        else if (time_info.hour < 19)
            weather_info.sunlight = SUN_LIGHT;
        else if (time_info.hour < 20)
            weather_info.sunlight = SUN_SET;
        else
            weather_info.sunlight = SUN_DARK;

        weather_info.change = 0;
        weather_info.mmhg = 960;
        if (time_info.month >= 7 && time_info.month <= 12)
            weather_info.mmhg += number_range (1, 50);
        else
            weather_info.mmhg += number_range (1, 80);        

		weather_info.moon_phase = number_range(0, 7);

    }    
		
	int cmd = 0;
	
	for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )    
		command_count++;
		
    create_world();
	
    //load all empires in


    //load all cities in

        
    //load all buildings in
    
    
    return;
}

//read single letter from file
char fread_letter (FILE * fp)
{
    char c;

    do
    {
        c = getc (fp);
    }
    while (isspace (c));

    return c;
}



//read single number from file
int fread_number (FILE * fp)
{
    long number;
    bool sign;
    char c;

    do
    {
        c = getc (fp);
    }
    while (isspace (c));

    number = 0;

    sign = FALSE;
    if (c == '+')
    {
        c = getc (fp);
    }
    else if (c == '-')
    {
        sign = TRUE;
        c = getc (fp);
    }

    if (!isdigit (c))
    {
        bug ("fread_number: alpha format.", 0);
        exit (1);
    }

    while (isdigit (c))
    {
        number = number * 10 + c - '0';
        c = getc (fp);
    }

    if (sign)
        number = 0 - number;

    if (c == '|')
        number += fread_number (fp);
    else if (c != ' ')
        ungetc (c, fp);

    return number;
}

long fread_flag (FILE * fp)
{
    long number;
    char c;
    bool negative = FALSE;

    do
    {
        c = getc (fp);
    }
    while (isspace (c));

    if (c == '-')
    {
        negative = TRUE;
        c = getc (fp);
    }

    number = 0;

    if (!isdigit (c))
    {
        while (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
        {
            number += flag_convert (c);
            c = getc (fp);
        }
    }

    while (isdigit (c))
    {
        number = number * 10 + c - '0';
        c = getc (fp);
    }

    if (c == '|')
        number += fread_flag (fp);

    else if (c != ' ')
        ungetc (c, fp);

    if (negative)
        return -1 * number;

    return number;
}

long flag_convert (char letter)
{
    long bitsum = 0;
    char i;

    if ('A' <= letter && letter <= 'Z')
    {
        bitsum = 1;
        for (i = letter; i > 'A'; i--)
            bitsum *= 2;
    }
    else if ('a' <= letter && letter <= 'z')
    {
        bitsum = 67108864;        /* 2^26 */
        for (i = letter; i > 'a'; i--)
            bitsum *= 2;
    }

    return bitsum;
}




/*
 * Read and allocate space for a string from a file.
 * These strings are read-only and shared.
 * Strings are hashed:
 *   each string prepended with hash pointer to prev string,
 *   hash code is simply the string length.
 *   this function takes 40% to 50% of boot-up time.
 */
char *fread_string (FILE * fp)
{
    char *plast;
    char c;

    plast = top_string + sizeof (char *);
    if (plast > &string_space[MAX_STRING - MSL])
    {
        bug ("Fread_string: MAX_STRING %d exceeded.", MAX_STRING);
        exit (1);
    }

    //skip blanks and only read first char.
    do
    {
        c = getc (fp);
    }
    while (isspace (c));

    if ((*plast++ = c) == '~')
        return &str_empty[0];

    for (;;)
    {
        //Back off the char type lookup,
        //it was too dirty for portability.                  

        switch (*plast = getc (fp))
        {
            default:
                plast++;
                break;

            case EOF:
                /* temp fix */
                bug ("Fread_string: EOF", 0);
                return NULL;
                /* exit( 1 ); */
                break;

            case '\n':
                plast++;
                *plast++ = '\r';
                break;

            case '\r':
                break;

            case '~':
                plast++;
                {
                    union {
                        char *pc;
                        char rgc[sizeof (char *)];
                    } u1;
                    int ic;
                    int iHash;
                    char *pHash;
                    char *pHashPrev;
                    char *pString;

                    plast[-1] = '\0';
                    iHash = UMIN (MAX_KEY_HASH - 1, plast - 1 - top_string);
                    for (pHash = string_hash[iHash]; pHash; pHash = pHashPrev)
                    {
                        for (ic = 0; ic < sizeof (char *); ic++)
                            u1.rgc[ic] = pHash[ic];
                        pHashPrev = u1.pc;
                        pHash += sizeof (char *);

                        if (top_string[sizeof (char *)] == pHash[0]
                            && !strcmp (top_string + sizeof (char *) + 1,
                                        pHash + 1))
                            return pHash;
                    }

                    if (fBootDb)
                    {
                        pString = top_string;
                        top_string = plast;
                        u1.pc = string_hash[iHash];
                        for (ic = 0; ic < sizeof (char *); ic++)
                            pString[ic] = u1.rgc[ic];
                        string_hash[iHash] = pString;

                        nAllocString += 1;
                        sAllocString += top_string - pString;
                        return pString + sizeof (char *);
                    }
                    else
                    {
                        return str_dup (top_string + sizeof (char *));
                    }
                }
        }
    }
}


char *fread_string_nohash( FILE * fp )
{
   char buf[MSL];
   char *plast;
   char c;
   int ln;

   plast = buf;
   buf[0] = '\0';
   ln = 0;

   /*
    * Skip blanks.
    * Read first char.
    */
   do
   {
      if( feof( fp ) )
      {
         log_string( "fread_string_no_hash: EOF encountered on read.\r\n" );
		 sprintf (buf, "This is %s() from %s, line %d\n",__FUNCTION__, __FILE__, __LINE__);
		 log_string(buf);		 
         if( fBootDb )
            exit( 1 );
         return str_dup( "" );
      }
      c = getc( fp );
   }
   while( isspace( c ) );

   if( ( *plast++ = c ) == '~' )
      return str_dup( "" );

   for( ;; )
   {
      if( ln >= ( MSL - 1 ) )
      {
         log_string( "fread_string_no_hash: string too long" );
         *plast = '\0';
         return str_dup( buf );
      }
      switch ( *plast = getc( fp ) )
      {
         default:
            plast++;
            ln++;
            break;

         case EOF:
            log_string( "Fread_string_no_hash: EOF" );
            if( fBootDb )
               exit( 1 );
            *plast = '\0';
            return str_dup( buf );
            break;

         case '\n':
            plast++;
            ln++;
            *plast++ = '\r';
            ln++;
            break;

         case '\r':
            break;

         case '~':
            *plast = '\0';
            return str_dup( buf );
      }
   }
}


char *fread_string_eol (FILE * fp)
{
    static bool char_special[256 - EOF];
    char *plast;
    char c;

    if (char_special[EOF - EOF] != TRUE)
    {
        char_special[EOF - EOF] = TRUE;
        char_special['\n' - EOF] = TRUE;
        char_special['\r' - EOF] = TRUE;
    }

    plast = top_string + sizeof (char *);
    if (plast > &string_space[MAX_STRING - MSL])
    {
        bug ("Fread_string: MAX_STRING %d exceeded.", MAX_STRING);
        exit (1);
    }

    /*
     * Skip blanks.
     * Read first char.
     */
    do
    {
        c = getc (fp);
    }
    while (isspace (c));

    if ((*plast++ = c) == '\n')
        return &str_empty[0];

    for (;;)
    {
        if (!char_special[(*plast++ = getc (fp)) - EOF])
            continue;

        switch (plast[-1])
        {
            default:
                break;

            case EOF:
                bug ("Fread_string_eol  EOF", 0);
                exit (1);
                break;

            case '\n':
            case '\r':
                {
                    union {
                        char *pc;
                        char rgc[sizeof (char *)];
                    } u1;
                    int ic;
                    int iHash;
                    char *pHash;
                    char *pHashPrev;
                    char *pString;

                    plast[-1] = '\0';
                    iHash = UMIN (MAX_KEY_HASH - 1, plast - 1 - top_string);
                    for (pHash = string_hash[iHash]; pHash; pHash = pHashPrev)
                    {
                        for (ic = 0; ic < sizeof (char *); ic++)
                            u1.rgc[ic] = pHash[ic];
                        pHashPrev = u1.pc;
                        pHash += sizeof (char *);

                        if (top_string[sizeof (char *)] == pHash[0]
                            && !strcmp (top_string + sizeof (char *) + 1,
                                        pHash + 1))
                            return pHash;
                    }

                    if (fBootDb)
                    {
                        pString = top_string;
                        top_string = plast;
                        u1.pc = string_hash[iHash];
                        for (ic = 0; ic < sizeof (char *); ic++)
                            pString[ic] = u1.rgc[ic];
                        string_hash[iHash] = pString;

                        nAllocString += 1;
                        sAllocString += top_string - pString;
                        return pString + sizeof (char *);
                    }
                    else
                    {
                        return str_dup (top_string + sizeof (char *));
                    }
                }
        }
    }
}



char *fread_line( FILE *fp )
{
    static char line[MSL];
	char buf[MSL];
    char *pline;
    char c;
    int ln;
 
    pline = line;
    line[0] = '\0';
    ln = 0;
 
    /*
     * Skip blanks.
     * Read first char.
     */
    do
    {
	if ( feof(fp) )
	{
	    bug("fread_line: EOF encountered on read.\n\r", 0);
		sprintf (buf, "This is %s() from %s, line %d\n",__FUNCTION__, __FILE__, __LINE__);
		 log_string(buf);		 
	    if ( fBootDb )
		exit(1);
	    strcpy(line, "");
	    return line;
	}
	c = getc( fp );
    }
    while ( isspace(c) );
 
    ungetc( c, fp );
    do
    {
	if ( feof(fp) )
	{
	    bug("fread_line: EOF encountered on read.\n\r", 0);
		sprintf (buf, "This is %s() from %s, line %d\n",__FUNCTION__, __FILE__, __LINE__);
		 log_string(buf);		 
	    if ( fBootDb )
		exit(1);
	    *pline = '\0';
	    return line;
	}
	c = getc( fp );
	*pline++ = c; ln++;
	if ( ln >= (MSL - 1) )
	{
	    bug( "fread_line: line too long", 0 );
	    break;
	}
    }
    while ( c != '\n' && c != '\r' );
 
    do
    {
	c = getc( fp );
    }
    while ( c == '\n' || c == '\r' );
 
    ungetc( c, fp );
    *pline = '\0';
    return line;
}
 


/*
 * Read to end of line (for comments).
 */
void fread_to_eol (FILE * fp)
{
    char c;

    do
    {
        c = getc (fp);
    }
    while (c != '\n' && c != '\r');

    do
    {
        c = getc (fp);
    }
    while (c == '\n' || c == '\r');

    ungetc (c, fp);
    return;
}



/*
 * Read one word (into static buffer).
 */
char *fread_word (FILE * fp)
{
    static char word[MIL];
    char *pword;
    char cEnd;

    do
    {
        cEnd = getc (fp);
    }
    while (isspace (cEnd));

    if (cEnd == '\'' || cEnd == '"')
    {
        pword = word;
    }
    else
    {
        word[0] = cEnd;
        pword = word + 1;
        cEnd = ' ';
    }

    for (; pword < word + MIL; pword++)
    {
        *pword = getc (fp);
        if (cEnd == ' ' ? isspace (*pword) : *pword == cEnd)
        {
            if (cEnd == ' ')
                ungetc (*pword, fp);
            *pword = '\0';
            return word;
        }
    }

    bug ("Fread_word: word too long.", 0);
    exit (1);
    return NULL;
}

/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void *alloc_mem (int sMem)
{
    void *pMem;


    //pMem = malloc(sMem);


    int *magic;
    int iList;

    sMem += sizeof (*magic);

    for (iList = 0; iList < MAX_MEM_LIST; iList++)
    {
        if (sMem <= rgSizeList[iList])
            break;
    }

    if (iList == MAX_MEM_LIST)
    {
        bug ("Alloc_mem: size %d too large.", sMem);
        exit (1);
    }

    if (rgFreeList[iList] == NULL)
    {
        pMem = alloc_perm (rgSizeList[iList]);
    }
    else
    {
        pMem = rgFreeList[iList];
        rgFreeList[iList] = *((void **) rgFreeList[iList]);
    }

    magic = (int *) pMem;
    *magic = MAGIC_NUM;
    pMem += sizeof (*magic);

    
    return pMem;
}



/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void free_mem (void *pMem, int sMem)
{
    //free (pMem);
    
    int iList;
    int *magic;

    pMem -= sizeof (*magic);
    magic = (int *) pMem;

    if (*magic != MAGIC_NUM)
    {
        bug ("Attempt to recyle invalid memory of size %d.", sMem);
        bug ((char *) pMem + sizeof (*magic), 0);
        return;
    }

    *magic = 0;
    sMem += sizeof (*magic);

    for (iList = 0; iList < MAX_MEM_LIST; iList++)
    {
        if (sMem <= rgSizeList[iList])
            break;
    }

    if (iList == MAX_MEM_LIST)
    {
        bug ("Free_mem: size %d too large.", sMem);
        exit (1);
    }

    *((void **) pMem) = rgFreeList[iList];
    rgFreeList[iList] = pMem;
    return;    
}


/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *alloc_perm (int sMem)
{
    void *pMem;

    //pMem = malloc(sMem);


    static char *pMemPerm;
    static int iMemPerm;    

    while (sMem % sizeof (long) != 0)
        sMem++;
    if (sMem > MAX_PERM_BLOCK)
    {
        bug ("Alloc_perm: %d too large.", sMem);
        exit (1);
    }

    if (pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK)
    {
        iMemPerm = 0;
        if ((pMemPerm = (char *)calloc (1, MAX_PERM_BLOCK)) == NULL)
        {
            perror ("Alloc_perm");
            exit (1);
        }
    }

    pMem = pMemPerm + iMemPerm;
    iMemPerm += sMem;
    nAllocPerm += 1;
    sAllocPerm += sMem;

    return pMem;

}



/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
 */
char *str_dup (const char *str)
{
    char *str_new;

    //str_new = strdup(str);


    if (str[0] == '\0')
        return &str_empty[0];

    if (str >= string_space && str < top_string)
        return (char *) str;

    str_new = alloc_mem (strlen (str) + 1);
    strcpy (str_new, str);


    return str_new;   
}



/*
 * Free a string.
 * Null is legal here to simplify callers.
 * Read-only shared strings are not touched.
 */
void free_string (char *pstr)
{
    if (pstr == NULL || pstr == &str_empty[0]
        || (pstr >= string_space && pstr < top_string))
        return;

    free_mem (pstr, strlen (pstr) + 1);
    return;
}



/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy (int number)
{
    switch (number_range(0,3))
    {
        case 0:
            number -= 1;
            break;
        case 3:
            number += 1;
            break;
        default:
            break;
    }

    return UMAX (1, number);
}



/*
 * Generate a random number.
 */
int number_range (int from, int to)
{
    int power;
    int number;

    if (from == 0 && to == 0)
        return 0;

    if ((to = to - from + 1) <= 1)
        return from;

    for (power = 2; power < to; power <<= 1);

    while ((number = number_mm () & (power - 1)) >= to);

    return from + number;
}



/*
 * Generate a percentile roll.
 */
int number_percent (void)
{
    int percent;

    while ((percent = number_mm () & (128 - 1)) > 99);

    return 1 + percent;
}



/*
 * Generate a random door.
 */
int number_door (void)
{
    int door;

    while ((door = number_mm () & (8 - 1)) > 5);

    return door;
}

/*
 * I've gotten too many bad reports on OS-supplied random number generators.
 * This is the Mitchell-Moore algorithm from Knuth Volume II.
 * Best to leave the constants alone unless you've read Knuth.
 * -- Furey
 */

/* I noticed streaking with this random number generator, so I switched
   back to the system srandom call.  If this doesn't work for you, 
   define OLD_RAND to use the old system -- Alander */

#if defined (OLD_RAND)
static int rgiState[2 + 55];
#endif

void init_mm ()
{
#if defined (OLD_RAND)
    int *piState;
    int iState;

    piState = &rgiState[2];

    piState[-2] = 55 - 55;
    piState[-1] = 55 - 24;

    piState[0] = ((int) current_time) & ((1 << 30) - 1);
    piState[1] = 1;
    for (iState = 2; iState < 55; iState++)
    {
        piState[iState] = (piState[iState - 1] + piState[iState - 2])
            & ((1 << 30) - 1);
    }
#else
    srandom (time (NULL) ^ getpid ());
#endif
    return;
}



long number_mm (void)
{
#if defined (OLD_RAND)
    int *piState;
    int iState1;
    int iState2;
    int iRand;

    piState = &rgiState[2];
    iState1 = piState[-2];
    iState2 = piState[-1];
    iRand = (piState[iState1] + piState[iState2]) & ((1 << 30) - 1);
    piState[iState1] = iRand;
    if (++iState1 == 55)
        iState1 = 0;
    if (++iState2 == 55)
        iState2 = 0;
    piState[-2] = iState1;
    piState[-1] = iState2;
    return iRand >> 6;
#else
    return random () >> 6;
#endif
}


/*
 * Roll some dice.
 */
int dice (int number, int size)
{
    int idice;
    int sum;

    switch (size)
    {
        case 0:
            return 0;
        case 1:
            return number;
    }

    for (idice = 0, sum = 0; idice < number; idice++)
        sum += number_range (1, size);

    return sum;
}



/*
 * Simple linear interpolation.
 */
int interpolate (int level, int value_00, int value_32)
{
    return value_00 + level * (value_32 - value_00) / 32;
}



// Append a string to a file.
// Currently used in: bug command, shutdown command, orphaned helps, and laston command
void append_file (ACCOUNT_DATA * account, char *file, char *str)
{
    FILE *fp;

    if (!account)
        return;

    if (str[0] == '\0')
        return;

    fclose (fpReserve);
    if ((fp = fopen (file, "a")) == NULL)
    {
        perror (file);
        WTB (account->d, "Could not open the file!\r\n", 0);
    }
    else
    {
        char *strtime = ctime (&current_time);
        strtime[strlen (strtime) - 1] = '\0';

        fprintf (fp, "[%5ld,%5ld] [%-14s] [%s] %s\n", account->x, account->y, account->name, strtime, str);
        fclose (fp);
    }

    fpReserve = fopen (NULL_FILE, "r");
    return;
}

//Reports a bug to the logfiles.
void bug (const char *str, int param)
{
    char buf[MSL];	

    if (bug_file != NULL)
    {
        int iLine;
        int iChar;

        if (bug_file == stdin)
        {
            iLine = 0;
        }
        else
        {
            iChar = ftell (bug_file);
            fseek (bug_file, 0, 0);
            for (iLine = 0; ftell (bug_file) < iChar; iLine++)
            {
                while (getc (bug_file) != '\n');
            }
            fseek (bug_file, iChar, 0);
        }

        sprintf (buf, "[*****] LINE: %d", iLine);
        log_string (buf);
    }

    strcpy (buf, "[*****] BUG: ");
    sprintf (buf + strlen (buf), str, param);
    log_string (buf);


	
	wiznet( buf,NULL,WIZ_BUGS,0);

	return;
}

/*
 * Writes a string to the log.
 */
void log_string (const char *str)
{
    char *strtime;

    strtime = ctime (&current_time);
    strtime[strlen (strtime) - 1] = '\0';
    fprintf (stderr, "%s :: %s\n", strtime, str);
    return;
}



/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain (void)
{
    return;
}

FILE *__FileOpen(char *filename, char *mode, const char *file, const char *function, int line)
{
	FILE *fp;
 
	if(IS_NULLSTR(filename) || IS_NULLSTR(mode))
	{
		log_string("FileOpen called improperly.");
		return NULL;
	}
 
	// *Close the reserve* //
	if(fpReserve)
		fclose(fpReserve);
 
	fp = fopen(filename, mode);
 
	if(!fp)
	{
		perror(filename);
		return NULL;
	}
	else
	{
		// *If you want to be really picky, define this* //
		#ifdef DEBUG_FILEDATA
		FILE_DATA *file_data;
		for(file_data = file_list; file_data; file_data->next)
		{
			if(file_data->fp == fp)
			{
				log_string("FileOpen: Double opening of a file!");
			}
		}
		#endif
 
		FILE_DATA *filedata;
 
		CREATE(filedata, FILE_DATA, 1);
		filedata->filename = str_dup(filename);
		filedata->mode = str_dup(mode);
		filedata->file = str_dup(file);
		filedata->function = str_dup(function);
		filedata->line = line;
		filedata->fp = fp;
		LINK_SINGLE(filedata, next, file_list);
		FilesOpen++;
	}
 
	return fp;
}
 
// *Close the file-data* //
void FileClose(FILE *fp)
{
	if(fp)
	{
		FILE_DATA *filedata, *filedata_next;
		for(filedata = file_list; filedata; filedata = filedata_next)
		{
			filedata_next = filedata->next;
			if(filedata->fp == fp)
			{
				UNLINK_SINGLE(filedata, next, FILE_DATA, file_list);
				DISPOSE(filedata->filename);
				DISPOSE(filedata->file);
				DISPOSE(filedata->function);
				DISPOSE(filedata->mode);
				DISPOSE(filedata);
				break;
			}
		}
 
		fclose(fp);
		FilesOpen--;
 
		// ReOpen the Reserve :)
		fpReserve = fopen(NULL_FILE, "r");
 
		if(FilesOpen < 0)
		{
			FilesOpen = 0;
			log_string("FileClose passed a null fp somewhere and schewed the list.");
		}
	}
 
}
