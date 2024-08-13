#include "main.h"
#include "parse.h"
#include "recycle.h"
#include <stdlib.h>
#include <dirent.h>

//Show account their score combines from all cities/troops.
CMD(do_score)
{
    bool found = FALSE;
    char buf[MSL];       
    EMPIRE_DATA * empire;
    EMPIRE_DATA * empire_next; 
    setlocale(LC_ALL,"");


    sprintf(buf, "Screen Name: %-16s\r\n", account->name);
    SEND(buf, account);

    for (empire = empire_list; empire != NULL; empire = empire_next)
    {
        empire_next = empire->next; 

        if (!str_cmp(empire->owner, account->name))
        {
            found = TRUE;
            break;
        }
        

        if (found)        
            empire_prompt(account);            
    }

    return;
}

CMD(do_memory)
{
    
    return;
}

CMD(do_city)
{
    CITY_DATA * city;
    char buf[MSL];
    char arg1[MIL];
    char arg2[MIL];

    city = account->cur_city;

    if (IS_NULLSTR(argument))
    {
        view_city(account, city);
        return;
    }

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);    

    if (!str_cmp(arg1, "name"))
    {
        if (IS_NULLSTR(arg2))
        {
            SEND("What would you rename the city to?\r\n",account);
            return;
        }

        city->name = str_dup(arg2);
        sprintf(buf, "Current city name changed to %s.\r\n", city->name);
        SEND(buf, account);
    }
    return;
}

CMD(do_empire)
{    
    CITY_DATA * city;
    CITY_DATA * city_next;
    char arg1[MIL];
    char arg2[MIL];
    char buf[MSL];

    if (IS_NULLSTR(argument))
    {
        empire_prompt(account);
        return;
    }

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);        

    return;
}

CMD(do_build)
{
    int i = 0;
    BUILDING_DATA * building;
    char buf[MSL];

    if (!account->cur_city)
        return;

    if (count_buildings(account->cur_city) >= max_buildings(account->cur_city))
    {
        SEND("Upgrade your townhall to build more buildings.\r\n",account);
        return;
    }

    if (IS_NULLSTR(argument) || !str_cmp(argument, "?"))
    {
        SEND("Buildings available:\r\n\r\n", account);
        for (i = 0; !IS_NULLSTR(building_table[i].name); i++)
        {
            sprintf(buf, "%s\r\n", building_table[i].name);
            SEND(buf, account);
        }
        return;
    }


    for (i = 0; !IS_NULLSTR(building_table[i].name); i++)
    {
        if (!str_cmp(building_table[i].name, argument))
        {
            if (building_table[i].type == BLD_TOWNHALL)
                continue;

            if (building_table[i].type == BLD_KEEP)
            {
                BUILDING_DATA * th;

                if ((th = get_townhall(account->cur_city)) == NULL)
                {
                    SEND("There's no townhall here.\r\n",account);
                    return;
                }

                if (th->level < 6)
                {
                    SEND("You must first improve your townhall to level 6.\r\n",account);
                    return;
                }
            }

            if (building_table[i].type == BLD_KEEP)
            {
                BUILDING_DATA * th;

                if ((th = get_townhall(account->cur_city)) == NULL)
                {
                    SEND("There's no townhall here.\r\n",account);
                    return;
                }

                if (th->level < 8)
                {
                    SEND("You must first improve your townhall to level 8.\r\n",account);
                    return;
                }
            }

            if (account->cur_city->wood < building_table[i].wood_cost[0])
            {
                SEND("You need more wood to build that.\r\n",account);
                return;
            }

            if (account->cur_city->stone < building_table[i].stone_cost[0])
            {
                SEND("You need more stone to build that.\r\n",account);
                return;
            }

            if (account->cur_city->iron < building_table[i].iron_cost[0])
            {
                SEND("You need more iron to build that.\r\n",account);
                return;
            }

            if (account->cur_city->food < building_table[i].food_cost[0])
            {
                SEND("You need more food to build that.\r\n",account);
                return;
            }

            account->cur_city->wood -= building_table[i].wood_cost[0];
            account->cur_city->stone -= building_table[i].stone_cost[0];
            account->cur_city->iron -= building_table[i].iron_cost[0];
            account->cur_city->food -= building_table[i].food_cost[0];

            building = new_building();
            building->level = 1;
            building->type = building_table[i].type;
            building->city = str_dup(account->cur_city->name);
            building->city_id = account->cur_city->id;

            sprintf(buf, "You build a %s.\r\n", building_table[i].name);
            SEND(buf, account);
            return;
        }
    }

    SEND("Not a valid building type.\r\n",account);
    return;
}

