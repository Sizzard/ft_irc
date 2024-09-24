#include "Includes/ft_irc.hpp"

int main(void)
{
    int i = 0;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(6660);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    string message = "PASS bonjour\r\nNICK Debugger\r\nUSER debug debug local :debug\r\n";
    string o;

    write(clientSocket, message.c_str(), message.length());

    while (std::getline(std::cin, o))
    {
        write(clientSocket, "DEBUG\r\n", 7);
    }

    listen(clientSocket, 5);

    accept(clientSocket, NULL, NULL);

    close(clientSocket);

    return 0;
}