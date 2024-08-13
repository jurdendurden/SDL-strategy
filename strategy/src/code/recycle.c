#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "recycle.h"

BUFFER *buf_free;
ACCOUNT_DATA * account_free;
DESCRIPTOR_DATA *descriptor_free;
EMPIRE_DATA * empire_free;
CITY_DATA * city_free;
BUILDING_DATA * building_free;
BAN_DATA *ban_free;
TROOP_DATA * troop_free;
RECRUIT_DATA * recruit_free;
TRADE_DATA * trade_free;

ROOM_INDEX_DATA *room_index_free;
HELP_DATA *help_free;

TROOP_DATA * new_troop(void)
{
    static TROOP_DATA troop_zero;
    TROOP_DATA * troop;


    if (troop_free == NULL)
        troop = (TROOP_DATA *)alloc_perm (sizeof (*troop));
    else
    {
        troop = troop_free;
        troop_free = troop_free->next;
    }

    *troop = troop_zero;
    VALIDATE (troop);
    troop->name = &str_empty[0];

    troop->next = troop_list;
    troop_list = troop;


    return troop;
}

void free_troop (TROOP_DATA * troop)
{
    if (!IS_VALID (troop))
        return;


    free_string (troop->name);
    
    troop->next = troop_free;
    troop_free = troop;

    INVALIDATE (troop);
    return;
}

void extract_troop (TROOP_DATA * troop)
{
    if (troop_list == troop)
    {
        troop_list = troop->next;
    }
    else
    {
        TROOP_DATA *prev;

        for (prev = troop_list; prev != NULL; prev = prev->next)
        {
            if (prev->next == troop)
            {
                prev->next = troop->next;
                break;
            }
        }

        if (prev == NULL)
        {
            bug ("extract_troop: troop not found.", 0);
            return;
        }
    }

    free_troop(troop);
    return;
}

ACCOUNT_DATA *new_account (void)
{
    static ACCOUNT_DATA account_zero;
    ACCOUNT_DATA * account;    

    if (account_free == NULL)
    {
        account = (ACCOUNT_DATA *)alloc_perm (sizeof (*account));
        top_account++;
    }
    else
    {
        account = account_free;
        account_free = account_free->next;
    }

    *account = account_zero;
    VALIDATE (account);
    account->name 	= str_dup("");
    account->screen_name = str_dup("");
    account->password 	= str_dup("");
    account->old_passwd = str_dup("");
    account->email 	= str_dup("");
    account->empire_name = str_dup("");

    account->act 	= 0;
    account->verify 	= 0;
    account->numb 	= -1;
    account->hours 	= 0;
    account->secret 	= 0;
    account->version 	= 0;    	    

    account->next = account_list;
    account_list = account;

    return account;
}

void extract_account (ACCOUNT_DATA * account)
{
    if (account_list == account)
    {
        account_list = account->next;
    }
    else
    {
        ACCOUNT_DATA *prev;

        for (prev = account_list; prev != NULL; prev = prev->next)
        {
            if (prev->next == account)
            {
                prev->next = account->next;
                break;
            }
        }

        if (prev == NULL)
        {
            bug ("extract_account: account not found.", 0);
            return;
        }
    }

    free_account(account);
    return;
}

BUILDING_DATA *new_building (void)
{
    static BUILDING_DATA building_zero;
    BUILDING_DATA *building;

    if (building_free == NULL)
        building = (BUILDING_DATA *)alloc_perm (sizeof (*building));
    else
    {
        building = building_free;
        building_free = building_free->next;
    }
    *building = building_zero;
    VALIDATE (building);    

    building->next = building_list;
    building_list = building;

    return building;
}


void free_building(BUILDING_DATA * building)
{
   
    if (!IS_VALID (building))
        return;

    INVALIDATE (building);

    building->next = building_free;
    building_free = building;
}

void extract_building (BUILDING_DATA * building)
{
    if (building_list == building)
    {
        building_list = building->next;
    }
    else
    {
        BUILDING_DATA *prev;

        for (prev = building_list; prev != NULL; prev = prev->next)
        {
            if (prev->next == building)
            {
                prev->next = building->next;
                break;
            }
        }

        if (prev == NULL)
        {
            bug ("Extract_building: building not found.", 0);
            return;
        }
    }

    free_building (building);
    return;
}



