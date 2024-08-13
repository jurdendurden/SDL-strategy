#include "main.h"

void view_city(ACCOUNT_DATA * account, CITY_DATA * city)
{
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    RECRUIT_DATA * recruit;
    RECRUIT_DATA * recruit_next;
    TRADE_DATA * trade;
    TRADE_DATA * trade_next;
    char buf[MSL];
    int count = 0;

    if (!city)
        return;    

    sprintf(buf, "\r\n{CCity Name{x: %s {c({CX{x: %ld {CY{x: %ld{c){x\r\n\r\n", city->name, city->x, city->y);
    SEND(buf, account);

    SEND("{CIndex Type       Level Income     Upgrade Cost{x\r\n",account);
    SEND("                                  {uWood  {DStone {kIron  {UFood{x\r\n",account);

    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            sprintf(buf, "%-5d %-10s %-5d %-4d %-5s %-5d %-5d %-5d %-5d\r\n", 
                count,
                building_table[building->type].name, building->level,
                RESOURCE_BUILDING(building) ? (int)((building->level * 1.35) * 50) : 0, RESOURCE_STRING(building_table[building->type].resource),
                building_table[building->type].wood_cost[building->level],
                building_table[building->type].stone_cost[building->level],
                building_table[building->type].iron_cost[building->level],
                building_table[building->type].food_cost[building->level]);
            SEND(buf, account);
            count++;
        }
    }

    SEND("\r\n",account);
    
    count = 0;
    SEND("{CTrading{x:\r\n",account);
    for (trade = city->trading; trade != NULL; trade = trade_next)
    {
        trade_next = trade->next;

        sprintf(buf, "%d %s - time left: %ld\r\n", trade->amount, RESOURCE_STRING(trade->resource), trade->time_left );
        SEND(buf, account);
        count++;
    }

    if (count == 0)    
        SEND("No current trades found.\r\n",account);

    SEND("\r\n",account);
    
    count = 0;
    SEND("{CRecruiting{x:\r\n",account);
    for (recruit = city->recruiting; recruit != NULL; recruit = recruit_next)
    {
        recruit_next = recruit->next;

        sprintf(buf, "%d %s - time left: %ld\r\n", recruit->amount, troop_table[recruit->type].name, recruit->time_left);
        SEND(buf, account);
        count++;        
    }

    if (count == 0)    
        SEND("No current recruit orders found.\r\n",account);
    

    SEND("\r\n\r\n",account);

    sprintf(buf, "Buildings: {c[{x%d{c/{x%d{c]{x\r\n",count_buildings(city), max_buildings(city));
    SEND(buf, account);

    sprintf(buf, "Carts:     {c[{x%d{c/{x%d{c]{x\r\n", count_carts(city), max_carts(city));
    SEND(buf, account);

    sprintf(buf, "Troops:    {c[{x%ld{c/{x%ld{c]{x\r\n", count_troops(city), max_troops(city));
    SEND(buf, account);

    sprintf(buf, "Settler: %ld\r\n", count_settler(city));    
    SEND(buf, account);

    sprintf(buf, "Footmen: %ld\r\n", count_footman(city));    
    SEND(buf, account);

    sprintf(buf, "Archer:  %ld\r\n", count_archer(city));    
    SEND(buf, account);

    SEND("\r\n\r\n",account);
    SEND("\r\n{CResource Income{x:\r\n",account);
    sprintf(buf, "wood {c[{x%ld{c/{x%ld{c]{x {c({x%d{c/{xhr{c){x stone {c[{x%ld{c/{x%ld{c]{x {c({x%d{c/{xhr{c){x iron {c[{x%ld{c/{x%ld{c]{x {c({x%d{c/{xhr{c){x food {c[{x%ld{c/{x%ld{c]{x {c({x%d{c/{xhr{c){x\r\n\r\n",
        city->wood, max_wood(city), get_wood_income(city),
        city->stone, max_stone(city), get_stone_income(city),
        city->iron, max_iron(city), get_iron_income(city),
        city->food, max_food(city), get_food_income(city));
    SEND(buf, account);


    return;
}


