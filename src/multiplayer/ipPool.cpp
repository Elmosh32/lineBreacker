#include <iostream>

#include "ipPool.hpp"

/* multicast ip addr (class d) 224.0.0.0 <=> 239.255.255.255
   pool ip: 224.0.0.0 <=> 224.0.0.100
   server give the same port for all groups
   when group closed the ip returns to pool ip*/

IpPool::IpPool()
{
    InitSafequeue();
}

void IpPool::InitSafequeue()
{
    for (size_t i = 0; i < 100; ++i) {
        m_ipPool.pushFront("224.0.0." + std::to_string(i));
    }
}

void IpPool::AddIpToPool(std::string a_ip)
{
    m_ipPool.pushFront(a_ip);
}

std::string IpPool::GetIpFromPool()
{   
    std::string ip;
    m_ipPool.pop(ip);
    return ip;
}

// void IpPool::RemoveIpFromPool(std::string a_ip)
// {
//     m_ipPool.Remove(a_ip);
// }

// bool IpPool::IsIpInPool(std::string a_ip)
// {
//     return m_ipPool.IsInQueue(a_ip);
// }