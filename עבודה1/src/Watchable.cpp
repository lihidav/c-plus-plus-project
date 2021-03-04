//
// Created by lihidav@wincs.cs.bgu.ac.il on 25/11/2019.
//

#include "../include/Watchable.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "User.h"

#include <iostream>
#include <string>
#include "User.h"
#include "Session.h"
#include "Action.h"
#include "Watchable.h"

using namespace std;

Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length), tags(tags) {}

std::vector<std::string> Watchable::getTags() {
    return tags;
}

long Watchable::getId() const {
    return id;
}

int Watchable::getLength() {
    return length;
}

Watchable::~Watchable() {

}
//copy constructor
//Watchable::Watchable(Watchable &other) : id(other.id), length(other.length), tags(other.tags) {
//}

//copy assignment operator
//Watchable &Watchable::operator=(const Watchable &other) { //check id
//
//    if(this != &other){
//        // id = other.id;
//        length = other.length;
//        tags = other.tags;
//    }
//    return (*this);
//}




std::string Movie::toString() const {
    return name;
}

Watchable *Movie::getNextWatchable(Session &s) const {
    return s.GetActiveUser()->getRecommendation(s);
}

Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags)
        : Watchable(id, length, tags), name(name) {}

std::string Movie::GetWatchableInfo() {
    string s;
    s.append(to_string(this->getId()));
    s.append(". ");
    s.append(this->name);
    s.append(" ");
    s.append(to_string(this->getLength()));
    s.append(" minutes [");
    for (unsigned int i = 0; i < this->getTags().size(); i++) {
        s.append(this->getTags()[i]);
        s.append(", ");
        s.at(s.size() - 2) = ']';
    }

    return s;
}

std::string Movie::GetWatchableInfoForRec() {
    return this->toString();
}

Watchable *Movie::clone() {
    return new Movie(*this);
}
//copy constructor
//Movie::Movie(Movie &other) : Watchable(other.getId(), other.getLength(), other.getTags()), name(other.name) {}

//copy assignment operator
//Movie &Movie::operator=(const Movie &other) {
//    if(this != &other){
//}


Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags)
        : Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode) {}

Watchable *Episode::getNextWatchable(Session &s) const {
    if (s.GetContent()[getId()]->toString() == this->toString())
        return s.GetContent()[getId()];
    else
        return s.GetActiveUser()->getRecommendation(s);
}

std::string Episode::toString() const {
    return seriesName;
}

std::string Episode::GetWatchableInfo() {
    string s;
    s.append(to_string(this->getId()));
    s.append(". ");
    s.append(this->seriesName);
    s.append(" ");
    s.append("S");
    s.append(to_string(this->GetSeason()));
    s.append("E");
    s.append(to_string(this->GetEpisode()));
    s.append(" ");
    s.append(to_string(this->getLength()));
    s.append(" minutes [");
    for (unsigned int i = 0; i < this->getTags().size(); i++) {
        s.append(this->getTags()[i]);
        s.append(", ");
        s.at(s.size() - 2) = ']';
    }
    return s;
}

int Episode::GetSeason() const {
    return season;
}

int Episode::GetEpisode() const {
    return episode;
}

std::string Episode::GetWatchableInfoForRec() {
    string s;
    s.append(to_string(this->getId()));
    s.append(". ");
    s.append(this->seriesName);
    s.append(" ");
    s.append("S");
    s.append(to_string(this->GetSeason()));
    s.append("E");
    s.append(to_string(this->GetEpisode()));
    return s;
}

Watchable *Episode::clone() {
    return new Episode(*this);
}

//copy constructor
//Episode::Episode(Episode &other) : Watchable(other.getId(), other.getLength(), other.getTags()),
//                                   seriesName(other.seriesName), season(other.season), episode(other.episode) {}
