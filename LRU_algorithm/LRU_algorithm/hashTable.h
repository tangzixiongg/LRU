#pragma once

/*
	1、求哈希值：除留余数法
	2、解决哈希冲突：拉链法
*/

#include <iostream>
#include <stdint.h>
#include <string>

//判断是否为素数
bool isPrim(uint32_t dwNum)
{
	if (dwNum <= 3)
	{
		return dwNum > 1;
	}
	uint32_t dwSq = (uint32_t)sqrt(dwNum);
	for (uint32_t dwIdx = 2; dwIdx <= dwSq; dwIdx++)
	{
		if (dwNum % dwIdx == 0)
			return false;
	}
	return true;
}

const uint32_t MAX_SIZE = 1024 * 1024;	//哈希表的长度
const int32_t MOD = 1048573;

// <------- test ------->
//const uint32_t MAX_SIZE = 10;	//哈希表的长度
//const int32_t MOD = 7;

uint32_t hash(uint32_t dwKey)
{
	return dwKey % MOD;
}

uint32_t hash(std::string strKey)
{
	uint32_t dwKey = 0;
	for (size_t i = 0; i < strKey.size(); i++)
	{
		dwKey = dwKey * 10 + (strKey[i] - '0');
	}
	return hash(dwKey);
}

template<class K, class V>
struct hashNode
{
	//hashNode(){}
	hashNode(K _key, V* _value)
	{
		key = _key;
		value = _value;
		pPre = pNext = nullptr;
	}
	K key;
	V* value;
	hashNode<K, V>* pPre;
	hashNode<K, V>* pNext;
};


template<class K, class V>
class hashTable
{
public:
	hashTable();
	~hashTable();
	bool isExist(const K key);
	bool insert(const K key, const V* value);
	bool remove(const K key);
	V* find(const K key) const;
	void print_all() const;
private:
	uint32_t myhash(const K key) const;
	struct hashNode<K, V>* m_theList[MAX_SIZE];
};

template<class K, class V>
hashTable<K, V>::hashTable()
{
	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		m_theList[i] = nullptr;
	}
}

template<class K, class V>
hashTable<K, V>::~hashTable()
{
	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		if (m_theList[i] != nullptr)
		{
			struct hashNode <K, V>* stListNode = m_theList[i];
			while (stListNode != nullptr)
			{
				struct hashNode <K, V>* stListTemp = stListNode;
				stListNode = stListNode->pNext;
				delete stListTemp;
			}
		}
	}
}

template<class K, class V>
bool hashTable<K, V>::isExist(const K key)
{
	uint32_t dwIdx = myhash(key);
	struct hashNode<K, V>* stListNode = m_theList[dwIdx];
	while (stListNode != nullptr)
	{
		if (stListNode->key == key)
			return true;
		stListNode = stListNode->pNext;
	}
	return false;
}

template<class K, class V>
bool hashTable<K, V>::insert(const K key, const V* value)
{
	if (isExist(key)) return false;
	uint32_t dwIdx = myhash(key);
	struct hashNode<K, V>* stHeadList = m_theList[dwIdx];
	struct hashNode<K, V>* stNewList = new struct hashNode<K, V>(key, const_cast<V*>(value));
	if (stHeadList == nullptr)
	{
		//stHeadList = new struct hashNode<K, V>();
		stHeadList = stNewList;
		m_theList[dwIdx] = stHeadList;
	}
	else
	{
		stNewList->pNext = stHeadList;
		stNewList->pPre = stHeadList->pPre;
		stHeadList->pPre = stNewList;
		m_theList[dwIdx] = stNewList;
	}
	return true;
}

template<class K, class V>
bool hashTable<K, V>::remove(const K key)
{
	if (!isExist(key)) return false;
	uint32_t dwIdx = myhash(key);
	struct hashNode<K, V>* stListNode = m_theList[dwIdx];
	if (stListNode == nullptr) return false;
	while (stListNode)
	{
		if (stListNode->key == key)
		{
			if (stListNode->pPre == nullptr && stListNode->pNext == nullptr)
			{
				m_theList[dwIdx] = nullptr;
			}
			else if (stListNode->pPre == nullptr)
			{
				stListNode->pNext->pPre = nullptr;
				stListNode->pNext = nullptr;
			}
			else if (stListNode->pNext == nullptr)
			{
				stListNode->pPre->pNext = nullptr;
				stListNode->pPre = nullptr;
			}
			else
			{
				stListNode->pPre->pNext = stListNode->pNext;
				stListNode->pNext->pPre = stListNode->pPre;
			}
			delete stListNode;
			stListNode = nullptr;
			return true;
		}
		stListNode = stListNode->pNext;
	}
	return false;
}

template<class K, class V>
V* hashTable<K, V>::find(const K key) const
{
	uint32_t dwIdx = myhash(key);
	struct hashNode<K, V>* stListNode = m_theList[dwIdx];
	while (stListNode != nullptr)
	{
		if (stListNode->key == key)
			return stListNode->value;
			//return stListNode;
		stListNode = stListNode->pNext;
	}
	return nullptr;
}


template<class K, class V>
uint32_t hashTable<K, V>::myhash(const K key) const
{
	return hash(key);
}

template<class K, class V>
void hashTable<K, V>::print_all() const
{
	for (uint32_t dwIdx = 0; dwIdx < MAX_SIZE; dwIdx++)
	{
		if (m_theList[dwIdx] != nullptr)
		{
			struct hashNode<K, V>* stListNode = m_theList[dwIdx];
			std::cout << "the idx: " << dwIdx << std::endl;
			while (stListNode != nullptr)
			{
				std::cout << "key: " << stListNode->key << ", value: " << stListNode->value << std::endl;
				stListNode = stListNode->pNext;
			}
			std::cout << std::endl;
		}
	}
}