CITY_DATA *new_city (void)
{
    static CITY_DATA city_zero;
    CITY_DATA *city;

    if (city_free == NULL)
        city = (CITY_DATA *)alloc_perm (sizeof (*city));
    else
    {
        city = city_free;
        city_free = city_free->next;
    }
    *city = city_zero;
    VALIDATE (city);

    city->next = city_list;
    city_list = city;

    city_count++;
    city->id = city_count;
    return city;
}


void free_city (CITY_DATA * city)
{
   
    if (!IS_VALID (city))
        return;

    free_string (city->name);    
    INVALIDATE (city);

    city->next = city_free;
    city_free = city;
    city_count--;
}

void extract_city (CITY_DATA * city)
{
    if (city_list == city)
    {
        city_list = city->next;
    }
    else
    {
        CITY_DATA *prev;

        for (prev = city_list; prev != NULL; prev = prev->next)
        {
            if (prev->next == city)
            {
                prev->next = city->next;
                break;
            }
        }

        if (prev == NULL)
        {
            bug ("Extract_city: city %d not found.", city->id - 1);
            return;
        }
    }

    free_city (city);
    return;
}
    

EMPIRE_DATA *new_empire (void)
{
    static EMPIRE_DATA empire_zero;
    EMPIRE_DATA *empire;

    if (empire_free == NULL)
        empire = (EMPIRE_DATA *)alloc_perm (sizeof (*empire));
    else
    {
        empire = empire_free;
        empire_free = empire_free->next;
    }
    *empire = empire_zero;
    VALIDATE (empire);

    empire->next = empire_list;
    empire_list = empire;

    empire_count++;
    empire->id = empire_count;
    return empire;
}


void free_empire (EMPIRE_DATA * empire)
{
   
    if (!IS_VALID (empire))
        return;

    free_string (empire->name);    
    free_string (empire->owner);
    INVALIDATE (empire);

    empire->next = empire_free;
    empire_free = empire;
    empire_count--;
}

void extract_empire (EMPIRE_DATA * emp)
{
    if (empire_list == emp)
    {
        empire_list = emp->next;
    }
    else
    {
        EMPIRE_DATA *prev;

        for (prev = empire_list; prev != NULL; prev = prev->next)
        {
            if (prev->next == emp)
            {
                prev->next = emp->next;
                break;
            }
        }

        if (prev == NULL)
        {
            bug ("Extract_empire: empire %d not found.", emp->id - 1);
            return;
        }
    }

    free_empire (emp);
    return;
}
    











BAN_DATA *new_ban (void)
{
    static BAN_DATA ban_zero;
    BAN_DATA *ban;

    if (ban_free == NULL)
        ban = (BAN_DATA *)alloc_perm (sizeof (*ban));
    else
    {
        ban = ban_free;
        ban_free = ban_free->next;
    }

    *ban = ban_zero;
    VALIDATE (ban);
    ban->name = &str_empty[0];
    return ban;
}

void free_ban (BAN_DATA * ban)
{
    if (!IS_VALID (ban))
        return;

    free_string (ban->name);
    INVALIDATE (ban);

    ban->next = ban_free;
    ban_free = ban;
}

DESCRIPTOR_DATA *new_descriptor (void)
{
    static DESCRIPTOR_DATA d_zero;
    DESCRIPTOR_DATA *d;

    if (descriptor_free == NULL)
        d = (DESCRIPTOR_DATA *)alloc_perm (sizeof (*d));
    else
    {
        d = descriptor_free;
        descriptor_free = descriptor_free->next;
    }

    *d = d_zero;
    VALIDATE (d);

    d->connected = CON_GET_NAME;
    d->showstr_head = NULL;
    d->showstr_point = NULL;
    d->outsize = 2000;
    d->outbuf = (char *)alloc_mem (d->outsize);
    d->account = new_account();

    return d;
}

void free_descriptor (DESCRIPTOR_DATA * d)
{
    if (!IS_VALID (d))
        return;

    free_string (d->host);
    free_mem (d->outbuf, d->outsize);
    INVALIDATE (d);
    d->next = descriptor_free;
    descriptor_free = d;
}


