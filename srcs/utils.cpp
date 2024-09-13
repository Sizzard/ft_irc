#include "../Includes/ft_irc.hpp"

bool ends_with(std::string const &str, std::string const &ends)
{
    if (str.size() < ends.size())
        return FAILURE;
    return (str.compare(str.size() - ends.size(), ends.size(), ends));
}

std::vector<std::string> split(std::string const &line, std::string const &to_split)
{
    std::string word;
    std::vector<std::string> v;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; end != std::string::npos; i++)
    {
        begin = line.find_first_not_of(to_split, i);
        end = line.find_first_of(to_split, begin);
        if (begin > line.length())
            break;
        word = line.substr(begin, end - begin);
        v.push_back(word);
        i = end;
    }
    return v;
}

std::string get_time()
{
    std::string result;

    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "Date: %a, %d %b %G %T %Z", timeinfo);
    result = buffer;

    return result;
}