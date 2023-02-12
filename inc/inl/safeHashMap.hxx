#ifndef SAFE_HASHMAP_HXX
#define SAFE_HASHMAP_HXX

#include "safeHashMap.hpp"

template <typename K, typename V>
SafeMap<K, V>::SafeMap(size_t a_numOfThreads, size_t a_numOfBuckets)
: m_numOfThreads(a_numOfThreads)
, m_maxBuckets(a_numOfBuckets)
, m_writerLocks(m_numOfThreads)
, m_hashmap(m_maxBuckets)
{
}

template <typename K, typename V>
void SafeMap<K, V>::Upsert(const K &a_key, std::function<void(V &)> &a_updater, std::function<V()> &a_factory)
{
    std::unique_lock lock(LockBucket(a_key));
    auto itr = m_hashmap.find(a_key);

    if (itr != m_hashmap.end())
    {
        try
        {
            a_updater(itr->second);
        }
        catch (...)
        {
            throw;
        }
    }
    else
    {
        try
        {
            m_hashmap.insert(std::make_pair(a_key, a_factory()));
        }
        catch (...)
        {
            throw;
        }
    }
}

template <typename K, typename V>
void SafeMap<K, V>::Upsert(K &&a_key, std::function<void(V &)> &&a_updater, std::function<V()> &&a_factory)
{
    std::unique_lock lock(LockBucket(a_key));
    auto itr = m_hashmap.find(std::move(a_key));

    if (itr != m_hashmap.end())
    {
        try
        {
            a_updater(itr->second);
        }
        catch (...)
        {
            throw;
        }
    }
    else
    {
        try {
            m_hashmap.insert(std::make_pair(a_key, a_factory()));
        }
        catch(...)
        {
            throw;
        }
        
    }
}

template <typename K, typename V>
void SafeMap<K, V>::Update(const K &a_key, std::function<V()> &a_factory)
{
    std::unique_lock lock(LockBucket(a_key));
    auto itr = m_hashmap.find(a_key);

    if (itr != m_hashmap.end())
    {
        try
        {
            a_factory(itr->second);
        }
        catch (...)
        {
            throw;
        }
    }
}

template <typename K, typename V>
void SafeMap<K, V>::Update(K &&a_key, std::function<V()> &&a_factory)
{
    std::unique_lock lock(LockBucket(a_key));
    auto itr = m_hashmap.find(std::move(a_key));

    if (itr != m_hashmap.end())
    {
        try
        {
            a_updater(itr->second);
        }
        catch (...)
        {
            throw;
        }
    }
}

template <typename K, typename V>
void SafeMap<K, V>::Remove(const K &a_key)
{
    std::unique_lock lock(LockBucket(a_key));
    m_hashmap.erase(a_key);
}

template <typename K, typename V>
void SafeMap<K, V>::Remove(K &&a_key)
{
    std::unique_lock lock(LockBucket(a_key));
    m_hashmap.erase(std::move(a_key));
}

template <typename K, typename V>
std::optional<V> SafeMap<K, V>::GetValue(const K &a_key) const
{
    std::shared_lock lock(m_readWriteMutex);
    auto itr = m_hashmap.find(a_key);

    if (itr != m_hashmap.end())
    {
        return itr->second;
    }
    else
    {
        return {};
    }
}

template <typename K, typename V>
void SafeMap<K, V>::SetValue(const K &a_key, V const &a_value)
{
    std::unique_lock lock(LockBucket(a_key));
    auto [it, ok] = m_hashmap.insert({a_key, a_value});

    if (!ok)
    {
        it->second = a_value;
    }
}

template <typename K, typename V>
void SafeMap<K, V>::SetValue(K &&a_key, V &&a_value)
{
    std::unique_lock lock(LockBucket(a_key));
    auto [it, ok] = m_hashmap.insert({std::move(a_key), std::move(a_value)});

    if (!ok)
    {
        it->second = a_value;
    }
}

template <typename K, typename V>
void SafeMap<K, V>::PrintHashmap()
{
    for (auto it = m_hashmap.begin(); it != m_hashmap.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

template <typename K, typename V>
std::shared_mutex &SafeMap<K, V>::LockBucket(const K &a_key)
{
    size_t areaSize = m_maxBuckets / m_numOfThreads;
    // std::cout << std::round(m_hashmap.bucket(a_key) / areaSize) << std::endl;

    return m_writerLocks.at(std::round(m_hashmap.bucket(a_key) / areaSize));
}

#endif /* SAFE_HASHMAP_XX */
