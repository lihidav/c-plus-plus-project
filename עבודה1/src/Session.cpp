

#include "../include/Session.h"
#include <iostream>
#include <vector>
#include <unordered_map>
//#include <string>
#include <fstream>
#include <json.hpp>
#include "Action.h"
#include "User.h"
#include "Watchable.h"
#include <boost/algorithm/string.hpp>


using namespace std;
//using namespace nlohmann;




Session::Session(const string &configFilePath): inputUser(), userInputString(), content(), actionsLog(), userMap(), activeUser(nullptr) {
    using json = nlohmann::json; //check problem with vectors
    ifstream file(configFilePath);
    json jsonContent;
    file >> jsonContent;
    //file.close();


    long movieId = 0;

    while (jsonContent["movies"][movieId] != nullptr) {
        string name = jsonContent["movies"][movieId]["name"];
        int length = jsonContent["movies"][movieId]["length"];
        vector<string> tags = jsonContent["movies"][movieId]["tags"];
        Watchable *movie = new Movie(content.size() + 1, name, length, tags);
        content.push_back(movie);
        movieId++;
    }
    long seriesId = 0;
    //int seasonNum = 1;

    do {
        string name = jsonContent["tv_series"][seriesId]["name"];
        int length = jsonContent["tv_series"][seriesId]["episode_length"];
        int season = jsonContent["tv_series"][seriesId]["seasons"].size();
        vector<string> tags = jsonContent["tv_series"][seriesId]["tags"];
        for (int numOfSeason = 0; numOfSeason < season; numOfSeason++) {
            for (int numOfEpisodes = 0;
                 numOfEpisodes < jsonContent["tv_series"][seriesId]["seasons"][numOfSeason]; numOfEpisodes++) {
                Watchable *episode = new Episode(content.size() + 1, name, length, numOfSeason + 1, numOfEpisodes + 1,
                                                 tags);
                content.push_back(episode);
            }
        }
        seriesId++;
    } while (jsonContent["tv_series"][seriesId] != nullptr);
}


string userInputString = "";


std::vector<std::string> inputUser;


void Session::start() {
    cout << "SPLFLIX is now on!" << endl;
    BaseAction *action;
    User *user = new LengthRecommenderUser("default");
    GetUserMap().insert({"default", user});
    SetActUser(user);

    do {
        //clearing up the inside vector
        inputUser.clear();
        //putting input of user inside our string
        getline(cin, userInputString);
        //split by space the input string and putting inside the inputString verctor
        for (unsigned int i = 0; i < userInputString.length(); i++) {
            string cut = "";
            while (i < userInputString.length() && userInputString.at(i) != ' ') {
                cut = cut + userInputString.at(i);
                i++;
            }
            inputUser.push_back(cut);
        }
        if ("exit" == inputUser[0]) {
            action = new Exit;
        } else if ("createuser" == inputUser[0]) {
            action = new CreateUser;
        } else if ("changeuser" == inputUser[0]) {
            action = new ChangeActiveUser;
        } else if ("deleteuser" == inputUser[0]) {
            action = new DeleteUser;
        } else if ("dupuser" == inputUser[0]) {
            action = new DuplicateUser;
        } else if ("content" == inputUser[0]) {
            action = new PrintContentList;
        } else if ("watchhist" == inputUser[0]) {
            action = new PrintWatchHistory;
        } else if ("watch" == inputUser[0]) {
            action = new Watch;
        } else if ("log" == inputUser[0]) {
            action = new PrintActionsLog;
        }
        action->act(*this);

    } while (inputUser[0] != "exit");
    delete(action);
}

void Session::SetActUser(User *u) {
    activeUser = u;

}

std::unordered_map<std::string, User *> &Session::GetUserMap() {
    return userMap;
}

std::vector<Watchable *> &Session::GetContent() {
    return content;
}

std::vector<BaseAction *> &Session::GetActionLog() {
    return actionsLog;
}

User *Session::GetActiveUser() {
    return activeUser;
}

void Session::ChangeActUser(User *user) {
    this->activeUser = user;
}

Session::~Session() {
    for (auto x: userMap) {
        delete (x.second);
        x.second= nullptr;
    }
    for (auto x:actionsLog) {
//        if (x != nullptr) {
        delete (x);
//            x = nullptr;
//        }

    }
    for (auto x:content) {
        delete (x);
    }
}

Session *Session::clone() {
    return new Session(*this);
}

//copy constructor
Session::Session(const Session &other) : inputUser(other.inputUser), userInputString(other.userInputString), content(other.content), actionsLog(other.actionsLog), userMap(other.userMap), activeUser(other.activeUser){
    activeUser = other.activeUser->clone();
    for (auto x: other.userMap) {
        userMap.insert({x.first, x.second->clone()});
    }
    for (auto x: other.actionsLog) {
        actionsLog.push_back(x->clone());
    }
    for (auto x: other.content) {
        content.push_back(x->clone());
    }
    inputUser = other.inputUser;
    userInputString = other.userInputString;
}

//copy assignment opertor
Session &Session::operator=(const Session &other) {
    if (this != &other) {
        inputUser = other.inputUser;
        userInputString = other.userInputString;
        delete activeUser;
        activeUser = other.activeUser->clone();
        //for content
        for (auto x: content) {
            delete x;
        }
        content.clear();
        for (auto x:other.content) {
            content.push_back(x);
        }
        //for action log
        for (auto x: actionsLog) {
            delete x;
        }
        actionsLog.clear();
        for (auto x:other.actionsLog) {
            actionsLog.push_back(x);
        }
        //for iser map
        for (auto x: userMap) {
            userMap.erase(x.first);
        }
        userMap.clear();
        for (auto x:other.userMap) {
            userMap.insert({x.first, x.second->clone()});
        }
    }
    return (*this);
}

//move constructor
Session::Session(Session &&other) :  inputUser(move(other.inputUser)), userInputString(other.userInputString), content(other.content), actionsLog(other.actionsLog),
                                    userMap(other.userMap),activeUser(other.activeUser) {
    other.activeUser = nullptr;
//    for (auto x: other.userMap) {
//        x.second = nullptr;
//    }
//    for (auto x: other.actionsLog) {
//        x = nullptr;
//    }
//    for (auto x: other.content) {
//        x = nullptr;
//    }
    other.actionsLog.clear();
    other.content.clear();
    other.userMap.clear();
    other.inputUser.clear();
}

Session &Session::operator=(Session &&other){
    if (this!=&other) {
        for (auto x: userMap) {
            delete x.second;
        }
        for (auto x:content) {
            delete (x);
        }
        for (auto x:actionsLog) {
            delete (x);
        }
        actionsLog.clear();
        content.clear();
        userMap.clear();
        inputUser.clear();
        activeUser = nullptr;
        activeUser = other.activeUser;
        for (auto x: other.userMap) {
            userMap.insert({x.first, x.second});
        }
        for (auto x: other.actionsLog) {
            actionsLog.push_back(x);
        }
        for (auto x: other.content) {
            content.push_back(x);
        }
        inputUser = other.inputUser;
        userInputString = other.userInputString;

//        other.activeUser = nullptr;
//        for (auto x: other.userMap) {
//            x.second = nullptr;
//        }
//        for (auto x: other.actionsLog) {
//            x = nullptr;
//        }
//        for (auto x: other.content) {
//            x = nullptr;
//        }
        other.actionsLog.clear();
        other.content.clear();
        other.userMap.clear();
        other.inputUser.clear();

    }

    return *this;
}


















