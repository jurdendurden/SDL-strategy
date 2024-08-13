#include "main.h"
#include "parse.h"


/* wiznet table and prototype for future flag setting */
const struct wiznet_type wiznet_table[] = {
    {"on       ", WIZ_ON},
    {"prefix   ", WIZ_PREFIX},
    {"ticks    ", WIZ_TICKS},
    {"logins   ", WIZ_LOGINS},
    {"sites    ", WIZ_SITES},
    {"links    ", WIZ_LINKS},
    {"newbies  ", WIZ_NEWBIE},
    {"spam     ", WIZ_SPAM},
    {"deaths   ", WIZ_DEATHS},
    {"resets   ", WIZ_RESETS},
    {"mobdeaths", WIZ_MOBDEATHS},
    {"flags    ", WIZ_FLAGS},
    {"penalties", WIZ_PENALTIES},
    {"saccing  ", WIZ_SACCING},
    {"levels   ", WIZ_LEVELS},
    {"load     ", WIZ_LOAD},
    {"restore  ", WIZ_RESTORE},
    {"snoops   ", WIZ_SNOOPS},
    {"switches ", WIZ_SWITCHES},
    {"secure   ", WIZ_SECURE},
     {"memory   ", WIZ_MEMORY},
     {"bugs	   ", WIZ_BUGS},
    {NULL, 0}
};


const struct flag_type room_flags[] = {
     {"dark",            ROOM_DARK, TRUE},
     {"no_mob",          ROOM_NO_MOB, TRUE},
     {"indoors",         ROOM_INDOORS, TRUE},
     {"private",         ROOM_PRIVATE, TRUE},
     {"safe",            ROOM_SAFE, TRUE},
     {"solitary",        ROOM_SOLITARY, TRUE},
     {"mount_shop",      ROOM_MOUNT_SHOP, TRUE},
     {"pet_shop",        ROOM_PET_SHOP, TRUE},
     {"no_recall",       ROOM_NO_RECALL, TRUE},    
     {"gods_only",       ROOM_GODS_ONLY, TRUE},
     {"heroes_only",     ROOM_HEROES_ONLY, TRUE},
     {"newbies_only",    ROOM_NEWBIES_ONLY, TRUE},
     {"law",             ROOM_LAW, TRUE},
     {"nowhere",         ROOM_NOWHERE, TRUE},
     {"anti_magic",      ROOM_ANTI_MAGIC, TRUE},
     {"nosummon",        ROOM_NOSUMMON, TRUE},
     {"nofight",         ROOM_NOFIGHT, TRUE},	
     {"no_tree",         ROOM_NO_TREE, TRUE},
     {"devotion",        ROOM_DEVOTION, TRUE},
     {"no_gate",         ROOM_NO_GATE, TRUE},
     {"no_teleport",     ROOM_NO_TELEPORT, TRUE},
     {"no_scry",         ROOM_NO_SCRY, TRUE},  
     {"player_owned",    ROOM_PLAYER_OWNED, TRUE},
     {"claimable",       ROOM_CLAIMABLE, TRUE},
     {NULL, 0, 0}
};


const struct flag_type sector_flags[] = {
     {"mine",		     SECT_MINE,		TRUE},
     {"dig",			SECT_DIG,		TRUE},
     {"plant",		     SECT_PLANT,		TRUE},
     {"outside",		SECT_OUTSIDE,	TRUE},
     {"prospect",	     SECT_PROSPECT,	TRUE},
     {"fish",		     SECT_FISH,		TRUE},
     {"fresh_water",	SECT_FRESH_WATER,TRUE},
     {"salt_water",	     SECT_SALT_WATER,TRUE},
     {"tracks",	     SECT_TRACKS,TRUE},
     {NULL, 0, 0}
};

