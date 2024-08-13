/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 */


#include "main.h"
#include "parse.h"
#include "recycle.h"

/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern int malloc_debug args ((int));
extern int malloc_verify args ((void));
#endif

#if defined(unix)
#include <signal.h>
#endif


const char go_ahead_str[] = { IAC, GA, '\0' };



/*
 * OS-dependent declarations.
 */
#if    defined(_AIX)
#include <sys/select.h>
int accept args ((int s, struct sockaddr * addr, int *addrlen));
int bind args ((int s, struct sockaddr * name, int namelen));
void bzero args ((char *b, int length));
int getpeername args ((int s, struct sockaddr * name, int *namelen));
int getsockname args ((int s, struct sockaddr * name, int *namelen));
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));
int listen args ((int s, int backlog));
int setsockopt args ((int s, int level, int optname, void *optval,
                      int optlen));
int socket args ((int domain, int type, int protocol));
#endif

#if    defined(apollo)
#include <unistd.h>
void bzero args ((char *b, int length));
#endif

#if    defined(__hpux)
int accept args ((int s, void *addr, int *addrlen));
int bind args ((int s, const void *addr, int addrlen));
void bzero args ((char *b, int length));
int getpeername args ((int s, void *addr, int *addrlen));
/nt getsockname args ((int s, void *name, int *addrlen));
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));
int listen args ((int s, int backlog));
int setsockopt args ((int s, int level, int optname,
                      const void *optval, int optlen));
int socket args ((int domain, int type, int protocol));
#endif

#if    defined(interactive)
#include <net/errno.h>
#include <sys/fnctl.h>
#endif

#if    defined(linux)
/* 
    Linux shouldn't need these. If you have a problem compiling, try
    uncommenting these functions.
*/
/*
int    accept        args( ( int s, struct sockaddr *addr, int *addrlen ) );
int    bind        args( ( int s, struct sockaddr *name, int namelen ) );
int    getpeername    args( ( int s, struct sockaddr *name, int *namelen ) );
int    getsockname    args( ( int s, struct sockaddr *name, int *namelen ) );
int    listen        args( ( int s, int backlog ) );
*/

int close args ((int fd));
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));
/* int    read        args( ( int fd, char *buf, int nbyte ) ); */
int select args ((int width, fd_set * readfds, fd_set * writefds,
                  fd_set * exceptfds, struct timeval * timeout));
int socket args ((int domain, int type, int protocol));

/* int    write        args( ( int fd, char *buf, int nbyte ) ); *//* read,write in unistd.h */
#endif

#if    defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct timeval {
    time_t tv_sec;
    time_t tv_usec;
};
#if    !defined(isascii)
#define    isascii(c)        ( (c) < 0200 )
#endif
static long theKeys[4];

int gettimeofday args ((struct timeval * tp, void *tzp));
#endif

#if    defined(MIPS_OS)
extern int errno;
#endif

#if    defined(MSDOS)
int gettimeofday args ((struct timeval * tp, void *tzp));
int kbhit args ((void));
#endif

#if    defined(NeXT)
int close args ((int fd));
int fcntl args ((int fd, int cmd, int arg));
#if    !defined(htons)
u_short htons args ((u_short hostshort));
#endif
#if    !defined(ntohl)
u_long ntohl args ((u_long hostlong));
#endif
int read args ((int fd, char *buf, int nbyte));
int select args ((int width, fd_set * readfds, fd_set * writefds,
                  fd_set * exceptfds, struct timeval * timeout));
int write args ((int fd, char *buf, int nbyte));
#endif

#if    defined(sequent)
int accept args ((int s, struct sockaddr * addr, int *addrlen));
int bind args ((int s, struct sockaddr * name, int namelen));
int close args ((int fd));
int fcntl args ((int fd, int cmd, int arg));
int getpeername args ((int s, struct sockaddr * name, int *namelen));
int getsockname args ((int s, struct sockaddr * name, int *namelen));
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));
#if    !defined(htons)
u_short htons args ((u_short hostshort));
#endif
int listen args ((int s, int backlog));
#if    !defined(ntohl)
u_long ntohl args ((u_long hostlong));
#endif
int read args ((int fd, char *buf, int nbyte));
int select args ((int width, fd_set * readfds, fd_set * writefds,
                  fd_set * exceptfds, struct timeval * timeout));
int setsockopt args ((int s, int level, int optname, caddr_t optval,
                      int optlen));
int socket args ((int domain, int type, int protocol));
int write args ((int fd, char *buf, int nbyte));
#endif

/* This includes Solaris Sys V as well */
#if defined(sun)
int accept args ((int s, struct sockaddr * addr, int *addrlen));
int bind args ((int s, struct sockaddr * name, int namelen));
void bzero args ((char *b, int length));
int close args ((int fd));
int getpeername args ((int s, struct sockaddr * name, int *namelen));
int getsockname args ((int s, struct sockaddr * name, int *namelen));
int listen args ((int s, int backlog));
int read args ((int fd, char *buf, int nbyte));
int select args ((int width, fd_set * readfds, fd_set * writefds,
                  fd_set * exceptfds, struct timeval * timeout));

#if !defined(__SVR4)
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));

#if defined(SYSV)
int setsockopt args ((int s, int level, int optname,
                      const char *optval, int optlen));
#else
int setsockopt args ((int s, int level, int optname, void *optval,
                      int optlen));
#endif
#endif
int socket args ((int domain, int type, int protocol));
int write args ((int fd, char *buf, int nbyte));
#endif

#if defined(ultrix)
int accept args ((int s, struct sockaddr * addr, int *addrlen));
int bind args ((int s, struct sockaddr * name, int namelen));
void bzero args ((char *b, int length));
int close args ((int fd));
int getpeername args ((int s, struct sockaddr * name, int *namelen));
int getsockname args ((int s, struct sockaddr * name, int *namelen));
int gettimeofday args ((struct timeval * tp, struct timezone * tzp));
int listen args ((int s, int backlog));
int read args ((int fd, char *buf, int nbyte));
int select args ((int width, fd_set * readfds, fd_set * writefds,
                  fd_set * exceptfds, struct timeval * timeout));
int setsockopt args ((int s, int level, int optname, void *optval,
                      int optlen));
int socket args ((int domain, int type, int protocol));
int write args ((int fd, char *buf, int nbyte));
#endif



void do_auto_shutdown args( (void) );


/*
 * Global variables.
 */
BAN_DATA *ban_list;
DESCRIPTOR_DATA *descriptor_list;    /* All open descriptors     */
DESCRIPTOR_DATA *d_next;        /* Next descriptor in loop  */
FILE *fpReserve;                /* Reserved file handle     */
bool god;                        /* All new chars are gods!  */
bool merc_down;                    /* Shutdown         */
char str_boot_time[MIL];
time_t current_time;            /* time of this pulse */
bool MOBtrigger = TRUE;            /* act() switch                 */


