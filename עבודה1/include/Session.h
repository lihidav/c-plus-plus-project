//
// Created by lihidav@wincs.cs.bgu.ac.il on 25/11/2019.
//

#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session {
public:
    Session(const std::string& configFilePath);
    ~Session();
    Session* clone();
    Session(const Session& other); //copy constructor
    Session& operator=(const Session& other); //copy assignment operator
    Session(Session&& other); //move constructor
    Session& operator=(Session&& other); //move assignment operator
    void start();
    std::vector<std::string> inputUser;
    std::string userInputString;
    std::unordered_map<std::string, User*>  & GetUserMap();
    std::vector<BaseAction*> &GetActionLog() ;
    User* GetActiveUser();
    void ChangeActUser(User*);
    std::vector<Watchable*>& GetContent() ;
    void SetActUser(User*);
private:
    std::vector<Watchable*> content ;
    std::vector<BaseAction*> actionsLog ;
    std::unordered_map<std::string, User*> userMap ;
    User* activeUser;
};
#endif