/* buffer sizes */
const int buf_size[MAX_BUF_LIST] = {
    16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384
};

/* local procedure for finding the next acceptable size */
/* -1 indicates out-of-boundary error */
int get_size (int val)
{
    int i;

    for (i = 0; i < MAX_BUF_LIST; i++)
        if (buf_size[i] >= val)
        {
            return buf_size[i];
        }

    return -1;
}

BUFFER *new_buf ()
{
    BUFFER *buffer;

    if (buf_free == NULL)
        buffer = (BUFFER *)alloc_perm (sizeof (*buffer));
    else
    {
        buffer = buf_free;
        buf_free = buf_free->next;
    }

    buffer->next = NULL;
    buffer->state = BUFFER_SAFE;
    buffer->size = get_size (BASE_BUF);

    buffer->string = (char *)alloc_mem (buffer->size);
    buffer->string[0] = '\0';
    VALIDATE (buffer);

    return buffer;
}

BUFFER *new_buf_size (int size)
{
    BUFFER *buffer;

    if (buf_free == NULL)
        buffer = (BUFFER *)alloc_perm (sizeof (*buffer));
    else
    {
        buffer = buf_free;
        buf_free = buf_free->next;
    }

    buffer->next = NULL;
    buffer->state = BUFFER_SAFE;
    buffer->size = get_size (size);
    if (buffer->size == -1)
    {
        bug ("new_buf: buffer size %d too large.", size);
        exit (1);
    }
    buffer->string = (char *)alloc_mem (buffer->size);
    buffer->string[0] = '\0';
    VALIDATE (buffer);

    return buffer;
}


void free_buf (BUFFER * buffer)
{
    if (!IS_VALID (buffer))
        return;

    free_mem (buffer->string, buffer->size);
    buffer->string = NULL;
    buffer->size = 0;
    buffer->state = BUFFER_FREED;
    INVALIDATE (buffer);

    buffer->next = buf_free;
    buf_free = buffer;
}


bool add_buf (BUFFER * buffer, char *string)
{
    int len;
    char *oldstr;
    int oldsize;

    oldstr = buffer->string;
    oldsize = buffer->size;

    if (buffer->state == BUFFER_OVERFLOW)    /* don't waste time on bad strings! */
        return FALSE;

    len = strlen (buffer->string) + strlen (string) + 1;

    while (len >= buffer->size)
    {                            /* increase the buffer size */
        buffer->size = get_size (buffer->size + 1);
        {
            if (buffer->size == -1)
            {                    /* overflow */
                buffer->size = oldsize;
                buffer->state = BUFFER_OVERFLOW;
                bug ("buffer overflow past size %d", buffer->size);
                return FALSE;
            }
        }
    }

    if (buffer->size != oldsize)
    {
        buffer->string = (char *)alloc_mem (buffer->size);

        strcpy (buffer->string, oldstr);
        free_mem (oldstr, oldsize);
    }

    strcat (buffer->string, string);
    return TRUE;
}


void clear_buf (BUFFER * buffer)
{
    buffer->string[0] = '\0';
    buffer->state = BUFFER_SAFE;
}


char *buf_string (BUFFER * buffer)
{
    return buffer->string;
}



/*
HELP_DATA *new_help (void)
{
    HELP_DATA *help;

    if (help_free)
    {
        help = help_free;
        help_free = help_free->next;
    }
    else
        help = alloc_perm (sizeof (*help));

    return help;
}
*/

void free_help (HELP_DATA * help)
{
    free_string (help->keyword);
    free_string (help->text);
    free_string (help->name);
    help->next = help_free;
    help_free = help;
}



/* stuff for recyling notes */
NOTE_DATA *note_free;

NOTE_DATA *new_note()
{
    NOTE_DATA *note;

    if (note_free == NULL)
    note = alloc_perm(sizeof(*note));
    else
    { 
    note = note_free;
    note_free = note_free->next;
    }
    VALIDATE(note);
    note->persist = FALSE;
    return note;
}

void free_note(NOTE_DATA *note)
{
    if (!IS_VALID(note))
    return;

    free_string( note->text    );
    free_string( note->subject );
    free_string( note->to_list );
    free_string( note->date    );
    free_string( note->sender  );
    INVALIDATE(note);

    note->persist = FALSE;
    note->next    = note_free;
    note_free     = note;
}




