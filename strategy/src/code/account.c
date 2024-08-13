//	This file contains all the menus for the account system.
//	They will actually be called from the nanny function.
//	Most of the account information/defines have been moved
//	from main.h in an attempt to encapsulate all the account
//	data.


#include "main.h"
#include "recycle.h"



int clan_lookup     args( (const char *name) );
bool reset_account	( ACCOUNT_DATA *acc );
bool find_account_online(DESCRIPTOR_DATA * d, char *name);


void intro_menu(DESCRIPTOR_DATA *d)
{
    STD("\r\n",d); 
    STD("(C)reate an account\r\n",d);
    STD("(L)og into existing account\r\n",d);
    STD("(E)xit\r\n",d);    
    STD(">  ",d);
}

void account_help(DESCRIPTOR_DATA *d)
{
    STD("\r\n\r\n",d);
    STD("To play the game you will need an account. With an account, you'll be\r\nable to save many games so that you don't feel the need to have to finish\r\n one before starting a new one.\r\n",d);
}

void create_account_menu(DESCRIPTOR_DATA *d)
{
    STD("Pick an appropriate account name and strong password that you'll remember.\r\n",d);
    STD("All your games will be saved under this password.\r\n",d);
}

void account_main_menu(DESCRIPTOR_DATA *d)
{
    STD("(P)lay game\r\n",d);    
    STD("(A)ccount info\r\n",d);
    STD("(C)hange password\r\n",d);
    STD("(D)elete account (irreversible)\r\n",d);
    STD("(Q)uit\r\n",d);
    STD(">  ",d);    
}

void acc_info ( ACCOUNT_DATA * account, DESCRIPTOR_DATA *d)
{
    char buf[MSL];    
    
    sprintf(buf, "Account Name:  %s\r\n", account->name);
    STD(buf, d);
    
    sprintf(buf, "Account Hours: %ld\r\n", (account->hours / 3600));	
    STD(buf, d);    

    sprintf(buf, "Login:         %s\r\n", ctime(&account->logon));
    STD(buf, d);

            
    sprintf(buf, "\r\nAccount Flags: ");    

    if (IS_SET(account->act, ACC_CLANMASTER))
        strcat(buf, "clan_master ");

    if (IS_SET(account->act, ACC_DENY))
        strcat(buf, "denied ");

    if (IS_SET(account->act, ACC_EXTENDED_VAULT))
        strcat(buf, "extended_vault ");

    if (IS_SET(account->act, ACC_HOSTMASK))
        strcat(buf, "hostmask ");
    
    if (IS_SET(account->act, ACC_IDLER))
        strcat(buf, "idler ");

    if (IS_SET(account->act, ACC_IMM))
        strcat(buf, "immortal ");

    if (IS_SET(account->act, ACC_IMP))
        strcat(buf, "implementor ");    

    if (IS_SET(account->act, ACC_NEWBIE))
        strcat(buf, "newbie ");
    
    if (IS_SET(account->act, ACC_NOCOLOR))
        strcat(buf, "no_color ");

    if (IS_SET(account->act, ACC_PERMIT))
        strcat(buf, "permit ");

    if (IS_SET(account->act, ACC_VISION_IMPAIRED))
        strcat(buf, "vision_impaired ");

    strcat(buf, "\r\n");
    STD(buf, d);

    return;
}

bool reset_account( ACCOUNT_DATA *acc )
{
    if (!acc)
        return FALSE;

    free_string(acc->name);
    acc->name = str_dup("");

    free_string(acc->screen_name);
    acc->screen_name = str_dup("");

    free_string(acc->password);
    acc->password = str_dup("");

    free_string(acc->old_passwd);
    acc->old_passwd = str_dup("");

    free_string( acc->email );
    acc->email 	= str_dup("");
    acc->act 	    = 0;
    acc->verify 	= 0;    
    acc->hours 	    = 0;
    acc->secret 	= 0;	
    acc->version 	= 0;
    acc->pen_pts    = 0;
    acc->numb       = -1;

    free_string(acc->pen_info);
    acc->pen_info   = str_dup("");
        

    VALIDATE(acc);
    return TRUE;
}

