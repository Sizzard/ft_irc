
#include "../Includes/Channels.hpp"

Channels::Channels()
{
    return;
}

Channels::Channels(Channels const &cpy)
{
    *this = cpy;
    return;
}

Channels &Channels::operator=(Channels const &cpy)
{
    this->_topic = cpy._topic;
    this->_users = cpy._users;
    return *this;
}

Channels::~Channels()
{
    return;
}

void Channels::set_topic(string const &topic)
{
    this->_topic = topic;
    return;
}
string Channels::get_topic() const
{
    return this->_topic;
}

void Channels::add_users(int const &fd, string const &name)
{
    this->_users[fd] = name;
    return;
}

void Channels::remove_users(int const &fd)
{
    this->_users.erase(fd);
    return;
}

map<int, string> &Channels::get_users()
{
    return this->_users;
}

string const Channels::append_all_users() const
{
    string str;
    // cout << "TEST DE BZ" << endl;
    map<int, string>::const_iterator ite;

    for (map<int, string>::const_iterator it = this->_users.begin(); it != this->_users.end(); it++)
    {
        str += it->second;
        ite = it;
        if (++ite != this->_users.end())
            str += " ";
    }
    return str;
}