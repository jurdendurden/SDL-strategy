#include "main.h"
#include "parse.h"
#include "recycle.h"

//Local functions.
void	msdp_update	args( ( void ) ); 
void 	time_update args((void));

//GLOBAL VARIABLES:
int 	save_number = 0;
int     global_exp;
int		global_gold;
int 	global_skill;
int		global_quest;
int		global_res;
//END GLOBALS


//This function sends prompts to accounts
void account_update (void)
{
    DESCRIPTOR_DATA * d;

    for (d = descriptor_list; d != NULL; d = d->next)
    {        
        if (d->connected == CON_PLAYING && d->account && d->account->valid)        
            city_prompt(d->account);
    }

    return;
}

//This function updates each empire current in game 
void empire_update (void)
{
	EMPIRE_DATA * empire;
	EMPIRE_DATA * emp_next;
    //CITY_DATA * city;
    //CITY_DATA * city_next;


	for (empire = empire_list; empire != NULL; empire = emp_next)
    {
        emp_next = empire->next;
	
		//update resources/troops/etc
	}

	return;
}

//This function updates an individual city (resource collection, etc)
void city_update (void)
{
    //BUILDING_DATA * building;
    //BUILDING_DATA * building_next;
	CITY_DATA * city;
	CITY_DATA * city_next;
    RECRUIT_DATA * recruit;
    RECRUIT_DATA * recruit_next;
    TROOP_DATA * troop;
    //ACCOUNT_DATA * account;
    //char buf[MSL];

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        city->wood += get_wood_income(city) / 60 / 5;
        city->stone += get_stone_income(city)  / 60 / 5;
        city->iron += get_iron_income(city) / 60 / 5;
        city->food += get_food_income(city) / 60 / 5;

                
        if (city->wood > max_wood(city))
            city->wood = max_wood(city);

        if (city->stone > max_stone(city))
            city->stone = max_stone(city);
    
        if (city->iron > max_iron(city))
            city->iron = max_iron(city);

        if (city->food > max_food(city))
            city->food = max_food(city);
        

        for (recruit = city->recruiting; recruit != NULL; recruit = recruit_next)
        {
            recruit_next = recruit->next;

            recruit->time_left -= PULSE_CITY;
            if (recruit->time_left <= 0)
            {
                int amount = 0;

                for (amount = 1; amount <= recruit->amount; amount++)
                {
                    troop = new_troop();
                    troop->type = troop_table[recruit->type].sub_type;
                    troop->city = str_dup(city->name);         
                    troop->city_id = city->id;
                    troop->empire_name = str_dup(city->empire_name);                       
                    extract_recruit(recruit, city);
                }
            }
        }
	    
    }
        
    return;
}

void time_update (void)
{	
	char buf[MSL];
    DESCRIPTOR_DATA *d;
	static int moon = 0;
	
	buf[0] = '\0';
	
	time_info.hour++;


    for (d = descriptor_list; d != NULL; d = d->next)
    {        
        if (d->connected == CON_PLAYING)
        {
            switch (time_info.hour)
            {
                case 4:
                    sprintf (buf, "The first breaking rays of the light of day permeate the horizon.\r\n");
                    break;
                case 5:
                    weather_info.sunlight = SUN_LIGHT;
                    sprintf (buf, "The landscape is slowly transformed by the trickling of light as the morning begins.\r\n");
                    break;

                case 6:
                    weather_info.sunlight = SUN_RISE;
                    sprintf (buf, "The morning sun rises in the east, basking the horizon in sunlight. Illuminating your surroundings.\r\n");
                    weather_info.tide = FALSE;
                    break;
                
                case 12:
                    weather_info.tide = TRUE;			
                    break;
                
                case 13:
                    sprintf (buf, "The afternoon sun hangs high in the sky, relentlessly lighting your surroundings.\r\n");
                    break;
                
                case 18:
                    weather_info.tide = FALSE;
                    break;
                    
                case 19:
                    weather_info.sunlight = SUN_SET;
                    sprintf (buf, "The sun slowly disappears, painting a beautiful medley of colors along the horizon leading to the west.\r\n");			
                    break;

                case 20:
                    weather_info.sunlight = SUN_DARK;
                    sprintf (buf, "Darkness creeps around you as the night begins.\r\n");
                    break;
            }
            SEND(buf, d->account);
        }
    }	
    if (time_info.hour == 24)
    {
        time_info.hour = 0;
        time_info.day++;
        weather_info.tide = TRUE;			
        moon++;
        if (moon == 2) //16 day cycles
        {
            weather_info.moon_phase++;
            if (weather_info.moon_phase > 7)
                weather_info.moon_phase = 0;
                    moon = 0;
        }
    }

	if (time_info.day >= 35)
	{
		time_info.day = 0;
		time_info.month++;
	}

	if (time_info.month >= 17)
	{
		time_info.month = 0;
		time_info.year++;
	}
		
	return;
} 
 
