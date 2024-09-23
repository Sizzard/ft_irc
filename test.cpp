#include "Includes/ft_irc.hpp"

// int main(void)
// {
//     // for (size_t i = 0; i < 10; i++)
//     // {
//     //     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

//     //     sockaddr_in serverAddress;
//     //     serverAddress.sin_family = AF_INET;
//     //     serverAddress.sin_port = htons(6660);
//     //     serverAddress.sin_addr.s_addr = INADDR_ANY;

//     //     connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

//     //     std::stringstream ss;
//     //     ss << i;
//     //     std::string message;
//     //     ss >> message;
//     //     std::cout << "Sending " << message << std::endl;
//     //     write(clientSocket, message.c_str(), message.length());
//     //     sleep(1);
//     // }
//     int i = 0;

//     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(6660);
//     serverAddress.sin_addr.s_addr = INADDR_ANY;

//     connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

//     sleep(1);

//     std::stringstream ss;
//     ss << i;
//     std::string message = "Ceci est le message numero ";
//     ss >> message;
//     std::cout << "Sending " << message << std::endl;
//     write(clientSocket, message.c_str(), message.length());

//     listen(clientSocket, 5);

//     accept(clientSocket, NULL, NULL);

//     sleep(1);

//     close(clientSocket);

//     return 0;
// }

// vector<string> split_first_word(string const &line, string const &to_split)
// {
//     vector<string> v;

//     if (line.empty() == true)
//         return v;

//     size_t begin = line.find_first_not_of(to_split, 0);
//     size_t end = line.find_first_of(to_split, begin);
//     if (begin > line.length())
//         return v;
//     string word = line.substr(begin, end - begin);
//     v.push_back(word);
//     if (end != string::npos)
//     {
//         v.push_back(line.substr(end + 1));
//     }
//     return v;
// }

// int main(void)
// {
//     string str = "    ";
//     vector<string> words = split_first_word(str, " ");

//     words[0];
//     cout << "TEST 1 : " << words.size() << endl;

//     if (words.size() == true)
//         return 1;
//     cout << "TEST 2" << endl;

//     if (words[2].empty() == true)
//         return 1;
//     return 0;
// }
