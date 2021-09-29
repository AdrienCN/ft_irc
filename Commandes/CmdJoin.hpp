// Class fille 
#ifndef CmdJoin_hpp
#define CmdJoin_hpp

#include "../headers.hpp"
#include "../Command.hpp"

class CmdJoin : public Command
{

    public:
        CmdJoin();
        virtual ~CmdJoin();

        void execute();
    
};

#endif