CMD(do_upgrade)
{
    int index = 0;
    int count = 0;
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    char buf[MSL];

    if (!account->cur_city)
        return;

    if (!is_number(argument))
    {
        SEND("Indicate the building index to upgrade.\r\n",account);
        return;
    }

    index = atoi(argument);

    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == account->cur_city->id)
        {
            if (count == index)
            {
                if (building->level == 10)
                {
                    SEND("This building is at max level already.\r\n",account);
                    return;
                }

                if (account->cur_city->wood < building_table[building->type].wood_cost[building->level])
                {
                    SEND("You need more wood to upgrade it.\r\n",account);
                    return;
                }

                if (account->cur_city->stone < building_table[building->type].stone_cost[building->level])
                {
                    SEND("You need more stone to upgrade it.\r\n",account);
                    return;
                }

                if (account->cur_city->iron < building_table[building->type].iron_cost[building->level])
                {
                    SEND("You need more iron to upgrade it.\r\n",account);
                    return;
                }

                if (account->cur_city->food < building_table[building->type].food_cost[building->level])
                {
                    SEND("You need more food to upgrade it.\r\n",account);
                    return;
                }

                account->cur_city->wood -= building_table[building->type].wood_cost[building->level];
                account->cur_city->stone -= building_table[building->type].stone_cost[building->level];
                account->cur_city->iron -= building_table[building->type].iron_cost[building->level];
                account->cur_city->food -= building_table[building->type].food_cost[building->level];
                building->level++;

                sprintf(buf, "%s upgraded to level %d.\r\n", building_table[building->type].name, building->level);
                SEND(buf, account);
                return;
            }
            count++;
        }
    }
    SEND("Building index not found in this city.\r\n",account);
    return;
}

   
CMD(do_recruit)
{
    char buf[MSL];
    RECRUIT_DATA * recruit;
    int i = 0;
    int amount = 0;

    char arg1[MIL];
    char arg2[MIL];

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);    

    if (count_troops(account->cur_city) >= max_troops(account->cur_city))
    {
        SEND("You need more troop space, build more or upgrade existing barracks.",account);
        return;
    }

    if (!is_number(arg1) || IS_NULLSTR(arg1))
    {
        SEND("First argument must be the amount of troops to be recruited.\r\n",account);
        return;
    }

    if (IS_NULLSTR(arg2))
    {
        SEND("Second argument must be the troop type name (footman,archer,etc)\r\n",account);
        return;
    }

    amount = atoi(arg1);

    for (i = 0; !IS_NULLSTR(troop_table[i].name); i++)
    {
        if (!str_cmp(arg2, troop_table[i].name))
        {
            if (troop_table[i].sub_type == TROOP_FOOTMAN && !has_barracks(account->cur_city))
            {
                SEND("You must build a barracks before you can recruit footmen.\r\n",account);
                return;
            }

            if (troop_table[i].sub_type == TROOP_ARCHER && !has_range(account->cur_city))
            {
                SEND("You must build a range before you can recruit archer.\r\n",account);
                return;
            }

            if (account->cur_city->wood < troop_table[i].wood_cost * amount)
            {
                SEND("You need more wood to recruit this troop.\r\n",account);
                return;
            }
            
            if (account->cur_city->stone < troop_table[i].stone_cost * amount)
            {
                SEND("You need more stone to recruit this troop.\r\n",account);
                return;
            }

            if (account->cur_city->iron < troop_table[i].iron_cost * amount)
            {
                SEND("You need more iron to recruit this troop.\r\n",account);
                return;
            }



            account->cur_city->wood -= troop_table[i].wood_cost * amount;
            account->cur_city->stone -= troop_table[i].stone_cost * amount;
            account->cur_city->iron -= troop_table[i].iron_cost * amount;
            
            recruit = new_recruit(account->cur_city);
            recruit->time_left = 4 * troop_table[i].recruit_time * amount;
            recruit->amount = amount;
            recruit->type = troop_table[i].sub_type;
            sprintf(buf, "You start recruiting %d %s troop%s.\r\n", amount, troop_table[i].name, amount != 1 ? "s" : "");
            SEND(buf, account);
            return;
        }
    }

    SEND("Not a valid troop.\r\n",account);
    return;
}


