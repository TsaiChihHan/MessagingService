#pragma once
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Message {
public:
    Message();
    Message(string, string, int);
    ~Message();

    bool needed();

    void toString();
    string getCommand();
    void setCommand(string);
    string getName();
    void setName(string);
    int getLength();
    void setLength(int);
    string getValue();
    void setValue(string);

private:
    string command;
    string name;
    int length;
    string value;
};
