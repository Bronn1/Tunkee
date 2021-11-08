#include "gtest/gtest.h"

class myTestFixture : public ::testing::Test {
public:
    myTestFixture() {
        // initialization;
        // can also be done in SetUp()
    }

    void SetUp() {
        // initialization or some code to run before each test
    }

    void TearDown() {
        // code to run after each test;
        // can be used instead of a destructor,
        // but exceptions can be handled in this function only
    }

    ~myTestFixture() {
        //resources cleanup, no exceptions allowed
    }

};

TEST_F(myTestFixture, UnitTest2) {
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true) << "diagnostic message";;
}