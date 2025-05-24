#include "matchmaker_server.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sstream>

MatchmakerServer::MatchmakerServer(int InPort)
    : Port(InPort), ServerSocket(-1), bRunning(false) {}

MatchmakerServer::~MatchmakerServer()
{
    Stop();
}

void MatchmakerServer::Start()
{
    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocket < 0)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return;
    }

    sockaddr_in ServerAddr{};
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_port = htons(Port);

    if (bind(ServerSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) < 0)
    {
        std::cerr << "Bind failed." << std::endl;
        return;
    }

    listen(ServerSocket, 5);
    bRunning = true;
    ListenThread = std::thread(&MatchmakerServer::Listen, this);
    std::cout << "Matchmaker server started on port " << Port << std::endl;
}

void MatchmakerServer::Stop()
{
    bRunning = false;
    if (ServerSocket >= 0)
    {
        close(ServerSocket);
        ServerSocket = -1;
    }
    if (ListenThread.joinable())
    {
        ListenThread.join();
    }
}

void MatchmakerServer::Listen()
{
    while (bRunning)
    {
        sockaddr_in ClientAddr;
        socklen_t ClientLen = sizeof(ClientAddr);
        int ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &ClientLen);
        if (ClientSocket >= 0)
        {
            std::thread(&MatchmakerServer::HandleClient, this, ClientSocket).detach();
        }
    }
}

void MatchmakerServer::HandleClient(int ClientSocket)
{
    char Buffer[512]{};
    int Received = recv(ClientSocket, Buffer, sizeof(Buffer), 0);
    if (Received > 0)
    {
        std::string Request(Buffer, Received);
        std::istringstream Stream(Request);
        std::string Segment;
        std::vector<std::string> Modes;
        int Ping = 0;
        std::string IP;
        bool bIsRegister = false;
        bool bIsFind = false;

        while (std::getline(Stream, Segment, ';'))
        {
            if (Segment.find("FIND:") == 0)
            {
                std::string ModeList = Segment.substr(5);
                std::istringstream ModeStream(ModeList);
                std::string Mode;
                while (std::getline(ModeStream, Mode, ','))
                {
                    Modes.push_back(Mode);
                }
                bIsFind = true;
            }
            else if (Segment.find("REG:") == 0)
            {
                std::string ModeList = Segment.substr(4);
                std::istringstream ModeStream(ModeList);
                std::string Mode;
                while (std::getline(ModeStream, Mode, ','))
                {
                    Modes.push_back(Mode);
                }
                bIsRegister = true;
            }
            else if (Segment.find("PING:") == 0)
            {
                Ping = std::stoi(Segment.substr(5));
            }
            else if (Segment.find("IP:") == 0)
            {
                IP = Segment.substr(3);
            }
        }

        if (bIsRegister && !IP.empty())
        {
            RegisterGame(IP, Modes, Ping);
            std::string OK = "REGISTERED";
            send(ClientSocket, OK.c_str(), OK.length(), 0);
        }
        else if (bIsFind)
        {
            std::string MatchIP = TryFindMatch(Modes, Ping);
            send(ClientSocket, MatchIP.c_str(), MatchIP.length(), 0);
        }
    }
    close(ClientSocket);
}

std::string MatchmakerServer::TryFindMatch(const std::vector<std::string>& RequestedModes, int MaxPing)
{
    std::lock_guard<std::mutex> Lock(SessionsMutex);
    for (const auto& Session : ActiveSessions)
    {
        if (Session.Ping <= MaxPing)
        {
            for (const auto& Mode : Session.Modes)
            {
                if (std::find(RequestedModes.begin(), RequestedModes.end(), Mode) != RequestedModes.end())
                {
                    return Session.IP;
                }
            }
        }
    }
    return "NONE";
}

void MatchmakerServer::RegisterGame(const std::string& IP, const std::vector<std::string>& Modes, int Ping)
{
    std::lock_guard<std::mutex> Lock(SessionsMutex);
    GameSession NewSession{IP, Modes, Ping};
    ActiveSessions.push_back(NewSession);
    std::cout << "Registered game at " << IP << " with ping " << Ping << std::endl;
}
