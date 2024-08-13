#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <regex.h>
#include <locale.h>


#include <sys/stat.h>
#include <sys/resource.h>
#include "protocol.h"


#if defined(TRADITIONAL)
#define DECLARE_OBJ_FUN( fun )		void fun( )
#define DECLARE_ROOM_FUN( fun )		void fun( )
#define const
#define args( list )            ( )
#define DECLARE_DO_FUN( fun )        	void fun( )
#define DECLARE_SPEC_FUN( fun )        	bool fun( )
#define DECLARE_SPELL_FUN( fun )    	void fun( )
#else
#define args( list )           			list
#define DECLARE_DO_FUN( fun )           DO_FUN    fun
#define DECLARE_SPEC_FUN( fun )         SPEC_FUN  fun
#define DECLARE_SPELL_FUN( fun )    	SPELL_FUN fun
#define DECLARE_OBJ_FUN( fun )			OBJ_FUN	  fun
#define DECLARE_ROOM_FUN( fun )			ROOM_FUN  fun
#endif


#define CMD( fun )	void fun(ACCOUNT_DATA * account, char * argument)


/* vals from db.c */
extern bool fBootDb;

/* macro for flag swapping */
#define GET_UNSET(flag1,flag2)	(~(flag1)&((flag1)|(flag2)))

/* Magic number for memory allocation */
#define MAGIC_NUM 52571214


/* system calls */
int unlink();
int system();

/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
 
#define SUCCESS 1
#define FAILURE 0

#if    !defined(FALSE)
#define FALSE     0
#endif

#if    !defined(TRUE)
#define TRUE     1
#endif

#if    !defined(false)
#define false    0
#endif

#if    !defined(true)
#define true     1
#endif

#if    defined(_AIX)
    #if    !defined(const)
        #define const
    #endif
    
    typedef int                sh_int;
    
    #define unix
    #else
        typedef short   int            sh_int;

#endif



/*
 * String and memory management parameters.
 */
#define MAX_KEY_HASH          1024
#define PAGELEN                 22
#define MSL 9216
#define MIL 512

#define SEND	send_to_account
#define ANNOUNCE send_to_world
#define MXP 	mxp_to_char
#define WTB		write_to_buffer
#define PRINTF 	printf_to_char
#define STD		send_to_desc

#define LAST_FILE_SIZE			200



//Strategy game structs
typedef struct      world_data      WORLD_DATA;
typedef struct      empire_data     EMPIRE_DATA;
typedef struct      city_data       CITY_DATA;
typedef struct      unit_data       UNIT_DATA;
typedef struct      troop_data      TROOP_DATA;
typedef struct      building_data   BUILDING_DATA;
typedef struct      recruit_data    RECRUIT_DATA;
typedef struct      trade_data      TRADE_DATA;


typedef struct    	ban_data         BAN_DATA;
typedef struct    	buf_type         BUFFER;
typedef struct    	descriptor_data  DESCRIPTOR_DATA;
typedef struct    	help_data        HELP_DATA;
typedef struct    	note_data        NOTE_DATA;
typedef struct    	room_index_data  ROOM_INDEX_DATA;
typedef struct    	time_info_data   TIME_INFO_DATA;
typedef struct    	weather_data     WEATHER_DATA;
typedef struct    	colour_data      COLOUR_DATA;
typedef struct 		account_data 	 ACCOUNT_DATA; 
typedef struct      game_data		 	GAME_DATA;
typedef struct 		file_data 			FILE_DATA;



//Function types.
typedef void DO_FUN    args( ( ACCOUNT_DATA * account, char *argument ) );
typedef void ROOM_FUN	args( ( ROOM_INDEX_DATA *room, char *argument ) );
                
                
#define MAX_LINE_LENGTH 100

#define ACCOUNT_VERSION 2

#define ACCOUNT_DIR "../account/"

#define VERIFY		1           // whether or not the character has been verified 
#define ACC_PERMIT	2           // allows accounts to play even if site is banned 
#define ACC_DENY	4           // if the account account was denied 
#define ACC_IMP		8           // is there an Implementor alt here
#define ACC_IMM		16          // is there a Immortal alt here
#define	ACC_NEWBIE	32          // is set if their account is under 20 hours total
#define	ACC_CLANMASTER	64      // let's them have multiple clans in account.  cheat?
#define ACC_HOSTMASK    128
#define ACC_IDLER       256     // if this is set, it will penalize chars for idling
#define ACC_NOCOLOR	512         // Prevents the account from using colors
#define ACC_EXTENDED_VAULT	1024	//Allows all accounts on account to have 200 items per vault instead of 100
#define ACC_VISION_IMPAIRED 2048    //Turns off some of the decorative ascii for account menus


#define COMM_MXP            A   //turn on mxp for a account
#define COMM_CHAT           B   //is chat turned on for this account?
#define COMM_TIME_STAMPS    C

#define PEN_LOG         1   //logging a account

#define NO_FLAG         -1



//Game parameters.
#define MAX_NAME_LEN            14      //max chars allowed in account's name
#define MAX_HINT                25      //total hints in hint table
#define MAX_CHAT_LOG			15
#define MAX_ALIAS          		40
#define MAX_LEVEL          		50
#define MAX_VNUM        		999999      // Upper vnum limit, can go to 2,000,000,000 
#define MAX_NOTES_PER_LIST      100
#define MAX_SECTOR              33

#define MAIN_PORT				9000
#define TEST_PORT				9001
#define TIMER					50

//Map stuff
#define MAP_WIDTH           1000
#define MAP_HEIGHT          1000
#define MAP_SIZE            (MAP_WIDTH * MAP_HEIGHT)                    //Total 2d layout of world

//Troop sub types
#define TROOP_SETTLER        0
#define TROOP_FOOTMAN        1
#define TROOP_ARCHER         2

//Troop type
#define INFANTRY            0
#define CAVALRY             1


//Building defines
#define BLD_TOWNHALL        0   //standard main building
#define BLD_KEEP            1   //upgraded form of townhall, adds hp to town etc
#define BLD_CASTLE          2   //further upgraded form of townhall, adds much more hp, recruitment capacity, etc
#define BLD_MILL            3   //increase supply of wood for city
#define BLD_QUARRY          4   //increase supply of stone for city
#define BLD_MINE            5   //increase supply of iron for city
#define BLD_FARM            6
#define BLD_MARKET          7   //open up trading in city, increase supply of gold in city
#define BLD_BARRACKS        8   //recruit footmen
#define BLD_RANGE           9   //recruit archers
#define BLD_STORAGE         10  //storage building to hold more resources
#define BLD_HIDEOUT         11  //safely store a small amount of resources from plunder
#define BLD_WALL            12  


#define RES_WOOD            0
#define RES_STONE           1
#define RES_IRON            2
#define RES_FOOD            3
#define RES_GOLD            4

#define RESOURCE_STRING(type)	(type == RES_WOOD ? "wood" : \
                                 type == RES_STONE ? "stone" : \
                                 type == RES_IRON ? "iron" : \
                                 type == RES_FOOD ? "food" : \
                                 type == RES_GOLD ? "gold" : \
                                 "none")

#define RESOURCE_BUILDING(building)     (building_table[building->type].resource != -1)

//End strategy game parameters

/* Added this for "orphaned help" code. Check do_help() -- JR */
#define MAX_CMD_LEN			50