void trade_update(void)
{
    CITY_DATA * city;
	CITY_DATA * city_next;
    CITY_DATA * to_city;
    TRADE_DATA * trade;
    TRADE_DATA * trade_next;

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        for (trade = city->trading; trade != NULL; trade = trade_next)
        {
            trade_next = trade->next;

            trade->time_left--;

            if (trade->time_left < 1)
            {
                
                to_city = find_city(trade->to_city_id);
                switch(trade->resource)
                {
                    default:
                        break;
                    case RES_WOOD:
                        to_city->wood += trade->amount;
                        break;
                    case RES_STONE:
                        to_city->stone += trade->amount;
                        break;
                    case RES_IRON:
                        to_city->iron += trade->amount;
                        break;
                    case RES_FOOD:
                        to_city->food += trade->amount;
                        break;
                }
                extract_trade(trade, city);
            }
        }
    }
    return;
}

void update_handler (void)
{
	static int pulse_empire;
    static int pulse_city;
    static int pulse_account;
    static int pulse_trade;

	/*if ( --pulse_msdp <= 0 )
    {
        pulse_msdp      = PULSE_PER_SECOND;
        msdp_update();
    }*/	

    if (--pulse_city <= 0)
    {
        pulse_city = PULSE_CITY;
        city_update();
    }

    if (--pulse_empire <= 0)
    {
        pulse_empire = PULSE_EMPIRE;
        empire_update();                
    }

    if (--pulse_account <= 0)
    {
        pulse_account = PULSE_ACCOUNT;
        account_update();
    }

    if (--pulse_trade <= 0)
    {
        pulse_trade = PULSE_TRADE;
        trade_update();
    }
    
    tail_chain ();
    return;
}



