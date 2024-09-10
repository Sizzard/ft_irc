#include "Includes/ft_irc.hpp"

int main(void)
{
    for (size_t i = 0; i < 10; i++)
    {
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(6660);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

        std::stringstream ss;
        ss << i;
        std::string message;
        ss >> message;
        std::cout << "Sending " << message << std::endl;
        write(clientSocket, message.c_str(), message.length());
        sleep(1);
    }
    return 0;
}