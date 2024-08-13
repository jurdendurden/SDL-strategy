//Defines command structure and all command prototypes.

//wrapper function for safe command execution
void do_function args((ACCOUNT_DATA * account, DO_FUN *do_fun, char *argument));


#define COM_INGORE	1


//Structure for a command in the command lookup table.
struct	cmd_type
{
    char * const		name;       //input trigger for command
    DO_FUN *			do_fun;     //function that command executes
    sh_int				log;        //log status of command
    bool                show;       //will it show up in the commands command?
    sh_int				cat;        //what type of command is it, CMD_INFO? etc
};

//The command table itself
extern	const	struct	cmd_type	cmd_table	[];

//Type of commands
#define CMD_INFO            1
#define CMD_CITY            2
#define CMD_COMM            3


//Information
DECLARE_DO_FUN(do_score);
DECLARE_DO_FUN(do_commands);
DECLARE_DO_FUN(do_city);
DECLARE_DO_FUN(do_empire);
DECLARE_DO_FUN(do_who);

//City related
DECLARE_DO_FUN(do_build);
DECLARE_DO_FUN(do_upgrade);
DECLARE_DO_FUN(do_settle);
DECLARE_DO_FUN(do_switch);
DECLARE_DO_FUN(do_trade);

//Military related
DECLARE_DO_FUN(do_recruit);

//Communication
DECLARE_DO_FUN(do_chat);
DECLARE_DO_FUN(do_timestamps);


//Test
DECLARE_DO_FUN(do_memory);
