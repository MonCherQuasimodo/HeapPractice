#include <iostream>
#include <sstream>
#include <ctime>


#include "binomialheap.h"
#include "leftistheap.h"
#include "obliqueheap.h"
#include "stdheap.h"

#include "test.h"

int solve(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
    return 0;
}

int main(int argc, char **argv)
{
    try {
        return solve(argc, argv);
    } catch(...) {
        std::cout << "Error\n";
    }
}
