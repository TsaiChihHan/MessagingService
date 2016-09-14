#include "message.h"

Message::Message() {
}

Message::Message(string command, string name, int length) {
  this->command = command;
  this->name = name;
  this->length = length;
  this->value = "";
}

Message::~Message() {
}

bool
Message::needed() {
  if (value.length() == length) {
    // cout << "not needed" << endl;
    return false;
  }
  else {
    // cout << "needed" << endl;
    return true;
  }
}

void
Message::toString() {
  cout << "***** Message *****" << endl;
  cout << "command: " << command << endl;
  cout << "name: " << name << endl;
  cout << "length: " << length << endl;
  cout << "value: " << value << endl;
  cout << "value.size(): " << value.size() << endl;
  cout << "**********" << endl;
}


string
Message::getCommand() {
  return this->command;
}

void
Message::setCommand(string str) {
  this->command = str;
}

string
Message::getName() {
  return this->name;
}

void
Message::setName(string name) {
  this->name = name;
}

int
Message::getLength() {
  return this->length;
}

void
Message::setLength(int length) {
  this->length = length;
}

string
Message::getValue() {
  return this->value;
}

void
Message::setValue(string value) {
  this->value = value;
}
