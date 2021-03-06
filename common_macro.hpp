#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

//Messages status 
# define COMPLETE 1
# define INCOMPLETE 0
# define DISCONNECT -1

#define EMO_ROBOT "\U0001F916"
#define END_CHAR "\r\n"
#define MAX_CHAR 4096
#define CHAN_MAXCAPACITY 3 // modifiable
#define USER_MAXCHAN 2 // modifiable

// Couleur pour affichage messages
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

//RPL
#define RPL_WELCOME				"001"
#define RPL_YOURHOST			"002"
#define RPL_CREATED				"003"
#define RPL_MYINFO				"004"
#define RPL_BOUNCE				"005"
#define RPL_UMODEIS				"221"
#define RPL_AWAY				"301"
#define RPL_UNAWAY				"305"
#define RPL_NOWAWAY				"306"
#define RPL_ENDOFWHO			"315"
#define RPL_LIST				"322" 
#define RPL_LISTEND				"323" 
#define RPL_NOTOPIC				"331" 
#define RPL_TOPIC				"332"
#define RPL_WHOREPLY			"352"
#define RPL_NAMEREPLY			"353"
#define RPL_ENDOFNAMES  		"366"
#define RPL_MOTD		  		"372"
#define RPL_MOTDSTART	  		"375"
#define RPL_ENDOFMOTD	  		"376"
#define RPL_YOUREOPER			"381"
#define RPL_CUSTOMCAP			"4242"
#define RPL_CUSTOMMODESUCCESS	"4243"
#define RPL_CUSTOMHELP      	"4244"
#define RPL_CUSTOMMOTD	     	"4245"

//Erros
#define ERR_NOSUCHNICK			"401"
#define ERR_NOSUCHCHANNEL		"403"
#define ERR_CANNOTSENDTOCHAN	"404"
#define ERR_TOOMANYCHANNELS		"405" 
#define ERR_TOOMANYTARGETS		"407" 
#define ERR_NORECIPIENT			"411"
#define ERR_NOTEXTTOSEND		"412"
#define ERR_UNKNOWNCOMMAND		"421"
#define ERR_NONICKNAMEGIVEN		"431"
#define ERR_ERRONEUSNICKNAME	"432"
#define ERR_NICKNAMEINUSE		"433"
#define ERR_USERNOTINCHANNEL    "441"
#define ERR_NOTONCHANNEL		"442"
#define ERR_NEEDMOREPARAMS		"461"
#define ERR_ALREADYREGISTERED	"462"
#define ERR_PASSWDMISMATCH  	"464"
#define ERR_CHANNELISFULL		"471"
#define	ERR_INVITEONLYCHAN		"473"
#define ERR_BANNEDFROMCHAN		"474"
#define ERR_BADCHANNELKEY		"475"
#define ERR_BADCHANMASK 		"476"
#define ERR_NOPRIVILEGES	    "481"
#define ERR_CHANOPRIVNEEDED     "482"
#define ERR_RESTRICTED			"484"
#define ERR_NOOPERHOST			"491"
#define ERR_UMODEUNKNOWNFLAG	"501"
#define ERR_USERSDONTMATCH		"502"
#define ERR_USERREALNAMEFORMAT	"4242"

#endif