CMD(do_settle)
{
    CITY_DATA * city;
    CITY_DATA * add_city;
    char arg1[MIL];
    char arg2[MIL];
    char buf[MSL];

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    city = account->cur_city;

    if (count_settler(city) < 1)
    {
        SEND("You need a settler to settle a new city.\r\n",account);
        return;
    }

    if (city->wood < 20000)
    {
        SEND("You need 20000 wood to start a new city.\r\n",account);
        return;
    }

    if (city->stone < 20000)
    {
        SEND("You need 20000 stone to start a new city.\r\n",account);
        return;
    }

    if (city->iron < 5000)
    {
        SEND("You need 5000 iron to start a new city.\r\n",account);
        return;
    }

    if (city->food < 5000)
    {
        SEND("You need 5000 food to start a new city.\r\n",account);
        return;
    }

    if (IS_NULLSTR(arg1))
    {
        SEND("Please enter an X coordinate to settle.\r\n",account);
        return;
    }

    if (IS_NULLSTR(arg2))
    {
        SEND("Please enter an Y coordinate to settle.\r\n",account);
        return;
    }

    if (!is_number(arg1) || !is_number(arg2))
    {
        SEND("Please enter an X and Y coordinate to settle. Must be a number.\r\n",account);
        return;
    }

    int x = atoi(arg1);
    int y = atoi(arg2);

    if (x < 0 || x > 999)
    {
        sprintf(buf, "X coord needs to be 0 to %d.\r\n", MAP_WIDTH);
        SEND(buf, account);
    }

    if (y < 0 || y > 999)
    {
        sprintf(buf, "Y coord needs to be 0 to %d.\r\n", MAP_HEIGHT);
        SEND(buf, account);
    }

    if (has_city(x,y))
    {
        SEND("There's already a city at those coordinates.\r\n",account);
        return;
    }

    city->wood -= 20000;
    city->stone -= 20000;
    city->iron -= 5000;
    city->food -= 5000;


    add_city = new_city();
    add_city->empire_name = str_dup(account->empire_name);
    add_city->x = x;
    add_city->name = str_dup("New City");
    add_city->y = y;

    add_city->wood = 1000;
    add_city->stone = 1000;
    add_city->iron = 500;
    add_city->food = 500;


    //Add townhall to city
    BUILDING_DATA * building;
    building = new_building();                    
    building->type = BLD_TOWNHALL;
    building->level = 1;
    building->city = str_dup(add_city->name);
    building->city_id = add_city->id;

    sprintf(buf, "You settle a new city! {c({xX: %d {c/{x Y: %d{c){x\r\n", x, y);
    SEND(buf, account);

    account->cur_city = add_city;
    account->city_id = add_city->id;
    
    TROOP_DATA * troop;
    TROOP_DATA * troop_next;    

    for (troop = troop_list; troop != NULL; troop = troop_next)
    {
        troop_next = troop->next;

        if (troop->city_id == city->id)
        {
            if (troop->type == TROOP_SETTLER)
            {
                extract_troop(troop);
                return;
            }
        }
    }

    return;
}


CMD(do_chat)
{
    DESCRIPTOR_DATA *d;
    char buf[MSL*3]; 
    char buf2[MSL];
     
    if (argument[0] == '\0' )
    {
          
        if (IS_SET(account->comm_flags,COMM_CHAT))
        {
            SEND("You turn off chat messages.\r\n",account);
            REMOVE_BIT(account->comm_flags,COMM_CHAT);
        }
        else
        {
            SEND("You turn on chat messages.\r\n",account);
            SET_BIT(account->comm_flags,COMM_CHAT);
        }
             
       return;
    }
          
    if ( !IS_SET(account->comm_flags,COMM_CHAT))    
        SET_BIT(account->comm_flags,COMM_CHAT);
    
    sprintf(buf2, "%s", timestamp(buf2));
    
    if (IS_SET (account->comm_flags, COMM_TIME_STAMPS))		
        sprintf (buf, "{D[{x%s{D]{x You {Uchat{x '%s{x'\r\n", buf2, argument);
    else
        sprintf (buf, "You {Uchat{x '%s{x'\r\n", argument);
    SEND (buf, account);
    
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        ACCOUNT_DATA *victim = NULL;        

        if ( d->connected == CON_PLAYING && d->account != account && IS_SET(victim->comm_flags, COMM_CHAT))
        {
            if (IS_SET (victim->comm_flags, COMM_TIME_STAMPS))	
            {				
                sprintf (buf, "{D[{x%s{D]{x %s {Uchats{x '%s{x'\r\n", buf2, account->name, argument);				
                SEND(buf, victim);
            }
            else
            {
                sprintf (buf, "%s {Uchats{x '%s{x'\r\n", account->name, argument);
                SEND(buf, victim);
            }            
        }
    }
    return;
}

