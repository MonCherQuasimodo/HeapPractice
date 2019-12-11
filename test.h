#ifndef TEST_H
#define TEST_H

#include <gtest/gtest.h>
#include "iheap.h"

enum methodsType {ADDHEAP = 0, INSERT = 1, GETMIN = 2, EXTRACTMIN = 3, MELD = 4};

class TestHeap : public ::testing::TestWithParam<typesHeap::types>
{
public:
	TestHeap();
protected:
	void SetUp();
	void TearDown();
    std::vector <IHeap<int>*> testingHeap_;
    std::vector <IHeap<int>*> standardHeap_;

	void initIHeapPtr(typesHeap::types type_, IHeap<int>*& ptr);

	void MELD (size_t ind1, size_t ind2);
 	void ADDHEAP(int key, typesHeap::types type);
	void INSERT(size_t ind, int key);
	bool GETMIN(size_t ind);
	bool EXTRACTMIN(size_t ind);

	bool equal();
	struct Call
	{
		int paramA;
		int paramB;
		int paramC;
		Call(int a, int b, int c) :
			paramA(a),
			paramB(b),
			paramC(c) {}
		Call() :
			paramA(0),
			paramB(0),
			paramC(0) {}
	};
	std::vector <Call> calls_;
	static const size_t MaxMethods = 500000;
};

INSTANTIATE_TEST_CASE_P(list, TestHeap,::testing::Values(typesHeap::types::BINOMIAL,
														 typesHeap::types::LEFTIST,
														 typesHeap::types::OBLIQUE));

TestHeap::TestHeap():
	calls_(MaxMethods + 5)
{
	srand(time(NULL));
	for (size_t i = 0; i < calls_.size(); ++i){
		calls_[i] = {rand(), rand(), rand()};
	}
}

void TestHeap::initIHeapPtr(typesHeap::types type, IHeap<int>*& ptr)
{
	switch (type) {
		case typesHeap::types::OBLIQUE:
			ptr = new ObliqueHeap<int>();
			break;
		case typesHeap::types::LEFTIST:
			ptr = new LeftistHeap<int>();
			break;
		case typesHeap::types::BINOMIAL:
			ptr = new BinomialHeap<int>();
			break;
		default:
			throw "Undefined heap type";
	}
}

///____________________Native_functions__________________________///

void TestHeap::MELD (size_t ind1, size_t ind2)
{
	testingHeap_[ind1]->meld(*testingHeap_[ind2]);
	standardHeap_[ind1]->meld(*standardHeap_[ind2]);
}

void TestHeap::ADDHEAP(int key, typesHeap::types type)
{
	IHeap<int>* add = nullptr;
	initIHeapPtr(type, add);

	add->insert(key);
	testingHeap_.push_back(add);
	standardHeap_.push_back(new STDHeap<int>(key));
}

void TestHeap::INSERT(size_t ind, int key)
{
	testingHeap_[ind]->insert(key);
	standardHeap_[ind]->insert(key);
}

bool TestHeap::GETMIN(size_t ind)
{
	if (testingHeap_[ind]->empty() || standardHeap_[ind]->empty()){
		return true;
	}
	return testingHeap_[ind]->getMin() == standardHeap_[ind]->getMin();
}

bool TestHeap::EXTRACTMIN(size_t ind)
{
	if (testingHeap_[ind]->empty() || standardHeap_[ind]->empty()){
		return true;
	}
	return testingHeap_[ind]->extractMin() == standardHeap_[ind]->extractMin();
}

bool TestHeap::equal()
{
	for (size_t i = 0; i < standardHeap_.size(); ++i){
		if (standardHeap_[i]->toVector() != testingHeap_[i]->toVector()){
			return false;
		}
	}
	return true;
}

///____________________SiftUp_TearDown___________________________///
void TestHeap::SetUp()
{
	for (size_t i = 0; i < MaxMethods; ++i){
		methodsType type = static_cast <methodsType> (calls_[i].paramA % 5);
		switch (type) {
			case methodsType::ADDHEAP:
				ADDHEAP(calls_[i].paramB, GetParam());
				break;
			case methodsType::INSERT:
				if (testingHeap_.size())
					INSERT(calls_[i].paramB % testingHeap_.size(), calls_[i].paramC);
				break;
			case methodsType::GETMIN:
				if (testingHeap_.size())
					GETMIN(calls_[i].paramB % testingHeap_.size());
				break;
			case methodsType::EXTRACTMIN:
				if (testingHeap_.size()){
					size_t ind = calls_[i].paramB % testingHeap_.size();
					EXTRACTMIN(ind);
				}
				break;
			case methodsType::MELD:
				if (testingHeap_.size())
					MELD(calls_[i].paramB % testingHeap_.size(), calls_[i].paramC % testingHeap_.size());
				break;
			default:
				break;
		}
	}
}

///________________________________TESTS_________________________///
void TestHeap::TearDown()
{
	for (size_t i = 0; i < testingHeap_.size(); ++i){
		delete testingHeap_[i];
		delete standardHeap_[i];
	}
}

TEST_P(TestHeap, AddHeap)
{
	std::cout << typesHeap::toString[static_cast<size_t>(GetParam())] << ' ';
	int key = calls_[MaxMethods + static_cast<size_t>(methodsType::ADDHEAP)].paramA;
	ADDHEAP(key, GetParam());
	ASSERT_TRUE(equal());
}

TEST_P(TestHeap, Insert)
{
	std::cout << typesHeap::toString[static_cast<size_t>(GetParam())] << ' ';
	if (testingHeap_.size()){
		size_t ind = calls_[MaxMethods + static_cast<size_t>(methodsType::INSERT)].paramA % testingHeap_.size();
		int key = rand();
		INSERT(ind, key);
		ASSERT_TRUE(equal());
	}
}

TEST_P(TestHeap, GetMin)
{
	std::cout << typesHeap::toString[static_cast<size_t>(GetParam())] << ' ';
	if (testingHeap_.size()){
		size_t ind = calls_[MaxMethods + static_cast<size_t>(methodsType::GETMIN)].paramA % testingHeap_.size();
		ASSERT_TRUE (GETMIN(ind));
		ASSERT_TRUE(equal());
	}
}

TEST_P(TestHeap, ExtractMin)
{
	std::cout << typesHeap::toString[static_cast<size_t>(GetParam())] << ' ';
	if (testingHeap_.size()){
		size_t ind = calls_[MaxMethods + static_cast<size_t>(methodsType::EXTRACTMIN)].paramA % testingHeap_.size();
		ASSERT_TRUE (EXTRACTMIN(ind));
		ASSERT_TRUE(equal());
	}
}

TEST_P(TestHeap, MeldHeap)
{
	std::cout << typesHeap::toString[static_cast<size_t>(GetParam())] << ' ';
	if (testingHeap_.size()){
		size_t ind1 = calls_[MaxMethods + static_cast<size_t>(methodsType::MELD)].paramA % testingHeap_.size();
		size_t ind2 = calls_[MaxMethods + static_cast<size_t>(methodsType::MELD)].paramB % testingHeap_.size();
		MELD(ind1, ind2);
		ASSERT_TRUE(equal());
	}
}
#endif /* TEST_H */