//Time definitions
#define PULSE_WEATHER_EFFECT		(15 * PULSE_PER_SECOND)   // 15 second delay
//Strategy game time defines
#define PULSE_SECOND                4
#define PULSE_EMPIRE                12      //every 3 seconds
#define PULSE_CITY                  48      //every 12 seconds
#define PULSE_ACCOUNT               120     //every 30 seconds
#define PULSE_TRADE                 4

//Real time:
#define TIME_ONE_SECOND				(1)
#define TIME_FIVE_SECONDS			(5)
#define TIME_TEN_SECONDS			(10)
#define TIME_FIFTEEN_SECONDS		(15)
#define TIME_THIRTY_SECONDS         (30)
#define TIME_ONE_MINUTE				(60)
#define TIME_ONE_HOUR				(60 * 60)	
//Game time:
#define TIME_TWICE_PER_DAY			((24 * 60) / 2)
#define TIME_THREE_PER_DAY			((24 * 60) / 3)
#define TIME_ONE_DAY				(24 * 60)
#define TIME_ONE_WEEK				(168 * 60)
#define TIME_ONE_MONTH				((4 * 168) * 60)

#define MINUTE 60
#define HOUR 3600
#define DAY (24*HOUR)
#define WEEK (7*DAY)
#define MONTH (30*DAY)
#define YEAR (365*DAY)


////////////////////////////Protocol Stuff/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////MXP (Mud Extension Protocol)////////////////////////////
                                                                            ///
#define USE_MXP(ch)		(IS_SET(account->comm_flags, COMM_MXP))              			///
                                                                            ///
/* strings */																///
                                                                            ///
#define MXP_BEG "\x03"    /* becomes < */									///
#define MXP_END "\x04"    /* becomes > */									///
#define MXP_AMP "\x05"    /* becomes & */									///
                                                                            ///
/* characters */															///
                                                                            ///
#define MXP_BEGc '\x03'    /* becomes < */									///
#define MXP_ENDc '\x04'    /* becomes > */									///
#define MXP_AMPc '\x05'    /* becomes & */									///
                                                                            ///
/* constructs an MXP tag with < and > around it */							///
                                                                            ///
#define MXPTAG(arg) MXP_BEG arg MXP_END										///
                                                                            ///
#define ESC "\x1B"  /* esc character */										///
                                                                            ///
#define MXPMODE(arg) ESC "[" #arg "z"										///
                                                                            ///
//////////////////////MSSP (Mud Server Status Protocol)////////////////////////
                                                                            ///
struct mssp_info															///
{																			///
   char * hostname;															///
   char * contact;															///
   char * icon;																///
   char * language;															///
   char * location;															///
   char * website;															///
   char * family;															///
   char * genre;															///
   char * gamePlay;															///
   char * gameSystem;														///
   char * intermud;															///
   char * status;															///
   char * subgenre;															///
   char * equipmentSystem;													///
   char * multiplaying;														///
   char * accountKilling;													///
   char * questSystem;														///
   char * roleplaying;														///
   char * trainingSystem;													///
   char * worldOriginality;													///
   sh_int created;															///
   sh_int minAge;															///
   sh_int worlds;															///
   bool ansi;																///
   bool mccp;																///
   bool mcp;																///
   bool msp;																///
   bool ssl;																///
   bool mxp;																///
   bool pueblo;																///
   bool vt100;																///
   bool xterm256;															///
   bool pay2play;															///
   bool pay4perks;															///
   bool hiringBuilders;														///
   bool hiringCoders;														///
   bool adultMaterial;														///
   bool multiclassing;														///
   bool newbieFriendly;														///
   bool accountCities;														///
   bool accountClans;														///
   bool accountCrafting;														///
   bool accountGuilds;														///
};    																		///
                                                                            ///
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*
 * Utility macros.
 */

#define LOG(str)			(log_string(str))
#define IS_VALID(data)        ((data) != NULL && (data)->valid)
#define VALIDATE(data)        ((data)->valid = TRUE)
#define INVALIDATE(data)    ((data)->valid = FALSE)
#define UMIN(a, b)        ((a) < (b) ? (a) : (b))
#define UMAX(a, b)        ((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)        ((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)        ((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)        ((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)    ((flag) & (bit))
#define SET_BIT(var, bit)    ((var) |= (bit))
#define REMOVE_BIT(var, bit)    ((var) &= ~(bit))
#define TOGGLE_BIT(var, bit)    ((var) ^= (bit))
#define PAUSE    	system("PAUSE")
#define IS_NULLSTR(str)        ((str) == NULL || (str)[0] == '\0' || !str_cmp(str, "(null)"))
#define ENTRE(min,num,max)    ( ((min) < (num)) && ((num) < (max)) )
#define CHECK_POS(a, b, c)    {                            \
                    (a) = (b);                    \
                    if ( (a) < 0 )                    \
                    bug( "CHECK_POS : " c " == %d < 0", a );    \
                }                            

#define IS_VOWEL(c) (   (c) == 'a' || (c) == 'A'  \
                        || (c) == 'e' || (c) == 'E'  \
                        || (c) == 'i' || (c) == 'I'  \
                        || (c) == 'o' || (c) == 'O'  \
                        || (c) == 'u' || (c) == 'U')

#define IS_NIGHT(hour)        (hour >= 18 || hour < 6)

#define CON_GET_NEW_NAME				0
#define CON_CONFIRM_NEW_PASSWORD		1
#define CON_GET_PASS					2
#define CON_GET_EMAIL					3
#define CON_ACC_VERIFY					4
#define CON_CONFIRM_NEW_ACC_NAME		5
#define CON_CHNG_PASS					6
#define CON_DEL_ACC						7
#define CON_PLAY_CHAR					8
#define CON_CONFIRM_DEL_ACC				9
#define CON_SEND_PASS					10
#define CON_EXTRACTED					11
#define CON_INTRO						12
#define CON_GET_OLD_ACCOUNT				13
#define CON_ACCOUNT						14
#define CON_GET_NAME				    15
#define CON_GET_OLD_PASSWORD			16
#define CON_CONFIRM_NEW_NAME			17
#define CON_PLAYING					    18
#define CON_READ_IMOTD					19
#define CON_READ_MOTD					20
#define CON_BREAK_CONNECT				21
#define CON_VERIFY_EMAIL				22
#define CON_GET_NEW_EMPIRE_NAME         23
#define CON_CONFIRM_EMPIRE_NAME         24
#define CON_ENTER_GAME                  25

#define GET_CONNECTED_STRING(d)	(	d->connected == 0 ? "get new account name" : \
                                    d->connected == 1 ? "confirm new password" : \
                                    d->connected == 2 ? "get password" : \
                                    d->connected == 3 ? "get email" : \
                                    d->connected == 4 ? "verify account" : \
                                    d->connected == 5 ? "confirm account name" : \
                                    d->connected == 6 ? "change password" : \
                                    d->connected == 7 ? "delete account" : \
                                    d->connected == 8 ? "play/pick char" : \
                                    d->connected == 9 ? "confirm delete account" : \
                                    d->connected == 10 ? "send password" : \
                                    d->connected == 11 ? "extracted" : \
                                    d->connected == 12 ? "intro" : \
                                    d->connected == 13 ? "get old account" : \
                                    d->connected == 14 ? "account screen" : \
                                    d->connected == 15 ? "get name" : \
                                    d->connected == 16 ? "get old password" : \
                                    d->connected == 17 ? "confirm new name" : \
                                    d->connected == 18 ? "playing" : \
                                    d->connected == 19 ? "read imotd" : \
                                    d->connected == 20 ? "read motd" : \
                                    d->connected == 21 ? "break connect" : \
                                    d->connected == 22 ? "verify email" : \
                                    d->connected == 23 ? "get new empire name" : \
                                    d->connected == 24 ? "confirm empire name" : \
                                    d->connected == 25 ? "entering game" : \
                                    "bad state")								 



