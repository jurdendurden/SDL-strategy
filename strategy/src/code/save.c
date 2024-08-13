#include "main.h"
#include "recycle.h"

//Array of containers read for proper re-nesting of objects.
#define MAX_NEST    100
//static OBJ_DATA *rgObjNest[MAX_NEST];


void save_who_count(int count)
{
    char strsave[MSL];    
    FILE *fp;

    fclose( fpReserve );
    sprintf( strsave, "who_count.txt" );
    if ( ( fp = fopen( TEMP_FILE, "w" ) ) == NULL )
    {
        bug( "Who_count: fopen", 0 );
        perror( strsave );
    }
    else    
        fprintf( fp, "%d\n", count);
        
        
    fclose( fp );
    rename(TEMP_FILE,strsave);
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}


/*void save_game_data()
{
    char strsave[MSL];    
    FILE *fp;
    int i = 0;

    fclose( fpReserve );
    sprintf( strsave, "game_stats.txt" );
    if ( ( fp = fopen( TEMP_FILE, "w" ) ) == NULL )
    {
        bug( "Save_game_data: fopen", 0 );
        perror( strsave );
    }
    else
    {
        fprintf( fp, "%s~\n", game_data.mkiller );
        fprintf( fp, "%ld\n", game_data.mkills );	
        fprintf( fp, "%ld\n", game_data.explored);
        fprintf( fp, "%s~\n", game_data.explorer);
        fprintf( fp, "%d\n", game_data.most_hours);
        fprintf( fp, "%s~\n", game_data.most_hour);
        fprintf( fp, "%s~\n", game_data.pkiller);
        fprintf( fp, "%d\n", game_data.pkills);
        fprintf( fp, "%s~\n", game_data.pdeath);
        fprintf( fp, "%d\n", game_data.pdeaths);
        fprintf( fp, "%d\n", game_data.arena_wins);
        fprintf( fp, "%s~\n", game_data.arena_winner);
        fprintf( fp, "%d\n", game_data.arena_losses);
        fprintf( fp, "%s~\n", game_data.arena_loser);

        fprintf( fp, "%d\n", game_data.most_players );
        fprintf( fp, "%s~\n", game_data.most_qp);
        fprintf( fp, "%ld\n", game_data.qp);
        fprintf( fp, "%ld\n", game_data.ever_created);

        for (i = 0; i < MAX_FISH; i++)        
        {
            fprintf( fp, "%s~\n", game_data.fisherman[i]);
            fprintf( fp, "%d\n", game_data.fish_weight[i]);            
        }
        fprintf( fp, "%s~\n", game_data.most_fish);
        fprintf( fp, "%d\n", game_data.most_fished);
        fprintf( fp, "%d\n", game_data.ever_fished);

        fprintf( fp, "%d\n", game_data.smith_count);
        fprintf( fp, "%s~\n", game_data.blacksmith);

        fprintf( fp, "%d\n", game_data.tailor_count);
        fprintf( fp, "%s~\n", game_data.tailor);

        fprintf( fp, "%d\n", game_data.wood_count);
        fprintf( fp, "%s~\n", game_data.woodworker);

        fprintf( fp, "%d\n", game_data.jewel_count);   
        fprintf( fp, "%s~\n", game_data.jewelcrafter);

        fprintf( fp, "%d\n", game_data.alchemy_count);
        fprintf( fp, "%s~\n", game_data.alchemist);

        fprintf( fp, "%d\n", game_data.meal_count);
        fprintf( fp, "%s~\n", game_data.chef);

        fprintf( fp, "%d\n", game_data.brew_count);
        fprintf( fp, "%s~\n", game_data.brewer);

        fprintf( fp, "%d\n", game_data.scroll_count);
        fprintf( fp, "%s~\n", game_data.scribe);

        fprintf( fp, "%d\n", game_data.crafted_count);
        fprintf( fp, "%s~\n", game_data.craftsman);

    }    
    fclose( fp );
    rename(TEMP_FILE,strsave);
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}



void load_game_data( void )
{
    FILE *fp;    
    char buf[MSL];
    int i = 0;

    fclose( fpReserve );
       
    if ( ( fp = fopen( GAME_STATS_FILE, "r" ) ) != NULL )   
    {
        sprintf(buf,"Loading Game Data Structure.");
        log_string(buf);
        

        game_data.mkiller = fread_string(fp);
        game_data.mkills = fread_number(fp);
        game_data.explored = fread_number(fp);
        game_data.explorer = fread_string(fp);
        game_data.most_hours = fread_number(fp);
        game_data.most_hour = fread_string(fp);
        game_data.pkiller = fread_string(fp);
        game_data.pkills = fread_number(fp);
        game_data.pdeath = fread_string(fp);
        game_data.pdeaths = fread_number(fp);
        game_data.arena_wins = fread_number(fp);
        game_data.arena_winner = fread_string(fp);
        game_data.arena_losses = fread_number(fp);
        game_data.arena_loser = fread_string(fp);
        game_data.most_players = fread_number(fp);
        game_data.most_qp = fread_string(fp);
        game_data.qp = fread_number(fp);
        game_data.ever_created = fread_number(fp);

        for (i = 0; i < MAX_FISH; i++)        
        {
            game_data.fisherman[i] = fread_string(fp);
            game_data.fish_weight[i] = fread_number(fp);
        }

        game_data.most_fish = fread_string(fp);
        game_data.most_fished = fread_number(fp);
        game_data.ever_fished = fread_number(fp);
    
        game_data.smith_count = fread_number(fp);
        game_data.blacksmith = fread_string(fp);

        game_data.tailor_count  = fread_number(fp);
        game_data.tailor = fread_string(fp);

        game_data.wood_count = fread_number(fp);
        game_data.woodworker = fread_string(fp);

        game_data.jewel_count = fread_number(fp);   
        game_data.jewelcrafter = fread_string(fp);

        game_data.alchemy_count = fread_number(fp);
        game_data.alchemist = fread_string(fp);

        game_data.meal_count = fread_number(fp);
        game_data.chef = fread_string(fp);

        game_data.brew_count = fread_number(fp);
        game_data.brewer = fread_string(fp);

        game_data.scroll_count = fread_number(fp);
        game_data.scribe = fread_string(fp);

        game_data.crafted_count = fread_number(fp);
        game_data.craftsman = fread_string(fp);

        fclose( fp );
    }
    else    
        log_string("Unable to open game data file.");
    

    fpReserve = fopen( NULL_FILE, "r" );
    return;
}*/


