#pragma once

#include "lru_interface.h"

template<class K, class V>
struct lru_node
{
	lru_node()
	{
		//key = value = 0;
		pPre = pNext = nullptr;
	}
	K key;
	V value;
	lru_node<K, V>* pPre;
	lru_node<K, V>* pNext;
};


template<class K, class V>
class lru_manager : public ILruManager<K, V>
{
public:
	lru_manager();
	~lru_manager();
public:
	void init(uint32_t dwMax, ILru_event<K, V>* pEvent = NULL) override;
	bool add(const K& key, const V* value) override;
	bool add(const K& key, const V& value) override;
	V* get(const K& key) override;
	uint32_t get_size() override;
	uint32_t get_max_size() override;
	void print_all() override;
private:
	hashTable<K, lru_node<K, V>> m_hashTab;
	lru_node<K, V>* m_pHead;
	lru_node<K, V>* m_pTail;
	ILru_event<K, V>* m_pEvent;
	uint32_t m_dwSize;
	uint32_t m_dwCapacity;
};

#include "lru_manager.inl"