// Weather Macros
#define PRECIPITATING(area)			(area->precip > -1)
#define RAINING(area)				(area->precip == PRECIP_RAIN)
#define SNOWING(area)				(area->precip == PRECIP_SNOW)
#define HAILING(area)				(area->precip == PRECIP_HAIL)
#define SLEETING(area)				(area->precip == PRECIP_SLEET)

#define MIN_TEMP(area)              (climate_table[area->climate].min_temp)
#define MAX_TEMP(area)              (climate_table[area->climate].max_temp)

#define MIN_HUM(area)              (climate_table[area->climate].min_humidity)
#define MAX_HUM(area)              (climate_table[area->climate].max_humidity)

#define PRECIP_NONE         A
#define PRECIP_RAIN         B
#define PRECIP_SLEET        C
#define PRECIP_SNOW         D
#define PRECIP_HAIL         E
#define PRECIP_ACID_RAIN    F
#define PRECIP_ASH_RAIN     G

#define TROPICAL            0
#define DRY                 1
#define TEMPERATE           2
#define CONTINENTAL         3
#define POLAR               4

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )                \
                if ( !str_cmp( word, literal ) )    \
                {                                   \
                    field  = value;                 \
                    fMatch = TRUE;                  \
                    break;                          \
                                }
#if defined(SKEY)
#undef SKEY
#endif

#define SKEY( string, field )                       \
                if ( !str_cmp( word, string ) )     \
                {                                   \
                    free_string( field );           \
                    field = fread_string( fp );     \
                    fMatch = TRUE;                  \
                    break;                          \
                                }

#if defined(KEYS)
#undef KEYS
#endif

#define KEYS( literal, field, value )                    \
    if ( !str_cmp( word, literal ) )    \
    {                    \
        free_string(field);            \
        field  = value;            \
        fMatch = TRUE;            \
        break;                \
    }


#define CREATE(result, type, number)                                    \
do                                                                      \
{                                                                       \
   if (!((result) = (type *) calloc ((number), sizeof(type))))          \
   {                                                                    \
      perror("malloc failure");                                         \
      fprintf(stderr, "Malloc failure @ %s:%d\n", __FILE__, __LINE__ ); \
      abort();                                                          \
   }                                                                    \
} while(0)

#define LINK(link, first, last, next, prev) \
do                                          \
{                                           \
   if ( !(first) )                          \
   {                                        \
      (first) = (link);                     \
      (last) = (link);                      \
   }                                        \
   else                                     \
      (last)->next = (link);                \
   (link)->next = NULL;                     \
   if ((first) == (link))                   \
      (link)->prev = NULL;                  \
   else                                     \
      (link)->prev = (last);                \
   (last) = (link);                         \
} while(0)

#define INSERT(link, insert, first, next, prev) \
do                                              \
{                                               \
   (link)->prev = (insert)->prev;               \
   if ( !(insert)->prev )                       \
      (first) = (link);                         \
   else                                         \
      (insert)->prev->next = (link);            \
   (insert)->prev = (link);                     \
   (link)->next = (insert);                     \
} while(0)

#define UNLINK(link, first, last, next, prev)   \
do                                              \
{                                               \
   if ( !(link)->prev )                         \
   {                                            \
      (first) = (link)->next;                   \
      if ((first))                              \
         (first)->prev = NULL;                  \
   }                                            \
   else                                         \
   {                                            \
      (link)->prev->next = (link)->next;        \
   }                                            \
   if ( !(link)->next )                         \
   {                                            \
      (last) = (link)->prev;                    \
      if((last))                                \
         (last)->next = NULL;                   \
   }                                            \
   else                                         \
   {                                            \
      (link)->next->prev = (link)->prev;        \
   }                                            \
} while(0)								