char *print_flags (int flag)
{
    int count, pos = 0;
    static char buf[52];


    for (count = 0; count < 32; count++)
    {
        if (IS_SET (flag, 1 << count))
        {
            if (count < 26)
                buf[pos] = 'A' + count;
            else
                buf[pos] = 'a' + (count - 26);
            pos++;
        }
    }

    if (pos == 0)
    {
        buf[pos] = '0';
        pos++;
    }

    buf[pos] = '\0';

    return buf;
}

//New help file system - Upro Feb 2021

void save_help_file (HELP_DATA *help)
{
    char strsave[MIL];
    FILE *fp;

    if (!help)
    {
        bug("Func save_help_file: No help file to save.",0);
        return;
    }

    fclose (fpReserve);
    sprintf (strsave, "%s%s.help", HELP_DIR, capitalize (help->name));
    if ((fp = fopen (TEMP_FILE, "w")) == NULL)
    {
        bug ("Func save_help_file: fopen", 0);
        perror (strsave);
    }
    else
    {	
        fwrite_help (help, fp);		
        fprintf (fp, "#END\n");
    }
    fclose (fp);
    rename (TEMP_FILE, strsave);
    fpReserve = fopen (NULL_FILE, "r");
    return;
}


void fwrite_help (HELP_DATA *help, FILE * fp)
{    
    fprintf (fp, "#HELP\n");

    fprintf (fp, "NAME %s~\n", help->name);	
    fprintf (fp, "ID   %d\n", help->id);
    fprintf (fp, "KEYWORDS %s~\n", help->keyword);
    fprintf (fp, "VERSION %d\n", help->version);	
    fprintf (fp, "LEVEL %d\n", help->level);
    fprintf (fp, "LAST_EDIT_BY %s~\n", help->last_editted_by);
    fprintf (fp, "LAST_EDIT_DATE %ld\n", current_time);
    fprintf (fp, "TEXT %s~\n", help->text);
    
    fprintf (fp, "End\n\n");
    return;
}



bool load_help_file (char * name, HELP_DATA *help)
{
    char strsave[MIL];
    char bug_buffer[MSL];
    FILE *fp;
    bool found;    	
    

    help = 	new_help ();	

    found = FALSE;
    fclose (fpReserve);

    sprintf (strsave, "%s%s.help", HELP_DIR, capitalize (name));
    if ((fp = fopen (strsave, "r")) != NULL)
    {
        //int iNest;

        //for (iNest = 0; iNest < MAX_NEST; iNest++)
        //    rgObjNest[iNest] = NULL;

        found = TRUE;
        for (;;)
        {
            char letter;
            char *word = "";

            letter = fread_letter (fp);
            if (letter == '*')
            {
                fread_to_eol (fp);
                continue;
            }			
            if (letter != '#')
            {
                bug ("Func load_help_file: # not found.", 0);
                break;
            }
            if (!str_cmp (word, "PLAYER"))	
                fread_help (help, fp);
            else if (!str_cmp (word, "END"))
                break;
            else
            {
                sprintf(bug_buffer, "Func load_help_file: bad section. (%s)", word);
                bug (bug_buffer, 0);
                break;
            }
        }
        fclose (fp);
    }

    fpReserve = fopen (NULL_FILE, "r");

    return found;
}