/* free the memory structures for the account */
void free_account( ACCOUNT_DATA * acc )
{
   /* want to make this robust so we don't lose memory to leaks, or crash */
   if( acc )
   {
    char buf[MSL];
    sprintf(buf, "free_account called on account: %s", acc->name);
    log_string(buf);

    if (!IS_VALID(acc))
        return;

    free_string( acc->name );
    free_string( acc->screen_name );
    free_string (acc->empire_name);
    free_string( acc->password );
    free_string( acc->old_passwd );
    free_string( acc->email );

    acc->act 	= 0;
    acc->verify 	= 0;
    acc->numb 	= -1;
    acc->hours 	= 0;
    acc->secret 	= 0;
    acc->version 	= 0;

    INVALIDATE(acc);

// going to see if we just keep it as is.  descriptors do not get memory freed, just
// recycled over and over.

    if (acc->d != NULL)
        acc->d->account = NULL;

    free_mem(acc, sizeof(*acc));
    return;
   }
}

ACCOUNT_DATA * load_temp_account( char * name )
{
    ACCOUNT_DATA * acc = NULL;

    FILE *fp = NULL;    
    char buf2[50];    

    if ( IS_NULLSTR(name))
        return NULL;

   sprintf(buf2, "%s%s", ACCOUNT_DIR, name);

    if ( (fp = fopen(buf2, "r")) != NULL )
    {        
        char buf[MSL];
        char word[MIL];
        char dump[MAX_LINE_LENGTH+1];
        char *string;
        int version = 0;

        
        while(TRUE)
        {
            if(feof( fp ))
            {
                sprintf(buf, "Account %s has no data", name);
                bug(buf, 0);
            }
            
            if (fscanf( fp, "%s %s", word, buf ) == '\0')
            {
                log_string("accounts: no data in file.");
                break;
            }
        
            if(IS_NULLSTR(word) || IS_NULLSTR(buf))
                continue;

           
            if(!(strcmp(word, "Email")))
            {
                free_string( acc->email );
                acc->email = str_dup(buf);
            }
            else if(!(strcmp(word, "Version")))
            {
                version = atoi(buf);
                acc->version = version;
            }
            else if(!(strcmp(word, "Name")))
            {
                acc = new_account();
                free_string( acc->name );
                acc->name = str_dup(buf);                                
            }
            else if(!(strcmp(word, "Screen")))
            {             
                free_string( acc->screen_name );
                acc->screen_name = str_dup(buf);
                                
            }
            else if (!(strcmp(word, "LogO")))
            {
                acc->logon = atol(buf);
            }
            else if(!(strcmp(word, "Pass")))
            {
                free_string( acc->password );
                acc->password = str_dup(buf);
            }
            else if(!(strcmp(word, "End")))
            {                
                fclose(fp);
                return acc;
            }
            else if(word[0] == '*')
            {
                 if (fgets(dump, MAX_LINE_LENGTH, fp) == NULL)
                    log_string ("Issue loading account: fgets");
            }
            else if(!(strcmp(word, "Act")))
                acc->act = atoi(buf);
            else if(!(strcmp(word, "Verify")))
                acc->verify = atoi(buf);
            else if (!(strcmp(word, "Plyd")))
                acc->hours = atol(buf);
            else if (!(strcmp(word, "Secret")))
                acc->secret = atol(buf);
            else if (!(strcmp(word, "Pen_pts")))
                acc->pen_pts = atol(buf);
            else if (!(strcmp(word, "Pen_info")))
            {
                free_string( acc->pen_info );
                string = fread_string(fp);
                acc->pen_info = str_dup(string);
            }			         
            else
            {
                sprintf(buf,"load_temp_account %s: no match for '%s'.", name, word);
                bug( buf, 0 );
                
                if (fgets(dump, MAX_LINE_LENGTH, fp) == NULL)
                    log_string(buf);
            }
            
            
        }
        fclose(fp);
    }    
    return acc;
}

