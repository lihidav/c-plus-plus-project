//
// Created by lihidav@wincs.cs.bgu.ac.il on 25/11/2019.
//

#include <iostream>
#include <string>
#include "User.h"
#include "Session.h"
#include "Action.h"
#include "Watchable.h"
#include <sstream>

using namespace std;

BaseAction::BaseAction() :errorMsg(), status(){
    status = PENDING;

}

//return this status
ActionStatus BaseAction::getStatus() const {
    return status;
}

//return text of status
string BaseAction::getStatusConv() const {
    if (status == PENDING) { return "PENDING"; }
    else if (status == COMPLETED) { return "COMPLETE"; }
    else { return "ERROR"; }
}

//changing this status to complete
void BaseAction::complete() {
    status = COMPLETED;
}

//updating this error message and error status
void BaseAction::error(const string &errorMsg) {
    BaseAction::errorMsg = errorMsg;
    status = ERROR;
}

//return the error status message
string BaseAction::getErrorMsg() const {
    return errorMsg;
}

BaseAction::~BaseAction() {

}

BaseAction::BaseAction(BaseAction &other) :errorMsg(other.errorMsg),status(other.status) {}


//the act of createuser
void CreateUser::act(Session &sess) {
    //check if user not exist in unordered map and then the ELSE means it does
    if (sess.GetUserMap().find(sess.inputUser[1]) == sess.GetUserMap().end()) {
        if (sess.inputUser[2] == "len") {
            User *user = new LengthRecommenderUser(sess.inputUser[1]);
            sess.GetUserMap().insert({sess.inputUser[1],user});
        } else if (sess.inputUser[2] == "rer") {
            User *user = new RerunRecommenderUser(sess.inputUser[1]);
            sess.GetUserMap().insert({sess.inputUser[1],user});
        } else {
            User *user = new GenreRecommenderUser(sess.inputUser[1]);
            sess.GetUserMap().insert({sess.inputUser[1],user});
        }
        complete();

    } else {
        error("ERROR - <user already exist>");
        cout<<getErrorMsg()<<endl;
    }
    //pushing to the actionLog the Base Action pointer
    sess.GetActionLog().push_back(this);
}

//return action and status if it
string CreateUser::toString() const {
    return "createuser " + getStatusConv();
}

BaseAction *CreateUser::clone() {
    return new CreateUser(*this);
}


void ChangeActiveUser::act(Session &sess) {
    if (sess.GetUserMap().find(sess.inputUser[1]) == sess.GetUserMap().end()) {
        error("ERROR - <user not exist>");
        cout<<getErrorMsg()<<endl;
    } else {
        //User *user = sess.GetUserMap().find(sess.inputUser[1])->second;
        sess.ChangeActUser(sess.GetUserMap().find(sess.inputUser[1])->second);
        complete();
    }
    //pushing to the actionLog the Base Action pointer
    sess.GetActionLog().push_back(this);
}

//return action and status if it
string ChangeActiveUser::toString() const {
    return "changeuser " + getStatusConv();
}

BaseAction *ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}


void DeleteUser::act(Session &sess) {
    if ((sess.GetUserMap().find(sess.inputUser[1]) == sess.GetUserMap().end())) {
        error("ERROR - <user does not exist>");
        cout<<getErrorMsg()<<endl;
    } else {
        sess.GetUserMap().erase(sess.inputUser[1]);
        complete();
    }
    //pushing to the actionLog the Base Action pointer
    sess.GetActionLog().push_back(this);
}

//return action and status if it
string DeleteUser::toString() const {
    return "deteleuser " + getStatusConv();
}

BaseAction *DeleteUser::clone() {
    return new DeleteUser(*this);
}


void DuplicateUser::act(Session &sess) {
    if ((sess.GetUserMap().find(sess.inputUser[1]) == sess.GetUserMap().end())) {
        error("ERROR - <original user does not exist>");
        cout<<getErrorMsg()<<endl;
    } else if (sess.GetUserMap().find(sess.inputUser[2]) != sess.GetUserMap().end()) {
        error("ERROR - <new user name is already exist>");
        cout<<getErrorMsg()<<endl;
    } else {
        (sess.GetUserMap().find(sess.inputUser[1])->second->DupUser(sess.inputUser[2],sess));
        complete();
    }
    //pushing to the actionLog the Base Action pointer
    sess.GetActionLog().push_back(this);
}

//return action and status if it
string DuplicateUser::toString() const {
    return "dupuser " + getStatusConv();
}

BaseAction *DuplicateUser::clone() {
    return new DuplicateUser(*this);
}


void PrintContentList::act(Session &sess) {

    for (unsigned int i = 0; i < (sess.GetContent().size()); i++) {
        cout << sess.GetContent()[i]->GetWatchableInfo() << endl;
    }
    sess.GetActionLog().push_back(this);

}


//return action and status if it
string PrintContentList::toString() const {
    return "content " + getStatusConv();
}

BaseAction *PrintContentList::clone() {
    return new PrintContentList(*this);
}


void PrintWatchHistory::act(Session &sess) {
    cout << "Watch history for " << sess.GetActiveUser()->getName() << endl;
    for (size_t i = 0; i < sess.GetActiveUser()->get_history().size(); ++i) {
        cout << i + 1 << ". " << sess.GetActiveUser()->get_history()[i]->GetWatchableInfoForRec()<< endl;
    }
    complete();
    //pushing to the actionLog the Base Action pointer
    sess.GetActionLog().push_back(this);
}

//return action and status if it
string PrintWatchHistory::toString() const {
    return "watchlist " + getStatusConv();
}

BaseAction *PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}


void Watch::act(Session &sess) {

    // object from the class stringstream
    //check if we can use stringstream
    stringstream stringId(sess.inputUser[1]); //change string to int

    double id = 0;
    stringId >> id;

    //do {
    sess.inputUser.clear();
    cout << "Watching " << sess.GetContent()[id - 1]->GetWatchableInfoForRec() << endl;
    sess.GetActiveUser()->get_history().push_back(sess.GetContent()[id - 1]);
    complete();
    sess.GetActionLog().push_back(this);
    cout << "We recommend watching " << sess.GetContent()[id - 1]->getNextWatchable(sess)->GetWatchableInfoForRec()
         << " continune watching? [y/n]" << endl;
    getline(cin, sess.userInputString);
    if (sess.userInputString == "y") {
        //id = sess.GetContent()[id - 1]->getNextWatchable(sess)->getId();
        sess.inputUser[1] = to_string(sess.GetContent()[id - 1]->getNextWatchable(sess)->getId());
        BaseAction *action = new Watch;
        action->act(sess);
    }

        //}
        //} while (sess.userInputString == "y");

}

//return action and status if it
    string Watch::toString() const {
        return "watch " + getStatusConv();
    }

    BaseAction *Watch::clone() {
        return new Watch(*this);
    }

    void PrintActionsLog::act(Session &sess) {

        for (int i = sess.GetActionLog().size() - 1; i >= 0; i--) {
            cout << sess.GetActionLog()[i]->toString() << endl;
        }
        complete();
        sess.GetActionLog().push_back(this);

    }

    std::string PrintActionsLog::toString() const {
        return "log " + getStatusConv();
    }

    BaseAction *PrintActionsLog::clone() {
        return new PrintActionsLog(*this);
    }

    void Exit::act(Session &sess) {

        if (sess.inputUser[0] == "exit") { //check if needed
            complete();
        }

    }

    std::string Exit::toString() const {
        return "exit " + getStatusConv();
    }

    BaseAction *Exit::clone() {
        return new Exit(*this);
    }

