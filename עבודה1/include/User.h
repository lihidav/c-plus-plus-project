//
// Created by lihidav@wincs.cs.bgu.ac.il on 25/11/2019.
//

#ifndef SPLFLIX_USER_H
#define SPLFLIX_USER_H

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <iostream>


class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual ~User();
    virtual User* clone()=0;
    virtual void DupUser(const std::string& name,Session& s)=0;
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> &get_history() ;
    std::string CreateTagCount(Session& s);
    std::vector<Watchable*> unWatched(Session& s);
    std::map <std::string, long> tagsCount;
    std::string MostPopTag (std::map <std::string, long>  & tagsCount);
    std::string popTag;
    int indexOfRec;
    User(const User& other);
    User& operator=(const User& other);
    User(User&& other);
    User& operator=(User&& other);

protected:
    std::vector<Watchable*> history;
private:
    std::string name;

};


class LengthRecommenderUser : public User {
public:
    virtual User* clone();
    virtual void DupUser(const std::string& name,Session& s);
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);

private:
};

class RerunRecommenderUser : public User {
public:
    virtual User* clone();
    virtual void DupUser(const std::string& name,Session& s);
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    int indexOfRec;
private:
};

class GenreRecommenderUser : public User {
public:
    virtual User* clone();
    virtual void DupUser(const std::string& name,Session& s);
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);

private:

};


#endif //SPLFLIX_USER_H
