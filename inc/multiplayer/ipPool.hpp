#ifndef IP_POOL_HPP
#define IP_POOL_HPP

#include <string>

#include "safeQueue.hpp"

class IpPool
{
public:
    IpPool();
    ~IpPool() = default;
    void AddIpToPool(std::string a_ip);
    std::string GetIpFromPool();
    void RemoveIpFromPool(std::string a_ip);
    bool IsIpInPool(std::string a_ip);

private:
    void InitSafequeue();
    
private:
    SafeQueue<std::string> m_ipPool;
};

#endif // IP_POOL_HPP
