#include "../Includes/ft_irc.hpp"

bool ends_with(string const &str, string const &ends)
{
    if (str.size() < ends.size())
        return FAILURE;
    return (str.compare(str.size() - ends.size(), ends.size(), ends));
}

vector<string> analyse_command(vector<string> const &v, size_t const &i)
{
    if (v[i].compare(0, 4, "PASS") == 0 || v[i].compare(0, 4, "QUIT") == 0 || v[i].compare(0, 4, "PING") == 0 || v[i].compare(0, 7, "PRIVMSG") == 0 || v[i].compare(0, 5, "TOPIC") == 0)
    {
        return split_first_word(v[i], " ");
    }
    else
    {
        return split(v[i], " ");
    }
}

vector<string> split(string const &line, string const &to_split)
{
    vector<string> v;

    size_t end = 0;

    for (size_t i = 0; end != string::npos; i++)
    {
        size_t begin = line.find_first_not_of(to_split, i);
        end = line.find(to_split, begin);
        if (begin > line.length())
            break;
        string word = line.substr(begin, end - begin);
        v.push_back(word);
        i = end;
    }
    return v;
}

vector<string> split_first_word(string const &line, string const &to_split)
{
    vector<string> v;

    if (line.empty() == true)
        return v;

    size_t begin = line.find_first_not_of(to_split, 0);
    size_t end = line.find(to_split, begin);
    if (begin > line.length())
        return v;
    string word = line.substr(begin, end - begin);
    v.push_back(word);
    if (end != string::npos)
    {
        v.push_back(line.substr(end + 1));
    }
    return v;
}

vec_pair split_mode(string line)
{
    vec_pair v;
    std::set<char> seen;

    int mode = -1;

    for (size_t i = 0; i < line.size(); i++)
    {
        char c = line[i];

        if (line[i] == '+' || line[i] == '-')
        {
            mode = i;
        }
        else if (seen.find(c) == seen.end() && mode != -1)
        {
            v.push_back(std::make_pair(c, line[mode]));
            seen.insert(c);
        }
    }

    return v;
}

string get_time()
{
    string result;

    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "Date: %a, %d %b %G %T %Z", timeinfo);
    result = buffer;

    return result;
}

string const get_epoch_time(time_t time)
{
    std::stringstream ss;
    string res;
    ss << time;
    ss >> res;

    return res;
}
