//
// Created by lihidav@wincs.cs.bgu.ac.il on 25/11/2019.
//

#include "../include/User.h"
#include "Session.h"
#include <iostream>
#include <string>
#include "User.h"
#include "Action.h"
#include "Watchable.h"
#include <sstream>
#include <unordered_map>
#include <map>

#include <limits>

#include <cmath>


using namespace std;

User::User(const std::string &name) :tagsCount(), popTag(),indexOfRec(),history(),name(){

    User::name = name;
    User::indexOfRec-=0;
}

std::string User::getName() const {
    return name;
}

std::vector<Watchable *> &User::get_history() {
    return history;
}


Watchable *LengthRecommenderUser::getRecommendation(Session &s) {

    long totalLength = 0;
    long counter = 0;
    Watchable *recommend = nullptr;
    for (unsigned int i = 0; i < s.GetActiveUser()->get_history().size(); i++) {
        totalLength = totalLength + s.GetActiveUser()->get_history()[i]->getLength();
        counter = counter + 1;
    }
    double average = totalLength / counter;
    long m = numeric_limits<int>::max(); // infinity value
    vector<Watchable *> unWat = s.GetActiveUser()->unWatched(s);
    for (unsigned int i = 0; i < unWat.size(); i++) {
        if (abs(average - unWat[i]->getLength()) < m)// check the closest content length to the average
        {
            m = abs(average - unWat[i]->getLength());
            recommend = unWat[i];
        }
    }
    return recommend;

}


LengthRecommenderUser::LengthRecommenderUser(const std::string &name): User(name) {}

void LengthRecommenderUser::DupUser(const std::string &name,Session& s ) {
    User *user;
    user = new LengthRecommenderUser(name);
    user->popTag=this->popTag;
    user->tagsCount=this->tagsCount;
    user->get_history()=this->get_history();
    s.GetUserMap().insert({name,user}); // adding yo user map the new duplicate user
}

User *LengthRecommenderUser::clone() {
    return new LengthRecommenderUser(*this);
}


Watchable *RerunRecommenderUser::getRecommendation(Session &s) {

    Watchable *lastRec = nullptr;
    if (s.GetActiveUser()->get_history().empty()) { //!check if the return of null pointer acceptable!!!
        return nullptr;
    } else {
        int n = s.GetActiveUser()->get_history().size(); //num of content in the history of user
        lastRec = s.GetActiveUser()->get_history()[indexOfRec % n];
        indexOfRec = (indexOfRec + 1)%n;
        return lastRec;
    }
}

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name),indexOfRec(0){}


void RerunRecommenderUser::DupUser(const std::string &name, Session &s) {
    User *user;
    user = new RerunRecommenderUser(name);
    user->get_history()=this->get_history();
    user->indexOfRec=this->indexOfRec;
    s.GetUserMap().insert({name,user}); // adding yo user map the new duplicate user
}

User *RerunRecommenderUser::clone() {
    return new RerunRecommenderUser(*this);
}


Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    popTag = CreateTagCount(s);//!
    vector<Watchable *> unWatch = unWatched(s);
    do {
        for (unsigned int i = 0; i < unWatch.size(); i++) {
            for (unsigned int j = 0; unWatch[i]->getTags().size(); j++) {
                if (unWatch[i]->getTags()[j] == popTag)
                    return unWatch[i];
            }
        }
        tagsCount.erase(popTag);
        popTag = MostPopTag(tagsCount);
    } while (!tagsCount.empty());
    return nullptr;
}


GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name)/*!,popTag("")*/ {}

void  GenreRecommenderUser::DupUser(const std::string &name, Session &s) {
    User *user;
    user = new GenreRecommenderUser(name);
    user->get_history()=this->get_history();
    s.GetUserMap().insert({name,user}); // adding yo user map the new duplicate user
}

User *GenreRecommenderUser::clone() {
    return new GenreRecommenderUser(*this);
}


std::vector<Watchable *> User::unWatched(Session &s) { //return all episodes/movies who hasn't watched by the user
    vector<Watchable *> unWatch;
    unsigned int j;
    bool b =false;
    for (unsigned int i = 0; i < s.GetContent().size(); i++) { //going through all content
        for (j = 0; j < s.GetActiveUser()->get_history().size(); j++) { //going through user watchable history
            if (s.GetContent()[i] == s.GetActiveUser()->get_history()[j])  //if episode/movie not watched
                b =true;
        }
        if(!b){unWatch.push_back(s.GetContent()[i]);}
        b=false;
    }
    return unWatch;
}

string User::CreateTagCount(Session &s) {
    long counter = 0;
    vector<Watchable *> history = s.GetActiveUser()->get_history(); //!check referece to history
    popTag = "";//!
    for (unsigned int i = 0; i < history.size(); i++) { //going through the watchable history
        for (unsigned int j = 0; j < history[i]->getTags().size(); j++) { //going through the tags of the movie/episode
            if (tagsCount.find(history[i]->getTags()[j]) == tagsCount.end()) {
                tagsCount.insert(make_pair(history[i]->getTags()[j], 1));
                if (counter < 1) {
                    counter = 1;
                    popTag = history[i]->getTags()[j];
                }
            } else {
                tagsCount[history[i]->getTags()[j]] = tagsCount[history[i]->getTags()[j]] + 1;
                if (counter < tagsCount[history[i]->getTags()[j]]) {
                    counter = tagsCount[history[i]->getTags()[j]];
                    popTag = history[i]->getTags()[j];
                }
            }

        }


    }
    return popTag;
}

std::string User::MostPopTag(std::map<std::string, long> &tagsCount) {
    popTag = "";//!
    int counter = 0;
    for (auto it = tagsCount.begin(); it != tagsCount.end(); it++) {
        if (counter < it->second) { // check for the larger Tag appearance in TagCounts
            counter = it->second;
            popTag = it->first;
        }
    }
    return popTag;
}
//copy constructor
User::User(const User &other) :tagsCount(), popTag(),indexOfRec(),history(),name(){
    name = other.name;
    indexOfRec=other.indexOfRec;
    popTag=other.popTag;
    tagsCount=other.tagsCount;
    for(auto x: other.history){
        history.push_back(x->clone());
    }
}

//copy assignment operator
User &User::operator=(const User &other) {
    if(this != &other){
        name = other.name;
        indexOfRec=other.indexOfRec;
        popTag=other.popTag;
        tagsCount=other.tagsCount;
        for(auto x: history){
            delete(x);
        }
        history.clear();
        for(auto x: other.history){
            history.push_back(x->clone());
        }
    }

    return (*this);
}
//move constructor
User &User::operator=(User &&other) {
    if(this!=&other){
        for(auto x:history){
            delete(x);
        }
        history.clear();
        tagsCount.clear();
        name=other.name;
        indexOfRec=other.indexOfRec;
        popTag=other.popTag;
        for(auto x: other.history){
            history.push_back(x);
        }
        other.history.clear();
        tagsCount=other.tagsCount;
        other.tagsCount.clear();
    }
    return *this;
}

//move assignment opertor
User::User(User &&other):tagsCount(), popTag(),indexOfRec(),history(),name() {
    name=other.name;
    indexOfRec=other.indexOfRec;
    popTag=other.popTag;
    for(auto x: other.history){
        history.push_back(x);
    }
    other.history.clear();
    tagsCount=other.tagsCount;
    other.tagsCount.clear();

}

User::~User() {
    history.clear();
    tagsCount.clear();

}

