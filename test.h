#ifndef TEST_H
#define TEST_H

#include <gtest/gtest.h>
#include "iheap.h"

namespace typesMethod
{
	enum type {ADDHEAP = 0, INSERT = 1, GETMIN = 2, EXTRACTMIN = 3, MELD = 4};
	const int numberMethods = 5;
	std::vector <std::string> toString = {"ADDHEAP", "INSERT", "GETMIN", "EXTRACTMIN", "MELD"};
}

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
	bool equal() const;
	bool eqEmptiness(size_t ind) const;

	void MELD (size_t ind1, size_t ind2);
 	void ADDHEAP(int key, typesHeap::types type);
	void INSERT(size_t ind, int key);
	bool GETMIN(size_t ind) const;
	bool EXTRACTMIN(size_t ind);
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

///____________________Constructor_______________________________///

TestHeap::TestHeap():
	calls_(MaxMethods + typesMethod::numberMethods)
{
	srand(time(NULL));
	for (size_t i = 0; i < calls_.size(); ++i){
		calls_[i] = {rand(), rand(), rand()};
	}
}

///____________________SiftUp_TearDown___________________________///
void TestHeap::SetUp()
{
	for (size_t i = 0; i < MaxMethods; ++i){
		typesMethod::type type = static_cast <typesMethod::type> (calls_[i].paramA % typesMethod::numberMethods);
		if (type == typesMethod::type::ADDHEAP){
			ADDHEAP(calls_[i].paramB, GetParam());
			continue;
		}
		if (testingHeap_.empty() || standardHeap_.empty()){
			continue;
		}
		switch (type) {
			case typesMethod::type::INSERT:
				INSERT(calls_[i].paramB % testingHeap_.size(), calls_[i].paramC);
				break;
			case typesMethod::type::GETMIN:
				GETMIN(calls_[i].paramB % testingHeap_.size());
				break;
			case typesMethod::type::EXTRACTMIN:
				EXTRACTMIN(calls_[i].paramB % testingHeap_.size());
				break;
			case typesMethod::type::MELD:
				MELD(calls_[i].paramB % testingHeap_.size(), calls_[i].paramC % testingHeap_.size());
				break;
			default:
				break;
		}
	}
}

void TestHeap::TearDown()
{
	for (size_t i = 0; i < testingHeap_.size(); ++i){
		delete testingHeap_[i];
		delete standardHeap_[i];
	}
}

///_____________________Helper_methods___________________________///
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


bool TestHeap::equal() const
{
	for (size_t i = 0; i < standardHeap_.size(); ++i){
		if (standardHeap_[i]->toVector() != testingHeap_[i]->toVector()){
			return false;
		}
	}
	return true;
}

bool TestHeap::eqEmptiness(size_t ind) const
{
	return testingHeap_[ind]->empty() && standardHeap_[ind]->empty();
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

bool TestHeap::GETMIN(size_t ind) const
{
	if (testingHeap_[ind]->empty() || standardHeap_[ind]->empty()){
		return eqEmptiness(ind);
	}
	return testingHeap_[ind]->getMin() == standardHeap_[ind]->getMin();
}

bool TestHeap::EXTRACTMIN(size_t ind)
{
	if (testingHeap_[ind]->empty() || standardHeap_[ind]->empty()){
		return eqEmptiness(ind);
	}
	return testingHeap_[ind]->extractMin() == standardHeap_[ind]->extractMin();
}

///________________________________TESTS_________________________///
TEST_P(TestHeap, AddHeap)
{
	std::cout << typesHeap::toString[static_cast<size_t>(GetParam())] << ' ';
	int key = calls_[MaxMethods + static_cast<size_t>(typesMethod::type::ADDHEAP)].paramA;
	ADDHEAP(key, GetParam());
	ASSERT_TRUE(equal());
}

TEST_P(TestHeap, Insert)
{
	std::cout << typesHeap::toString[static_cast<size_t>(GetParam())] << ' ';
	if (!testingHeap_.empty()){
		size_t ind = calls_[MaxMethods + static_cast<size_t>(typesMethod::type::INSERT)].paramA % testingHeap_.size();
		int key = rand();
		INSERT(ind, key);
		ASSERT_TRUE(equal());
	}
}

TEST_P(TestHeap, GetMin)
{
	std::cout << typesHeap::toString[static_cast<size_t>(GetParam())] << ' ';
	if (!testingHeap_.empty()){
		size_t ind = calls_[MaxMethods + static_cast<size_t>(typesMethod::type::GETMIN)].paramA % testingHeap_.size();
		ASSERT_TRUE (GETMIN(ind));
		ASSERT_TRUE(equal());
	}
}

TEST_P(TestHeap, ExtractMin)
{
	std::cout << typesHeap::toString[static_cast<size_t>(GetParam())] << ' ';
	if (!testingHeap_.empty()){
		size_t ind = calls_[MaxMethods + static_cast<size_t>(typesMethod::type::EXTRACTMIN)].paramA % testingHeap_.size();
		ASSERT_TRUE (EXTRACTMIN(ind));
		ASSERT_TRUE(equal());
	}
}

TEST_P(TestHeap, MeldHeap)
{
	std::cout << typesHeap::toString[static_cast<size_t>(GetParam())] << ' ';
	if (!testingHeap_.empty()){
		size_t ind1 = calls_[MaxMethods + static_cast<size_t>(typesMethod::type::MELD)].paramA % testingHeap_.size();
		size_t ind2 = calls_[MaxMethods + static_cast<size_t>(typesMethod::type::MELD)].paramB % testingHeap_.size();
		MELD(ind1, ind2);
		ASSERT_TRUE(equal());
	}
}
#endif /* TEST_H */