void city_prompt(ACCOUNT_DATA * account)
{
    CITY_DATA * city;

    city = account->cur_city;

    if (!city)
        return;

    char buf[MSL];

    sprintf(buf, "%s\r\n\r\nResource Income:\r\nwood {c[{x%ld{c/{x%ld{c]{x {c({x%d{c/{xhr{c){x stone {c[{x%ld{c/{x%ld{c]{x {c({x%d{c/{xhr{c){x iron {c[{x%ld{c/{x%ld{c]{x {c({x%d{c/{xhr{c){x food {c[{x%ld{c/{x%ld{c]{x {c({x%d{c/{xhr{c){x\r\n\r\n", city->name, 
        city->wood, max_wood(city), get_wood_income(city),
        city->stone, max_stone(city), get_stone_income(city),
        city->iron, max_iron(city), get_iron_income(city),
        city->food, max_food(city), get_food_income(city));
    SEND(buf, account);

    return;
}

int get_wood_income(CITY_DATA * city)
{
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    int income = 0;

    income = 400; //base/hour from a townhall

    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_MILL)
            {
                income += (building->level * 1.35) * 50;
            }
        }
    }

    return income;
}

int get_stone_income(CITY_DATA * city)
{
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    int income = 0;

    income = 400; //base/hour from a townhall

    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_QUARRY)
            {
                income += (building->level * 1.35) * 50;
            }
        }
    }

    return income;
}

int get_iron_income(CITY_DATA * city)
{
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    int income = 0;

    income = 200; //base/hour from a townhall

    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_MINE)
            {
                income += (building->level * 1.35) * 50;
            }
        }
    }

    return income;
}

int get_food_income(CITY_DATA * city)
{
    int income = 0;
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;

    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_FARM)
            {
                income += (building->level * 1.35) * 50;
            }
        }
    }

    return income;
}

int get_gold_income(CITY_DATA * city)
{
    int income = 0;

    BUILDING_DATA * building;
    BUILDING_DATA * building_next;    

    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_MARKET)
            {
                income += (building->level * 1.35) * 50;
            }
        }
    }

    return income;
}

//how much wood can this city store?
long max_wood(CITY_DATA * city)
{
    int amt = 0;

    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    
    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_STORAGE)            
                amt += (building->level * 5000);

            if (building->type == BLD_TOWNHALL)
                amt += (building->level * 2000);
            
            if (building->type == BLD_KEEP)
                amt += (building->level * 3500);

            if (building->type == BLD_CASTLE)
                amt += (building->level * 6000);
        }
    }

    return amt;
}

//how much stone can this city store?
long max_stone(CITY_DATA * city)
{
    int amt = 0;

    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    
    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_STORAGE)            
                amt += (building->level * 5000);

            if (building->type == BLD_TOWNHALL)
                amt += (building->level * 2000);
            
            if (building->type == BLD_KEEP)
                amt += (building->level * 3500);

            if (building->type == BLD_CASTLE)
                amt += (building->level * 6000);
        }
    }

    return amt;
}

//how much iron can this city store?
long max_iron(CITY_DATA * city)
{
    int amt = 0;

    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    
    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_STORAGE)            
                amt += (building->level * 5000);

            if (building->type == BLD_TOWNHALL)
                amt += (building->level * 2000);
            
            if (building->type == BLD_KEEP)
                amt += (building->level * 3500);

            if (building->type == BLD_CASTLE)
                amt += (building->level * 6000);
        }
    }

    return amt;
}

//how much food can this city store?
long max_food(CITY_DATA * city)
{
    int amt = 0;

    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    
    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_STORAGE)            
                amt += (building->level * 5000);

            if (building->type == BLD_TOWNHALL)
                amt += (building->level * 2000);
            
            if (building->type == BLD_KEEP)
                amt += (building->level * 3500);

            if (building->type == BLD_CASTLE)
                amt += (building->level * 6000);
        }
    }

    return amt;
}

