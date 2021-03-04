//
// Created by lihidav@wincs.cs.bgu.ac.il on 25/11/2019.
//

#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable {
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual Watchable* clone()=0;
    virtual std::string toString() const = 0;
    virtual std::string GetWatchableInfo()=0;
    virtual std::string GetWatchableInfoForRec()=0;
    virtual std::vector<std::string> getTags();
    virtual Watchable* getNextWatchable(Session&) const = 0;
    int getLength();
    long getId() const;
//    Watchable( Watchable& );
//    Watchable& operator=(const Watchable& other);
private:
    const long id;
    int length{};
    std::vector<std::string> tags;
};

class Movie : public Watchable {
public:
    virtual Watchable* clone();
    std::string GetWatchableInfo() override;
    std::string GetWatchableInfoForRec() override;
    Movie(long id, const std::string &name, int length,const std::vector<std::string> &tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
//    Movie( Movie& ) ;
//    Movie& operator=(const Movie& other);
private:
    std::string name;
};


class Episode : public Watchable {
public:
    virtual Watchable* clone();
    virtual std::string GetWatchableInfo();
    std::string GetWatchableInfoForRec() override;
    Episode(long id, const std::string& seriesName, int length, int season, int episode, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual int GetSeason() const;
    virtual int GetEpisode() const;
//    Episode( Episode& );
//    Episode& operator=(const Episode& other);
private:
    std::string seriesName;
    int season;
    int episode;
};

#endif