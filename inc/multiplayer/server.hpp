#ifndef SERVER_HPP
#define SERVER_HPP

#include <SFML/Network.hpp>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <memory>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <csignal>
#include <mutex>

#include "ipPool.hpp"

class Server
{
public:
    // Server(int a_width, int a_height, sf::RenderWindow &win);
    Server() = default;
    ~Server();
    void Connect();
    void ReceiveMessages(std::unique_ptr<sf::TcpSocket>);
    void AcceptClients();
    // static void signalHandler(int signum);
    void sendGameRequest(std::string);
    void rejectInvite(std::string a_msg);
    void acceptInvite(std::string a_msg);

private:
    std::tuple<std::string, std::string> getNames(std::string a_msg);
    void pushToClients(std::unique_ptr<sf::TcpSocket> &a_clients);
    std::vector<std::string> getSubstringsBetweenBars(const std::string &s);

private: 
    std::vector<std::unique_ptr<sf::TcpSocket>> m_clients;
    sf::IpAddress m_ip = sf::IpAddress::getLocalAddress();
    sf::TcpListener m_listener;
    std::unordered_map<size_t, std::string> m_activePlayers;
    IpPool m_poolIP;    
    size_t m_numOfPlayers = 0;
    size_t m_port = 5000;
    // Canvas m_canvas;
};

#endif // SERVER_HPP