#define UNLINK_SINGLE(pdata,pnext,type,list) \
do                                          \
{                                          \
    if (list == pdata)                      \
    {                                       \
        list = pdata->pnext;                 \
    }                                       \
    else                                    \
    {                                       \
        type *prev;                         \
        for (prev = list; prev != NULL; prev = prev->pnext) \
        {                                   \
            if (prev->pnext == pdata)        \
            {                               \
                prev->pnext = pdata->pnext;   \
                break;                      \
            }                               \
        }                                   \
        if (prev == NULL)                   \
        {                                   \
            bugf (#pdata " not found in " #list "."); \
        }                                   \
    }                                       \
} while(0)
 
#define LINK_SINGLE(pdata,pnext,list) \
do \
{ \
        pdata->pnext = list; \
        list = pdata; \
} \
while (0)

#define DISPOSE(point)                      \
do                                          \
{                                           \
   if( (point) )                            \
   {                                        \
      free( (point) );                      \
      (point) = NULL;                       \
   }                                        \
} while(0)

#define STRFREE(point)		DISPOSE((point))							



//OLC

#define ALT_FLAGVALUE_SET( _blargh, _table, _arg )        \
    {                            \
        int blah = flag_value( _table, _arg );        \
        _blargh = (blah == NO_FLAG) ? 0 : blah;        \
    }

#define ALT_FLAGVALUE_TOGGLE( _blargh, _table, _arg )        \
    {                            \
        int blah = flag_value( _table, _arg );        \
        _blargh ^= (blah == NO_FLAG) ? 0 : blah;    \
    }

/*
 * ColoUr stuff v2.0, by Lope.
 */
 
//Updated 2020 by Upro.
#define CLEAR       "\e[0m"        	// Resets Colour   
#define C_RED       "\e[0;31m"    	// Normal Colours    
#define C_GREEN     "\e[0;32m"
#define C_YELLOW    "\e[0;33m"
#define C_BLUE      "\e[0;34m"
#define C_MAGENTA   "\e[0;35m"
#define C_CYAN      "\e[0;36m"
#define C_WHITE     "\e[0;37m"
#define C_D_GREY    "\e[1;30m"      // Light Colors       
#define C_B_RED     "\e[1;31m"
#define C_B_GREEN   "\e[1;32m"
#define C_B_YELLOW  "\e[1;33m"
#define C_B_BLUE    "\e[1;34m"
#define C_B_MAGENTA "\e[1;35m"
#define C_B_CYAN    "\e[1;36m"
#define C_B_WHITE   "\e[1;37m"
#define BG_BLACK	"\e[0;40m"
#define BG_RED		"\e[0;41m"
#define BG_GREEN  	"\e[0;42m"
#define BG_YELLOW	"\e[0;43m"
#define BG_BLUE		"\e[0;44m"   
#define BG_MAGENTA	"\e[0;45m"
#define BG_CYAN		"\e[0;46m"		
#define BG_WHITE	"\e[0;47m"
#define FRAMED      "\e[0;51m"
#define C_BROWN		"\e[38;5;94m"
#define C_GOLD		"\e[38;5;220m"
#define C_LIGHT_ORANGE "\e[38;5;208m"
#define C_DARK_ORANGE "\e[38;5;202m"
#define C_CERULEAN	"\e[38;5;26m"
#define C_AQUAMARINE "\e[38;5;33m"
#define C_PURPLE	"\e[38;5;93m"
#define C_PINK		"\e[38;5;207m"
#define C_DARKER_GREY "\e[38;5;239m"
#define C_LIGHT_GREEN "\e[38;5;76m"
#define C_FLESHTONE "\e[38;5;223m"
#define C_TEAL "\e[38;5;120m"
#define C_SEPIA "\e[38;5;166m"
#define C_FUSCHIA "\e[38;5;197m"
#define C_BRICK_RED "\e[38;5;124m"   //brick red

#define COLOR_PREF(ch)          (color_pref(ch))


#define COLOUR_NONE 7        // White, hmm...    
#define RED         1        // Normal Colours   
#define GREEN       2
#define YELLOW      3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define WHITE       7
#define BLACK       0

#define NORMAL      0        // Bright/Normal colours
#define BRIGHT      1





/*
 * Site ban structure.
 */

#define BAN_SUFFIX     A
#define BAN_PREFIX     B
#define BAN_NEWBIES    C
#define BAN_ALL        D    
#define BAN_PERMIT     E
#define BAN_PERMANENT  F

struct    ban_data
{
    BAN_DATA *  next;
    bool        valid;
    sh_int      ban_flags;
    sh_int      level;
    char *      name;
};

struct game_data
{
    int 	most_accounts;
    
    long 	mkills;
    char 	* mkiller;
    
    int		pkills;
    char 	* pkiller;
    
    int		pdeaths;
    char	* pdeath;

    int     arena_wins;
    char *  arena_winner;

    int     arena_losses;
    char *  arena_loser;

    long	explored;
    char	* explorer;
    
    int		most_hours;
    char	* most_hour;
    
    long	qp;
    char	* most_qp;
    
    long	ever_created;		//how many have ever created.

    
    //Crafting stuff
    int     smith_count;
    char *  blacksmith;

    int     tailor_count;
    char *  tailor;

    int     wood_count;
    char *  woodworker;

    int     jewel_count;   
    char *  jewelcrafter;

    int     alchemy_count;
    char *  alchemist;

    int     meal_count;
    char *  chef;

    int     brew_count;
    char *  brewer;

    int     scroll_count;
    char *  scribe;

    int     crafted_count;
    char *  craftsman;
};

struct account_data
{
    DESCRIPTOR_DATA *   d;
    char *              name;
    char *              empire_name;
    char *              screen_name;
    char *              password;    
    char *              email;
    long                act;
    int		        	verify;
    int		       	 	numb;
    long	        	hours;
    long	        	secret;
    bool				valid;   
    int 				version;
    long                pen_pts;
    char *              pen_info;    
    char *              old_passwd;    
    char *              real_name;
    time_t              logon;


    ACCOUNT_DATA *   next;
    CITY_DATA *     cur_city;           //which city are you currently working in
    int             city_id;    
    //current x/y center on map
    long            x;
    long            y;

    long            comm_flags;
    long            settings_flags;
    long            combat_flags;
    long            trade_flags;
    long            alliance_flags;
    long            penalty_flags;
    long            wiznet;

    char *			alias[MAX_ALIAS];

    int             lines;              //how many lines of text can they recieve at once?
    char *          pString;
    char *          prefix;
};

struct file_data
{
    FILE_DATA *next;
    FILE *fp;
    char *filename;
    char *mode;
 
    // *Where they were called from* //
    char *file;
    char *function;
    int line;
};

struct buf_type
{
    BUFFER *    next;
    bool        valid;
    sh_int      state;  /* error state of the buffer */
    sh_int      size;   /* size in k */
    char *      string; /* buffer's string */
};

#define MAX_MAP 77 // Shows 1/2 the given rooms (links) Always odd
#define MAP_MID 38 // Always 2x+1 to get MAX_MAP (makes the middle
#define MAX_MAP_HASH 16

// -1 is already taken, all positives are taken, so start lower
#define NS_2WAY		-2
#define NS_1WAYN	-3
#define NS_1WAYS	-4
#define NS_HIT		-5
#define EW_2WAY		-6
#define EW_1WAYE	-7
#define EW_1WAYW	-8
#define EW_HIT		-9
#define NS_2WAYD	-10
#define NS_1WAYND	-11
#define NS_1WAYSD	-12
#define NS_HITD		-13
#define EW_2WAYD	-14
#define EW_1WAYED	-15
#define EW_1WAYWD	-16
#define EW_HITD		-17
#define NS_UNN		-18
#define NS_UNS		-19
#define EW_UNE		-20
#define EW_UNW		-21
#define NS_UNND		-22
#define NS_UNSD		-23
#define EW_UNED		-24
#define EW_UNWD		-25

// Structures that are used.
typedef struct	map_qd		MAP_QD;
typedef struct	map_visit	MAP_VISIT;
typedef struct	map_param	MAP_PARAM;

// External information, used by many things.

extern MAP_QD *map_qd_free;
extern MAP_QD *map_queue;
extern MAP_VISIT *map_visit[MAX_MAP_HASH];
extern MAP_VISIT *map_visit_free;
extern int top_map_visit;
extern int top_map_qd;
//extern int map[MAX_MAP][MAX_MAP];
extern MAP_PARAM *mp;


//Game info stuff:
extern bool newlock;
extern bool wizlock;


//Time and weather stuff, all gathered/modified 3/23/21 Upro
 
struct    time_info_data
{
    int        hour;
    int        day;
    int        month;
    int        year;
};

struct    weather_data
{
    int        	mmhg;
    int        	change;    
    int        	sunlight;	
    int			moon_phase;
    bool		tide;
    int         season;
};

struct      seasonal_desc_data
{
    int         sector;
    char *      desc[4];
};

#define SUN_DARK       0
#define SUN_RISE       1
#define SUN_LIGHT      2
#define SUN_SET        3

#define SKY_CLOUDLESS		    	0
#define SKY_CLOUDY		    		1
#define SKY_RAINING		    		2
#define SKY_LIGHTNING		   		3
#define SKY_SNOWING                 4
#define SKY_BLIZZARD                5
#define SKY_FOGGY                   6
#define SKY_HAILSTORM               7
#define SKY_THUNDERSTORM            8
#define SKY_ICESTORM                9

#define CHECK_DATE(d, m)		( ( ( time_info.day + 1 ) == d ) && ( time_info.month == m ) )						


#define SEASON_SPRING			(time_info.month > 4 && time_info.month < 9)
#define SEASON_SUMMER			(time_info.month > 8 && time_info.month < 12)
#define SEASON_AUTUMN			(time_info.month > 11 && time_info.month < 17)
#define SEASON_FALL				(time_info.month > 11 && time_info.month < 17) //Just for my laziness.
#define SEASON_WINTER			(time_info.month > -1 && time_info.month < 5)

#define SEASON                  (get_season())

#define MOON_NEW_MOON          0
#define MOON_WAXING_CRESCENT   1
#define MOON_FIRST_QUARTER     2
#define MOON_WAXING_GIBBOUS    3
#define MOON_FULL_MOON         4
#define MOON_WANING_GIBBOUS    5
#define MOON_LAST_QUARTER      6
#define MOON_WANING_CRESCENT   7


//Descriptor (data stream to account) structure.
struct    descriptor_data
{		
    DESCRIPTOR_DATA *	next;
    DESCRIPTOR_DATA *	snoop_by;
    time_t		snoop_time;
    ACCOUNT_DATA *	account;
    bool		valid;
    char *      host;
    char *      hostmask;
    char * 		ip_string;

    sh_int		descriptor;
    sh_int		connected;
    bool		fcommand;
    char		inbuf		[4 * MIL];
    char		incomm		[MIL];
    char		inlast		[MIL];
    int			repeat;
    char *		outbuf;
    int			outsize;
    int			outtop;
    char *		showstr_head;
    char *		showstr_point;    
    bool        mxp;
    void *              pEdit;		/* OLC */
    char **             pString;	/* OLC */
    int			editor;		        /* OLC */
    sh_int		strip;
    sh_int		color;
    bool		pload;
    protocol_t *        pProtocol;
};


//Help file structure
struct    help_data
{
    char * 			name;
    HELP_DATA *  	next;
    HELP_DATA *  	next_area;
    sh_int       	level;
    char *       	keyword;
    int				id;				//let's keep track of numbers too maybe we can use this for SQL later
    int				version;		//Maybe revision history later?
    char *       	text;
    bool 		 	to_delete;    	
    char * 		 	last_editted_by;		//who last editted this helpfile?
    char *		 	edit_date;
    long            editted;    //when was is last editted.
};

struct wiznet_type
{
    char *   name;
    long     flag;
};



#define NOTE_NOTE		0
#define NOTE_IDEA		1
#define NOTE_HISTORY	2
#define NOTE_NEWS		3
#define NOTE_CHANGES	4
#define NOTE_PENALTY	5
#define NOTE_GOSSIP    	6
#define NOTE_BUG		7
#define NOTE_TODO		8


//Notes data structure
struct	note_data
{
    NOTE_DATA *	next;
    bool 	valid;
    bool        persist;
    sh_int	type;
    char *	sender;
    char *	date;
    char *	to_list;
    char *	subject;
    char *	text;
    time_t  	date_stamp;
};


//Structure to store info in building_table (tables.c)
struct building_info
{
    char *          name;
    int             type;
    int             resource;
    int             wood_cost[10];
    int             stone_cost[10];
    int             iron_cost[10];
    int             food_cost[10];
    int             required_building;      //what building if any is required in city before you can construct this one.
};

//Structure to store individual building data for cities
struct building_data
{
    BUILDING_DATA * next;
    char *          city;
    int             city_id;    
    int             type;
    int             level;

    bool            valid;
};


//Troop information stored in troop_table (tables.c)
struct troop_info
{
    char *          name;
    int             type;       //infantry, cavalry, etc
    int             sub_type;   //footman, archer
    int             hp;
    int             speed;
    int             wood_cost;
    int             stone_cost;
    int             iron_cost;
    int             upkeep;     //how much food every hour do they need
    int             req_building;   //what building do they need to be able to recruit these
    int             recruit_time;
};

//Stores individual troop information for cities.
struct troop_data
{
    TROOP_DATA *    next;
    char *          name;
    int             type;    
    bool            valid;
    char *          city;
    char *          empire_name;
    int             city_id;
};

//Structure for trade information
struct trade_data
{
    TRADE_DATA *    next;
    int             resource;   //wood/stone/iron/food
    int             amount;
    long            time_left;
    int             dest_x;
    int             dest_y;
    bool            valid;
    int             to_city_id;
    int             from_city_id;
};

//Structure to store all the world data for the server.
struct world_data
{
    ROOM_INDEX_DATA * tiles[MAP_WIDTH][MAP_HEIGHT];
    EMPIRE_DATA * empires;


    int days;       //how many days has this world been open
};

//Structure to store empire data
struct empire_data
{    
    int             id;
    char *          owner;      //name of account that owns this empire
    EMPIRE_DATA *   next;       //linked list tracker
    char *          name;    

    bool            valid;      //sanity checker
};

struct empire_account_data
{
    
    
};

struct recruit_data
{
    RECRUIT_DATA *  next;
    int             type;
    int             amount;
    long            time_left;
    bool            valid;
};

//Structure to store city information
struct city_data
{
    char *          name;
    char *          empire_name;

    CITY_DATA *     next;       //linked list tracker
    TROOP_DATA *    troops;
    RECRUIT_DATA *  recruiting;
    TRADE_DATA *    trading;
    long            room_vnum;  //what tile/room index is this city stationed in.

    long            wood;
    long            stone;
    long            iron;
    long            food;
    long            gold;

    long            x;          //x coords on map
    long            y;          //y coords on map

    int             id;
    bool            valid;      //sanity checker
};


/* RT ASCII conversions -- used so we can have letters in this file */

#define A            1
#define B            2
#define C            4
#define D            8
#define E           16
#define F           32
#define G           64
#define H          128

#define I          256
#define J          512
#define K         1024
#define L         2048
#define M         4096
#define N         8192
#define O        16384
#define P        32768

#define Q        65536
#define R       131072
#define S       262144
#define T       524288
#define U      1048576
#define V      2097152
#define W      4194304
#define X      8388608

#define Y     16777216
#define Z     33554432
#define aa    67108864
#define bb   134217728
#define cc   268435456    
#define dd   536870912
#define ee  1073741824
#define ff	2147483648
#define gg  4294967296
#define hh  8589934592
#define ii  17179869184
#define jj  34359738368
#define kk  68719476736
#define ll  137438853472
#define mm  274877906944
#define nn  549755813888


//Room flags
#define ROOM_DARK        		(A)
#define ROOM_ANTI_WIZARD		(B)
#define ROOM_NO_MOB      		(C)
#define ROOM_INDOORS        	(D)
#define ROOM_DEVOTION			(E)
#define ROOM_MOUNT_SHOP			(F)
#define ROOM_NO_SCRY            (G)
#define ROOM_PLAYER_OWNED       (H)
#define ROOM_CLAIMABLE          (I)
#define ROOM_PRIVATE        	(J)
#define ROOM_SAFE        		(K)
#define ROOM_SOLITARY        	(L)
#define ROOM_PET_SHOP        	(M)
#define ROOM_NO_RECALL        	(N)
#define ROOM_NO_TREE        	(O)
#define ROOM_GODS_ONLY        	(P)
#define ROOM_HEROES_ONLY    	(Q)
#define ROOM_NEWBIES_ONLY    	(R)
#define ROOM_LAW        		(S)
#define ROOM_NOWHERE        	(T)
#define ROOM_NOFIGHT			(U)
#define ROOM_WORSHIPPER			(X)
#define ROOM_STABLE				(Y)
#define ROOM_NO_GATE            (Z)
#define ROOM_CELL				(aa)
#define ROOM_SAVEOBJS			(bb)
#define ROOM_CLIMBABLE			(cc)
#define ROOM_ANTI_MAGIC			(dd)
#define ROOM_NOSUMMON			(ee)
#define ROOM_NO_TELEPORT        (ff)

//Directions. 
#define DIR_NORTH  0
#define DIR_EAST   1
#define DIR_SOUTH  2
#define DIR_WEST   3

// Sector Flags
#define SECT_MINE		(A) //can mine here.
#define SECT_DIG		(B) //can dig here.
#define SECT_PLANT		(C)	//plants can grow here
#define SECT_OUTSIDE	(D) //can you see the weather/time change?
#define SECT_PROSPECT	(E) //can you prospect here?
#define SECT_FISH		(F) //can you fish here?
#define SECT_FRESH_WATER (G) //is this fresh water? (ie river/lake)
#define SECT_SALT_WATER (H) //is this salt water? (ie sea/ocean)
#define SECT_TRACKS (I) //can people leave tracks here

// Sector types. Used in room_index_data
#define SECT_INSIDE         	0
#define SECT_CITY           	1
#define SECT_FIELD          	2
#define SECT_FOREST         	3
#define SECT_HILLS          	4
#define SECT_MOUNTAIN       	5
#define SECT_LAKE		     	6
#define SECT_OCEAN			  	7
#define SECT_UNUSED         	8
#define SECT_AIR            	9
#define SECT_DESERT        		10
#define SECT_ROAD				11
#define SECT_BEACH              12
#define SECT_SHORELINE          13
#define SECT_CAVE               14
#define SECT_OCEANFLOOR         15
#define SECT_LAVA               16
#define SECT_SWAMP              17
#define SECT_UNDERGROUND        18
#define SECT_ICE                19
#define SECT_SNOW               20
#define SECT_VOID               21
#define SECT_RUINS              22
#define SECT_WASTELAND          23
#define SECT_JUNGLE             24
#define SECT_DOCK		      	25
#define SECT_SHIP				26
#define SECT_RIVER				27
#define SECT_BLOOD_RIVER		28
#define SECT_SUNKEN_CAVE		29
#define SECT_FORTRESS			30
#define SECT_MUSH_FOREST		31

//Sector Macros
#define CLAIMABLE(room)             (IS_SET(room->room_flags, ROOM_CLAIMABLE))
#define CLAIMED(room)               (room->owner != NULL)
#define IN_WATER(ch)				((sector_table[(ch)->in_room->sector_type].fresh_water == TRUE) || (sector_table[(ch)->in_room->sector_type].salt_water == TRUE))
#define FRESH_WATER(room)           (sector_table[room->sector_type].fresh_water == TRUE)
#define SALT_WATER(room)            (sector_table[room->sector_type].salt_water == TRUE)
#define CAN_TREES_GROW(room)        (sector_table[room->sector_type].plant == TRUE)

#define IN_ARENA(room)              (IS_SET(room->area->area_flags, AREA_ARENA))

#define BLOOD_SECTOR(room)          (room->sector_type == SECT_BLOOD_RIVER)
#define WATER_SECTOR(room)          ((sector_table[room->sector_type].fresh_water == TRUE) || (sector_table[room->sector_type].salt_water == TRUE))
#define CAN_DIG(ch)					(sector_table[(ch)->in_room->sector_type].dig == TRUE)
#define LEAVE_TRACKS(room)          (sector_table[(ch)->in_room->sector_type].tracks == TRUE)

#define IS_COOKING_VESSEL(obj)      (obj->item_type == ITEM_STEW_POT || obj->item_type == ITEM_SKILLET || obj->item_type == ITEM_SAUCE_PAN || obj->item_type == ITEM_BAKING_PAN || obj->item_type == ITEM_GRIDDLE)


#define IN_MINING_SECTOR(ch)		(sector_table[(ch)->in_room->sector_type].mine == TRUE)
#define IN_PROSPECTING_SECTOR(ch)	(sector_table[(ch)->in_room->sector_type].prospect == TRUE)
#define IN_PLANTING_SECTOR(ch)        (sector_table[(ch)->in_room->sector_type].plant == TRUE)

#define IS_OUTSIDE(ch)        		(sector_table[(ch)->in_room->sector_type].outside == TRUE)

#define IN_PLANE(ch)                (ch->in_room->area->plane)
#define IN_SECTOR(ch, sector)       (ch->in_room->sector_type == sector)

#define GET_SECTOR(room)            (get_sector(room))

#define HAS_NUTS(tree)              (tree->item_type == ITEM_TREE && (get_material(tree->material) == MAT_PINE || get_material(tree->material) == MAT_PECAN ||  get_material(tree->material) == MAT_WALNUT))

#define HAS_FRUIT(tree)             (tree->item_type == ITEM_TREE && (get_material(tree->material) == MAT_APPLE || get_material(tree->material) == MAT_MULBERRY || get_material(tree->material) == MAT_CITRUS || get_material(tree->material) == MAT_CHERRY))



/* WIZnet flags */
#define WIZ_ON                  (A)
#define WIZ_TICKS               (B)
#define WIZ_LOGINS              (C)
#define WIZ_SITES               (D)
#define WIZ_LINKS               (E)
#define WIZ_DEATHS              (F)
#define WIZ_RESETS              (G)
#define WIZ_MOBDEATHS           (H)
#define WIZ_FLAGS               (I)
#define WIZ_PENALTIES           (J)
#define WIZ_SACCING             (K)
#define WIZ_LEVELS              (L)
#define WIZ_SECURE              (M)
#define WIZ_SWITCHES            (N)
#define WIZ_SNOOPS              (O)
#define WIZ_RESTORE             (P)
#define WIZ_LOAD                (Q)
#define WIZ_NEWBIE              (R)
#define WIZ_PREFIX              (S)
#define WIZ_SPAM                (T)
#define WIZ_MEMORY				(U)
#define WIZ_BUGS				(V)



//Data structure for a single "room"
struct    room_index_data
{
    ROOM_INDEX_DATA *    next;        
    char *        				name;
    char *        	description;
    char *        	owner;
    int             god;
    long        	vnum;
    int            	room_flags;
    long            sector_flags;
    sh_int        	sector_type;	
    sh_int			temp_sector;
    sh_int        	heal_rate;
    sh_int         	mana_rate;
    sh_int        	clan;        
    int				x;
    int 			y;
};


//Utility stuff:
extern int FilesOpen;
extern FILE_DATA *file_list;



//Global constants. 
extern    const    struct    wiznet_type     wiznet_table   [];
extern const struct building_info building_table[];
extern const struct troop_info troop_table[];

struct flag_type
{
    char *name;
    long bit;
    bool settable;
};

struct sector_desc_info
{
    //Dynamic room description generation
    char *      seasons[4];
    char *      time_desc[6];
};

struct sector_info
{
    char * 	name;	
    char *	non_wall;				//map color/symbol (no wall present)
    char *  wall;					//map color/symbol (wall present)
    int 	type;
    int		move_cost;
    //bool	settable;
    int		heal_rate;
    int		mana_rate;	


    //flags
    bool	mine;
    bool	dig;
    bool	plant;
    bool	outside;
    bool	prospect;
    bool	fish;
    bool	fresh_water;
    bool	salt_water;	
    bool	tracks;  //can people leave tracks passing through here? 
    
};

struct	bit_type
{
    const	struct	flag_type *	table;
    char *				help;
};


//Lookup Info
int	        flag_lookup	    args( (const char *, const struct flag_type *) );


// game tables
extern  const   struct  sector_info 	sector_table[];
extern  const   struct  sector_desc_info sector_description_table[];

// flag tables 
extern	const	struct	flag_type	room_flags[];
extern	const	struct	flag_type	sector_flags[];
extern	const	struct	bit_type	bitvector_type[];


//Global strategy game variables
extern EMPIRE_DATA * empire_list;
extern CITY_DATA * city_list;
extern BUILDING_DATA * building_list;
extern TROOP_DATA * troop_list;
extern ACCOUNT_DATA * account_list;

extern EMPIRE_DATA * empire_last;
extern CITY_DATA * city_last;
extern BUILDING_DATA * building_last;

extern EMPIRE_DATA * empire_first;
extern CITY_DATA * city_first;
extern BUILDING_DATA * building_first;

extern int empire_count;
extern int city_count;


//Global variables.
extern HELP_DATA		*	help_first;
extern DESCRIPTOR_DATA	*	descriptor_list;
extern DESCRIPTOR_DATA *d_next;

extern  char				bug_buf[];
extern  time_t				current_time;
extern  bool				fLogAll;
extern  FILE			*	fpReserve;
extern  char				log_buf[];
extern  TIME_INFO_DATA		time_info;
extern  WEATHER_DATA		weather_info;
extern  char                    last_command[MSL];
extern  bool				MOBtrigger;
extern  sh_int                  display;



#if    defined(linux)
char *    crypt        args( ( const char *key, const char *salt ) );
#endif



/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if    defined(NOCRYPT)
#define crypt(s1, s2)    (s1)
#endif

/*
 * Socket and TCP/IP stuff.
 */


#include "telnet.h"

#if    defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif


/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so accounts can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */

#define HELP_DIR		"../help/"
#define NOTE_FILE       "../notes/notes.not"             /* For 'notes'*/
#define IDEA_FILE		"../notes/ideas.not"
#define HISTORY_FILE	"../notes/history.not"
#define NEWS_FILE		"../notes/news.not"
#define CHANGES_FILE	"../notes/chang.not"
#define PENALTY_FILE    "../notes/penalty.not"
#define BUG_FILE		"../notes/bug.not"
#define TODO_FILE		"../notes/todo.not"
#define BUG_LIST        "../log/bugs.list"
#define LASTON_LIST     "../log/laston.list"
#define TEMP_FILE       "../account/temp"

#define NULL_FILE       "/dev/null"          /* To reserve one stream  */
#define GAME_STATS_FILE	"game_stats.txt"


#define SHUTDOWN_FILE   "shutdown.txt"        /* For 'shutdown'        */
#define BAN_FILE        "ban.txt"
#define OHELPS_FILE	    "../dev/orphaned_helps.txt"  /* Unmet 'help' requests */
#define LAST_COMMAND    "../log/last_command.txt"  /*For the signal handler.*/
#define MAIN_DIR        "../"
#define TEST_DIR        "../../TP/"


//Strategy game files/directories
#define ACCOUNT_DIR		"../account/"	//holds all account accounts
#define EMPIRE_DIR      "../empire/"    //hold each individual empire data
#define LOG_DIR         "../log/"       //stores any relevant log files



#define FCLOSE(file)        fclose(file); \
                            file = NULL;


FILE *__FileOpen(char *filename, char *mode, const char *file, const char *function, int line);
 
void FileClose(FILE *fp);
 
#define FileOpen(filename, mode)  __FileOpen(file, mode, __FILE__, __FUNCTION__, __LINE__)


///////////////////////////////////////////////////////////////////////////
//////////////////// GLOBAL FUNCTION PROTOTYPES ///////////////////////////
///////////////////////////////////////////////////////////////////////////

//account.c
ACCOUNT_DATA * load_temp_account( char * name );
void acc_info(ACCOUNT_DATA * account, DESCRIPTOR_DATA *d);
void account_main_menu(DESCRIPTOR_DATA *d);
void account_email_msg(DESCRIPTOR_DATA *d);
void create_account_menu(DESCRIPTOR_DATA *d);
void account_help(DESCRIPTOR_DATA *d);
void intro_menu(DESCRIPTOR_DATA *d);
ACCOUNT_DATA * new_account(void);
void free_account(ACCOUNT_DATA * acc);

int ACC_IS_SET(int bits, int check);
void ACC_SET(ACCOUNT_DATA *, int check);
int create_key( void );
void send_mail(DESCRIPTOR_DATA *d);

bool load_account_obj 	args(( DESCRIPTOR_DATA *d, char *name ));
void save_account_obj	args(( ACCOUNT_DATA *acc ));
int check_account		args((DESCRIPTOR_DATA *, char *));
void del_account_name	args(( char * ));
void mail_pass			args((char *));
void account_char_save	args((ACCOUNT_DATA *, char *, int, int, int, int, int));
bool account_get_new_char_num	args((ACCOUNT_DATA *acc));
void account_free_char			args((ACCOUNT_DATA *, int));
bool check_account_name			args((char *));
bool account_is_playing	args((DESCRIPTOR_DATA *));

int	    stringSearch		args((char *, char *));
void     log_f                args( ( char * fmt, ... ) );
                                    



// admin.c 
void     wiznet              args(( char *string, ACCOUNT_DATA * account, long flag, long flag_skip));

void     substitute_alias    args( ( DESCRIPTOR_DATA *d, char *input ) );
bool     check_ban           args( ( char *site, int type) );



// boot.c
char *   print_flags         args( ( int flag ));
void     boot_db             args( ( void ) );
ROOM_INDEX_DATA *    get_room_index      args( (long vnum ) );
char     fread_letter        args( ( FILE *fp ) );
int      fread_number        args( ( FILE *fp ) );
long     fread_flag          args( ( FILE *fp ) );
char *   fread_string        args( ( FILE *fp ) );
char *   fread_string_eol    args( ( FILE *fp ) );
void     fread_to_eol        args( ( FILE *fp ) );
char *	 fread_line			 args( ( FILE *fp ) );
char *   fread_word          args( ( FILE *fp ) );
long     flag_convert        args( ( char letter) );
void *   alloc_mem           args( ( int sMem ) );
void *   alloc_perm          args( ( int sMem ) );
void     free_mem            args( ( void *pMem, int sMem ) );
char *   str_dup             args( ( const char *str ) );
void     free_string         args( ( char *pstr ) );
int      number_fuzzy        args( ( int number ) );
int      number_range        args( ( int from, int to ) );
int      number_percent      args( ( void ) );
long     number_mm           args( ( void ) );
int      dice                args( ( int number, int size ) );
int      interpolate         args( ( int level, int value_00, int value_32 ) );
void     append_file         args( ( ACCOUNT_DATA * account, char *file, char *str ) );
void     bug                 args( ( const char *str, int param ) );
void     log_string          args( ( const char *str ) );
void     tail_chain          args( ( void ) );



int   colour          args( ( char type, ACCOUNT_DATA * account, char *string ) );
void  colourconv      args( ( char *buffer, const char *txt, ACCOUNT_DATA * account));



// city.c
CITY_DATA * find_city args((int id));
CITY_DATA * find_city_coords args((int x, int y));

int count_carts args((CITY_DATA * city));
int max_carts args((CITY_DATA * city));

BUILDING_DATA * get_townhall args((CITY_DATA * city));

bool has_barracks args((CITY_DATA * city));
bool has_market args((CITY_DATA * city));
bool has_range args((CITY_DATA * city));


int count_buildings args((CITY_DATA * city));
int max_buildings args((CITY_DATA * city));

long count_footman args((CITY_DATA * city));
long count_archer args((CITY_DATA * city));
long count_settler args((CITY_DATA * city));
long count_troops args((CITY_DATA * city));
long max_troops args((CITY_DATA * city));

void view_city args((ACCOUNT_DATA * account, CITY_DATA * city));

int get_wood_income args((CITY_DATA * city));
int get_stone_income args((CITY_DATA * city));
int get_iron_income args((CITY_DATA * city));
int get_food_income args((CITY_DATA * city));

long max_wood args((CITY_DATA * city));
long max_stone args((CITY_DATA * city));
long max_iron args((CITY_DATA * city));
long max_food args((CITY_DATA * city));


bool has_city args((int x, int y));


// comm.c
int count_mxp_tags args((const int bMXP, const char *txt, int length));
void convert_mxp_tags args((const int bMXP, char * dest, const char *src, int length));
void     show_string         args( ( struct descriptor_data *d, char *input) );
void     close_socket        args( ( DESCRIPTOR_DATA *dclose ) );
void     write_to_buffer     args( ( DESCRIPTOR_DATA *d, const char *txt, int length ) );
void     send_to_desc        args( ( const char *txt, DESCRIPTOR_DATA *d ) );
void    send_to_account args((const char *txt, ACCOUNT_DATA * account));
void     send_to_world       args( ( const char *txt) );
void     printf_to_char      args( ( ACCOUNT_DATA *, char *, ... ) );
void     printf_to_desc      args( ( DESCRIPTOR_DATA *, char *, ... ) );
void     bugf                args( ( char *, ... ) );
bool     write_to_descriptor args( ( int desc, char *txt, int length ) );



// empire.c
int num_cities args((EMPIRE_DATA * empire));

int empire_score args((EMPIRE_DATA * empire));
long total_wood_income args((EMPIRE_DATA * empire));
long total_stone_income args((EMPIRE_DATA * empire));
long total_iron_income args((EMPIRE_DATA * empire));
long total_food_income args((EMPIRE_DATA * empire));

long total_troop_count args((EMPIRE_DATA * empire));
long total_troop_strength args((EMPIRE_DATA * empire));
long total_offensive_strength args((EMPIRE_DATA * empire));
long total_defensive_strength args((EMPIRE_DATA * empire));

void empire_prompt args((ACCOUNT_DATA * account));
void city_prompt args((ACCOUNT_DATA * account));


// handler.c

char * format_date args((ACCOUNT_DATA * account, long temp));
int count_sectors args(());
long get_random_color args (());
char * get_biome_string     args((int biome));
char * get_rarity_string    args((int rarity));
char  *add_border args((int length));
char *flag_string		args ( ( const struct flag_type *flag_table, int bits ) );
int get_sector  args((ROOM_INDEX_DATA *room));
bool room_is_affected args((ROOM_INDEX_DATA * room, int sn));
char *  port_directory  args((int port));
int     get_season      args(());
char    *timestamp      args( ( char strtime[16] ) );
void read_last_file args((ACCOUNT_DATA * account, int count, char *name));
void write_last_file args((char *entry));

bool    		is_name        	args( ( char *str, char *namelist ) );
bool			is_full_name	args( ( const char *str, char *namelist ) );
bool   	 		is_exact_name    args( ( char *str, char *namelist ) );


// parse.c 
void    interpret    args( ( ACCOUNT_DATA * account, char *argument ) );
bool    is_number    args( ( char *arg ) );
int    number_argument    args( ( char *argument, char *arg ) );
int    mult_argument    args( ( char *argument, char *arg) );
char *    one_argument    args( ( char *argument, char *arg_first ) );


// map_creation.c
void create_world args(());

// mem.c
ROOM_INDEX_DATA *new_room_index args((void));


// recycle.c

EMPIRE_DATA * new_empire args((void));
CITY_DATA * new_city args((void));
BUILDING_DATA * new_building args((void));
TROOP_DATA * new_troop args((void));
TRADE_DATA * new_trade args((CITY_DATA * city));
void free_trade args((TRADE_DATA * trade));
void extract_trade args((TRADE_DATA * trade, CITY_DATA * city));
RECRUIT_DATA * new_recruit args((CITY_DATA * city));
void free_recruit args((RECRUIT_DATA * recruit));
void extract_recruit args ((RECRUIT_DATA * recruit, CITY_DATA * city));

void extract_troop args ((TROOP_DATA * troop));

void free_note args((NOTE_DATA *note));
void free_buf args((BUFFER * buffer));
BUFFER *new_buf args(());
bool add_buf args((BUFFER * buffer, char * string));
char * buf_string args((BUFFER *buffer));

NOTE_DATA *new_note args(());

// mxp.c 
void  mxp_to_char               ( ACCOUNT_DATA * account, char *txt, int mxp_style );
void  shutdown_mxp              ( DESCRIPTOR_DATA *d );
void  init_mxp                  ( DESCRIPTOR_DATA *d );


// protocol.c
bool load_mssp_data args(( void ));
void send_mssp_data args(( DESCRIPTOR_DATA * d ));

// save.c
bool load_empire_obj args((DESCRIPTOR_DATA * d, char * name));
bool load_account args((DESCRIPTOR_DATA * d, char * name));
void fread_account args((ACCOUNT_DATA * account, FILE * fp));


void free_empire args((EMPIRE_DATA * empire));

void save_who_count args ((int count));
void    save_game_data  args(( void ));
void    load_game_data  args(( void ));
void	check_daily_login	args(( ACCOUNT_DATA * account));
void fread_help 		args((HELP_DATA *help, FILE * fp));
void fwrite_help 		args((HELP_DATA *help, FILE * fp));
void  save_game_data	args((void));
void  load_game_data	args((void));


// update.c
void    time_update     args (( void));
void    update_handler    args( ( void ) );


// string.c
char *to_lower args((const char * str));
char *smash_crlf    args(( const char *str ));
char * strip_quotes args((const char *str));
char * strip_single_quotes args((const char *str));
void     strip_char            args((char *str, char character));
char *     smash_period        args((char *str));
void     smash_tilde         args( ( char *str ) );
void     smash_dollar        args( ( char *str ) );
bool     str_cmp             args( ( const char *astr, const char *bstr ) );
bool     str_prefix          args( ( const char *astr, const char *bstr ) );
bool     str_infix           args( ( const char *astr, const char *bstr ) );
bool     str_suffix          args( ( const char *astr, const char *bstr ) );
char *   capitalize          args( ( const char *str ) );
char * strip_articles args((char *str));
char * strip_color args ((const char * str));
void    string_edit    args( ( ACCOUNT_DATA * account, char **pString ) );
void    string_append   args( ( ACCOUNT_DATA * account, char **pString ) );
char *    string_replace    args( ( char * orig, char * old, char * new_str ) );
void    string_add      args( ( ACCOUNT_DATA * account, char *argument ) );
char *  format_string   args( ( char *oldstring /*, bool fSpace */ ) );
char *  first_arg       args( ( char *argument, char *arg_first, bool fCase ) );
char *    string_unpad    args( ( char * argument ) );
char *    string_proper    args( ( char * argument ) );


/*
 * Global variables
 */
 
extern DESCRIPTOR_DATA *descriptor_list;    /* All open descriptors     */
extern WORLD_DATA * world;


extern GAME_DATA game_data;

 
extern HELP_DATA * help_last;

extern HELP_DATA *help_free;

extern bool wizlock;                    /* Game is wizlocked        */
extern bool newlock;                    /* Game is newlocked        */

extern        int   top_help;
extern        long   top_room;
extern        long  top_vnum_room;
extern        int top_empire;
extern        int top_city;
extern        int top_account;
extern        char  str_empty [1];
extern        ROOM_INDEX_DATA *    room_index_hash [MAX_KEY_HASH];

extern int      port, control;
