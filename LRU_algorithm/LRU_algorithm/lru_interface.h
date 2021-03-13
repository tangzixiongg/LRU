#pragma once

#include "hashTable.h"

template<class K, class V>
class ILru_event
{
public:
	virtual void on_remove(K& key, V* value) = 0;
};

template<class K, class V>
class ILruManager
{
public:
	virtual void init(uint32_t dwMax, ILru_event<K, V>* pEvent = NULL) = 0;
	virtual bool add(const K& key, const V* value) = 0;
	virtual bool add(const K& key, const V& value) = 0;
	virtual V* get(const K& key) = 0;
	virtual uint32_t get_size() = 0;
	virtual uint32_t get_max_size() = 0;
	virtual void print_all() = 0;
};

template<class K, class V>
ILruManager<K, V>* create_lru_manager();