bool load_account_obj( DESCRIPTOR_DATA *d, char *name )
{
   FILE *fp = NULL;
   bool found = FALSE;
   char buf2[50];

   if (d->account != NULL)
    return TRUE;

   if ( IS_NULLSTR(name) || !d )
    return FALSE;

   sprintf(buf2, "%s%s", ACCOUNT_DIR, name);

    if ( (fp = fopen(buf2, "r")) != NULL )
    {
        ACCOUNT_DATA *acc = NULL;
        char buf[MSL];
        char word[MIL];
        char dump[MAX_LINE_LENGTH+1];
        char *string;
        int version = 0;

        found = TRUE;
        
        while(TRUE)
        {
            if(feof( fp ))
            {
                sprintf(buf, "Account %s has no data", name);
                bug(buf, 0);
            }
            
            if (fscanf( fp, "%s %s", word, buf ) == '\0')
            {
                log_string("accounts: no data in file.");
                break;
            }   

            if(IS_NULLSTR(word) || IS_NULLSTR(buf))
                continue;

            
            if(!(strcmp(word, "Email")))
            {
                free_string( acc->email );
                acc->email = str_dup(buf);
            }
            else if(!(strcmp(word, "Version")))
            {
                version = atoi(buf);
                acc->version = version;
            }
            else if(!(strcmp(word, "Name")))
            {
                acc = new_account();
                free_string( acc->name );
                acc->name = str_dup(buf);

                        d->account = acc;
                acc->d = d;
            }
            else if (!(strcmp(word, "Screen")))
            {
                free_string(acc->screen_name);
                acc->screen_name = str_dup(buf);
            }
            else if (!(strcmp(word, "LogO")))
            {
                acc->logon = atol(buf);
            }
            else if(!(strcmp(word, "Pass")))
            {
                free_string( acc->password );
                acc->password = str_dup(buf);
            }
            else if(!(strcmp(word, "End")))
            {
                acc->d = d;				
                fclose(fp);
                return found;
            }
            else if(word[0] == '*')
            {
                 if (fgets(dump, MAX_LINE_LENGTH, fp) == NULL)
                    log_string ("Issue loading account: fgets");
            }
            else if(!(strcmp(word, "Act")))
                acc->act = atoi(buf);
            else if(!(strcmp(word, "Verify")))
                acc->verify = atoi(buf);
            else if (!(strcmp(word, "Plyd")))
                acc->hours = atol(buf);
            else if (!(strcmp(word, "Secret")))
                acc->secret = atol(buf);
            else if (!(strcmp(word, "Pen_pts")))
                acc->pen_pts = atol(buf);
            else if (!(strcmp(word, "Pen_info")))
            {
                free_string( acc->pen_info );
                string = fread_string(fp);
                acc->pen_info = str_dup(string);
            }			                        
            else
            {
                sprintf(buf,"load_account_obj %s: no match for '%s'.", name, word);
                bug( buf, 0 );
                
                if (fgets(dump, MAX_LINE_LENGTH, fp) == NULL)
                    log_string(buf);
            }
        }
        fclose(fp);
    }
    return FALSE;
}


bool new_account_obj( DESCRIPTOR_DATA *d, char *name )
{
    ACCOUNT_DATA *acc;
    FILE *fp;
    char filename[50];

    sprintf(filename, "%s%s", ACCOUNT_DIR, name);
    if ((fp = fopen(filename, "r")) == NULL)
    {
        acc = new_account();

        free_string( acc->name );
        acc->name = str_dup(name);

        acc->d = d;
        d->account = acc;
        return SUCCESS;
    }
    else
    {
        /* return 0 if an account wasn't made */
        fclose(fp);
        return FAILURE;
    }
}


void save_account_obj( ACCOUNT_DATA *acc )
{
    FILE *fp;
    char filename[MSL];    

    if ( !acc || IS_NULLSTR(acc->name))
        return;

    sprintf(filename, "%s%s", ACCOUNT_DIR, acc->name);
    if((fp = fopen(filename, "w")) == NULL )
        return;
        
    fprintf(fp, "Name %s\n", acc->name);
    fprintf(fp, "Pass %s\n", acc->password);
    fprintf(fp, "Screen %s\n", acc->screen_name);
    fprintf(fp, "Version %d\n", ACCOUNT_VERSION);
    if (!IS_NULLSTR(acc->email))
        fprintf(fp, "Email %s\n", acc->email);
    fprintf(fp, "Act %ld\n", acc->act);
    fprintf(fp, "Verify %d\n", acc->verify);
    fprintf(fp, "Plyd %ld\n", acc->hours);
    fprintf(fp, "LogO %ld\n", acc->logon);
    fprintf(fp, "Secret %ld\n", acc->secret);
    fprintf(fp, "Pen_pts %ld\n", acc->pen_pts);
    if (!IS_NULLSTR(acc->pen_info))
        fprintf(fp, "Pen_info buffer %s~\n", acc->pen_info);
    
    fprintf(fp,"\n");	

    fprintf(fp, "End\n");
    fclose(fp);
}

int ACC_IS_SET(int bits, int check)
{
    if (bits & check)
        return 1;
    else 
        return 0;
}

void ACC_SET(ACCOUNT_DATA * acc, int check)
{
    int bits;

    bits = acc->act;
    if(bits & check)
        return;
    else
        bits = bits | check;
    acc->act = bits;
}

