#include <gtest/gtest.h>
#include <parent.hpp>
#include <sstream>
#include <vector>
#include <numeric>
#include <filesystem>

namespace fs = std::filesystem;

void TestParent(const std::string& input, const std::string& expectedOutput) {
    std::stringstream inFile(input);
    std::stringstream outFile;

    const char* pathToChild1 = getenv("PATH_TO_CHILD1");
    const char* pathToChild2 = getenv("PATH_TO_CHILD2");

    if (pathToChild1 != nullptr && pathToChild2 != nullptr) {
        if (fs::exists(pathToChild1) && fs::exists(pathToChild2)) {
            ParentProcess(pathToChild1, pathToChild2, inFile, outFile);
            std::string result;
            std::string expectedLine;
            std::istringstream expectedStream(expectedOutput);
            bool match = true;

            while (std::getline(expectedStream, expectedLine)) {
                if (!std::getline(outFile, result) || result != expectedLine) {
                    match = false;
                    break;
                }
            }

            if (std::getline(outFile, result)) {
                match = false;
            }

            EXPECT_TRUE(match);
        } else {
            std::cout << "PATH DOES NOT EXIST" << std::endl;
        }
    } else {
        std::cout << "ENV VAR DOES NOT EXIST" << std::endl;
    }
}

TEST(ParentTest, ConvertsStringCorrectly) {
    std::string input = "hello world\n";
    std::string expected_output = "HELLO_WORLD\n"; 
    TestParent(input, expected_output);
}

TEST(ParentTest, HandlesNoSpaces) {
    std::string input = "helloworld\n";
    std::string expected_output = "HELLOWORLD\n";
    TestParent(input, expected_output);
}

TEST(ParentTest, HandlesOnlySpaces) {
    std::string input = "     \n";
    std::string expected_output = "_____";
    TestParent(input, expected_output);
}

TEST(ParentTest, HandlesEmptyString) {
    std::string input = "";
    std::string expected_output = "";
    TestParent(input, expected_output);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
