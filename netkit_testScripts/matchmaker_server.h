#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <atomic>
#include <netinet/in.h>

struct GameSession
{
    std::string IP;
    std::vector<std::string> Modes;
    int Ping;
};

class MatchmakerServer
{
public:
    MatchmakerServer(int Port);
    ~MatchmakerServer();

    void Start();
    void Stop();

private:
    void Listen();
    void HandleClient(int ClientSocket);
    std::string TryFindMatch(const std::vector<std::string>& RequestedModes, int MaxPing);
    void RegisterGame(const std::string& IP, const std::vector<std::string>& Modes, int Ping);

    int ServerSocket;
    int Port;
    std::thread ListenThread;
    std::atomic<bool> bRunning;
    std::vector<GameSession> ActiveSessions;
    std::mutex SessionsMutex;
};