/* For time/dates */
char *const day_name2[] = {
    "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};
//0 1 2 3 4 winter
// 5 6 7 8 spring
// 9 10 11 summer
// 12 13 14 15 16 fall
char *const month_name2[] = {
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void achievement_update(void)
{
	return;
}

void msdp_update( void )
{
    DESCRIPTOR_DATA *d;
    int PlayerCount = 0;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	/*if ( d->account && d->connected == CON_PLAYING && !IS_NPC(d->account) )
        {
            char buf[MSL];
            ROOM_INDEX_DATA *pRoom = d->account->in_room;
            AFFECT_DATA *paf;

            ++PlayerCount;

            MSDPSetString( d, eMSDP_CHARACTER_NAME, d->account->name );
            MSDPSetNumber( d, eMSDP_ALIGNMENT, d->account->alignment );
            MSDPSetNumber( d, eMSDP_EXPERIENCE, d->account->exp );

            MSDPSetNumber( d, eMSDP_HEALTH, d->account->hit );
            MSDPSetNumber( d, eMSDP_HEALTH_MAX, d->account->max_hit );
            MSDPSetNumber( d, eMSDP_LEVEL, d->account->level );

            MSDPSetNumber( d, eMSDP_MANA, d->account->mana );
            MSDPSetNumber( d, eMSDP_MANA_MAX, d->account->max_mana );
            MSDPSetNumber( d, eMSDP_WIMPY, d->account->wimpy );
            MSDPSetNumber( d, eMSDP_PRACTICE, d->account->practice );
            MSDPSetNumber( d, eMSDP_MONEY, d->account->gold );
            MSDPSetNumber( d, eMSDP_MOVEMENT, d->account->move );
            MSDPSetNumber( d, eMSDP_MOVEMENT_MAX, d->account->max_move );
            MSDPSetNumber( d, eMSDP_HITROLL, HITROLL(d->account) );
            MSDPSetNumber( d, eMSDP_DAMROLL, MELEE_DAM(d->account) );
            //MSDPSetNumber( d, eMSDP_AC, GET_AC(d->account) );
            MSDPSetNumber( d, eMSDP_STR, GET_STR(d->account) );
            MSDPSetNumber( d, eMSDP_INT, GET_INT(d->account) );
            MSDPSetNumber( d, eMSDP_WIS, GET_WIS(d->account) );
            MSDPSetNumber( d, eMSDP_DEX, GET_DEX(d->account) );
			MSDPSetNumber( d, eMSDP_CON, GET_CON(d->account) );
            MSDPSetNumber( d, eMSDP_STR_PERM, BASE_STR(d->account) );
            MSDPSetNumber( d, eMSDP_INT_PERM, BASE_INT(d->account) );
            MSDPSetNumber( d, eMSDP_WIS_PERM, BASE_WIS(d->account) );
            MSDPSetNumber( d, eMSDP_DEX_PERM, BASE_DEX(d->account) );
            MSDPSetNumber( d, eMSDP_CON_PERM, BASE_CON(d->account) );

            // Only update room stuff if they've changed room 
            if ( pRoom && pRoom->vnum != d->pProtocol->pVariables[eMSDP_ROOM_VNUM]->ValueInt )
            {
                int i; // Loop counter 
                buf[0] = '\0';

                for ( i = DIR_NORTH; i < MAX_DIR; ++i )
                {
                    if ( pRoom->exit[i] != NULL )
                    {
                        const char MsdpVar[] = { (char)MSDP_VAR, '\0' };
                        const char MsdpVal[] = { (char)MSDP_VAL, '\0' };
                        extern char *const dir_name[];

                        strcat( buf, MsdpVar );
                        strcat( buf, dir_name[i] );
                        strcat( buf, MsdpVal );

                        if ( IS_SET(pRoom->exit[i]->exit_info, EX_CLOSED) )
                            strcat( buf, "C" );
                        else // The exit is open 
                            strcat( buf, "O" );
                    }
                }

                if ( pRoom->area != NULL )
                    MSDPSetString( d, eMSDP_AREA_NAME, pRoom->area->name );

                MSDPSetString( d, eMSDP_ROOM_NAME, pRoom->name );
                MSDPSetTable( d, eMSDP_ROOM_EXITS, buf );
                MSDPSetNumber( d, eMSDP_ROOM_VNUM, pRoom->vnum );
            }

            buf[0] = '\0';
            for ( paf = d->account->affected; paf; paf = paf->next )
            {
                char skill_buf[MSL];
                sprintf( skill_buf, "%c%s%c%d",
                    (char)MSDP_VAR, paf->type < 9000 ? skill_table[paf->type].name : "", 
                    (char)MSDP_VAL, paf->duration );
                strcat( buf, skill_buf );
            }
            MSDPSetTable( d, eMSDP_AFFECTS, buf );

            MSDPUpdate( d );
        }*/	
	}
    /* Ideally this should be called once at startup, and again whenever 
     * someone leaves or joins the mud.  But this works, and it keeps the 
     * snippet simple.  Optimise as you see fit.
     */
    MSSPSetPlayers( PlayerCount );
}