int count_carts(CITY_DATA * city)
{
    TRADE_DATA * trade;
    TRADE_DATA * trade_next;
    int amt = 0;

    amt = max_carts(city);

    for (trade = city->trading; trade != NULL; trade = trade_next)
    {
        trade_next = trade->next;

        amt -= trade->amount / 500;
    }

    return amt;
}

int max_carts(CITY_DATA * city)
{
    int amt = 0;

    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    
    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_MARKET)            
                amt += (building->level * 5);            
        }
    }

    return amt;
}

int count_buildings(CITY_DATA * city)
{
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    int count = 0;

    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            count++;
        }

    }

    return count;
}

int max_buildings(CITY_DATA * city)
{
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    int count = 0;

    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_TOWNHALL || building->type == BLD_KEEP || building->type == BLD_CASTLE)
            {
                count = building->level * 10;
            }            
        }

    }

    return count;
}

long count_troops(CITY_DATA * city)
{
    TROOP_DATA * troop;
    TROOP_DATA * troop_next;
    int count = 0;

    for (troop = troop_list; troop != NULL; troop = troop_next)
    {
        troop_next = troop->next;

        if (troop->city_id == city->id)
        {
            count++;
        }

    }

    return count;
}

long count_footman(CITY_DATA * city)
{
    TROOP_DATA * troop;
    TROOP_DATA * troop_next;
    int count = 0;

    for (troop = troop_list; troop != NULL; troop = troop_next)
    {
        troop_next = troop->next;

        if (troop->type == TROOP_FOOTMAN && troop->city_id == city->id)
        {
            count++;
        }

    }

    return count;
}

long count_archer(CITY_DATA * city)
{
    TROOP_DATA * troop;
    TROOP_DATA * troop_next;
    int count = 0;

    for (troop = troop_list; troop != NULL; troop = troop_next)
    {
        troop_next = troop->next;

        if (troop->type == TROOP_ARCHER && troop->city_id == city->id)
        {
            count++;
        }

    }

    return count;
}


long count_settler(CITY_DATA * city)
{
    TROOP_DATA * troop;
    TROOP_DATA * troop_next;
    int count = 0;

    for (troop = troop_list; troop != NULL; troop = troop_next)
    {
        troop_next = troop->next;

        if (troop->type == TROOP_SETTLER && troop->city_id == city->id)
        {
            count++;
        }

    }

    return count;
}

long max_troops(CITY_DATA * city)
{
    int amt = 0;

    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    
    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_BARRACKS)            
                amt += (building->level * 50);
            
            if (building->type == BLD_KEEP)            
                amt *= 1.5;

            if (building->type == BLD_CASTLE)            
                amt *= 2.5;
        }
    }
    
    return amt;
}

BUILDING_DATA * get_townhall(CITY_DATA * city)
{
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    
    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_TOWNHALL)            
                return building;
        }
    }

    return NULL;
}


bool has_barracks(CITY_DATA * city)
{
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    
    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_BARRACKS)            
                return TRUE;
        }
    }

    return FALSE;
}

bool has_range(CITY_DATA * city)
{
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    
    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_BARRACKS)            
                return TRUE;
        }
    }

    return FALSE;
}

bool has_market(CITY_DATA * city)
{
    BUILDING_DATA * building;
    BUILDING_DATA * building_next;
    
    for (building = building_list; building != NULL; building = building_next)
    {
        building_next = building->next;

        if (building->city_id == city->id)
        {
            if (building->type == BLD_MARKET)            
                return TRUE;
        }
    }

    return FALSE;
}

//Does a specific coordinates have a city on it?
bool has_city(int x, int y)
{
    CITY_DATA * city;   
    CITY_DATA * city_next;

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        if (city->x == x && city->y == y)
            return TRUE;
    }

    return FALSE;
}

//Lookup a city by id
CITY_DATA * find_city(int id)
{
    CITY_DATA * city;
    CITY_DATA * city_next;

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        if (city->id == id)
            return city;
    }
    
    
    return NULL;
}

CITY_DATA * find_city_coords(int x, int y)
{
    CITY_DATA * city;
    CITY_DATA * city_next;

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        if (city->x == x && city->y == y)
            return city;
    }

    return NULL;
}
