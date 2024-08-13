#include "main.h"

//return total cities in an empire
int num_cities(EMPIRE_DATA * empire)
{
    int count = 0;
    CITY_DATA * city;
    CITY_DATA * city_next;

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        if (!str_cmp(city->empire_name, empire->name))
            count++;    
    }

    return count;

}


long total_wood_income(EMPIRE_DATA * empire)
{
    CITY_DATA * city;
    CITY_DATA * city_next;
    long income = 0;    

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        if (!str_cmp(city->empire_name, empire->name))
        {
            income += get_wood_income(city);
        }
    }

    return income;    
}

long total_stone_income(EMPIRE_DATA * empire)
{
    CITY_DATA * city;
    CITY_DATA * city_next;
    long income = 0;    

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        if (!str_cmp(city->empire_name, empire->name))
        {
            income += get_stone_income(city);
        }
    }

    return income;    
}

long total_iron_income(EMPIRE_DATA * empire)
{
    CITY_DATA * city;
    CITY_DATA * city_next;
    long income = 0;    

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        if (!str_cmp(city->empire_name, empire->name))
        {
            income += get_iron_income(city);
        }
    }

    return income;    
}

long total_food_income(EMPIRE_DATA * empire)
{
        CITY_DATA * city;
    CITY_DATA * city_next;
    long income = 0;    

    for (city = city_list; city != NULL; city = city_next)
    {
        city_next = city->next;

        if (!str_cmp(city->empire_name, empire->name))
        {
            income += get_food_income(city);
        }
    }

    return income;    
}

long total_troop_count(EMPIRE_DATA * empire)
{
    long count = 0;

    TROOP_DATA * troop;
    TROOP_DATA * troop_next;


    for (troop = troop_list; troop != NULL; troop = troop_next)
    {
        troop_next = troop->next;

        if (!str_cmp(troop->empire_name, empire->name))        
            count++;
    }        

    return count;
}

long total_troop_strength(EMPIRE_DATA * empire)
{
    long count = 0;

    return count;
}

long total_offensive_strength(EMPIRE_DATA * empire)
{
    long count = 0;

    return count;
}

long total_defensive_strength(EMPIRE_DATA * empire)
{
    long count = 0;

    return count;
}


void empire_prompt (ACCOUNT_DATA * account)
{    
    CITY_DATA * city;
    CITY_DATA * city_next;
    char buf[MSL];
    
    if (!account || !account->valid)
        return;
    
    for (city = city_list; city != NULL; city = city_next;)
    {
        city_next = city->next;

        if (!str_cmp(city->empire_name, account->empire_name))
        {
            sprintf(buf, "%s\r\n", city->name);
            SEND(buf, account);
        }
    }

    SEND("\r\n\r\n",account);

    sprintf(buf, "Empire: [%s]   Day: [%d]   Score: [%d]   Cities: [%d]   Troops: [%ld]\r\n", empire->name, 1, empire_score(empire), num_cities(empire), total_troop_count(empire));
    SEND(buf, account);
    
    return;
}

int empire_score(ACCOUNT_DATA * account)
{
    int count = 0;

    count += num_cities(empire) * 100;

    return count;
}
