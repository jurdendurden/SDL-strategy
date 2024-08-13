#include "main.h"
#include "parse.h"
#include "recycle.h"


#if defined(unix)
    void game_loop_unix args ((int control));
    int init_socket args ((int port));
    void init_descriptor args ((int control));
    bool read_from_descriptor args ((DESCRIPTOR_DATA * d));
    bool write_to_descriptor args ((int desc, char *txt, int length));
#endif

bool check_parse_name args ((char *name));
bool check_reconnect args ((DESCRIPTOR_DATA * d, char *name, bool fConn));
bool check_playing args ((DESCRIPTOR_DATA * d, char *name, bool account));

const char echo_off_str[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const char echo_on_str[] = { IAC, WONT, TELOPT_ECHO, '\0' };


/*
 * Global variables.
 */
extern FILE *fpReserve;                /* Reserved file handle     */
extern bool merc_down;                    /* Shutdown         */
extern char str_boot_time[MIL];
extern time_t current_time;            /* time of this pulse */

//Account globals.
bool loading = FALSE;
int points = 0;

/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny (DESCRIPTOR_DATA * d, char *argument)
{
    DESCRIPTOR_DATA *d_old;
    char buf[MSL * 2];
    EMPIRE_DATA * empire = NULL;
    ACCOUNT_DATA * account;    
    int i = 0;
    bool fOld;
    char emp_name[50];
    
    while ( isspace(*argument) )
        argument++;
        
    account = d->account;
    
    switch (d->connected)
    {
        default:
            bug ("Connection handler: bad d->connected state: %d.", d->connected);
            close_socket (d);
            return;

        case CON_EXTRACTED:
            close_socket( d );
            return;

        case CON_INTRO:

            if(argument[0] =='\0' )
            {
                intro_menu(d);
                break;
            }
            switch(argument[0])
            {
                case 'c':	//create a new account
                case 'C':
                    if(wizlock || newlock)
                    {
                        WTB(d, "The game is locked for development.  Please try again later.\r\n", 0);
                        close_socket( d );
                        return;
                    }

                    WTB(d, "Account Name:  ", 0);
                    d->connected = CON_GET_NAME;
                    break;
                
                case 'e':	//exit
                case 'E':
                    WTB(d, "Goodbye.\r\n", 0);
                    close_socket( d );
                    return;
                
                case 'f':	//mail password out
                case 'F':
                    WTB(d, "This is still being implemented.\r\n", 0);
                    break;
                
                case 'l':	/* log into an account */
                case 'L':
                    WTB(d,"Name:  ", 0);
                    d->connected = CON_GET_OLD_ACCOUNT;
                    break;              

                case 'h':	/* get account help */
                case 'H':
                    account_help( d );
                    break;                                

                default:
                    WTB( d, "Not a valid choice, please choose another.\r\n", 0);
                    break;
            }
            
            //if we get to here, we didn't change d->connected
            intro_menu(d);
            return;
        break;

        case CON_SEND_PASS:
            d->connected = CON_INTRO;
            break;
        break;

        case CON_GET_OLD_ACCOUNT:
            if ( argument[0] == '\0' )
            {
                close_socket( d );
                return;
            }

            argument[0] = UPPER(argument[0]);
            if ( !check_account_name( argument ) )
            {
                WTB( d, "Illegal name, try another \n\rName:  ", 0);
                break;
            }
            d->account = NULL;
            fOld = load_account_obj( d, argument );

            if(!check_ban(d->host, BAN_PERMIT) 
                && (check_ban(d->host, BAN_ALL) || check_ban(d->host, BAN_NEWBIES)))
            {
                if (!fOld || (fOld && !ACC_IS_SET(d->account->act, ACC_PERMIT)) )
                {
                WTB(d,"\r\n\r\nYou are banned from this game.\r\n",0);
                WTB(d,"\r\n\r\nGood Bye.\r\n",0);
                close_socket(d);
                return;
                }
            }

            if (fOld)   /* Old account */
            {
                if ( IS_NULLSTR(d->account->name))
                {
                    sprintf( log_buf, "Account bugged:  %s@%s.",argument, d->host);
                    log_string( log_buf );
                    WTB(d, "Your account is bugged/broken, contact admin for assistance.\r\n",0);
                    close_socket(d);
                    return;
                }
                if(ACC_IS_SET(d->account->act, ACC_DENY))
                {
                    sprintf( log_buf, "Denying access to account %s@%s.", d->account->name, d->host);
                    log_string( log_buf );
                    WTB( d, "Your account has been denied.\r\n", 0);
                    close_socket(d);
                    return;
                }
                if(wizlock)
                {
                    WTB(d, "{RATTENTION{x:  We are currently {YLOCKED FOR DEVELOPMENT{x.\r\n",0);
                    if(!ACC_IS_SET(d->account->act, ACC_IMM))
                    {
                        close_socket( d );			
                        return;
                    }
                }

                WTB( d, "Password: ", 0 );
                WTB( d, echo_off_str, 0 );
                sprintf(buf, "%s", d->account->password);
                STD(buf, d);
                d->connected = CON_GET_OLD_PASSWORD;
                break;
            }
            else 
            {
                WTB( d, "No account by that name exists.\r\n",0);
                intro_menu(d);
                d->connected = CON_INTRO;
                break;
            }
        break;

        case CON_GET_NAME:
            if ( argument[0] == '\0' )
            {
                close_socket( d );
                return;
            }

            argument[0] = UPPER(argument[0]);
            if ( !check_account_name( argument ) )
            {
                WTB( d, "Illegal name, try another.\n\rName:  ", 0 );
                break;
            }
            
            d->account = NULL;
            fOld = load_account_obj( d, argument );

            if (check_ban(d->host,BAN_PERMIT))
            {
                if (!fOld || (fOld && !ACC_IS_SET(d->account->act, ACC_PERMIT)) )
                {
                    WTB(d,"Your site has been banned from this mud.\r\n",0);
                    close_socket(d);
                    return;
                }
            }

            if (fOld)   /* Old account */
            {
                WTB(d, "That account name already exists, try another.\n\rName:  ", 0);
                break;
            }

            else	/* New account */
            {
                if (newlock)
                {
                    WTB( d, "The game is newlocked.\r\n", 0 );
                    close_socket( d );
                    return;
                }
            
            
                if (check_ban(d->host,BAN_NEWBIES))
                {
                    WTB(d, "New accounts aren't allowed from your site.\r\n",0);
                    close_socket(d);
                    return;
                }
        
                create_account_menu( d );
                sprintf( buf, "\n\rDid I get that right, %s (Y/N)? ", argument );
                WTB( d, buf, 0 );
                d->account = new_account();
                free_string( d->account->name);
                d->account->name = str_dup(argument);
                d->connected = CON_CONFIRM_NEW_ACC_NAME;
                break;
            }

        break;

        case CON_GET_OLD_PASSWORD:
            #if defined(unix)
            WTB( d, "\r\n", 2 );
            #endif

            if ( strcmp( argument, d->account->password ))
            {
                WTB( d, "Wrong password.\r\n", 0 );
                close_socket( d );
                return;
            }
            WTB( d, echo_on_str, 0 );
            /*
            if(!ACC_IS_SET(d->account->act, VERIFY)) 
            {
                STD("This account needs to be verified.\r\n", d);
                STD("Failure to verify your account will limit the number of characters\r\n", d);
                STD("you may have to {YONE{x, as well as reduce their maximum level to\r\n",d);
                STD("{Y15{x.  If you are having problems getting your account verified,\r\n",d);
                STD("speak with one of the immortals.\r\n", d);
                STD("Please enter your verification number:  ", d);
                d->connected = CON_ACC_VERIFY;
            }
            else*/
            {
                account_main_menu(d);
                d->connected = CON_ACCOUNT;
            }
            break;

        case CON_ACC_VERIFY:
            if ( atoi( argument) != d->account->verify )
            {
                WTB( d, "Incorrect verification number.\r\n", 0);
                WTB( d, "Your account has not yet been verified.\r\n", 0);
                account_main_menu( d );
                d->connected = CON_ACCOUNT;
                break;
            }

            WTB( d, "Account has been verified.\r\n", 0);
            ACC_SET(d->account, VERIFY);
            save_account_obj(d->account);			
            account_main_menu(d);
            d->connected = CON_ACCOUNT;
            break;

        case CON_CONFIRM_NEW_ACC_NAME:
            switch ( argument[0] )
            {
            case 'y': 
            case 'Y':

                wiznet ("New account alert!", NULL, WIZ_NEWBIE, 0);                
                
                STD ("New account. Please enter a password for this account.\r\n", d);
                sprintf (log_buf, "%s@%s new account.%c", d->account->name, d->host, 007);
                LOG (log_buf);
                wiznet (log_buf, NULL, WIZ_SITES, 0);		

                // set newbie thing for new accounts.
                // all accounts currently start verified                
                d->account->act = d->account->act | ACC_NEWBIE; 
                d->account->act = d->account->act | VERIFY; 
                
                d->connected = CON_GET_PASS;
                break;

            case 'n':
            case 'N':
                WTB( d, "Ok, what IS it, then? ", 0 );
                free_string( d->account->name );
                d->account->name = str_dup("");
                d->connected = CON_GET_NAME;
                break;

            default:
                WTB( d, "Yes or No? ", 0 );
                break;
            }
        break;
/*
        case CON_GET_EMAIL: // if you change this, need to change it in account.c send_mail as well.
            d->account->verify = create_key();
            if(argument[0] == '\0')
                WTB( d, "You will need to speak with an immortal to get your account approved\r\n", 0);
            else 
            {
                if(!strcasecmp(argument, "NULL") )
                {
                    WTB( d, "Remember to contact Upro to\r\n", 0);
                    WTB( d, "get your account verified.  If you don't,\r\n", 0);
                    WTB( d, "you won't be limited in play.\r\n",0);	
                }
            }

            WTB( d, "What do you want your password to be?\r\n", 0);
            WTB( d, "Remember to choose something hard to guess,\r\n", 0);
            WTB( d, "because this allows acces to all your characters.\r\n", 0);
            WTB( d, "Password:  ", 0);
            WTB( d, echo_off_str, 0);
            d->connected = CON_GET_PASS;
            return;
        break;
*/
        
/*
        case CON_VERIFY_EMAIL:
            if ((argument[0] == 'y') || (argument[0] == 'Y'))
            {
                    WTB( d, "\n\rWhat do you want your password to be?\r\n", 0);
                    WTB( d, "Remember to choose something hard to guess,\r\n", 0);
                    WTB( d, "because this allows access to all your characters.\r\n", 0);
                    WTB( d, "Password:  ", 0);
                    WTB( d, echo_off_str, 0);
            d->connected = CON_GET_PASS;
            return;	
            }
            else
            {
            WTB( d, "\n\r( New account ) Please enter an email address.\r\n", 0);
            WTB( d, "If you don't have a valid email, you should enter\r\n", 0 );
            WTB( d, "NULL as your email address, then write a note to\r\n", 0);
                    WTB( d, "powerimms about it, or talk to Upro\r\n", 0);
            WTB( d, "to get your account approved.\r\n", 0);
            WTB( d, "\n\rEmail address:  ", 0);
            d->connected = CON_GET_EMAIL;
            return;
            }
        break;

*/

        case CON_GET_PASS:
            #if defined(unix)
                WTB( d, "\r\n", 2 );
            #endif

            if ( strlen(argument) < 6 )
            {
                WTB( d, "Password must be at least six characters long.\n\rPassword: ",0 );
                return;
            }
            if(strstr(argument," ") != NULL)
            {
                WTB(d,"Your password cannot contain spaces.\n\rPassword: ",0);
                return;
            }
            if(strstr(argument,"~") != NULL)
            {
                WTB(d,"Your password cannot contain tildes.\n\rPassword: ",0);
                return;
            }
            if(strstr(argument,"#") != NULL)
            {
                WTB(d,"Your password cannot contain pound signs.\n\rPassword: ",0);
                return;
            }
            if(strstr(argument,"{") != NULL)
            {
                WTB(d,"Your password cannot contain squiggly brackets.\n\rPassword: ",0);
                return;
            }
            free_string(d->account->password );
            d->account->password = str_dup( argument );
            WTB( d, "Please retype password: ", 0 );
            d->connected = CON_CONFIRM_NEW_PASSWORD;
            break;

        case CON_ACCOUNT:

            if(d->account != NULL)	
            {			                    
                if(account_is_playing(d))
                    break;
            }

            switch(argument[0])
            {
                case 'p': //play the game
                case 'P':
                    if(wizlock || newlock)
                    {
                        WTB(d, "The server is currently locked to new connections for development.\r\n", 0);
                        account_main_menu(d);
                        break;
                    }                
                    WTB(d,"\r\n\r\n",0);
                    WTB(d, "What name do you wish your screen name to be known as?  ",0);
                    d->connected = CON_GET_NEW_NAME;
                    break;                

                case 'd': /* delete account */
                case 'D':                
                    WTB(d, "Are you sure about this?  Deleting your account is irreversable...", 0);
                    d->connected = CON_DEL_ACC;
                    break;                        
                case 'e': //email needs finished
                case 'E': 
                    WTB(d, "Temporarily disabled.\r\n", 0);
                    account_main_menu( d );
                    d->connected = CON_ACCOUNT;
                    break;
                case 'a':
                case 'A':
                    WTB(d,"",0);
                    acc_info( d->account, d );
                    account_main_menu(d);
                    break;
                case 'c': //change password
                case 'C':
                    WTB(d, "What do you want to change your password too?  ", 0);
                    WTB( d, echo_off_str, 0 );
                    d->connected = CON_CHNG_PASS;
                    break;
                case 'q': //quit
                case 'Q':
                    WTB(d, "Good bye.\r\n", 0);
                    close_socket( d );
                    return;            
                /*case 'h':
                case 'H':
                    do_function (ch, &do_help, "name");								
                    return;            */
                default:
                    WTB( d, "That's not a valid command.\r\n", 0);
                    account_main_menu( d );
                    return;
            }
        break;

        case  CON_CHNG_PASS:	
            if ( argument[0] != '\0' )
            {
                if ( strlen(argument) < 6 )
                {
                    WTB( d, "\n\rPassword must be at least six characters long.\n\rPassword: ",0 );
                    return;
                }

                if(strstr(argument," ") != NULL)
                {
                    WTB(d,"\n\rYour password cannot contain spaces.\n\rPassword: ",0);
                    return;
                }

                if(strstr(argument,"~") != NULL)
                {
                    WTB(d,"\n\rYour password cannot contain tildes.\n\rPassword: ",0);
                    return;
                }
                if(strstr(argument,"#") != NULL)
                {
                    WTB(d,"Your password cannot contain pound signs.\n\rPassword: ",0);
                    return;
                }
                if(strstr(argument,"{") != NULL)
                {
                    WTB(d,"Your password cannot contain squiggly brackets.\n\rPassword: ",0);
                    return;
                }
                free_string(d->account->old_passwd);
                d->account->old_passwd = str_dup(d->account->password);

                free_string(d->account->password);
                d->account->password = str_dup(argument);

                WTB(d, "\n\rConfirm your password by typing it again: ", 0);
                d->connected = CON_CONFIRM_NEW_PASSWORD; 
            }
            else
            {
                WTB(d, echo_on_str, 0);
                WTB(d, "\n\rPassword not changed!\r\n", 0);
                account_main_menu( d );
                d->connected = CON_ACCOUNT;
            }
            break;
        
        case CON_CONFIRM_NEW_PASSWORD:
            #if defined(unix)
                WTB( d, "\r\n", 2 );
            #endif
        
            if ( !str_cmp( argument, d->account->password ) )
            {
                WTB(d, "Password changed!\r\n", 0);

                free_string(d->account->old_passwd);
                d->account->old_passwd = str_dup( "" );

                free_string(d->account->password);
                d->account->password = str_dup(argument);

                save_account_obj(d->account);
            } 
            else
            {
                WTB(d, "Passwords don't match!\r\n", 0);

                free_string(d->account->password);
                d->account->password = str_dup(d->account->old_passwd);

                free_string(d->account->old_passwd);
                d->account->old_passwd = str_dup( "" );
            }

            WTB( d, echo_on_str, 0 );
            account_main_menu( d );
            d->connected = CON_ACCOUNT;                    
            break;	

        case CON_DEL_ACC:
            switch( argument[0] )
            {
            case 'y':
            case 'Y':
                WTB(d, "Please enter your password:  ", 0);
                d->connected = CON_CONFIRM_DEL_ACC;
                return;
            default:
                WTB(d, "Whew... that was close.\n\r\n", 0);
                account_main_menu( d );
                d->connected = CON_ACCOUNT;
                return;
            }
        break;

        case CON_CONFIRM_DEL_ACC:
            if(!str_cmp(argument, d->account->password))
            {
                char delbuf[MSL];
                //del_account_name( d->account->email );                
                sprintf(delbuf, "%s%s", ACCOUNT_DIR, d->account->name);
                unlink(delbuf);
                close_socket(d);
                return;
            }
            else
            {
                WTB(d, "Incorrect password.\n\r\n", 0);
                account_main_menu( d );
                d->connected = CON_ACCOUNT;
                return;
            }
        break;        
        
        case CON_BREAK_CONNECT:
            //CHAR_DATA *pet;

            switch( *argument )
            {
            case 'y' : case 'Y':
                for ( d_old = descriptor_list; d_old != NULL; d_old = d_next )
                {
                    d_next = d_old->next;
                    if (d_old == d || d_old->account == NULL)
                        continue;                    

                    close_socket(d_old);
                }
                if (check_reconnect(d, d->account->screen_name, TRUE))
                    return;
                write_to_buffer(d,"Reconnect attempt failed.\n\r ",0);
                if ( d->account != NULL )
                {                                    
                    free_account(d->account);                    
                    d->account = NULL;
                    d->account->valid = FALSE;
                }
                account_main_menu(d);
                d->connected = CON_ACCOUNT;
                break;

            case 'n' : case 'N':
                    if ( d->account != NULL )
                    {                       
                        free_account( d->account );
                        d->account = NULL;
                    }
                    account_main_menu(d);
                    d->connected = CON_ACCOUNT;
                    break;

            default:
                write_to_buffer(d,"Please type Y or N? ",0);
                break;
            }
            break;
        
        //Choose a screen name
        case CON_GET_NEW_NAME:
            if ( argument[0] == '\0' )
            {
                WTB( d, "You didn't type anything, what would you like your screen name to be?\r\n", 0 );                
                break;
            }
            
            for ( i = 0; argument[i] != '\0'; i++ )
                argument[i] = LOWER(argument[i]);        
            //checking for other people creating names
            for (DESCRIPTOR_DATA *dlist = descriptor_list; dlist; dlist = d_next)
            {
                d_next = dlist->next;
                
                if (!dlist->account || !dlist->account->screen_name)
                    continue;
                
                if (!str_cmp(argument,dlist->account->screen_name))
                {
                    WTB(d,"Someone else already has that name in use.\n\rName: ",0);
                    break;
                }
            } 
            
            argument[0] = UPPER(argument[0]);

            if (!check_parse_name (argument))
            {
                WTB( d, "Illegal name, try another.\n\rName: ", 0 );
                return;
            }

            if (ACC_IS_SET(d->account->act, ACC_DENY))
            {
                sprintf( log_buf, "Denying access to account %s@%s.", d->account->name, d->host);
                log_string( log_buf );
                WTB( d, "You are denied access.\r\n", 0 );
                account_main_menu( d );
                d->connected = CON_ACCOUNT;
                return;
            }
            
            free_string(d->account->screen_name);
            d->account->screen_name = str_dup(argument);
                            
            if ((fOld = load_empire_obj(d, argument)) != FALSE)                
            {
                if (!str_cmp(d->account->screen_name, argument))
                {                      
                    d->connected = CON_GET_NEW_EMPIRE_NAME;                
                    break;
                }                                                
                else
                {
                    WTB(d, "That empire name doesn't belong to your account.\r\n",0);
                    break;
                }            
            }			                                
            	
            sprintf( buf, "Did I get that right, %s (Y/N)? ", argument );                        
            WTB( d, buf, 0 );
            d->connected = CON_CONFIRM_NEW_NAME;			
            break;   

        case CON_CONFIRM_NEW_NAME:
            switch (argument[0])
            {
                case 'y':
                case 'Y':	
                			
                    sprintf (buf, "New screen name saved as %s for account %s.\r\n\r\n", d->account->screen_name, d->account->name);					                    

                    free_string(d->account->screen_name);                    

                    STD (buf, d);					
                    WTB (d, "\r\n", 0);                                                   
                    save_account_obj(d->account);    

                    WTB(d, "Now choose the banner name your empire will be called under.\r\n", 0);
                    d->connected = CON_GET_NEW_EMPIRE_NAME;                				
                    break;

                case 'n':
                case 'N':
                    STD ("Ok, what IS it, then? ", d);                    
                    d->connected = CON_GET_NEW_NAME;
                    break;

                default:
                    STD ("Is this correct?\r\n",d);
                    STD ("Please type ({cY{x)es or ({cN{x)o? ", d);
                    break;
            }
            break;
                           
        case CON_GET_NEW_EMPIRE_NAME:
            if ( argument[0] == '\0' )
            {
                WTB( d, "You didn't type anything, what would you like your empire banner name to be?\r\n", 0 );                
                break;
            }
            
            if (newlock)
            {
                WTB( d, "New empires are currently locked for game development.\r\n", 0 );
                account_main_menu( d );
                d->connected = CON_ACCOUNT;
            }

            if (check_ban(d->host,BAN_NEWBIES))
            {
                WTB(d,
                "Your site is banned from making a new empire. Contact admin regarding this status.\r\n",0);
                d->connected = CON_ACCOUNT;
                return;
            }

            for ( i = 0; argument[i] != '\0'; i++ )
                argument[i] = LOWER(argument[i]);   

            EMPIRE_DATA *e_next;
            for (EMPIRE_DATA *elist = empire_list; elist != NULL; elist = e_next)
            {
                e_next = elist->next;
                if (!str_cmp(elist->name, argument))
                {
                    WTB(d, "That empire name already exists. Try another.",0);
                    WTB(d, "Empire banner name:",0);
                    break;
                }
            }            
            
            argument[0] = UPPER(argument[0]);

            if ( !check_parse_name( argument ) )
            {
                WTB( d, "Illegal name, try another.\r\nName: ", 0 );
                break;
            }

            if (newlock)
            {
                WTB( d, "New empires cannot be made at the moment, for game development.\r\n", 0 );
                account_main_menu( d );
                d->connected = CON_ACCOUNT;
            }

            if (check_ban(d->host,BAN_NEWBIES))
            {
                WTB(d,
                "New empires are not allowed from your site.\r\n",0);
                d->connected = CON_ACCOUNT;
                return;
            }

            {
                FILE * ifp;
                char ibuff[1000];
                
                sprintf(ibuff, "%s%s", EMPIRE_DIR, capitalize(argument));
                if((ifp = fopen( ibuff, "r")))
                {
                    WTB(d, "This banner name already exists, please choose another.  Name: ", 0);
                    fclose(ifp);
                    break;
                }						
            }            
            
            sprintf( buf, "Did I get that right, %s (Y/N)? ", argument );                                    
            sprintf(emp_name, "%s", argument);
            WTB( d, buf, 0 );
            d->connected = CON_CONFIRM_EMPIRE_NAME;			
            break;   

        case CON_CONFIRM_EMPIRE_NAME:
            switch (argument[0])
            {
                case 'y':
                case 'Y':				
                    WTB (d, "Creating new empire.\r\n\r\n", 0);					                                        
                    WTB (d, "\r\n", 0);            


                    empire = new_empire();                    
                    free_string(empire->name);
                    empire->name = str_dup(emp_name);		            
                    empire->owner = str_dup(d->account->screen_name);                    
                    
                    //Add first city to empire
                    CITY_DATA * city;                                                            
                    city = new_city();                    
                    city->name = "new city";
                    city->x = number_range(0, MAP_WIDTH);
                    city->y = number_range(0, MAP_HEIGHT);
                    city->empire_name = str_dup(empire->name);
                    city->wood = 100000;
                    city->stone = 100000;
                    city->iron = 25000;
                    city->food = 10000;                    
                    d->account->cur_city = city;
                    d->account->city_id = city->id;


                    //Add townhall to city
                    BUILDING_DATA * building;
                    building = new_building();                    
                    building->type = BLD_TOWNHALL;
                    building->level = 1;
                    building->city = str_dup(city->name);
                    building->city_id = city->id;

                    
                    d->account->x = city->x;
                    d->account->y = city->y;                    
                    
                    view_city(d->account, city);
                    
                    save_account_obj(d->account);                        
                    d->connected = CON_ENTER_GAME;
                    break;

                case 'n':
                case 'N':
                    STD ("Ok, what IS it, then? ", d);
                    free_empire(empire);                    
                    d->connected = CON_GET_NEW_EMPIRE_NAME;
                    break;

                default:
                    STD ("Is this correct?\r\n",d);
                    STD ("Please type ({cY{x)es or ({cN{x)o? ", d);
                    break;
            }
        case CON_ENTER_GAME:
        {                            
            //fOld = load_account( d, d->account->screen_name);
            //fOld = Load_account( d, );

            fOld = TRUE;

            if (fOld)
            {                
                account = d->account;		
                account->valid = TRUE;

                if (check_playing(d, d->account->screen_name, FALSE))
                    return;

                if ( check_reconnect( d, d->account->screen_name, TRUE ) )            
                    return;
                
                if (IS_SET(d->account->act, ACC_HOSTMASK))
                { 
                    wiznet ("$N has joined the game.", account, WIZ_LOGINS, 0);
                    
                    sprintf( log_buf, "%s@%s has connected", d->account->screen_name,  d->host);
                    wiznet(log_buf,NULL,WIZ_SITES,0);
                    sprintf( log_buf, "%s real host is: {C%s{x", d->account->screen_name, d->host );
                    wiznet(log_buf,NULL,WIZ_SITES,0);      
                }			
                else 
                {
                    wiznet ("$N has joined the game.", account, WIZ_LOGINS, 0);
                    
                    sprintf( log_buf, "%s@%s has connected", account->screen_name, d->host);
                    wiznet(log_buf,NULL,WIZ_SITES,0);
                }					
            
            
                account->next = account_list;
                account_list = account;

                save_account_obj(d->account);
                d->connected = CON_PLAYING;
                return;
            }
            else
            {
                WTB(d, "There was an issue loading your account.\r\n",0);
                account_main_menu(d);
                d->connected = CON_ACCOUNT;
                return;
            }
        }        
        break;
    }    
}
