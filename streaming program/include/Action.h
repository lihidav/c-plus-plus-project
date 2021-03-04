//
// Created by lihidav@wincs.cs.bgu.ac.il on 25/11/2019.
//

#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus {
    PENDING, COMPLETED, ERROR
};


class BaseAction {
public:
    BaseAction();
    virtual ~BaseAction();
    virtual BaseAction* clone()=0;
    ActionStatus getStatus() const;
    std::string getStatusConv() const;
    virtual void act(Session& sess) = 0;
    virtual std::string toString() const = 0;
    BaseAction(BaseAction&);
protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser : public BaseAction {
public:
    virtual BaseAction* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class ChangeActiveUser : public BaseAction {
public:
    BaseAction* clone() override;
    virtual void act(Session& sess);
    virtual std::string toString() const;

};

class DeleteUser : public BaseAction {
public:
    virtual BaseAction* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};


class DuplicateUser : public BaseAction {
public:
    virtual BaseAction* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class PrintContentList : public BaseAction {
public:
    virtual BaseAction* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class PrintWatchHistory : public BaseAction {
public:
    virtual BaseAction* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};


class Watch : public BaseAction {
public:
    virtual BaseAction* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};


class PrintActionsLog : public BaseAction {
public:
    virtual BaseAction* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
    virtual BaseAction* clone();
    virtual void act(Session& sess);
    virtual std::string toString() const;
};
#endif