#include "Includes/ft_irc.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

bool g_sigint = false;

void sigint_handler(int signal)
{
    (void)signal;
    g_sigint = true;
    cout << RED << "SIGINT DETECTED " << RESET << endl;
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

int main(void)
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(6660);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Connection failed." << std::endl;
        return -1;
    }

    std::string message = "PASS bonjour\r\nNICK pinger\r\nUSER ping ping local :ping\r\n";
    std::string pingMsg = "PING";

    write(clientSocket, message.c_str(), message.length());

    fd_set readfds;
    char buffer[1024];
    std::string line;

    signal(SIGINT, sigint_handler);

    while (!g_sigint)
    {
        FD_ZERO(&readfds);
        FD_SET(clientSocket, &readfds);

        int activity = select(clientSocket + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0)
        {
            std::cerr << "Select error." << std::endl;
            break;
        }

        if (FD_ISSET(clientSocket, &readfds))
        {
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

            if (bytesReceived > 0)
            {
                std::string serverMessage(buffer, bytesReceived);
                std::string userName = serverMessage.substr(1, serverMessage.find_first_of("!") - 1);
                vector<string> words = split(serverMessage, " ");
                std::cout << serverMessage;

                if (serverMessage.find("PRIVMSG pinger") != std::string::npos && (words[3].find("ping") != std::string::npos || words[3].find("PING") != std::string::npos))
                {
                    std::string pongMessage = "PRIVMSG " + userName + " :PONG\r\n";
                    write(clientSocket, pongMessage.c_str(), pongMessage.length());
                    std::cout << "Sent: " << pongMessage;
                    sleep(1);
                }
            }
            else if (bytesReceived == 0)
            {
                std::cout << "Server closed the connection." << std::endl;
                break;
            }
        }
    }

    close(clientSocket);
    return 0;
}
