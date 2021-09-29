// Class pour stocker les differentes ActionList possible

#ifndef ActionList_hpp
#define ActionList_hpp

#include "headers.hpp"
#include "Command.hpp"

class ActionList
{

    public:
        ActionList(); 
        virtual ~ActionList(); 
  
    protected:
        ActionList(ActionList const& src);
        ActionList& operator=(ActionList const& src); 

        std::vector<ACommand *> _action_list; // on liste toutes les commandes et les ActionList associees , un peu comme SpellBook
    };

#endif
