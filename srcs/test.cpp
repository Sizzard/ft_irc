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

