
#include "../Includes/Channels.hpp"

Channels::Channels() : _creationTime(std::time(0)), _mode("+")
{
    return;
}

Channels::Channels(int const &clientFd, string const &clientNick) : _creationTime(std::time(0)), _mode("+")
{
    this->_users[clientNick] = std::make_pair(clientFd, true);
}

Channels::Channels(Channels const &cpy)
{
    *this = cpy;
    return;
}

Channels &Channels::operator=(Channels const &cpy)
{
    this->_topic = cpy._topic;
    this->_creationTime = cpy._creationTime;
    this->_password = cpy._password;
    this->_mode = cpy._mode;
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

void Channels::set_password(string const &password)
{
    this->_password = password;
}

string const &Channels::get_password()
{
    return this->_password;
}

size_t const &Channels::get_limit()
{
    return this->_limit;
}

void Channels::set_limit(string const &limit)
{
    this->_limit = std::atoi(limit.c_str());
    if (this->_limit > 100)
    {
        this->_limit = 100;
    }
}

time_t const &Channels::get_creationTime()
{
    return this->_creationTime;
}

string const &Channels::get_mode() const
{
    return this->_mode;
}

bool Channels::mode_contains(char const &c) const
{
    return this->_mode.find(c) != string::npos;
}

void Channels::add_mode(char const &newMode)
{
    if (this->_mode.find_first_of(newMode) == string::npos)
        this->_mode += newMode;
}

void Channels::remove_mode(char const &modeToRemove)
{
    size_t charPos = this->_mode.find_first_of(modeToRemove);
    if (charPos != string::npos)
        this->_mode.erase(charPos, 1);
}

void Channels::add_users(int const &fd, string const &name)
{
    this->_users[name].first = fd;
    this->_users[name].second = false;
    return;
}

void Channels::remove_users(string const &clientNick)
{
    this->_users.erase(clientNick);
    return;
}

mapPair const &Channels::get_users()
{
    return this->_users;
}

string const Channels::append_all_users() const
{
    string str;
    mapPair::const_iterator ite;

    for (mapPair::const_iterator it = this->_users.begin(); it != this->_users.end(); it++)
    {
        str += it->first;
        ite = it;
        if (++ite != this->_users.end())
            str += " ";
    }
    return str;
}

void Channels::add_operator(string const &clientNick)
{
    this->_users[clientNick].second = true;
}

void Channels::remove_operator(string const &clientNick)
{
    this->_users[clientNick].second = false;
}