const struct sector_desc_info sector_description_table[] = {
     //{
     //   seasons descs, 
     //   time descs
     //}

     //inside
     {
          //seasons
          {"You are inside a room.","You are inside a room.","You are inside a room.","You are inside a room."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The moon's light casts shadows which bounce eerily off the window sill. ", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          } 
     }, 
     //city
     {
          //seasons
          {"You are in a bustling city.","You are in a bustling city.","You are in a bustling city.","You are in a bustling city."},
           //time
          {
               /*Midnight (12am- 4am)*/ "The moon's light casts both dim light and eerie shadows on the buildings nearby.", 
               /*Pre Dawn ( 4am- 6am)*/ "Peeking light from the sun ascending over the horizon begins to slowly flood the city.",
               /*EarlyMorn( 6am-11am)*/ "Early morning rays from the sun illuminate the shadowy corners of the city's alleys.",
               /*Mid Day  (11am- 4pm)*/ "The city before you is fully awash in the midday light of the sun.", 
               /*Dusk     ( 5pm- 8pm)*/ "Shadows flicker and play across the walls of buildings as the sun slowly descends into slumber.",
               /*Night    ( 8pm-12am)*/ "The moon's light casts both dim light and eerie shadows on the buildings nearby."
          } 
     },
     //field
     {
          {"A field spreads out before you.","A field spreads out before you.","A field spreads out before you.","A field spreads out before you."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The moonlight dimly illuminates the grassy fields before you.", 
               /*Pre Dawn ( 4am- 6am)*/ "The moonlight is beginning to fade as it chases the moon behind the horizon of a new day.",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "The moonlight dimly illuminates the grassy fields before you."
          } 
     },
     //forest
     {
          {"Budding branches and leaves above indicate the start of spring in the forest.","A thick canopy of trees overhead denotes your location in a forest.","A beautiful silhouette of autumnal colors plays out in the tree tops before you. ","The leafless trees of the forest around you stand ready for a change of season."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The moonlight trickles down softly through the surrounding canopy of trees.", 
               /*Pre Dawn ( 4am- 6am)*/ "The receding moonlight leaning across the surrounding trees alludes to the approaching dawn.",
               /*EarlyMorn( 6am-11am)*/ "A crowning sun unleashes the first rays of sunlight across the forest floor.",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          } 
     },
     //hills
     {
          {"Rolling hills stretch lazily before you.","Rolling hills stretch lazily before you.","Rolling hills stretch lazily before you.","Rolling hills stretch lazily before you."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The hills nearby glow with the soft pale light of the moon.", 
               /*Pre Dawn ( 4am- 6am)*/ "The hills nearby glow with the soft pale light of the moon.",
               /*EarlyMorn( 6am-11am)*/ "The early morning rays of sunshine pour across the rolling hills.",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          } 
     },
     //mountains
     {
          {"A majestic mountain range looms all around you.","A majestic mountain range looms all around you.","A majestic mountain range looms all around you.","A majestic mountain range looms all around you."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The mountains nearby seem to glow white in the moon's light.", 
               /*Pre Dawn ( 4am- 6am)*/ "The mountains nearby seem to glow white in the moon's light.",
               /*EarlyMorn( 6am-11am)*/ "The rising sun blankets the mountain in warm morning light.",
               /*Mid Day  (11am- 4pm)*/ "The midday sun shines brightly, reflecting from deep crevices in the mountain.", 
               /*Dusk     ( 5pm- 8pm)*/ "Light begins to fade as the sun makes its way across the sky.",
               /*Night    ( 8pm-12am)*/ ""
          } 
     },
     //lake
     {
          {"You are swimming in the cool water.","You are swimming in the warm water.","You are swimming in the cool water.","You are swimming in the cold water."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The pale water reflects the moon's pale light.", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          } 
     },
     //ocean
     {
          {"You are swimming in the cool water.","You are swimming in the warm water.","You are swimming in the cool water.","You are swimming in the cold water."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The dark water reflects the moon's pale light.", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "The dark water reflects the moon's pale light."
          } 
     },
     //unused
     {
          {"THIS IS UNUSED FOR SOME UNUSUAL REASON.","THIS IS UNUSED FOR SOME UNUSUAL REASON.","THIS IS UNUSED FOR SOME UNUSUAL REASON.","THIS IS UNUSED FOR SOME UNUSUAL REASON."},
          //time
          {
               /*Midnight (12am- 4am)*/ "UNUSED", 
               /*Pre Dawn ( 4am- 6am)*/ "UNUSED",
               /*EarlyMorn( 6am-11am)*/ "UNUSED",
               /*Mid Day  (11am- 4pm)*/ "UNUSED", 
               /*Dusk     ( 5pm- 8pm)*/ "UNUSED",
               /*Night    ( 8pm-12am)*/ "UNUSED"
          } 
     },
     //air
     {
          {"In the cool air.", "In the warm air.", "In the cool air.", "In the frigid air."},
          //time
          {
               /*Midnight (12am- 4am)*/ "Soft rays of the moon's pale light illuminate the ground below.", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "Soft rays of the moon's pale light illuminate the ground below."
          } 
     },
     //desert
     {
          {"Dunes of varying sizes present themselves before you.","Dunes of varying sizes present themselves before you.","Dunes of varying sizes present themselves before you.","Dunes of varying sizes present themselves before you."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The moon casts an eerie shadow across the lonely desert.", 
               /*Pre Dawn ( 4am- 6am)*/ "Early morning sunlight showers the surface of the desert sand in brilliant orange.",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "The moon casts an eerie shadow across the lonely desert."
          } 
     },
     //road
     {
          {"You are on a road.","You are on a road.","You are on a road.","You are on a road."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The road is coated with rays of moonlight.", 
               /*Pre Dawn ( 4am- 6am)*/ "The shadows begin to recede as the sun makes it's daily appearance above the horizon.",
               /*EarlyMorn( 6am-11am)*/ "The road before you is illuminated by the bright morning sun.",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "Long shadows trickle across the road as the sun dips slowly into the horizon.",
               /*Night    ( 8pm-12am)*/ "The road is coated with rays of moonlight."
          } 
     },
     //beach
     {
          {"Beautiful, fine sand stretches before your vision.","Beautiful, fine sand stretches before your vision.","Beautiful, fine sand stretches before your vision.","Beautiful, fine sand stretches before your vision."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The sand before you appears a pale white in the soft moonlight.", 
               /*Pre Dawn ( 4am- 6am)*/ "Patches of golden sunlight begin to make their way onto the sand of the beach.",
               /*EarlyMorn( 6am-11am)*/ "Streaking rays of the morning sun's light stretch lazily along the beach.",
               /*Mid Day  (11am- 4pm)*/ "The midday sun beams down onto the beach, warming the sand.", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "The sand before you appears a pale white in the soft moonlight."
          } 
     },
     //shoreline
     {
          {"You are near the shore. A vast body of water stretches before your eyes.","You are near the shore. A vast body of water stretches before your eyes.","You are near the shore. A vast body of water stretches before your eyes.","You are near the shore. A vast body of water stretches before your eyes."},
          //time
          {
               /*Midnight (12am- 4am)*/ "Waves crash upon the shoreline, the moon's reflection keen in the wake.", 
               /*Pre Dawn ( 4am- 6am)*/ "Patches of golden sunlight begin to make their way onto the shoreline.",
               /*EarlyMorn( 6am-11am)*/ "Streaking rays of the morning sun's light stretch lazily across the shoreline.",
               /*Mid Day  (11am- 4pm)*/ "The midday sun beams down onto the shoreline, warming the sand.", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "Waves crash upon the shoreline, the moon's reflection keen in the wake."
          } 
     },
     //cave
     {
          {"You are somewhere in a cave.","You are somewhere in a cave.","You are somewhere in a cave.","You are somewhere in a cave."},
          //time
          {
               /*Midnight (12am- 4am)*/ "Darkness threatens to swallow the cave whole.", 
               /*Pre Dawn ( 4am- 6am)*/ "The outlines of the cave walls seem to dance as shadows play across their surface.",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "The outlines of the cave walls seem to dance as shadows play across their surface.",
               /*Night    ( 8pm-12am)*/ "Darkness threatens to swallow the cave whole."
          } 
     },
     //oceanfloor
     {
          {"You are on the ocean floor.","You are on the ocean floor.","You are on the ocean floor.","You are on the ocean floor."},
          //time
          {
               /*Midnight (12am- 4am)*/ "Absolute darkness threatens to consume the water around you.", 
               /*Pre Dawn ( 4am- 6am)*/ "Absolute darkness threatens to consume the water around you.",
               /*EarlyMorn( 6am-11am)*/ "Absolute darkness threatens to consume the water around you.",
               /*Mid Day  (11am- 4pm)*/ "Absolute darkness threatens to consume the water around you.", 
               /*Dusk     ( 5pm- 8pm)*/ "Absolute darkness threatens to consume the water around you.",
               /*Night    ( 8pm-12am)*/ "Absolute darkness threatens to consume the water around you."
          } 
     },
     //lava
     {
          {"Molten lava covers the ground here.","Molten lava covers the ground here.","Molten lava covers the ground here.","Molten lava covers the ground here."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The moon's light is washed out by the glowing molten lava surrounding you.", 
               /*Pre Dawn ( 4am- 6am)*/ "Thick bubbles of magma sprout and suddenly burst, flicking white hot lava about.",
               /*EarlyMorn( 6am-11am)*/ "Thick bubbles of magma sprout and suddenly burst, flicking white hot lava about.",
               /*Mid Day  (11am- 4pm)*/ "Thick bubbles of magma sprout and suddenly burst, flicking white hot lava about.", 
               /*Dusk     ( 5pm- 8pm)*/ "Thick bubbles of magma sprout and suddenly burst, flicking white hot lava about.",
               /*Night    ( 8pm-12am)*/ "The moon's light is washed out by the glowing molten lava surrounding you."
          } 
     },
     //swamp
     {
          {"The depressing scenery of a swamp unfolds before you.","The depressing scenery of a swamp unfolds before you.","The depressing scenery of a swamp unfolds before you.","The depressing scenery of a swamp unfolds before you."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The pale moonlight plays off the murky swamp water casting light across the reeds and grass.", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "The pale moonlight plays off the murky swamp water casting light across the reeds and grass."
          } 
     },
     //underground
     {
          {"You are somewhere underground.","You are somewhere underground.","You are somewhere underground.","You are somewhere underground."},
          //time
          {
               /*Midnight (12am- 4am)*/ "", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          } 
     },
     //ice
     {
          {"A thick sheet of ice covers the ground in your field of vision.","A thick sheet of ice covers the ground in your field of vision.","A thick sheet of ice covers the ground in your field of vision.","A thick sheet of ice covers the ground in your field of vision."},
          //time
          {
               /*Midnight (12am- 4am)*/ "", 
               /*Pre Dawn ( 4am- 6am)*/ "Pre dawn rays of orange sunlight glimmer across the ice before you.",
               /*EarlyMorn( 6am-11am)*/ "The ice around you glistens in the morning light.",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          } 
     },
     //snow
     {
          {"A blanket of snow covers the ground in your field of vision.","A blanket of snow covers the ground in your field of vision.","A blanket of snow covers the ground in your field of vision.","A blanket of snow covers the ground in your field of vision."},
          //time
          {
               /*Midnight (12am- 4am)*/ "A sea of glimmering snow surrounds you under the moon's gentle light.", 
               /*Pre Dawn ( 4am- 6am)*/ "Pre dawn rays of orange sunlight glimmer across the snow before you.",
               /*EarlyMorn( 6am-11am)*/ "The snow around you glistens in the morning light.",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "A sea of glimmering snow surrounds you under the moon's gentle light."
          } 
     },
     //void
     {
          {"You are in the void.","You are in the void.","You are in the void.","You are in the void."},
          //time
          {
               /*Midnight (12am- 4am)*/ "", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          } 
     },
     //ruins
     {
          {"Decrepit ruins dot the landscape about you.","Decrepit ruins dot the landscape about you.","Decrepit ruins dot the landscape about you.","Decrepit ruins dot the landscape about you."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The moon casts shadows across a landscape of crumbling antiquities and rubble.", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "The moon casts shadows across a landscape of crumbling antiquities and rubble."
          } 
     },
     //wasteland
     {
          {"The scene of a barren wasteland presents itself before you.","The scene of a barren wasteland presents itself before you.","The scene of a barren wasteland presents itself before you.","The scene of a barren wasteland presents itself before you."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The moon's pale rays illuminate a morose scene of desolation before you.", 
               /*Pre Dawn ( 4am- 6am)*/ "The retreating moons casts long shadows across the landscape, shrouding the barren landscape from view.",
               /*EarlyMorn( 6am-11am)*/ "The sun's morning rise gives way to the grim and desolate landscape before you.",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "The moon's pale rays illuminate a morose scene of desolation before you."
          } 
     },
     //jungle
     {
          {"Thick shrubbery and vines obscure vision as untold things move around in the jungle.","Thick shrubbery and vines obscure vision as untold things move around in the jungle.","Thick shrubbery and vines obscure vision as untold things move around in the jungle.","Thick shrubbery and vines obscure vision as untold things move around in the jungle."},
          //time
          {
               /*Midnight (12am- 4am)*/ "Eerie pale light softly illuminates the surrounding jungle undergrowth.", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "The humidity seems to thicken as the sun rises into the sky.",
               /*Mid Day  (11am- 4pm)*/ "The humidity of the jungle before you makes the air feel sticky as you move about.", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "Eerie pale light softly illuminates the surrounding jungle undergrowth."
          } 
     },
     //dock
     {
          {"You are on a dock.","You are on a dock.","You are on a dock.","You are on a dock."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The moon casts a dim glow upon the docks before you.", 
               /*Pre Dawn ( 4am- 6am)*/ "Looming shadows stretch across the docks as the sun rises.",
               /*EarlyMorn( 6am-11am)*/ "The morning sun plays across the dock, lighting the way.",
               /*Mid Day  (11am- 4pm)*/ "The midday sun casts long shadows across the beaten and worn planks of the docks.", 
               /*Dusk     ( 5pm- 8pm)*/ "Light begins to fade somewhat as the sun makes it's way across the evening sky.",
               /*Night    ( 8pm-12am)*/ "The moon casts a dim glow upon the docks before you."
          } 
     },
     //ship
     {
          {"You are on a ship.","You are on a ship.","You are on a ship.","You are on a ship."},
          //time
          {
               /*Midnight (12am- 4am)*/ "The moon is high above.", 
               /*Pre Dawn ( 4am- 6am)*/ "The moon is retreating for the morning sun.",
               /*EarlyMorn( 6am-11am)*/ "The morning sun is rising ever higher into the sky.",
               /*Mid Day  (11am- 4pm)*/ "The midday sun is directly above you.", 
               /*Dusk     ( 5pm- 8pm)*/ "The sun is setting into it's bed under the horizon.",
               /*Night    ( 8pm-12am)*/ "The moon is high above."
          } 
     },
     //river
     {
          {"You are swimming in the cool water.","You are swimming in the warm water.","You are swimming in the cool water.","You are swimming in the cold water."},
          //time
          {
               /*Midnight (12am- 4am)*/ "Beams of the moon's light reflect off the imperfections in the water.", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          } 
     },
     //blood river
     {
          {"You are swimming in blood.","You are swimming in blood.","You are swimming in blood.","You are swimming in blood."},
          //time
          {
               /*Midnight (12am- 4am)*/ "", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          }  
     },
     //sunken cave
     {
          {"You are somewhere in a cave.","You are somewhere in a cave.","You are somewhere in a cave.","You are somewhere in a cave."},
          //time
          {
               /*Midnight (12am- 4am)*/ "", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          } 
     },
     //fortress
     {
          {"You stand within a walled fortress.","You stand within a walled fortress.","You stand within a walled fortress.","You stand within a walled fortress."},
          //time
          {
               /*Midnight (12am- 4am)*/ "Beams of the moon's light play off the intricately carved stones of the fortess walls.", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ "Beams of the moon's light play off the intricately carved stones of the fortess walls."
          } 
     },
     //mushroom forest
     {
          {"Medium mushrooms dot the wooded landscape.","Large mushrooms dot the wooded landscape.","Small mushrooms dot the wooded landscape.","Small mushrooms dot the wooded landscape."},
          //time
          {
               /*Midnight (12am- 4am)*/ "", 
               /*Pre Dawn ( 4am- 6am)*/ "",
               /*EarlyMorn( 6am-11am)*/ "",
               /*Mid Day  (11am- 4pm)*/ "", 
               /*Dusk     ( 5pm- 8pm)*/ "",
               /*Night    ( 8pm-12am)*/ ""
          } 
     },
     {
          {NULL, NULL, NULL, NULL},
          {NULL, NULL, NULL, NULL, NULL, NULL}
     }

};

const struct sector_info sector_table[] = {
     
// Sector Booleans
/*
#define SECT_MINE		 //can mine here.
#define SECT_DIG		 //can dig here.
#define SECT_PLANT		 //plants can grow here
#define SECT_OUTSIDE	 //can you see the weather/time change?
#define SECT_PROSPECT	 //can you prospect here?
#define SECT_FISH		 //can you fish here?
#define SECT_FRESH_WATER  //is this fresh water? (ie river/lake)
#define SECT_SALT_WATER   //is this salt water? (ie sea/ocean)
#define SECT_TRACKS       //can players leave tracks when passing through?
*/

     //name,			non wall,	     wall, 		sector type (merc.h), 	move cost,	def heal?	def mana?	          mine	     dig 	     plant	outside	prospect  fish	     salt	     fresh  tracks
    {"inside",			"{x.",		"{8 {1", 		SECT_INSIDE,		     1, 	          30,			20,            FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
    {"city", 			"{D.",		"{8 {1",		SECT_CITY,			1, 			30,			20,			FALSE,	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
    {"field", 			"{Gv",		"{3 {1",		SECT_FIELD,			1, 			30,			20,			FALSE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, TRUE},
    {"forest", 		"{G.",		"{g@{1",	     SECT_FOREST,   		4, 			30,			20,		     FALSE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, TRUE},
    {"hills", 			"{Gn",		"{3 {1",		SECT_HILLS,			5, 			30,			20,			TRUE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, TRUE},
    {"mountain",		"{D^",		"{8 {1",		SECT_MOUNTAIN,		     6, 			20,			15, 		     TRUE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, TRUE},
    {"lake",			"{b~",		"{B~{1",		SECT_LAKE,		     10, 			10,			10,		     FALSE,	FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	TRUE,	FALSE, FALSE},
    {"ocean",			"{c~",		"{C~{1",		SECT_OCEAN,		     40, 			10,			10,		     FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	TRUE,	FALSE,	TRUE, FALSE},
    {"unused", 		"{D.",		"{8 {1",	     SECT_UNUSED,		     0, 			0,			0,			FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"air", 			"{x ",		"{x {1",		SECT_AIR, 		     1,			0,			0,		     FALSE,	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
    {"desert", 		"{Y.",		"{4 {1",	     SECT_DESERT,		     5, 			5,			5,			FALSE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, TRUE},
     {"road", 			"{y+",		"{8 {1",		SECT_ROAD, 			1,			10,			10,			FALSE,	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"beach", 		"{Y+",		"{4 {1",	     SECT_BEACH, 		     2,		 	10,			10,			FALSE,	TRUE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE, TRUE},
     {"shoreline",		"{y.",		"{4 {1", 		SECT_SHORELINE,		2, 			10,			10,			FALSE,	TRUE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE, TRUE},
     {"cave", 			"{D.",		"{8 {1",		SECT_CAVE, 			2,			20,			20,			TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"oceanfloor", 	"{b~",		"{b~{1",	     SECT_OCEANFLOOR,	     10, 		     0,			0,			FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	TRUE,	FALSE, FALSE},
     {"lava", 			"{R~",		"{2 {1",		SECT_LAVA, 			12,			0,			0,			FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"swamp", 		"{D~",		"{7 {1",		SECT_SWAMP, 		     4,			10,			10,			FALSE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"underground", 	"{D.",		"{8 {1",		SECT_UNDERGROUND,	     2, 			20,			20,			TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"ice", 			"{W_",		"{W#{1",		SECT_ICE, 		     5,			5,			5,		     FALSE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"snow", 			"{W.",		"{W#{1",		SECT_SNOW, 		     8,			10,			5,		     FALSE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, TRUE},
     {"void", 			"{x ",		"{x {1",		SECT_VOID, 		     1,			10,			40,		     FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"ruins", 		"{Dx",		"{8 {1",		SECT_RUINS, 		     3,			10,			10,			FALSE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"wasteland", 		"{xx",		"{4 {1",		SECT_WASTELAND,		2,	 		10,			10,			FALSE,	TRUE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, TRUE},
     {"jungle", 		"{Dv",		"{g@{1",		SECT_JUNGLE, 	          5,			20,			20,		     FALSE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, TRUE},
     {"dock", 			"{y#",		"{y#{1",		SECT_DOCK, 		     1,			0,			0,		     FALSE,	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"ship", 			"{y#",		"{x#{1",		SECT_SHIP, 		     1,			0,			0,		     FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"river",			"{B+",		"{5 {1",		SECT_RIVER,			6,			0,			0,			FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	TRUE,	TRUE,	FALSE, FALSE},
     {"blood river",	"{r+",		"{2 {1",		SECT_BLOOD_RIVER,	     6,			0,			0,			FALSE,	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE, FALSE},
     {"sunken cave",	"{c+",		"{5 {1",		SECT_SUNKEN_CAVE,	     6,			0,			0,			FALSE,	FALSE,	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	FALSE, FALSE},
     {"fortress",		"{D.",		"{8 {1",		SECT_FORTRESS,		     1,			0,			0,			FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE,	FALSE, FALSE},
     {"mushroom forest",	"{W.",		"{m@{1",		SECT_MUSH_FOREST,	     4, 			30,			20,			FALSE,	TRUE,	TRUE,	TRUE,	FALSE,	FALSE,	FALSE,	FALSE, TRUE},
     {NULL, NULL, NULL, 0,0,0,0,  0,0,0,0,0,0,0,0,0}
};

const struct troop_info troop_table[] = {
     /*char *          name;
     int             type;
     int             sub_type;
     int             hp;
     int             speed;
     int             wood_cost;
     int             stone_cost;
     int             iron_cost;
     int             upkeep;     //how much food every hour do they need
     int             req_building;*/
     
     {
          "settler",    INFANTRY,      TROOP_SETTLER,      
          12,       15, 
          100,   100,   100,      1,
          BLD_TOWNHALL, 10  
     },

     {
          "footman",    INFANTRY,      TROOP_FOOTMAN,      
          18,       12, 
          0,   0,   100,      1,
          BLD_BARRACKS, 60  
     },

     {
          "archer",    INFANTRY,      TROOP_ARCHER,      
          14,       15, 
          100,   0,   0,      1,
          BLD_RANGE, 60
     },

     {NULL, -1, -1, -1, -1, -1, -1, -1, -1}
};

const struct building_info building_table[] = {

     /*char *          name;
     int             type;
     int                 resource;
     int             wood_cost[10];
     int             stone_cost[10];
     int             iron_cost[10];
     int             food_cost[10];
     int             required_building;      //what building if any is required in city before you can construct this one.*/

     {"townhall", BLD_TOWNHALL, -1,
          {0, 250, 500, 1500, 5000, 10000, 20000, 35000, 50000, 100000},
          {0, 250, 500, 1500, 5000, 10000, 20000, 35000, 50000, 100000},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          -1
     },

     {"keep", BLD_KEEP, -1, 
          {0, 250, 500, 1500, 5000, 10000, 20000, 35000, 50000, 100000},
          {0, 250, 500, 1500, 5000, 10000, 20000, 35000, 50000, 100000},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          BLD_TOWNHALL
     },

     {"castle", BLD_CASTLE, -1, 
          {0, 250, 500, 1500, 5000, 10000, 20000, 35000, 50000, 100000},
          {0, 250, 500, 1500, 5000, 10000, 20000, 35000, 50000, 100000},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          BLD_KEEP
     },

     {"mill", BLD_MILL, RES_WOOD,
          {100, 200, 400, 800, 1200, 2000, 3500, 5000, 10000, 20000},
          {50,  100, 150, 250, 400,  800,  1200, 2000, 3000,  5000},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          -1
     },

     {"quarry", BLD_QUARRY, RES_STONE,
          {100, 200, 400, 800, 1200, 2000, 3500, 5000, 10000, 20000},
          {50,  100, 150, 250, 400,  800,  1200, 2000, 3000,  5000},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          -1
     },

     {"mine", BLD_MINE, RES_IRON,
          {100, 200, 400, 800, 1200, 2000, 3500, 5000, 10000, 20000},
          {50,  100, 150, 250, 400,  800,  1200, 2000, 3000,  5000},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          -1
     },

     {"farm", BLD_FARM, RES_FOOD,
          {100, 200, 400, 800, 1200, 2000, 3500, 5000, 10000, 20000},
          {50,  100, 150, 250, 400,  800,  1200, 2000, 3000,  5000},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          -1
     },

     {"market", BLD_MARKET, RES_FOOD,
          {100, 200, 400, 800, 1200, 2000, 3500, 5000, 10000, 20000},
          {50,  100, 150, 250, 400,  800,  1200, 2000, 3000,  5000},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          -1
     },

     {"barracks", BLD_BARRACKS, -1,
          {0,   50,  150, 300,  500,  1000, 1500, 2500, 5000,  10000},
          {120, 300, 700, 1200, 2000, 3500, 5000, 8000, 15000, 25000},          
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          -1
     },

     {"range", BLD_RANGE, -1,
          {120, 300, 700, 1200, 2000, 3500, 5000, 8000, 15000, 25000},
          {0,   50,  150, 300,  500,  1000, 1500, 2500, 5000,  10000},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          -1
     },

     {"storage", BLD_STORAGE, -1,
          {50, 150, 350, 700, 1000, 1500, 2500, 5000, 7500, 10000},
          {50, 150, 350, 700, 1000, 1500, 2500, 5000, 7500, 10000},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          -1
     },

     {"hideout", BLD_HIDEOUT, -1,
          {30, 60, 100, 250, 500, 1000, 2000, 3500, 5000, 7500},
          {30, 60, 100, 250, 500, 1000, 2000, 3500, 5000, 7500},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          -1
     },

     {NULL, 0, -1,
     {0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0},
     {0,0,0,0,0,0,0,0,0,0},
     -1}
};


