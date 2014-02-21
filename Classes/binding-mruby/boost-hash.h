#ifndef BOOSTHASH_H
#define BOOSTHASH_H

#include <boost/unordered/unordered_map.hpp>
#include <boost/unordered/unordered_set.hpp>

#include <utility>

/* Wrappers around the boost unordered template classes,
 * exposing an interface similar to Qt's QHash/QSet */

template<typename K, typename V>
class BoostHash
{
private:
	typedef boost::unordered_map<K, V> BoostType;
	typedef std::pair<K, V> PairType;
	BoostType p;

public:
	typedef typename BoostType::const_iterator const_iterator;

	inline bool contains(const K &key) const
	{
		const_iterator iter = p.find(key);

		return (iter != p.cend());
	}

	inline void insert(const K &key, const V &value)
	{
		p.insert(PairType(key, value));
	}

	inline void erase(const K &key)
	{
		p.erase(key);
	}

	inline const V value(const K &key) const
	{
		const_iterator iter = p.find(key);

		if (iter == p.cend())
			return V();

		return iter->second;
	}

	inline const V value(const K &key, const V &defaultValue) const
	{
		const_iterator iter = p.find(key);

		if (iter == p.cend())
			return defaultValue;

		return iter->second;
	}

	inline V &operator[](const K &key)
	{
		return p[key];
	}

	inline const_iterator cbegin() const
	{
		return p.cbegin();
	}

	inline const_iterator cend() const
	{
		return p.cend();
	}
};

template<typename K>
class BoostSet
{
private:
	typedef boost::unordered_set<K> BoostType;
	BoostType p;

public:
	typedef typename BoostType::const_iterator const_iterator;

	inline bool contains(const K &key)
	{
		const_iterator iter = p.find(key);

		return (iter != p.cend());
	}

	inline void insert(const K &key)
	{
		p.insert(key);
	}

	inline const_iterator cbegin() const
	{
		return p.cbegin();
	}

	inline const_iterator cend() const
	{
		return p.cend();
	}
};

#endif // BOOSTHASH_H