/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos args ((void));
bool read_from_descriptor args ((DESCRIPTOR_DATA * d));
bool write_to_descriptor args ((int desc, char *txt, int length));
#endif

#if defined(unix)
void game_loop_unix args ((int control));
int init_socket args ((int port));
void init_descriptor args ((int control));
bool read_from_descriptor args ((DESCRIPTOR_DATA * d));
bool write_to_descriptor args ((int desc, char *txt, int length));
#endif




/*
 * Other local functions (OS-independent).
 */
bool check_parse_name args ((char *name));
bool check_reconnect args ((DESCRIPTOR_DATA * d, char *name, bool fConn));
bool check_playing args ((DESCRIPTOR_DATA * d, char *name, bool account));
int main args ((int argc, char **argv));
void nanny args ((DESCRIPTOR_DATA * d, char *argument));
bool process_output args ((DESCRIPTOR_DATA * d, bool fPrompt));
void read_from_buffer args ((DESCRIPTOR_DATA * d));
void game_loop_unix args ((int control));
void make_who2html(void);



/* Put global mud config values here. Look at qmconfig command for clues.     */
/*   -- JR 09/23/2000                                                         */
/* Set values for all but IP address in ../area/qmconfig.rc file.             */
/*   -- JR 05/10/2001                                                         */
int mud_ansiprompt, mud_ansicolor, mud_telnetga;

/* Set this to the IP address you want to listen on (127.0.0.1 is good for    */
/* paranoid types who don't want the 'net at large peeking at their MUD)      */
char *mud_ipaddress = "0.0.0.0";

int port, control;

int main (int argc, char **argv)
{
    struct timeval now_time;
    /*
     * Memory debugging if needed.
     */
#if defined(MALLOC_DEBUG)
    malloc_debug (2);
#endif

    /*
     * Init time.
     */
    gettimeofday (&now_time, NULL);
    current_time = (time_t) now_time.tv_sec;
    strcpy (str_boot_time, ctime (&current_time));

    /*
     * Reserve one channel for our use.
     */
    if ((fpReserve = fopen (NULL_FILE, "r")) == NULL)
    {
        perror (NULL_FILE);
        exit (1);
    }

    /*
     * Get the port number.
     */
    port = 9001 ;
    if (argc > 1)
    {
        if (!is_number (argv[1]))
        {
            fprintf (stderr, "Usage: %s [port #]\n", argv[0]);
            exit (1);
        }
        else if ((port = atoi (argv[1])) <= 1024)
        {
            fprintf (stderr, "Port number must be above 1024.\n");
            exit (1);
        }
    }

#if defined(unix)    
    control = init_socket (port);           
    //load_game_world();
    boot_db();
    log_f ("Strategy game has successfully booted on port %d (%s).", port, mud_ipaddress);    
    game_loop_unix (control);
    close (control);
#endif

    log_string ("Normal termination of game.");
    exit (0);
    return 0;
}



#if defined(unix)
int init_socket (int port)
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
    int x = 1;
    int fd;

    if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror ("Init_socket: socket");
        exit (1);
    }

    if (setsockopt (fd, SOL_SOCKET, SO_REUSEADDR,
                    (char *) &x, sizeof (x)) < 0)
    {
        perror ("Init_socket: SO_REUSEADDR");
        close (fd);
        exit (1);
    }

#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
        struct linger ld;

        ld.l_onoff = 1;
        ld.l_linger = 1000;

        if (setsockopt (fd, SOL_SOCKET, SO_DONTLINGER,
                        (char *) &ld, sizeof (ld)) < 0)
        {
            perror ("Init_socket: SO_DONTLINGER");
            close (fd);
            exit (1);
        }
    }
#endif

    sa = sa_zero;
    sa.sin_family = AF_INET;
    sa.sin_port = htons (port);
    sa.sin_addr.s_addr = inet_addr( mud_ipaddress );
    //log_f("Set IP address to %s", mud_ipaddress);

    if (bind (fd, (struct sockaddr *) &sa, sizeof (sa)) < 0)
    {
        perror ("Init socket: bind");
        close (fd);
        exit (1);
    }


    if (listen (fd, 3) < 0)
    {
        perror ("Init socket: listen");
        close (fd);
        exit (1);
    }

    return fd;
}
#endif