CMD(do_switch)
{
    CITY_DATA * city;
    CITY_DATA * city_next;
    char buf[MSL];  

    if (IS_NULLSTR(argument))
    {
        SEND("You must specify which city you want to switch to.\r\n",account);
        return;
    }

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        if (!str_cmp(city->name, argument) && !str_cmp(city->empire_name, account->empire_name))
        {
            account->cur_city = city;

            sprintf(buf, "Switched to city: %s\r\n",city->name);
            SEND(buf, account);
            return;
        }
    }

    SEND("City not found in your empire.\r\n",account);
    return;
}

CMD(do_timestamps)
{
    if (IS_SET(account->comm_flags, COMM_TIME_STAMPS)) 
    {
        SEND("You turn {Roff{x time stamps.\r\n",account);
        REMOVE_BIT(account->comm_flags, COMM_TIME_STAMPS);
        return;
    }
    else
    {
        SEND("You turn {Gon{x time stamps.\r\n",account);
        SET_BIT(account->comm_flags, COMM_TIME_STAMPS);
    }

    return;
}


CMD(do_trade)
{
    TRADE_DATA * trade;
    CITY_DATA * city = account->cur_city;
    CITY_DATA * to_city;
    char arg1[MIL];
    char arg2[MIL];
    char arg3[MIL];
    char arg4[MIL];
    int amount = 0;
    int res = -1;
    int x = 0;
    int y = 0;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);
    argument = one_argument(argument, arg3);
    argument = one_argument(argument, arg4);
    

    if (!has_market(account->cur_city))
    {
        SEND("You must build a market here first!\r\n",account);
        return;
    }

    if (IS_NULLSTR(arg1) || IS_NULLSTR(arg2) || IS_NULLSTR(arg3))
    {
        SEND("Syntax: trade <amount> <resource> <city name>\r\n",account);
        return;
    }

    if (!is_number(arg1))
    {
        SEND("First argument must be the amount to send.\r\n",account);
        return;
    }

    amount = atoi(arg1);

    if (amount / 500 > count_carts(city))
    {
        SEND("You don't have enough carts to send that much.\r\n",account);
        return;
    }

    if (!str_cmp(arg2, "wood"))
    {
        if (city->wood < amount)
        {
            SEND("This city doesn't have that much wood.\r\n",account);
            return;
        }

        res = RES_WOOD;
        city->wood -= amount;
    }

    if (!str_cmp(arg2, "stone"))
    {
        if (city->stone < amount)
        {
            SEND("This city doesn't have that much stone.\r\n",account);
            return;
        }

        res = RES_STONE;
        city->stone -= amount;
    }

    if (!str_cmp(arg2, "iron"))
    {
        if (city->iron < amount)
        {
            SEND("This city doesn't have that much iron.\r\n",account);
            return;
        }

        res = RES_IRON;
        city->iron -= amount;
    }

    if (!str_cmp(arg2, "food"))
    {
        if (city->food < amount)
        {
            SEND("This city doesn't have that much food.\r\n",account);
            return;
        }

        res = RES_FOOD;
        city->food -= amount;
    }

    if (!is_number(arg3))
    {
        SEND("X coordinate must be a number.\r\n",account);
        return;
    }

    if (!is_number(arg4))
    {
        SEND("Y coordinate must be a number.\r\n",account);
        return;
    }

    x = atoi(arg3);
    y = atoi(arg4);

    if ((to_city = find_city_coords(x,y)) != NULL)
    {
        trade = new_trade(city);
        trade->amount = amount;
        trade->resource = res;
        trade->time_left = ((city->x - to_city->x * 60) * (city->y - to_city->y));
        trade->from_city_id = city->id;
        trade->to_city_id = to_city->id;


        SEND("Trade sent!\r\n",account);
        return;
    }

    SEND("City not found.\r\n",account);
    return;
}

CMD(do_who)
{
    DESCRIPTOR_DATA * d;
    char buf[MSL];

    SEND("Who's playing currently:\r\n",account);

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        ACCOUNT_DATA * waccount;
        
        if (d->connected != CON_PLAYING)
            continue;

        waccount = d->account;
    
        if (waccount)
        {
            sprintf(buf, "%s", waccount->name);
            SEND(buf, account);        
        }
    }

    SEND("\r\n", account);
    //do_count(account, "");
    SEND("\r\n", account);

    return;
}