void fread_help (HELP_DATA *help, FILE * fp)
{
    char buf[MSL];
    char *word;
    bool fMatch;

    sprintf (buf, "Loading %s.", help->name);
    log_string (buf);

    for (;;)
    {
        word = feof (fp) ? (char *)"End" : (char *)fread_word (fp);
        fMatch = FALSE;

        switch (UPPER (word[0]))
        {
            case '*':
                fMatch = TRUE;
                fread_to_eol (fp);
                break;
        
            case 'I':
                KEY ("ID", help->id, fread_number (fp));                
                break;
            
            case 'K':
                KEY ("KEYWORDS", help->keyword, fread_string(fp));
                break;

            case 'L':
                KEY ("LAST_EDIT_BY", help->last_editted_by, fread_string(fp));
                KEY ("LAST_EDIT_DATE", help->edit_date, fread_string(fp));
                KEY ("LEVEL", help->level, fread_number (fp));
                break;

        
                
            case 'N':
                KEY ("NAME", help->name, fread_string (fp));				
                break;
           

            case 'T':
                KEY ("TEXT", help->text, fread_string(fp));

                break;

            case 'V':
                KEY ("VERSION", help->version, fread_number (fp));              
                break;

        }

        if (!fMatch)
        {
            bug ("Func fread_help: no match.", 0);
            bug (word, 0);
            fread_to_eol (fp);
        }
    }
        
    
}

bool load_empire_obj(DESCRIPTOR_DATA * d, char * name)
{

    char strsave[MIL];
    char bug_buffer[MSL];
    //EMPIRE_DATA * emp;
    FILE *fp;
    bool found = FALSE;    

    //emp = new_empire();    

    fclose (fpReserve);

    sprintf (strsave, "%s%s", EMPIRE_DIR, capitalize (name));
    if ((fp = fopen (strsave, "r")) != NULL)
    {
        //int iNest;

        //for (iNest = 0; iNest < MAX_NEST; iNest++)
            //rgObjNest[iNest] = NULL;

        found = TRUE;
        for (;;)
        {
            char letter;
            char *word;

            letter = fread_letter (fp);
            if (letter == '*')
            {
                fread_to_eol (fp);
                continue;
            }

            if (letter != '#')
            {
                bug ("load_empire_obj: # not found.", 0);
                break;
            }

            word = fread_word (fp);
            /*if (!str_cmp (word, "PLAYER"))
                fread_char (ch, fp);
            else if (!str_cmp (word, "OBJECT"))            
                fread_obj (ch, fp, OBJ_NORMAL);
            else if (!str_cmp (word, "O"))
                fread_obj (ch, fp, OBJ_NORMAL);
            else if (!str_cmp (word, "VAULTOBJ"))
                fread_obj (ch, fp, OBJ_VAULT);
            else if (!str_cmp (word, "QUESTOBJ"))
                fread_obj (ch, fp, OBJ_QUEST);
            else if (!str_cmp (word, "PET"))
                fread_pet (ch, fp);
            else if (!str_cmp (word, "PETOBJ"))
                fread_charm_obj( ch, "pet", fp );
            else if (!str_cmp (word, "MOUNT"))
                fread_mount (ch, fp);
            else if (!str_cmp (word, "MOUNTOBJ"))
                fread_charm_obj( ch, "mount", fp );
            else if ( !str_cmp( word, "MATERIAL")) 
                fread_material( ch, fp ); 
            else if ( !str_cmp( word, "FACTIONS"))
                fread_factions( ch, fp);
            else if ( !str_cmp( word, "EXPERTISE"))
                fread_expertise( ch, fp);
            else if (!str_cmp (word, "QUEST"))
                fread_quest (ch, fp);	*/	
            if (!str_cmp (word, "END"))
                break;
            else
            {
                sprintf(bug_buffer, "load_empire_obj: bad section. (%s)", word);
                bug (bug_buffer, 0);
                break;
            }
        }
        fclose (fp);
    }

    fpReserve = fopen (NULL_FILE, "r");
    
    if (found)
    {
        //
    }

    return found;
}


//End strategy game