#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos (void)
{
    struct timeval last_time;
    struct timeval now_time;
    static DESCRIPTOR_DATA dcon;

    gettimeofday (&last_time, NULL);
    current_time = (time_t) last_time.tv_sec;
    
    log_string("Last compiled on " __DATE__ " at " __TIME__".");
    /*
     * New_descriptor analogue.
     */
    dcon.descriptor = 0;
    if (!mud_ansiprompt)
        dcon.connected = CON_GET_ACCOUNT;
    else
        dcon.connected = CON_ANSI;
    dcon.ansi = mud_ansicolor;
    dcon.host = str_dup ("localhost");
    dcon.outsize = 2000;
    dcon.outbuf = alloc_mem (dcon.outsize);
    dcon.next = descriptor_list;
    dcon.showstr_head = NULL;
    dcon.showstr_point = NULL;
    dcon.pEdit = NULL;            /* OLC */
    dcon.pString = NULL;        /* OLC */
    dcon.editor = 0;            /* OLC */
    descriptor_list = &dcon;

    /*
     * First Contact!
     */
    
    extern char * help_greeting;
    if ( help_greeting[0] == '.' )
        send_to_desc ( help_greeting+1, 0 );
    else
        send_to_desc ( help_greeting  , 0 );
    

    else
        WTB (&dcon, "Do you want ANSI color? (Y)es/(N)o", 0);

    /* Main loop */
    while (!merc_down)
    {
        DESCRIPTOR_DATA *d;

        /*
         * Process input.
         */
        for (d = descriptor_list; d != NULL; d = d_next)
        {
            d_next = d->next;
            d->fcommand = FALSE;

#if defined(MSDOS)
            if (kbhit ())
#endif
            {
                if (d->account != NULL)
                    //d->account->timer = 0;
                if (!read_from_descriptor (d))
                {
                    if (d->account != NULL && d->connected == CON_PLAYING)
                        //save_char_obj (d->account);
                    d->outtop = 0;
                    close_socket (d);
                    continue;
                }
            }         

            read_from_buffer (d);
            if (d->incomm[0] != '\0')
            {
                d->fcommand = TRUE;                

                /* OLC */
                if (d->showstr_point)
                    show_string (d, d->incomm);
                else if (d->pString)
                    string_add (d->account, d->incomm);
                else
                    switch (d->connected)
                    {
                        case CON_PLAYING:
                            //interpret (d->account, argument);

                            //if (!run_olc_editor (d))
                            substitute_alias (d, d->incomm);                                                        
                            break;
                        default:
                            nanny (d, d->incomm);
                            break;
                    }

                d->incomm[0] = '\0';
            }
        }

        //update empire/city/troop information in game.
        update_handler();



        /*
         * Output.
         */
        for (d = descriptor_list; d != NULL; d = d_next)
        {
            d_next = d->next;

            if ((d->fcommand || d->outtop > 0))
            {
                if (!process_output (d, TRUE))
                {
                    if (d->account != NULL && d->connected == CON_PLAYING)
                        //save_char_obj (d->account);
                    d->outtop = 0;
                    close_socket (d);
                }
            }
        }



        /*
         * Synchronize to a clock.
         * Busy wait (blargh).
         */
        now_time = last_time;
        for (;;)
        {
            int delta;

#if defined(MSDOS)
            if (kbhit ())
#endif
            {
                if (dcon.character != NULL)
                    dcon.character->timer = 0;
                if (!read_from_descriptor (&dcon))
                {
                    //if (dcon.account != NULL && d->connected == CON_PLAYING)
                        //save_char_obj (d->account);
                    dcon.outtop = 0;
                    close_socket (&dcon);
                }
#if defined(MSDOS)
                break;
#endif
            }

            gettimeofday (&now_time, NULL);
            delta = (now_time.tv_sec - last_time.tv_sec) * 1000 * 1000
                + (now_time.tv_usec - last_time.tv_usec);
            if (delta >= 1000000 / PULSE_SECOND)
                break;
        }
        last_time = now_time;
        current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix)
void game_loop_unix (int control)
{
    static struct timeval null_time;
    struct timeval last_time;

    signal (SIGPIPE, SIG_IGN);
    gettimeofday (&last_time, NULL);
    current_time = (time_t) last_time.tv_sec;
    log_string("Last compiled on " __DATE__ " at " __TIME__".");
    
    /* Main loop */
    while (!merc_down)
    {
        fd_set in_set;
        fd_set out_set;
        fd_set exc_set;
        DESCRIPTOR_DATA *d;
        int maxdesc;

#if defined(MALLOC_DEBUG)
        if (malloc_verify () != 1)
            abort ();
#endif

        
        //Poll all active connections 
        FD_ZERO (&in_set);
        FD_ZERO (&out_set);
        FD_ZERO (&exc_set);
        FD_SET (control, &in_set);
        maxdesc = control;
        for (d = descriptor_list; d; d = d->next)
        {
            maxdesc = UMAX (maxdesc, d->descriptor);
            FD_SET (d->descriptor, &in_set);
            FD_SET (d->descriptor, &out_set);
            FD_SET (d->descriptor, &exc_set);
        }

        if (select (maxdesc + 1, &in_set, &out_set, &exc_set, &null_time) < 0)
        {
            perror ("Game_loop: select: poll");
            exit (1);
        }

        //Initialize a new descriptor for a new connection
        if (FD_ISSET (control, &in_set))
            init_descriptor (control);

        /*
         * Kick out the freaky folks.
         */
        for (d = descriptor_list; d != NULL; d = d_next)
        {
            d_next = d->next;
            if (FD_ISSET (d->descriptor, &exc_set))
            {
                FD_CLR (d->descriptor, &in_set);
                FD_CLR (d->descriptor, &out_set);
                //if (d->account && d->connected == CON_PLAYING)
                    //save_char_obj (d->account);
                d->outtop = 0;
                close_socket (d);
            }
        }

       //Begin processing input from each connection
        for (d = descriptor_list; d != NULL; d = d_next)
        {
            d_next = d->next;
            d->fcommand = FALSE;

            if (FD_ISSET (d->descriptor, &in_set))
            {                
                if (!read_from_descriptor (d))
                {
                    FD_CLR (d->descriptor, &out_set);
                    //if (d->account != NULL && d->connected == CON_PLAYING)
                        //save_char_obj (d->account);
                    d->outtop = 0;
                    close_socket (d);
                    continue;
                }
            }          

            read_from_buffer (d);
            if (d->incomm[0] != '\0')
            {
                d->fcommand = TRUE;
                
                if ( d->pProtocol != NULL )
                    d->pProtocol->WriteOOB = 0;
                

                /* OLC */
                if (d->showstr_point)
                    show_string (d, d->incomm);
                else if (d->pString)
                    string_add (d->account, d->incomm);
                else
                    switch (d->connected)
                    {
                        case CON_PLAYING:                            
                            interpret (d->account, d->incomm);
                            //substitute_alias (d, d->incomm);
                            break;
                        default:
                            nanny (d, d->incomm);
                            break;
                    }

                d->incomm[0] = '\0';
            }
        }



        /*
         * Autonomous game motion.
         */
        update_handler ();



        /*
         * Output.
         */
        for (d = descriptor_list; d != NULL; d = d_next)
        {
            d_next = d->next;

            if ((d->fcommand || d->outtop > 0)
                && FD_ISSET (d->descriptor, &out_set))
            {
                if (!process_output (d, TRUE))
                {
                    //if (d->account != NULL && d->connected == CON_PLAYING)
                        //save_char_obj (d->account);
                    d->outtop = 0;
                    close_socket (d);
                }
            }
        }



        /*
         * Synchronize to a clock.
         * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
         * Careful here of signed versus unsigned arithmetic.
         */
        {
            struct timeval now_time;
            long secDelta;
            long usecDelta;

            gettimeofday (&now_time, NULL);
            usecDelta = ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
                + 1000000 / PULSE_SECOND;
            secDelta = ((int) last_time.tv_sec) - ((int) now_time.tv_sec);
            while (usecDelta < 0)
            {
                usecDelta += 1000000;
                secDelta -= 1;
            }

            while (usecDelta >= 1000000)
            {
                usecDelta -= 1000000;
                secDelta += 1;
            }

            if (secDelta > 0 || (secDelta == 0 && usecDelta > 0))
            {
                struct timeval stall_time;

                stall_time.tv_usec = usecDelta;
                stall_time.tv_sec = secDelta;
                if (select (0, NULL, NULL, NULL, &stall_time) < 0)
                {
                    perror ("Game_loop: select: stall");
                    exit (1);
                }
            }
        }

        gettimeofday (&last_time, NULL);
        current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix)

void init_descriptor (int control)
{
    char buf[MSL];
    DESCRIPTOR_DATA *dnew;
    struct sockaddr_in sock;
    struct hostent *from;
    size_t desc, size;

    size = sizeof (sock);
    getsockname (control, (struct sockaddr *)&sock, &size);
    if ((desc = accept (control, (struct sockaddr *) &sock, &size)) < 0)
    {
        perror ("New_descriptor: accept");
        return;
    }    

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

    if (fcntl (desc, F_SETFL, FNDELAY) == -1)
    {
        perror ("New_descriptor: fcntl: FNDELAY");
        return;
    }

    /*
     * Cons a new descriptor.
     */
    dnew = new_descriptor ();

    dnew->descriptor = desc;
    dnew->connected = CON_INTRO;    
    dnew->showstr_head = NULL;
    dnew->showstr_point = NULL;
    dnew->outsize = 2000;
    dnew->pEdit = NULL;            /* OLC */
    dnew->pString = NULL;        /* OLC */
    dnew->editor = 0;            /* OLC */	
    dnew->outbuf = (char *)alloc_mem (dnew->outsize);
    dnew->pProtocol     = ProtocolCreate(); 
    dnew->color = 1;	

    size = sizeof (sock);
    if (getpeername (desc, (struct sockaddr *) &sock, &size) < 0)
    {
        perror ("New_descriptor: getpeername");
        dnew->host = str_dup ("(unknown)");
    }
    else
    {
        /*
         * Would be nice to use inet_ntoa here but it takes a struct arg,
         * which ain't very compatible between gcc and system libraries.
         */
        int addr;

        addr = ntohl (sock.sin_addr.s_addr);
        sprintf (buf, "%d.%d.%d.%d",
                 (addr >> 24) & 0xFF, (addr >> 16) & 0xFF,
                 (addr >> 8) & 0xFF, (addr) & 0xFF);
        sprintf (log_buf, "Sock.sinaddr:  %s", buf);
        log_string (log_buf);
        from = gethostbyaddr ((char *) &sock.sin_addr,
                              sizeof (sock.sin_addr), AF_INET);
        dnew->host = str_dup (from ? from->h_name : buf);
    }

    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    if (check_ban (dnew->host, BAN_ALL))
    {
        write_to_descriptor (desc, "Your site has been banned from this mud.\r\n",
                             0);
        close (desc);
        free_descriptor (dnew);
        return;
    }
    /*
     * Init descriptor data.
     */
    dnew->next = descriptor_list;
    descriptor_list = dnew;

    ProtocolNegotiate(dnew); 

    
    //Check for mxp
    //WTB(dnew, mxp_will, 0);
    
    /*
     * First Contact!
     */
    
    extern char * help_greeting;
    if (!IS_NULLSTR(help_greeting))
        WTB ( dnew, help_greeting+1, 0);
    
    
    intro_menu(dnew);

    return;
}
#endif



void close_socket (DESCRIPTOR_DATA * dclose)
{
    ACCOUNT_DATA * account;

    if (dclose->outtop > 0)
        process_output (dclose, FALSE);

    if (dclose->snoop_by != NULL)
        WTB (dclose->snoop_by, "Your victim has left the game.\r\n", 0);
    

    {
        DESCRIPTOR_DATA *d;

        for (d = descriptor_list; d != NULL; d = d->next)
        {
            if (d->snoop_by == dclose)
                d->snoop_by = NULL;
        }
    }

    if ((account = dclose->account) != NULL)
    {
        sprintf (log_buf, "Closing link to %s.", account->name);
        log_string (log_buf);
       
        /* cut down on wiznet spam when rebooting */
        /* If ch is writing note or playing, just lose link otherwise clear char */
        if ( (dclose->connected == CON_PLAYING && !merc_down)  )
        {            
            wiznet ("Net death has claimed $N.", account, WIZ_LINKS, 0);			
            account->d = NULL;
        }
        
        free_account (dclose->account);
    }

    if (d_next == dclose)
        d_next = d_next->next;

    if (dclose == descriptor_list)
    {
        descriptor_list = descriptor_list->next;
    }
    else
    {
        DESCRIPTOR_DATA *d;

        for (d = descriptor_list; d && d->next != dclose; d = d->next);
        if (d != NULL)
            d->next = dclose->next;
        else
            bug ("Close_socket: dclose not found.", 0);
    }

    ProtocolDestroy( dclose->pProtocol ); 
    
    close (dclose->descriptor);
    free_descriptor (dclose);

    return;
}



bool read_from_descriptor (DESCRIPTOR_DATA * d)
{
    int iStart;
    
    static char read_buf[MAX_PROTOCOL_BUFFER]; /* <--- Add this line */
    read_buf[0] = '\0';                        /* <--- Add this line */

    /* Hold horses if pending command already. */
    if (d->incomm[0] != '\0')
        return TRUE;

    /* Check for overflow. */
    iStart = 0;
    if ( strlen(d->inbuf) >= sizeof(d->inbuf) - 10 )
    {
        sprintf (log_buf, "%s input overflow!", d->host);
        log_string (log_buf);
        write_to_descriptor (d->descriptor,
                             "\r\n*** PUT A LID ON IT!!! ***\r\n", 0);
        return FALSE;
    }

    /* Snarf input. */
#if defined(macintosh)
    for (;;)
    {
        int c;
        c = getc (stdin);
        if (c == '\0' || c == EOF)
            break;
        putc (c, stdout);
        if ( c == '\r' )
            putc( '\n', stdout );
        read_buf[iStart++] = c;
        if (iStart > sizeof (d->inbuf) - 10)
            break;
    }
#endif

#if defined(MSDOS) || defined(unix)
    for (;;)
    {
        int nRead;

        nRead = read( d->descriptor, read_buf + iStart,
        sizeof(read_buf) - 10 - iStart );
        if ( nRead > 0 )
        {
            iStart += nRead;
            if ( read_buf[iStart-1] == '\n' || read_buf[iStart-1] == '\r' )
            break;
        }
        else if (nRead == 0)
        {
            log_string ("EOF encountered on read.");
            return FALSE;
        }
        else if (errno == EWOULDBLOCK)
            break;
        else
        {
            perror ("Read_from_descriptor");
            return FALSE;
        }
    }
#endif

    read_buf[iStart] = '\0';
    ProtocolInput( d, read_buf, iStart, d->inbuf );
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer (DESCRIPTOR_DATA * d)
{
    int i, j;    
    unsigned int k;

    //Tilde Stuff
    i=0;
	while ( d->inbuf[i] != '~' && d->inbuf[i] != '\0' )
	    i++;
    if (d->inbuf[i]=='~')
    {
        if (i==0) /*tilde at start */
        {
            i++;
            for (j=0;(d->inbuf[j] = d->inbuf[i+j] )!= '\0'; j++ )
                ;
            return;
        }
        else
        {
            if (d->inbuf[i-1] != '\\')
            {
                i++;
                for (j=0;(d->inbuf[j] = d->inbuf[i+j]) != '\0'; j++)
                    ;
                return;
            }
        }
    }

    /*
     * Hold horses if pending command already.
     */
    if (d->incomm[0] != '\0')
        return;

    /*
     * Look for at least one new line.
     */
    for (i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++)
    {
        if (d->inbuf[i] == '\0')
            return;
    }

    /*
     * Canonical input processing.
     */
    for (i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++)
    {
        if (k >= MIL - 2)
        {
            write_to_descriptor (d->descriptor, "Line too long.\r\n", 0);

            /* skip the rest of the line */
            for (; d->inbuf[i] != '\0'; i++)
            {
                if (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
                    break;
            }
            d->inbuf[i] = '\n';
            d->inbuf[i + 1] = '\0';
            break;
        }

        /*if (d->inbuf[i] == '\b' && k > 0)
            --k;
        else if (isascii (d->inbuf[i]) && isprint (d->inbuf[i]))
            d->incomm[k++] = d->inbuf[i];
        */
        
        if ( d->inbuf[i] == (signed char) IAC)
        {
          if (!memcmp(&d->inbuf[i], mxp_do, strlen(mxp_do)))
          {
            i += strlen(mxp_do) - 1;
            init_mxp(d);
          }
          else if (!memcmp(&d->inbuf[i], mxp_dont, strlen(mxp_dont)))
          {
            i += strlen(mxp_dont) - 1;
            shutdown_mxp(d);
          }
        }
        else if ( d->inbuf[i] == '\b' && k > 0 )
            --k;
        else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
            d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if (k == 0)
        d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */

    if (k > 1 || d->incomm[0] == '!')
    {
        if (d->incomm[0] != '!' && strcmp (d->incomm, d->inlast))
        {
            d->repeat = 0;
        }
        else
        {
            if (++d->repeat >= 25 && d->account
                && d->connected == CON_PLAYING)
            {
                sprintf (log_buf, "%s input spamming!", d->host);
                log_string (log_buf);
                wiznet ("Spam spam spam $N spam spam spam spam spam!", d->account, WIZ_SPAM, 0);
                if (d->incomm[0] == '!')
                    wiznet (d->inlast, d->account, WIZ_SPAM, 0);
                else
                    wiznet (d->incomm, d->account, WIZ_SPAM, 0);

                d->repeat = 0;
            }
        }
    }


    /*
     * Do '!' substitution.
     */
    if (d->incomm[0] == '!')
        strcpy (d->incomm, d->inlast);
    else
        strcpy (d->inlast, d->incomm);

    /*
     * Shift the input buffer.
     */
    while (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
        i++;
    for (j = 0; (d->inbuf[j] = d->inbuf[i + j]) != '\0'; j++);
    return;
}



/*
 * Low level output function.
 */
bool process_output (DESCRIPTOR_DATA * d, bool fPrompt)
{
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
    if ( d->pProtocol->WriteOOB ) /* <-- Add this, and the ";" and "else" */
        ; /* The last sent data was OOB, so do NOT draw the prompt */    
    else if (!merc_down)
    {
        if (d->showstr_point)
            WTB (d, "\r\n[Hit Return to continue]\r\n", 0);
        else if (fPrompt && d->pString && d->connected == CON_PLAYING)
            WTB (d, "> ", 2);
        else if (fPrompt && d->connected == CON_PLAYING)
        {            
            empire_prompt(d->account);
        }
    }

    /*
     * Short-circuit if nothing to write.
     */
    if (d->outtop == 0)
        return TRUE;

    /*
     * Snoop-o-rama.
     */
    if (d->snoop_by != NULL)
    {
        if (d->account != NULL)
            WTB (d->snoop_by, d->account->name, 0);
        WTB (d->snoop_by, "> ", 2);
        WTB (d->snoop_by, d->outbuf, d->outtop);
    }

    /*
     * OS-dependent output.
     */
    if (!write_to_descriptor (d->descriptor, d->outbuf, d->outtop))
    {
        d->outtop = 0;
        return FALSE;
    }
    else
    {
        d->outtop = 0;
        return TRUE;
    }
}

/*
 * Append onto an output buffer.
 */
void WTB (DESCRIPTOR_DATA * d, const char *txt, int length)
{
    int origlength = 0;
     
    txt = ProtocolOutput( d, txt, &length );  /* <--- Add this line */
    if ( d->pProtocol->WriteOOB > 0 )         /* <--- Add this line */
        --d->pProtocol->WriteOOB;             /* <--- Add this line */

    
    //Find length in case caller didn't.     
    if (length <= 0)
        length = strlen (txt);

    origlength = length;
    // work out how much we need to expand/contract it
    //if (d->mxp)
    length += count_mxp_tags (d->mxp, txt, length);
        
    
    //Initial \r\n if needed.
    
    if ( d->outtop == 0 && !d->fcommand && !d->pProtocol->WriteOOB )
    {
        d->outbuf[0] = '\r';
        d->outbuf[1] = '\n';
        d->outtop = 2;
    }


    //Expand the buffer as needed.
    while (d->outtop + length >= d->outsize)
    {
        char *outbuf;

        if (d->outsize >= 32000)
        {
            bug ("Buffer overflow. Closing.\r\n", 0);
            close_socket (d);
            return;
        }
        outbuf = (char *)alloc_mem (2 * d->outsize);
        strncpy (outbuf, d->outbuf, d->outtop);
        free_mem (d->outbuf, d->outsize);
        d->outbuf = outbuf;
        d->outsize *= 2;
    }

    // Copy
    //if (d->mxp)
        convert_mxp_tags (d->mxp, d->outbuf + d->outtop, txt, origlength );    
    d->outtop += length;
    return;
}



/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor (int desc, char *txt, int length)
{
    int iStart;
    int nWrite;
    int nBlock;

#if defined(macintosh) || defined(MSDOS)
    if (desc == 0)
        desc = 1;
#endif

    if (length <= 0)
        length = strlen (txt);

    for (iStart = 0; iStart < length; iStart += nWrite)
    {
        nBlock = UMIN (length - iStart, 4096);
        if ((nWrite = write (desc, txt + iStart, nBlock)) < 0)
        {
            perror ("Write_to_descriptor");
            return FALSE;
        }
    }

    return TRUE;
}



void log_f (char *fmt, ...)
{
    char buf[2 * MSL];
    va_list args;
    va_start (args, fmt);
    vsprintf (buf, fmt, args);
    va_end (args);

    log_string (buf);
}

/*
 * Parse a name for acceptability.
 */
bool check_parse_name (char *name)
{
    /*
     * Reserved words.
     */
    if (is_name (name,
                    "all auto immortal self someone something the you loner none aragond no yes hello bye it lit lot pal lap ooc tell reply"))					  
        return FALSE;
    
    //Restricted words.
    if (is_name (name,
                    "death demise balance honor kill slay slayer conclave guild clan cabal kingdom evil good neutral new rear front flank"))					  
        return FALSE;
    
    //Rom names
    if (is_name (name,
                    "alander liralen onivel erwin andreason zump"))
        return FALSE;
    
    //Curse words
    if (is_name (name,
                    "butt dick pussy faggot ass whore slut fag gay shit fuck bitch cunt clit queer prick bastard cock kuk cuck cis lgbtq lgbt"))		    
        return FALSE;
    
    //Egyptian Gods
    if (is_name (name,
                    "amun anubis aten atum bastet bes geb hapy hathor horus isis khepri khnum nun osiris ra sekhmet seshat seth shu thoth"))		    
        return FALSE;
    
    //King Arthur
    if (is_name (name,
                    "arthur merlin morgana lancelot gawain galahad percival tristram bors ector mordred"))		    
        return FALSE;
    
    //Norse Gods
    if (is_name (name,
                    "baldr odin njord freyr frigg freyja freya nanna skade skadi svipdag hermod bor thor tyr heimdall ulle sif bragi idun balder ve vili vidar hod forseti aegir ran hel fenrir jormangund"))		    
        return FALSE;
    
    //Roman Gods
    if (is_name (name,
                    "apollo ceres diana juno jupiter mars mercury minerva neptune venus vesta vulcan"))		    
        return FALSE;
    
    //Greek Gods
    if (is_name (name,
                    "aphrodite ares artemis athena atlas chronus cronus demeter dionysus eros hades helios hephaestus hera hermes hestia nyx poseidon prometheus styx uranus zeus"))
        return FALSE;
    
    //Mythological
    if (is_name (name,
                    "medusa sphynx sphinx"))
        return FALSE;
    
    //Tolkien Names
    if (is_name (name,
                    "frodo bilbo thorin gimli aragorn legolas elrond sauron gollum smaug morgoth galadriel luthien beren vala samwise merry pippin nazgul ulmo eru iluvatar turin celebrimbor beorn manwe boromir radagast elros"))
        return FALSE;
    
    //Dragonlance
    if (is_name (name,
                    "regis "))
        return FALSE;
    
    //RA Salvatore
    if (is_name (name,
                    "alustriel artemis belwar brizza cattiebrie drizzt bruenor hallistra pharaun denin gelroos guenhwyvar jarlaxle baenre malice sinafey wulfgar zaknafein vierna pwent "))
        return FALSE;
    
    //AD&D Pantheon
    if (is_name (name,
                    "lloth majere habbakuk mielikki ehlonna corellon solonor iuz nerull reorx morgion istus gruumsh hiddukel issek baphomet solinari zeboim lolth chemosh dispater orcus sargonnas"))
        return FALSE;
    
    
    //Famous AD&D Game Characters
    if (is_name (name,
                    "bigby rary otiluke nystul vecna melf elminster magius evard leomund mordenkainen boccob riggby tenser drawmij strahd"))
        return FALSE;
    
    //Real Names
    if (is_name (name,
                    "drew derek sam samuel sammy david john jeremy jared jackson jimmy jim james paul tom thomas ken kenny kenneth larry bobby bob robert randy jon ned edward steve steven stephen anthony tony brian bryan ryan alan allen adam michael mike mikey"))
        return FALSE;

    //Female Real Names
    if (is_name (name,
                    "tammy sammi toni becky rebecca tori tory kathy cathy susie susan jennifer jenny "))
        return FALSE;
        
    //Colors...
    if (is_name (name, 
                    "black blue brown green white orange lilac magenta cerulean lavender charcoal tope pink purple yellow red maroon"))
        return FALSE;
                    
    //System type names
    if (is_name (name,
                    "sysop admin root administrator test tester mudop opsys sudo su make cd ps ls rm"))
        return FALSE;
    
    /*
     * Length restrictions.
     */

    if (strlen (name) < 2)
        return FALSE;

#if defined(MSDOS)
    if (strlen (name) > 8)
        return FALSE;
#endif

#if defined(macintosh) || defined(unix)
    if (strlen (name) > 12)
        return FALSE;
#endif

    if (strlen(name) > MAX_NAME_LEN)
        return FALSE;

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
        char *pc;
        bool fIll, adjcaps = FALSE, cleancaps = FALSE;
        int total_caps = 0;

        fIll = TRUE;
        for (pc = name; *pc != '\0'; pc++)
        {
            if (!isalpha (*pc))
                return FALSE;

            if (isupper (*pc))
            {                    /* ugly anti-caps hack */
                if (adjcaps)
                    cleancaps = TRUE;
                total_caps++;
                adjcaps = TRUE;
            }
            else
                adjcaps = FALSE;

            if (LOWER (*pc) != 'i' && LOWER (*pc) != 'l')
                fIll = FALSE;
        }

        if (fIll)
            return FALSE;

        if (cleancaps
            || (total_caps > (strlen (name)) / 2
                && strlen (name) < 3)) return FALSE;
    }

    if (descriptor_list)
    {
        int count=0;
        DESCRIPTOR_DATA *d, *dnext;

        for (d = descriptor_list; d != NULL; d = dnext)
        {
            dnext=d->next;
            if (d->connected!=CON_PLAYING&&d->account&&d->account->name
                && d->account->name[0] && !str_cmp(d->account->name,name))
            {
                count++;
                close_socket(d);
            }
        }
        if (count)
        {
            sprintf(log_buf,"Double newbie alert (%s)",name);
            wiznet(log_buf,NULL,WIZ_LOGINS,0);

            return FALSE;
        }
    }

    return TRUE;
}



/*
 * Look for link-dead account to reconnect.
 */
bool check_reconnect (DESCRIPTOR_DATA * d, char *name, bool fConn)
{    
    ACCOUNT_DATA * account;

    for (account = account_list; account != NULL; account = account->next)
    {
        if ((!fConn || account->d == NULL)
            && !str_cmp (d->account->name, account->name))
        {            
            free_account (d->account);
            d->account = account;
            account->d = d;            
            SEND ("Reconnecting. Type replay to see missed tells.\r\n", account);                
            sprintf (log_buf, "%s@%s reconnected.", account->name, d->host);
            log_string (log_buf);
            wiznet ("$N groks the fullness of $S link.", account, WIZ_LINKS, 0);
            d->connected = CON_PLAYING;                                            
            return TRUE;
        }
    }

    return FALSE;
}

/*
 * Check if already playing.
 */
bool check_playing (DESCRIPTOR_DATA * d, char *name, bool account )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold; dold = dold->next )
    {
    if ( dold != d
    &&   dold->account != NULL
    &&   dold->connected != CON_GET_NAME
    &&   dold->connected != CON_GET_OLD_PASSWORD )
    {
        if ( account && !ACC_IS_SET(d->account->act, ACC_IMP) )
        {
            WTB(d, "You already have another character playing from your account.\n\r",0);
            WTB( d, "Please enter Y to accept and connect. (enter Y)",0);
        }
        else
        {
            WTB( d, "That character is already playing.\n\r",0);
            WTB( d, "Do you wish to connect anyway (Y/N)?",0);
        }
        d->connected = CON_BREAK_CONNECT;
        return TRUE;
    }
    }
    return FALSE;
}

void send_to_world (const char *txt)
{
    ACCOUNT_DATA * account;

    for (account = empire_list; account != NULL; account = account->next)
    {
        if (!account)
            continue;

        const char *point;
        char *point2;
        char buf[MSL * 4];
        int skip = 0;

        buf[0] = '\0';
        point2 = buf;
        if (txt && account->d)
        {            
            for (point = txt; *point; point++)
            {
                if (*point == '{')
                {
                    point++;
                    skip = colour (*point, account, point2);
                    while (skip-- > 0)
                        ++point2;
                    continue;
                }
                *point2 = *point;
                *++point2 = '\0';
            }
            *point2 = '\0';
            WTB (account->d, buf, point2 - buf);            
        }
        
    }
}

void send_to_account (const char *txt, ACCOUNT_DATA * account)
{
    const char *point;
    char *point2;
    char buf[MSL * 4];
    int skip = 0;

    buf[0] = '\0';
    point2 = buf;
    if (txt && account->d)
    {        
        for (point = txt; *point; point++)
        {
            if (*point == '{')
            {
                point++;
                skip = colour (*point, account, point2);
                while (skip-- > 0)
                    ++point2;
                continue;
            }
            *point2 = *point;
            *++point2 = '\0';
        }
        *point2 = '\0';
        WTB (account->d, buf, point2 - buf);
    }
    return;
}

/*
 * Page to one descriptor using Lope's color.
 */
void send_to_desc (const char *txt, DESCRIPTOR_DATA * d)
{
    const char *point;
    char *point2;
    char buf[MSL * 4];
    int skip = 0;

    buf[0] = '\0';
    point2 = buf;
    if (txt && d)
    {
        if (d->color == TRUE)
        {
            for (point = txt; *point; point++)
            {
                if (*point == '{')
                {
                    point++;
                    skip = colour (*point, NULL, point2);
                    while (skip-- > 0)
                        ++point2;
                    continue;
                }
                *point2 = *point;
                *++point2 = '\0';
            }
            *point2 = '\0';
            WTB (d, buf, point2 - buf);
        }
        else
        {
            for (point = txt; *point; point++)
            {
                if (*point == '{')
                {
                    point++;
                    continue;
                }
                *point2 = *point;
                *++point2 = '\0';
            }
            *point2 = '\0';
            WTB (d, buf, point2 - buf);
        }
    }
    return;
}

/* string pager */
void show_string (struct descriptor_data *d, char *input)
{
    char buffer[4 * MSL];
    char buf[MIL];
    register char *scan, *chk;
    int lines = 0, toggle = 1;
    int show_lines;

    one_argument (input, buf);
    if (buf[0] != '\0')
    {
        if (d->showstr_head)
        {
            free_mem (d->showstr_head, strlen (d->showstr_head));
            d->showstr_head = 0;
        }
        d->showstr_point = 0;
        return;
    }

    if (d->account)
        show_lines = d->account->lines;
    else
        show_lines = 0;

    for (scan = buffer;; scan++, d->showstr_point++)
    {
        if (((*scan = *d->showstr_point) == '\n' || *scan == '\r')
            && (toggle = -toggle) < 0)
            lines++;

        else if (!*scan || (show_lines > 0 && lines >= show_lines))
        {
            *scan = '\0';
            WTB (d, buffer, strlen (buffer));
            for (chk = d->showstr_point; isspace (*chk); chk++)
            {
                if (!*chk)
                {
                    if (d->showstr_head)
                    {
                        free_mem (d->showstr_head, strlen (d->showstr_head));
                        d->showstr_head = 0;
                    }
                    d->showstr_point = 0;
                }
            }
            return;
        }
    }
    return;
}

int colour (char type, ACCOUNT_DATA * account, char *string)
{
    //PC_DATA *col;
    char code[20];
    char *p = '\0';

    if (!account)
        return (0);

    //col = ch ? ch->pcdata : NULL;

    switch (type)
    {
        default:
        case 'x':
            strcpy (code, CLEAR);
            break;        
        case 'p':
            strcpy (code, C_PURPLE);
            break;
        case 'P':
            strcpy (code, C_PINK);
            break;
        case 'a':
            strcpy (code, C_CERULEAN);
            break;
        case 'A':
            strcpy (code, C_AQUAMARINE);
            break;
        case 'o':
            strcpy (code, C_DARK_ORANGE);
            break;
        case 'O':
            strcpy (code, C_LIGHT_ORANGE);
            break;
        case 'u':
            strcpy (code, C_BROWN);
            break;
        case 'U':
            strcpy (code, C_GOLD);
            break;
        case 'b':
            strcpy (code, C_BLUE);
            break;
        case 'c':
            strcpy (code, C_CYAN);
            break;
        case 'g':
            strcpy (code, C_GREEN);
            break;
        case 'm':
            strcpy (code, C_MAGENTA);
            break;
        case 'r':
            strcpy (code, C_RED);
            break;
        case 'k':
            strcpy (code, C_BRICK_RED);
            break;
        case 'w':
            strcpy (code, C_WHITE);
            break;
        case 'y':
            strcpy (code, C_YELLOW);
            break;
        case 'B':
            strcpy (code, C_B_BLUE);
            break;
        case 'C':
            strcpy (code, C_B_CYAN);
            break;
        case 'G':
            strcpy (code, C_B_GREEN);
            break;
        case 'M':
            strcpy (code, C_B_MAGENTA);
            break;
        case 'R':
            strcpy (code, C_B_RED);
            break;
        case 'W':
            strcpy (code, C_B_WHITE);
            break;
        case 'Y':
            strcpy (code, C_B_YELLOW);
            break;
        case 'D':
            strcpy (code, C_D_GREY);
            break;
        case '1':
            strcpy (code, BG_BLACK);
            break;
        case '2':
            strcpy (code, BG_RED);
            break;
        case '3':
            strcpy (code, BG_GREEN);
            break;
        case '4':
            strcpy (code, BG_YELLOW);
            break;
        case '5':
            strcpy (code, BG_BLUE);
            break;
        case '6':
            strcpy (code, BG_MAGENTA);
            break;
        case '7':
            strcpy (code, BG_CYAN);
            break;
        case '8':
            strcpy (code, BG_WHITE);
            break;    
        case 'd':          
            strcpy (code, C_DARKER_GREY);
            break;
        case 'l':          
            strcpy (code, C_LIGHT_GREEN);
            break;
        case 'F':          
            strcpy (code, C_FUSCHIA);
            break;
        case 'f':          
            strcpy (code, C_FLESHTONE);
            break;
        case 't':          
            strcpy (code, C_TEAL);
            break;
        case 's':          
            strcpy (code, C_SEPIA);
            break;
        case '*':
            sprintf (code, "%c", '\a');
            break;
        case '/':
            strcpy (code, "\r\n");
            break;
        case '-':
            sprintf (code, "%c", '~');
            break;
        case '{':
            sprintf (code, "%c", '{');
            break;		
    }

    p = code;
    while (*p != '\0')
    {
        *string = *p++;
        *++string = '\0';
    }

    return (strlen (code));
}

void colourconv (char *buffer, const char *txt, ACCOUNT_DATA * account)
{
    const char *point;
    int skip = 0;

    if (account && account->d && txt)
    {
        for (point = txt; *point; point++)
        {
            if (*point == '{')
            {
                point++;
                if (*point != '\n') {
                    skip = colour (*point, account, buffer);
                    while (skip-- > 0)
                            ++buffer;
                    continue;
                }
            }
            *buffer = *point;
            *++buffer = '\0';
        }
        *buffer = '\0';        
    }
    return;
}


void printf_to_desc (DESCRIPTOR_DATA * d, char *fmt, ...)
{
    char buf[MSL];
    va_list args;
    va_start (args, fmt);
    vsprintf (buf, fmt, args);
    va_end (args);

    send_to_desc (buf, d);
}

void bugf (char *fmt, ...)
{
    char buf[2 * MSL];
    va_list args;
    va_start (args, fmt);
    vsprintf (buf, fmt, args);
    va_end (args);

    bug (buf, 0);
}


int count_mxp_tags (const int bMXP, const char *txt, int length)
  {
  char c;
  const char * p;
  int count;
  int bInTag = FALSE;
  int bInEntity = FALSE;

  for (p = txt, count = 0; 
       length > 0; 
       p++, length--)
    {
    c = *p;

    if (bInTag)  /* in a tag, eg. <send> */
      {
      if (!bMXP)
        count--;     /* not output if not MXP */   
      if (c == MXP_ENDc)
        bInTag = FALSE;
      } /* end of being inside a tag */
    else if (bInEntity)  /* in a tag, eg. <send> */
      {
      if (!bMXP)
        count--;     /* not output if not MXP */   
      if (c == ';')
        bInEntity = FALSE;
      } /* end of being inside a tag */
    else switch (c)
      {

      case MXP_BEGc:
        bInTag = TRUE;
        if (!bMXP)
          count--;     /* not output if not MXP */   
        break;

      case MXP_ENDc:   /* shouldn't get this case */
        if (!bMXP)
          count--;     /* not output if not MXP */   
        break;

      case MXP_AMPc:
        bInEntity = TRUE;
        if (!bMXP)
          count--;     /* not output if not MXP */   
        break;

      default:
        if (bMXP)
          {
          switch (c)
            {
            case '<':       /* < becomes &lt; */
            case '>':       /* > becomes &gt; */
              count += 3;    
              break;

            case '&':
              count += 4;    /* & becomes &amp; */
              break;

            case '"':        /* " becomes &quot; */
              count += 5;    
              break;

            } /* end of inner switch */
          }   /* end of MXP enabled */
      } /* end of switch on character */

     }   /* end of counting special characters */

  return count;
  } /* end of count_mxp_tags */



  void convert_mxp_tags (const int bMXP, char * dest, const char *src, int length)
  {
char c;
const char * ps;
char * pd;
int bInTag = FALSE;
int bInEntity = FALSE;

  for (ps = src, pd = dest; 
       length > 0; 
       ps++, length--)
    {
    c = *ps;
    if (bInTag)  /* in a tag, eg. <send> */
      {
      if (c == MXP_ENDc)
        {
        bInTag = FALSE;
        if (bMXP)
          *pd++ = '>';
        }
      else if (bMXP)
        *pd++ = c;  /* copy tag only in MXP mode */
      } /* end of being inside a tag */
    else if (bInEntity)  /* in a tag, eg. <send> */
      {
      if (bMXP)
        *pd++ = c;  /* copy tag only in MXP mode */
      if (c == ';')
        bInEntity = FALSE;
      } /* end of being inside a tag */
    else switch (c)
      {
      case MXP_BEGc:
        bInTag = TRUE;
        if (bMXP)
          *pd++ = '<';
        break;

      case MXP_ENDc:    /* shouldn't get this case */
        if (bMXP)
          *pd++ = '>';
        break;

      case MXP_AMPc:
        bInEntity = TRUE;
        if (bMXP)
          *pd++ = '&';
        break;

      default:
        if (bMXP)
          {
          switch (c)
            {
            case '<':
              memcpy (pd, "&lt;", 4);
              pd += 4;    
              break;

            case '>':
              memcpy (pd, "&gt;", 4);
              pd += 4;    
              break;

            case '&':
              memcpy (pd, "&amp;", 5);
              pd += 5;    
              break;

            case '"':
              memcpy (pd, "&quot;", 6);
              pd += 6;    
              break;

            default:
              *pd++ = c;
              break;  /* end of default */

            } /* end of inner switch */
          }
        else
          *pd++ = c;  /* not MXP - just copy character */
        break;  

      } /* end of switch on character */

    }   /* end of converting special characters */
  } /* end of convert_mxp_tags */
  
  

bool check_ban (char *site, int type)
{
    BAN_DATA *pban;
    char host[MSL];

    strcpy (host, capitalize (site));
    host[0] = LOWER (host[0]);

    for (pban = ban_list; pban != NULL; pban = pban->next)
    {
        if (!IS_SET (pban->ban_flags, type))
            continue;

        if (IS_SET (pban->ban_flags, BAN_PREFIX)
            && IS_SET (pban->ban_flags, BAN_SUFFIX)
            && strstr (pban->name, host) != NULL)
            return TRUE;

        if (IS_SET (pban->ban_flags, BAN_PREFIX)
            && !str_suffix (pban->name, host))
            return TRUE;

        if (IS_SET (pban->ban_flags, BAN_SUFFIX)
            && !str_prefix (pban->name, host))
            return TRUE;
    }

    return FALSE;
}

void substitute_alias (DESCRIPTOR_DATA * d, char *argument)
{
    ACCOUNT_DATA * account;
    char buf[MSL], prefix[MIL], name[MIL];
    char *point;
    int alias;

    account = d->account;

    /* check for prefix */
    if (account->prefix[0] != '\0' && str_prefix ("prefix", argument))
    {
        if (strlen (account->prefix) + strlen (argument) > MIL - 2)
            SEND ("Line to long, prefix not processed.\r\n", account);
        else
        {
            sprintf (prefix, "%s %s", account->prefix, argument);
            argument = prefix;
        }
    }

    if (account->alias[0] == NULL
        || !str_prefix ("alias", argument) || !str_prefix ("una", argument)
        || !str_prefix ("prefix", argument))
    {
        interpret (d->account, argument);
        return;
    }

    strcpy (buf, argument);

    for (alias = 0; alias < MAX_ALIAS; alias++)
    {                            /* go through the aliases */
        if (account->alias[alias] == NULL)
            break;

        if (!str_prefix (account->alias[alias], argument))
        {
            point = one_argument (argument, name);
            if (!strcmp (account->alias[alias], name))
            {                
                buf[0] = '\0';
                strcat(buf,account->alias[alias]);
                if (point[0])
                {
                    strcat(buf," ");
                    strcat(buf,point);
                }

                if (strlen (buf) > MIL - 1)
                {
                    SEND
                        ("Alias substitution too long. Truncated.\r\n", account);
                    buf[MIL - 1] = '\0';
                }
                break;
            }
        }
    }
    interpret (d->account, buf);
}
