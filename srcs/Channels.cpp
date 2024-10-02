
#include "../Includes/Channels.hpp"

Channels::Channels() : _creationTime(std::time(0)), _mode("+"), _limit(CHAN_LIMIT)
{
    return;
}

Channels::Channels(int const &clientFd, string const &clientNick) : _creationTime(std::time(0)), _mode("+"), _limit(CHAN_LIMIT)
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
    this->_limit = cpy._limit;
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

int const &Channels::get_limit()
{
    return this->_limit;
}

int ft_atoi_check(const char *nptr, long long *nombre)
{
    int i;
    int min;

    i = 0;
    min = 1;
    while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
        i++;
    if (nptr[i] == '-')
    {
        min = -1;
        i++;
    }
    else if (nptr[i] == '+')
        i++;
    while (nptr[i] >= '0' && nptr[i] <= '9')
    {
        *nombre = *nombre * 10 + (nptr[i] - 48);
        if (*nombre * min < INT_MIN || *nombre * min > INT_MAX)
            return (1);
        i++;
    }
    *nombre *= min;
    return (0);
}

void Channels::set_limit(string const &limit)
{
    int res;
    long long nb = 0;
    res = ft_atoi_check(limit.c_str(), &nb);

    cout << res << ":" << nb << endl;

    if (res == 1 || nb <= 0)
    {
        throw std::out_of_range("out_of_range error");
    }
    this->_limit = (nb < CHAN_LIMIT ? nb : CHAN_LIMIT);
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
void Channels::invite_user(int const &clientfd)
{
    for (vector<int>::iterator it = this->_invitedUsers.begin(); it != this->_invitedUsers.end(); it++)
    {
        if (*it == clientfd)
            return;
    }
    this->_invitedUsers.push_back(clientfd);
}

void Channels::remove_users(string const &clientNick)
{
    this->_users.erase(clientNick);
    return;
}

void Channels::change_nick(string const &oldNick, string const &newNick)
{
    mapPair::iterator it = this->_users.find(oldNick);

    pair<int, bool> pair = it->second;

    this->remove_users(oldNick);

    this->_users[newNick] = pair;
}

mapPair const &Channels::get_users()
{
    return this->_users;
}

vector<int> const &Channels::get_invitedUsers()
{
    return this->_invitedUsers;
}


void Channels::remove_invited_user(int const &clientfd)
{
    for (vector<int>::iterator it = this->_invitedUsers.begin(); it != this->_invitedUsers.end(); it++)
    {
        if (*it == clientfd)
        {
            this->_invitedUsers.erase(it);
            return;
        }
    }
}

string const Channels::append_all_users() const
{
    string str;
    mapPair::const_iterator ite;

    for (mapPair::const_iterator it = this->_users.begin(); it != this->_users.end(); it++)
    {
        if (it->second.second == true)
            str += "@";
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

bool Channels::is_operator(string const &clientNick)
{
    mapPair::iterator it = this->_users.find(clientNick);
    if (it == this->_users.end())
        ;
    else if (it->second.second == true)
        return true;
    return false;
}