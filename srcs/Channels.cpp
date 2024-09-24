
#include "../Includes/Channels.hpp"

Channels::Channels() : _creationTime(std::time(0)), _mode("+")
{
    return;
}

Channels::Channels(int const &clientFd, string const &clientNick) : _creationTime(std::time(0)), _mode("+")
{
    this->_users[clientFd].first = clientNick;
    this->_users[clientFd].second = true;
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

void Channels::remove_mode(string const &modeToRemove)
{
    for (string::const_iterator it = modeToRemove.begin(); it != modeToRemove.end(); it++)
    {
        if (*it == 'i' || *it == 't' || *it == 'k' || *it == 'o' || *it == 'l')
        {
            for (size_t pos = this->_mode.find_first_of(*it); pos != string::npos; pos = this->_mode.find_first_of(*it))
            {
                this->_mode.erase(pos, 1);
            }
        }
    }
}

void Channels::add_users(int const &fd, string const &name)
{
    this->_users[fd].first = name;
    this->_users[fd].second = false;
    return;
}

void Channels::remove_users(int const &fd)
{
    this->_users.erase(fd);
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
        str += it->second.first;
        ite = it;
        if (++ite != this->_users.end())
            str += " ";
    }
    return str;
}