RECRUIT_DATA *new_recruit (CITY_DATA * city)
{
    static RECRUIT_DATA recruit_zero;
    RECRUIT_DATA * recruit;    

    if (recruit_free == NULL)
        recruit = (RECRUIT_DATA *)alloc_perm (sizeof (*recruit));
    else
    {
        recruit = recruit_free;
        recruit_free = recruit_free->next;
    }

    *recruit = recruit_zero;
    VALIDATE (recruit);

    recruit->next = city->recruiting;
    city->recruiting = recruit;

    return recruit;
}


void free_recruit (RECRUIT_DATA * recruit)
{
    if (!IS_VALID (recruit))
        return;    
    
    recruit->next = recruit_free;
    recruit_free = recruit;

    INVALIDATE (recruit);

    return;
}

void extract_recruit (RECRUIT_DATA * recruit, CITY_DATA * city)
{
    if (city->recruiting == recruit)
    {
        city->recruiting = recruit->next;
    }
    else
    {
        RECRUIT_DATA *prev;

        for (prev = city->recruiting; prev != NULL; prev = prev->next)
        {
            if (prev->next == recruit)
            {
                prev->next = recruit->next;
                break;
            }
        }

        if (prev == NULL)
        {
            bug ("extract_recruit: recruit not found.", 0);
            return;
        }
    }    

    free_recruit(recruit);
    return;
}


TRADE_DATA *new_trade (CITY_DATA * city)
{
    static TRADE_DATA trade_zero;
    TRADE_DATA * trade;    

    if (trade_free == NULL)
        trade = (TRADE_DATA *)alloc_perm (sizeof (*trade));
    else
    {
        trade = trade_free;
        trade_free = trade_free->next;
    }

    *trade = trade_zero;
    VALIDATE (trade);

    trade->next = city->trading;
    city->trading = trade;

    return trade;
}


void free_trade (TRADE_DATA * trade)
{
    if (!IS_VALID (trade))
        return;    
    
    trade->next = trade_free;
    trade_free = trade;

    INVALIDATE (trade);

    return;
}

void extract_trade (TRADE_DATA * trade, CITY_DATA * city)
{
    if (city->trading == trade)
    {
        city->trading = trade->next;
    }
    else
    {
        TRADE_DATA *prev;

        for (prev = city->trading; prev != NULL; prev = prev->next)
        {
            if (prev->next == trade)
            {
                prev->next = trade->next;
                break;
            }
        }

        if (prev == NULL)
        {
            bug ("extract_trade: trade not found.", 0);
            return;
        }
    }    

    free_trade(trade);
    return;
}





HELP_DATA *new_help(void)
{
     HELP_DATA *new_help;

     new_help = (HELP_DATA*)alloc_perm(sizeof(*new_help) );

     new_help->level    = 0;
     new_help->id		= 0;
     new_help->version	= 1;
     new_help->keyword = str_dup("");
     new_help->text    = str_dup("");
     new_help->name		= str_dup("");
     new_help->last_editted_by = str_dup("");
     new_help->next    = NULL;
     

     return new_help;
}

void free_room_index (ROOM_INDEX_DATA * pRoom)
{
    free_string (pRoom->name);
    free_string (pRoom->description);
    free_string (pRoom->owner);

    pRoom->x = 0;
    pRoom->y = 0;    

    pRoom->next = room_index_free;
    room_index_free = pRoom;
    return;
}


ROOM_INDEX_DATA *new_room_index (void)
{
    ROOM_INDEX_DATA *pRoom;

    if (!room_index_free)
    {
        pRoom = (ROOM_INDEX_DATA*)alloc_perm (sizeof (*pRoom));
        top_room++;
    }
    else
    {
        pRoom = room_index_free;
        room_index_free = room_index_free->next;
    }

    pRoom->next = NULL;

    pRoom->name = &str_empty[0];
    pRoom->description = &str_empty[0];
    pRoom->owner = &str_empty[0];
    pRoom->vnum = 0;
    pRoom->room_flags = 0;
    pRoom->sector_type = 0;

    
    pRoom->x = 0;
    pRoom->y = 0;
    
    return pRoom;
}
