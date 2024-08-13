#include "main.h"

void create_world()
{
  
    ROOM_INDEX_DATA * room;    
    int vnum = 0;
    int iHash;
    int i = 0;
    int x = 0;
    int y = 0;
    int room_count = 0;

    long min_vnum;

    min_vnum = 100;


    for (x = 0; x < MAP_WIDTH; x++)
    {
        for (y = 0; y < MAP_HEIGHT; y++)
        {
            vnum = min_vnum + room_count;
            
            room = new_room_index();        
            room->vnum = vnum;
            room->name = str_dup("Tile Test");
            room->description = str_dup("");        

            room->x = x;
            room->y = y;

            i = number_range(1,4);
            switch(i)
            {
                default:
                case 1:
                    room->sector_type = SECT_FIELD;
                    break;
                case 2:
                    room->sector_type = SECT_MOUNTAIN;
                    break;
                case 3:
                    room->sector_type = SECT_HILLS;
                    break;
                case 4:
                    room->sector_type = SECT_DESERT;
                    break;
            }
            
            if (vnum > top_vnum_room)
                top_vnum_room = vnum;

            iHash = vnum % MAX_KEY_HASH;
            room->next = room_index_hash[iHash];
            room_index_hash[iHash] = room;			                    
            room_count++;
        }
    }
    return;
}

