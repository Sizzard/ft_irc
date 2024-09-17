#include "../Includes/ft_irc.hpp"

bool ends_with(string const &str, string const &ends)
{
    if (str.size() < ends.size())
        return FAILURE;
    return (str.compare(str.size() - ends.size(), ends.size(), ends));
}

vector<string> split(string const &line, string const &to_split)
{
    vector<string> v;

    size_t end = 0;

    for (size_t i = 0; end != string::npos; i++)
    {
        size_t begin = line.find_first_not_of(to_split, i);
        end = line.find_first_of(to_split, begin);
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

    size_t begin = line.find_first_not_of(to_split, 0);
    size_t end = line.find_first_of(to_split, begin);
    if (begin > line.length())
        return v;
    string word = line.substr(begin, end - begin);
    v.push_back(word);
    if (end == string::npos)
    {
        v.push_back("");
        return v;
    }
    v.push_back(line.substr(end + 1));

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