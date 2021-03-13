#include "lru_manager.h"

class lru_event : public ILru_event<int32_t, int32_t>
{
public:
	virtual void on_remove(int32_t& nKey, int32_t* pValue)
	{
		std::cout << "on_remove,key: " << nKey << ", value: " << *pValue << std::endl;
	}
};


struct testNode
{
	uint64_t qwRoleId;
	uint32_t dwGlobalId;
	char szName[50];
	testNode()
	{
		memset(this, 0, sizeof(testNode));
	}
	testNode(uint64_t _qwRoldId, uint32_t _dwGlobalId)
	{
		qwRoleId = _qwRoldId;
		dwGlobalId = _dwGlobalId;
		//memset(this, 0, sizeof(testNode));
	}
};

const uint64_t qwMaxn = 5;

int32_t main(int argc, char** argv)
{
	//	<--------------------- hashTable test_1 --------------------->
	/* 
	hashTable<int32_t, int32_t>* myMap = new hashTable<int32_t, int32_t>();
	for (uint32_t dwIdx = 1; dwIdx <= 100; dwIdx++)
	{
		myMap->insert(dwIdx, dwIdx + 1000);
	}
	myMap->print_all();

	std::cout << " <-------------------- remove --------------------> " << std::endl;

	for (uint32_t dwIdx = 10; dwIdx <= 20; dwIdx++)
	{
		myMap->remove(dwIdx);
	}
	myMap->print_all();
	*/
	
	//	<--------------------- hashTable test_2 --------------------->
	/*
	hashTable<int32_t, testNode>* myMap = new hashTable<int32_t, testNode>();
	testNode tmp(6, 8);
	myMap->insert(11, tmp);
	testNode*ttt = myMap->find(11);
	std::cout << ttt->qwRoleId << "," << ttt->dwGlobalId << std::endl;
	*/

	//  <--------------------- 无脑测试 --------------------->
	/*
	testNode tmp(6, 8);
	lru_event Event;
	ILruManager<int32_t, testNode>* pLruMgr = create_lru_manager<int32_t, testNode>();
	pLruMgr->add(11, tmp);
	*/


	/***********************************************************************
	
							以下是 LRU算法 测试
	
	***********************************************************************/

	//  <--------------------- LRU alrigothm test_1 --------------------->
	lru_event Event;
	ILruManager<int32_t, int32_t>* pLruMgr = create_lru_manager<int32_t, int32_t>();
	
	pLruMgr->init(qwMaxn, &Event);

	for (int32_t nIndex = 1; nIndex <= 10; nIndex++)
	{
		std::cout << " =========== add num: " << nIndex << std::endl;
		pLruMgr->add(nIndex, nIndex);
		std::cout << " =========== cache curSize is: " << pLruMgr->get_size() << std::endl;
		std::cout << " =========== cache capacity is: " << pLruMgr->get_max_size() << std::endl;
		pLruMgr->print_all();
	}
	std::cout << " =========== begin status =========== " << std::endl;
	std::cout << " =========== cache curSize is: " << pLruMgr->get_size() << std::endl;
	std::cout << " =========== cache capacity is: " << pLruMgr->get_max_size() << std::endl;
	pLruMgr->print_all();

	std::cout << " =========== operator the num 8 =========== " << std::endl;
	std::cout << " =========== cache curSize is: " << pLruMgr->get_size() << std::endl;
	std::cout << " =========== cache capacity is: " << pLruMgr->get_max_size() << std::endl;
	pLruMgr->get(8);
	pLruMgr->print_all();
	
	std::cout << " =========== add num 11 =========== " << std::endl;
	std::cout << " =========== cache curSize is: " << pLruMgr->get_size() << std::endl;
	std::cout << " =========== cache capacity is: " << pLruMgr->get_max_size() << std::endl;
	pLruMgr->add(11, 11);
	pLruMgr->print_all();
	

	return 0;
}