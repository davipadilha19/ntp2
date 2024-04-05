#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Endereço IP e porta do servidor NTP
    std::string serverAddress = "senac-alunos.alunos.sc.senac.br";  // Endereço do servidor
    int serverPort = 123;  // Porta do servidor NTP

    // Criação do socket
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Erro ao criar o socket" << std::endl;
        return 1;
    }

    // Configuração do endereço do servidor
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_aton(serverAddress.c_str(), &serverAddr.sin_addr);

    // Envio da solicitação de hora para o servidor
    std::string request = "\x1b" + std::string(47, '\0');  // Cabeçalho NTP para obter a hora
    sendto(clientSocket, request.c_str(), request.size(), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // Recebimento da resposta do servidor
    char buffer[48] = {0};
    struct sockaddr_in serverResponse;
    socklen_t serverResponseLen = sizeof(serverResponse);
    recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverResponse, &serverResponseLen);

    // Fechamento do socket
    close(clientSocket);

    // Processamento da resposta para obter a hora
    uint64_t secondsSince1900 = ntohl(*((uint32_t *)(buffer + 40))) - 2208988800U;
    time_t currentTime = secondsSince1900;
    struct tm *timeInfo = gmtime(&currentTime);

    // Formatação da hora
    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    std::string currentTimeStr = oss.str();

    // Exibição da hora obtida do servidor
    std::cout << "Hora do servidor NTP (" << serverAddress << ":" << serverPort << "): " << currentTimeStr << std::endl;

    return 0;
}