int create_key( void )
{
    int key = 0;
    /*struct timeval ts; 
    

    //this should create a verification key 
    //gettimeofday(&ts, NULL);

    srandom(time(NULL)^getpid());
    key = ((ts.tv_sec + 644536) * random()) % 164832;
    if(key < 0)
        key = -key;
    */
    return key;
}

void send_mail(DESCRIPTOR_DATA * d)
{
    /*char *email, *account;
    int verify;
    FILE * outmail;
    char buf[300], outfile[300];

    if(d->connected != CON_GET_EMAIL)
        return;

    if ( !d->account || IS_NULLSTR(d->account->email) || IS_NULLSTR(d->account->name))
    {
        WTB(d, "There was problem in verification.  Seek a high level immortal for help.\r\n",0);
        return;
    }

    email = d->account->email;
    verify = d->account->verify;
    account = d->account->name;

    // send an email to email with the account verify # verify 
    sprintf(outfile, "%s%s.txt", ACCOUNT_DIR, account); // save message
    sprintf(buf, "mail -s \"Aragond Account Verification\" %s < %s", email, outfile);
    outmail = fopen(outfile, "w");
    fprintf(outmail, "This is your verification number for account %s.\n\n%d\n", 
        account, verify);
    fclose(outmail);

    system(buf);*/
    return;
}

/* returns a 1 if the email address in account has already been used, 
   a 0 otherwise.  A 2 is returned if the email address is from a 
   free service (that we are checking */
  /*
int check_account( DESCRIPTOR_DATA *d, char * account )
{
// alphabetize? 
    FILE * acmail;
    char name[100], acc[100], buf[100], outfile[100];
    sprintf(outfile, "%saccount.lst", ACCOUNT_DIR);
    
    if ( IS_NULLSTR(account))
        return 1;

    acmail = fopen(outfile, "r+");
    while(fscanf(acmail, "%s %s", name, acc) == 2)
    {
        if(!(strcmp(name, account)))
        { // email already exists 
            sprintf(buf, "%s is already registered to the account %s.\r\n",
                account, acc);
            WTB( d, buf, 0 );
            fclose(acmail);
            return(1);
        }
    }
    fprintf(acmail,"%.40s\t%s\n", account, d->account->name);
    fclose(acmail);
    return(0);
}
*/
void del_account_name( char * account )
{
    FILE * acmail, * outf;
    char name[100], buf[100], outfile[100];
    sprintf(outfile, "%saccount.txt", ACCOUNT_DIR);
    acmail = fopen(outfile, "r");
    sprintf(outfile, "%saccount.new", ACCOUNT_DIR);
    outf = fopen(outfile, "w");
    while(fscanf(acmail, "%s %s", name, buf))
    {
        if(strcmp(name, account))
            fprintf(outf, "%s %s", name, buf);
    }
    fclose(acmail);
    fclose(outf);
    sprintf(buf, "cp %saccount.new %saccount.txt", ACCOUNT_DIR, ACCOUNT_DIR);
    //system(buf);
}

void mail_pass(char * name)
{
      /*  char *email, *password;
        FILE * outmail, * infile;
        char buf[100], outfile[100], word[100];

        email = NULL;
        password = NULL;

        infile = fopen(name, "r");
    
    while(1)
    {
        fscanf(infile, "%s %s", word, buf);
        if(!(strcmp(word, "email")))
        email = str_dup(buf);
        if(!(strcmp("Pass", word)))
        password = str_dup(buf);
        if(!(strcmp("End", word)))
        break;
    }

    fclose(infile);
        // send an email to email with the account password 
        sprintf(outfile, "%s%s.txt", ACCOUNT_DIR, name); // save message 
        sprintf(buf, "mail -s \"Aragond Account Password\" %s < %s",email, outfile);
        outmail = fopen(outfile, "w");
        fprintf(outmail, "This is your password for account %s.\n\n%s\n",
                name, password);
        fclose(outmail);

        system(buf);*/
}

int stringSearch(char * a, char * b)
{
    register int i, j=0, slot=0;
    for(i=0; a[i]; i++)
    {
        for(j=i, slot=0; a[j] == b[slot]; j++, slot++)
        {
            if(b[slot+1] == '\0')
                return(1);
        }
    }
    return(0);
}


