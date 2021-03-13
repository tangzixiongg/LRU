#include "lru_manager.h"

template<class K, class V>
lru_manager<K, V>::lru_manager()
{
	m_dwCapacity = m_dwSize = 0;
	m_pEvent = nullptr;
	m_pHead = nullptr;
	m_pTail = nullptr;
}

template<class K, class V>
lru_manager<K, V>::~lru_manager()
{
	m_dwCapacity = m_dwSize = 0;
}

template<class K, class V>
void lru_manager<K, V>::init(uint32_t dwMax, ILru_event<K, V>* pEvent)
{
	m_dwCapacity = dwMax;
	m_pEvent = pEvent;
}



template<class K, class V>
bool lru_manager<K, V>::add(const K& key, const V& value)
{
	lru_node<K, V>* pLruNode = m_hashTab.find(key);
	
	if (pLruNode != nullptr)
	{
		if (m_pHead == m_pTail)	//只有一个节点
		{
			return true;
		}
		else if (pLruNode->pPre == nullptr)
		{
			return true;
		}
		else if (pLruNode->pNext == nullptr)
		{
			pLruNode->pPre->pNext = nullptr;
			pLruNode->pPre = nullptr;
			delete pLruNode;
		}
	}

	lru_node<K, V>* pLruNewNode = new lru_node<K, V>();
	pLruNewNode->key = key;
	pLruNewNode->value = value;
	if (m_pHead == nullptr)
	{
		m_pHead = pLruNewNode;
		m_pTail = pLruNewNode;
	}
	else
	{
		pLruNewNode->pNext = m_pHead;
		m_pHead->pPre = pLruNewNode;
		m_pHead = pLruNewNode;
	}
	m_hashTab.insert(key, pLruNewNode);

	if (m_dwSize + 1 > m_dwCapacity)
	{
		pLruNode = m_pTail;
		if (m_pTail->pPre != nullptr)
		{
			m_pTail->pPre->pNext = nullptr;
			m_pTail = m_pTail->pPre;
			if (m_pEvent == nullptr)
			{
				return false;
			}
			m_pEvent->on_remove(pLruNode->key, &pLruNode->value);
			m_hashTab.remove(pLruNode->key);
			delete pLruNode;
			//pLruNode = nullptr;
		}
	}
	else
	{
		m_dwSize++;
	}
	
	return true;
}

template<class K, class V>
bool lru_manager<K, V>::add(const K& key, const V* value)
{
	return add(key, *value);
}

template<class K, class V>
V* lru_manager<K, V>::get(const K& key)
{
	lru_node<K, V>* pLruNode = m_hashTab.find(key);
	if (pLruNode == nullptr)
		return nullptr;
	if (pLruNode != m_pHead)
	{
		if (pLruNode->pPre)
		{
			pLruNode->pPre->pNext = pLruNode->pNext;
		}
		if (pLruNode->pNext)
		{
			pLruNode->pNext->pPre = pLruNode->pPre;
		}
		pLruNode->pNext = m_pHead;
		m_pHead->pPre = pLruNode;
		m_pHead = pLruNode;
		//m_pHead->pPre = nullptr;
	}
	return &pLruNode->value;
}

template<class K, class V>
uint32_t lru_manager<K, V>::get_size()
{
	return m_dwSize;
}

template<class K, class V>
uint32_t lru_manager<K, V>::get_max_size()
{
	return m_dwCapacity;
}

template<class K, class V>
void lru_manager<K, V>::print_all()
{
	std::cout << " <-------------------------- print begin --------------------------> " << std::endl;
	lru_node<K, V>* pLruNode = m_pHead;
	uint32_t dwIndex = 0;
	while (pLruNode != nullptr)
	{
		std::cout << "idx: " << ++dwIndex << ", key: " << pLruNode->key << ", value: " << pLruNode->value << std::endl;
		pLruNode = pLruNode->pNext;
	}
	std::cout << " <-------------------------- print end --------------------------> " << std::endl;
	std::cout << std::endl;
}


template<class K, class V>
ILruManager<K, V>* create_lru_manager()
{
	return new lru_manager<K, V>();
}