bool check_account_name(char * name)
{
         /*
     * Reserved words.
     */
    if ( is_name( name,
        "all auto immortal self someone something the you demise balance circle loner honor none hassan") )
        return FALSE;

    if ( is_name( name,
        "horde rogue apostle warlock barbarian dragon loner outcast esper nocturne rose Unlinked powerimms" ))
        return FALSE;

    if ( is_name( name,
        "bregan obsidian guardian ravager conclave" ))
        return FALSE;

    if( ( strlen(name) <=  2 ) || ( strlen(name) > 12 ) )
        return FALSE;

    /*
     * Alpha only.
     * Lock out IllIll twits.
     */
    {
        char *pc;
        bool fIll,adjcaps = FALSE,cleancaps = FALSE;
        size_t total_caps = 0;

        fIll = TRUE;
        for ( pc = name; *pc != '\0'; pc++ )
        {
            if ( !isalpha(*pc) )
                return FALSE;

            if ( isupper(*pc)) /* ugly anti-caps hack */
            {
                if (adjcaps)
                    cleancaps = TRUE;
                total_caps++;
                adjcaps = TRUE;
            }
            else
                adjcaps = FALSE;

            if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
                fIll = FALSE;
        }

        if ( fIll )
            return FALSE;

        if (cleancaps || (total_caps > (strlen(name)) / 2 && strlen(name) < 3))
            return FALSE;
    }
    return TRUE;
}

/*
ACCOUNT_DATA * get_account_world( char * arg )
{

}
*/
/*
void do_adeny( CHAR_DATA *ch, char *argument )
{
    char arg[MIL],buf[MSL];
    ACCOUNT_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        SEND( "Deny whom?\r\n", ch );
        return;
    }

    if ( ( victim = get_account_world( arg ) ) == NULL )
    {  
    DESCRIPTOR_DATA dnewtemp;
    ACCOUNT_DATA * temp = load_account_obj(&dnewtemp, arg);
    
    if ( ACC_IS_SET( victim->act, ACC_IMP ) )
        {
            SEND( "You failed.\r\n", ch );
            return;
        }

        ACC_SET(victim->act, ACC_DENY);
        save_account_obj(temp);
    free_account(temp);
    sprintf(buf,"$N denies access to account %s",victim->name);
        wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    SEND( "OK.\r\n", ch );
        return;
    }

    if ( ACC_IS_SET( victim->act, ACC_IMP ) )
    {
        SEND( "You failed.\r\n", ch );
        return;
    }

    ACC_SET(victim->act, ACC_DENY);
    sprintf(buf,"$N denies access to account %s",victim->name);
    wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    SEND( "OK.\r\n", ch );
    SEND( "Remember, this doesn't force the player to quit, just prevents them from relogging.\r\n", ch);
    return;
}

*/

bool account_is_playing(DESCRIPTOR_DATA * d)
{
    DESCRIPTOR_DATA *dold;

    if ( !d || !d->account || IS_NULLSTR( d->account->name ))
        return FALSE;

    if ( !ACC_IS_SET(d->account->act, ACC_IMP) )
    {
        for ( dold = descriptor_list; dold != NULL; dold = d_next)
        {
            d_next = dold->next;

            if ( d == dold || !dold->account)
                continue;

            if (IS_NULLSTR(dold->account->name ))
                continue;

            if ( !strcasecmp( d->account->name, dold->account->name))
            {
                if (dold->connected != CON_PLAYING && dold->connected != CON_BREAK_CONNECT)
                {
                    close_socket( dold );
                    return TRUE;
                }                
                
                WTB(d, "\r\n{RYou already have a connection playing from your account.{x\r\n",0);                

                save_account_obj( dold->account );                

                if ( !load_account_obj( d, dold->account->name ))
                    return FALSE;                

                close_socket( dold );

                WTB(d, "Reconnecting.\r\n", 0);                
                sprintf( log_buf, "%s@%s reconnected.", d->account->screen_name, d->host);
                log_string( log_buf );
                                    
                //wiznet("$N groks the fullness of $S link.", vch,NULL,WIZ_LINKS,0,0);                
                d->connected = CON_PLAYING;	
                return TRUE;
            }
        }
    }
    return FALSE;
}

bool find_account_online(DESCRIPTOR_DATA * d, char *name)
{
    DESCRIPTOR_DATA *dold;

    if ( !d || !d->account || IS_NULLSTR(name))
    return FALSE;

    for ( dold = descriptor_list; dold != NULL; dold = d_next)
    {
        d_next = dold->next;

        if ( d == dold )
        continue;

        if ( !dold->account || IS_NULLSTR(dold->account->name ))
        continue;

            if ( !strcasecmp( name, dold->account->name))
        {
        save_account_obj( dold->account );

        if ( !load_account_obj( d, dold->account->name ))
            return FALSE;

        return TRUE;
        }
    }
    return FALSE;
}
