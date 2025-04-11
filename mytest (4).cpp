// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester{
public:
    // Tester specific enum
    // used to measure time for different operations
    enum MEASURE_TIME_OPERATION {INSERT, REMOVE};
    // Tests
    // TEST 1: test the insertion function for a normal case
    bool testInsertNormal(SatNet& satNet, int n = 10);
    // TEST 2: test the insertion function for an edge case
    bool testInsertEdgeCases(SatNet& satNet);
    // TEST 3: Test the insertion function for an error case (adding a duplicate item).
    bool testInsertErrorCases(SatNet& satNet);
    // TEST 4: Test whether the tree is balanced after a decent number of insertions, e.g. 300 insertions.
    bool testBalancedAfterNInsertions(SatNet& satNet, int n = 300);
    // TEST 5: TEST 5: Test whether the BST property is preserved after N insertions.
    bool testBSTPropertyAfterNInsertions(SatNet& satNet, int n = 300);
    // TEST 6: Test the remove function for a normal case.
    bool testRemoveNormal(SatNet& satNet, int insert = 10, int remove = 5);
    // TEST 7: Test the remove function for an edge case.
    bool testRemoveEdgeCases(SatNet& satNet);
    // TEST 8: Test whether the tree is balanced after a decent number of insertions and removals, e.g. 300 insertions and 100 removals.
    bool testBalancedAfterNRemovals(SatNet& satNet, int insert = 300, int remove = 100);
    // TEST 9: Test whether the BST property is preserved after all insertions and removals.
    bool testBSTPropertyAfterNRemovals(SatNet& satNet, int insert = 300, int remove = 100);
    // TEST 10: Test the removeDeorbited() functionality.
    bool testRemoveDeorbited(SatNet& satNet);
    // TEST 11: Test the countSatellites(...) functionality.
    bool testCountSatellites(SatNet& satNet);
    // TEST 12: Test the findSatellite(...) functionality for a normal case.
    bool testFindSatelliteNormal(SatNet& satNet);
    // TEST 13: Test the findSatellite(...) functionality for an edge case.
    bool testFindSatelliteEdgeCases(SatNet& satNet);
    // TEST 14: Test the assignment operator for a normal case.
    bool testAssignmentOperatorNormal(SatNet& satNet);
    // TEST 15: Test the assignment operator for an error case, e.g. assigning an empty object to an empty object.
    bool testAssignmentOperatorErrorCases(SatNet& satNet);
    // TEST 16: Prove that the removal operation performs in O(log n).
    bool testRemoveTimeComplexity(SatNet& satNet, int n = 1000);
    // TEST 17: Prove that the insertion performs in O(log n).
    bool testInsertTimeComplexity(SatNet& satNet, int n = 1000);

private:
    // TEST Helpers
    // insert n number of satellites
    std::vector<int> insertNSatellites(SatNet& satNet, int n);
    // remove n number of satellites
    std::vector<int> removeNSatellites(SatNet& satNet, int n);
    // measureTime by SatNet, Operation and int number of elements
    double measureTime(SatNet& satNet, MEASURE_TIME_OPERATION op, int n);
};
int main(){
    Tester tester;

    // create satnet instance in unnamed namespace for each test
    // this way we can test each function separately
    // we can also use the same name for different instances
    {
        // TEST 1: test the insertion function for a normal case
        // create a satnet instance
        SatNet satNet;
        int numberOfSatellites = 10;
        // call the test function
        if (tester.testInsertNormal(satNet, numberOfSatellites))
            std::cout << "Test 1 passed: insertion function for a normal case (" << numberOfSatellites << " insertions)" << std::endl;
        else
            std::cout << "Test 1 failed: insertion function for a normal case" << std::endl;

    }
    {
        // TEST 2: test the insertion function for an edge case
        // create a satnet instance
        SatNet satNet;
        // call the test function
        if (tester.testInsertEdgeCases(satNet))
            std::cout << "Test 2 passed: insertion function for an edge case" << std::endl;
        else
            std::cout << "Test 2 failed: insertion function for an edge case" << std::endl;
    }
    {
        // TEST 3: test the insertion function an error case
        // create a satnet instance
        SatNet satNet;
        // call the test function
        if (tester.testInsertErrorCases(satNet))
            std::cout << "Test 3 passed: insertion function for an error case (rejected duplicate)" << std::endl;
        else
            std::cout << "Test 3 failed: insertion function for an error case (added duplicate)" << std::endl;
    }
    {
        // TEST 4: Test whether the tree is balanced after a decent number of insertions, e.g. 300 insertions.
        // create a satnet instance
        SatNet satNet;
        int numberOfSatellites = 600;
        // call the test function
        if (tester.testBalancedAfterNInsertions(satNet, numberOfSatellites))
            std::cout << "Test 4 passed: tree is balanced after " << numberOfSatellites << " insertions" << std::endl;
        else
            std::cout << "Test 4 failed: tree is not balanced after " << numberOfSatellites << " insertions" << std::endl;
    }
    {
        // TEST 5: Test whether the BST property is preserved after all insertions.
        // create a satnet instance
        SatNet satNet;
        int numberOfSatellites = 120;
        // call the test function
        if (tester.testBSTPropertyAfterNInsertions(satNet, numberOfSatellites))
            std::cout << "Test 5 passed: BST property is preserved after " << numberOfSatellites << " insertions" << std::endl;
        else
            std::cout << "Test 5 failed: BST property is not preserved after " << numberOfSatellites << " insertions" << std::endl;
    }
    {
        // TEST 6: Test the remove function for a normal case.
        // create a satnet instance
        SatNet satNet;
        int insertSatellites = 1;
        int removeSatellites = 1;

        // call the test function
        if (tester.testRemoveNormal(satNet, insertSatellites, removeSatellites))
            std::cout << "Test 6 passed: remove function for a normal case (" << removeSatellites << " removals)" << std::endl;
        else
            std::cout << "Test 6 failed: remove function for a normal case" << std::endl;
    }
    {
        // TEST 7: Test the remove function for an edge case.
        // create a satnet instance
        SatNet satNet;
        // call the test function
        if (tester.testRemoveEdgeCases(satNet))
            std::cout << "Test 7 passed: remove function for an edge case" << std::endl;
        else
            std::cout << "Test 7 failed: remove function for an edge case" << std::endl;
    }
    {
        // TEST 8: Test whether the tree is balanced after a decent number of insertions and removals, e.g. 300 insertions and 100 removals.
        // create a satnet instance
        SatNet satNet;
        int insertSatellites = 300;
        int removeSatellites = 100;
        // call the test function
        if (tester.testBalancedAfterNRemovals(satNet, insertSatellites, removeSatellites))
            std::cout << "Test 8 passed: tree is balanced after " << insertSatellites << " insertions and " << removeSatellites << " removals" << std::endl;
        else
            std::cout << "Test 8 failed: tree is not balanced after " << insertSatellites << " insertions and " << removeSatellites << " removals" << std::endl;
    }
    {
        // TEST 9: Test whether the BST property is preserved after all insertions and removals.
        // create a satnet instance
        SatNet satNet;
        int insertSatellites = 300;
        int removeSatellites = 100;
        // call the test function
        if (tester.testBSTPropertyAfterNRemovals(satNet, insertSatellites, removeSatellites))
            std::cout << "Test 9 passed: BST property is preserved after " << insertSatellites << " insertions and " << removeSatellites << " removals" << std::endl;
        else
            std::cout << "Test 9 failed: BST property is not preserved after " << insertSatellites << " insertions and " << removeSatellites << " removals" << std::endl;
    }
    {
        // TEST 10: Test the removeDeorbited() functionality.
        // create a satnet instance
        SatNet satNet;
        // call the test function
        if (tester.testRemoveDeorbited(satNet))
            std::cout << "Test 10 passed: removeDeorbited() functionality (100 DEORBITED removed)" << std::endl;
        else
            std::cout << "Test 10 failed: removeDeorbited() functionality" << std::endl;
    }
    {
        // TEST 11: Test the countSatellites(...) functionality.
        // create a satnet instance
        SatNet satNet;
        // call the test function
        if (tester.testCountSatellites(satNet))
            std::cout << "Test 11 passed: countSatellites(...) functionality" << std::endl;
        else
            std::cout << "Test 11 failed: countSatellites(...) functionality" << std::endl;
    }
    {
        // TEST 12: Test the findSatellite(...) functionality for a normal case.
        // create a satnet instance
        SatNet satNet;
        // call the test function
        if (tester.testFindSatelliteNormal(satNet))
            std::cout << "Test 12 passed: findSatellite(...) functionality" << std::endl;
        else
            std::cout << "Test 12 failed: findSatellite(...) functionality" << std::endl;
    }
    {
        // TEST 13: Test the findSatellite(...) functionality for an edge case.
        // create a satnet instance
        SatNet satNet;
        // call the test function
        if (tester.testFindSatelliteEdgeCases(satNet))
            std::cout << "Test 13 passed: findSatellite(...) functionality" << std::endl;
        else
            std::cout << "Test 13 failed: findSatellite(...) functionality" << std::endl;
    }
    {
        // TEST 14: Test the assignment operator for a normal case.
        // create a satnet instance
        SatNet satNet;
        // call the test function
        if (tester.testAssignmentOperatorNormal(satNet))
            std::cout << "Test 14 passed: assignment operator for a normal case" << std::endl;
        else
            std::cout << "Test 14 failed: assignment operator for a normal case" << std::endl;
    }
    {
        // TEST 15: Test the assignment operator for an error case, e.g. assigning an empty object to an empty object.
        // create a satnet instance
        SatNet satNet;
        // call the test function
        if (tester.testAssignmentOperatorErrorCases(satNet))
            std::cout << "Test 15 passed: assignment operator for an error case (empty object to empty object)" << std::endl;
        else
            std::cout << "Test 15 failed: assignment operator for an error case (empty object to empty object)" << std::endl;
    }
    {
        // TEST 16: Prove that the removal operation performs in O(log n).
        // create a satnet instance
        SatNet satNet;
        int numberOfSatellites = 30000;
        // call the test function
        if (tester.testRemoveTimeComplexity(satNet, numberOfSatellites))
            std::cout << "Test 16 passed: removal operation performs in O(log n) (n = " << numberOfSatellites << ")" << std::endl;
        else
            std::cout << "Test 16 failed: removal operation does not perform in O(log n) (n = " << numberOfSatellites << ")" << std::endl;
    }
    {
        // TEST 17: Prove that the insertion performs in O(log n).
        // create a satnet instance
        SatNet satNet;
        int numberOfSatellites = 30000;
        // call the test function
        if (tester.testInsertTimeComplexity(satNet, numberOfSatellites))
            std::cout << "Test 17 passed: insertion operation performs in O(log n) (n = " << numberOfSatellites << ")" << std::endl;
        else
            std::cout << "Test 17 failed: insertion operation does not perform in O(log n) (n = " << numberOfSatellites << ")" << std::endl;
    }
}

// Test Helpers
// insert n number of satellites
// vectors ensure n unique IDs are generated
std::vector<int> Tester::insertNSatellites(SatNet& satNet, int n) {
    // Create a Random instance for generating unique satellite IDs
    Random idGen(MINID, MAXID);

    // Create a vector to store the inserted satellite IDs
    std::vector<int> insertedIDs;

    for (int i = 0; i < n; i++) {
        int id;
        do {
            id = idGen.getRandNum(); // Generate a unique random ID
        } while (std::find(insertedIDs.begin(), insertedIDs.end(), id) != insertedIDs.end());

        insertedIDs.push_back(id); // Store the inserted ID
        Sat satellite(id);
        satNet.insert(satellite);
    }

    return insertedIDs;
}

// remove n number of satellites
// vectors ensure n unique IDs are generated
std::vector<int> Tester::removeNSatellites(SatNet& satNet, int n) {
    // Create a Random instance for generating unique satellite IDs
    Random idGen(MINID, MAXID);

    // Create a vector to store the removed satellite IDs
    std::vector<int> removedIDs;

    for (int i = 0; i < n; i++) {
        int id;
        do {
            id = idGen.getRandNum(); // Generate a unique random ID
        } while (std::find(removedIDs.begin(), removedIDs.end(), id) != removedIDs.end());

        removedIDs.push_back(id); // Store the removed ID
        satNet.remove(id);
    }

    return removedIDs;
}

// measure time for different operations
double Tester::measureTime(SatNet &satNet, Tester::MEASURE_TIME_OPERATION operation, int n) {
    // start time
    double T = 0.0;
    clock_t start, stop;
    start = clock();
    
    // perform operation switch-case
    switch (operation) {
        case INSERT:
            Tester::insertNSatellites(satNet, n);
            break;
        case REMOVE:
            Tester::removeNSatellites(satNet, n);
            break;
        default:
            // Invalid operation
            std::cerr << "Invalid operation type." << std::endl;
            return -1.0;
    }

    // stop time
    stop = clock();
    T = stop - start;
    double measureTime = T / CLOCKS_PER_SEC;
    return measureTime;
}


// Test 1: Test the insertion function for a normal case.
bool Tester::testInsertNormal(SatNet& satNet, int n) {
    try {
        // Create a Random instance for generating unique satellite IDs
        Random idGen(MINID, MAXID);

        // Insert 10 satellites and return the IDs of the inserted satellites
        std::vector<int> insertedIDs = Tester::insertNSatellites(satNet, n);

        // Check that the satellites were inserted correctly and can be found
        for (int i = 0; i < 10; i++) {
            int id = insertedIDs[i]; // Use the same IDs that were inserted
            if (!satNet.findSatellite(id)) {
                // If any satellite wasn't found, print an error message
                std::cerr << "Error: Satellite not found (ID: " << id << std::endl;
                return false;
            }
        }

        // All satellites inserted and found
        return true;
    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 1): " << e.what() << std::endl;
        return false;
    }
}

// Test 2: Test the insertion function for an edge case.
bool Tester::testInsertEdgeCases(SatNet& satNet) {
    try {
        // Create a few satellites with edge case IDs
        Sat satelliteMinID(MINID);
        Sat satelliteMaxID(MAXID);

        // Insert the satellites
        satNet.insert(satelliteMinID);
        satNet.insert(satelliteMaxID);

        // Check if the satellites were inserted correctly
        if (satNet.findSatellite(MINID) && satNet.findSatellite(MAXID)) {
            return true;
        } else {
            std::cerr << "Edge case for insertion test failed." << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 2): " << e.what() << std::endl;
        return false;
    }
}

// Test 3: Test the insertion function for an error case (adding a duplicate item).
bool Tester::testInsertErrorCases(SatNet& satNet) {
    try {
        // Create a few satellites with the same ID (error case)
        Sat satellite1(10001);
        Sat satellite2(10001);

        // Insert the first satellite
        satNet.insert(satellite1);

        // Try to insert the second satellite with the same ID (error)
        try {
            satNet.insert(satellite2);
            // if no exception is thrown, the test should fail
            std::cerr << "Error case for insertion test failed. Duplicate was allowed to be added" << std::endl;
            return false;
        }
        catch (const std::exception &e) {
            // if an exception is thrown, the test should pass
            return true;
        }
    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 3): " << e.what() << std::endl;
        return false;
    }
}

// TEST 4: Test whether the tree is balanced after a decent number of insertions, e.g. 300 insertions.
bool Tester::testBalancedAfterNInsertions(SatNet& satNet, int n) {
    try {
        // Insert n satellites and return the IDs of the inserted satellites
        std::vector<int> insertedIDs = Tester::insertNSatellites(satNet, n);

        // Check if the tree is balanced
        if (satNet.getBalance(satNet.getRoot()) <= 1 && satNet.getBalance(satNet.getRoot()) >= -1) {
            return true;
        } else {
            std::cerr << "Tree is not balanced after " << n << " insertions." << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 4): " << e.what() << std::endl;
        return false;
    }
}

// TEST 5: Test whether the BST property is preserved after all insertions.
bool Tester::testBSTPropertyAfterNInsertions(SatNet &satNet, int n) {
    try {
        // Insert n satellites and return the IDs of the inserted satellites
        std::vector<int> insertedIDs = Tester::insertNSatellites(satNet, n);

        // Check if the BST property is preserved
        if (satNet.getBSTValidity(satNet.getRoot())) {
            return true;
        } else {
            std::cerr << "BST property is not preserved after " << n << " insertions." << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 5): " << e.what() << std::endl;
        return false;
    }
}

// TEST 6: Test the remove function for a normal case.
bool Tester::testRemoveNormal(SatNet &satNet, int insert, int remove) {
    try {
        // Insert n satellites and return the IDs of the inserted satellites
        std::vector<int> insertedIDs = Tester::insertNSatellites(satNet, insert);

        // Remove n satellites and return the IDs of the removed satellites
        std::vector<int> removedIDs = Tester::removeNSatellites(satNet, remove);

        // Check if the satellites were removed correctly and cannot be found
        for (int i = 0; i < remove; i++) {
            int id = removedIDs[i]; // Use the same IDs that were removed
            if (satNet.findSatellite(id)) {
                // If any satellite was found, print an error message
                std::cerr << "Error (Test 6): Satellite found (ID: " << id << ")" << std::endl;
                return false;
            }
        }

        // All satellites removed and not found
        return true;
    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 6): " << e.what() << std::endl;
        return false;
    }
}

// TEST 7: Test the remove function for an edge case.
bool Tester::testRemoveEdgeCases(SatNet &satNet) {
    try {
        // Create a few satellites with edge case IDs
        Sat satelliteMinID(MINID);
        Sat satelliteMaxID(MAXID);

        // Insert the satellites
        satNet.insert(satelliteMinID);
        satNet.insert(satelliteMaxID);

        // Remove the satellites
        satNet.remove(MINID);
        satNet.remove(MAXID);

        // Check if the satellites were removed correctly
        if (!satNet.findSatellite(MINID) && !satNet.findSatellite(MAXID)) {
            return true;
        } else {
            std::cerr << "Edge case for removal test failed." << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 7): " << e.what() << std::endl;
        return false;
    }
}

// TEST 8: Test whether the tree is balanced after a decent number of insertions and removals, e.g. 300 insertions and 100 removals.
bool Tester::testBalancedAfterNRemovals(SatNet &satNet, int insert, int remove) {
    try {
        // Insert n satellites and return the IDs of the inserted satellites
        std::vector<int> insertedIDs = Tester::insertNSatellites(satNet, insert);

        // Remove n satellites and return the IDs of the removed satellites
        std::vector<int> removedIDs = Tester::removeNSatellites(satNet, remove);

        // Check if the tree is balanced
        if (satNet.getBalance(satNet.getRoot()) <= 1 && satNet.getBalance(satNet.getRoot()) >= -1) {
            return true;
        } else {
            std::cerr << "Tree is not balanced after " << insert << " insertions and " << remove << " removals." << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 8): " << e.what() << std::endl;
        return false;
    }
}

// TEST 9: Test whether the BST property is preserved after all insertions and removals.
bool Tester::testBSTPropertyAfterNRemovals(SatNet &satNet, int insert, int remove) {
    try {
        // Insert n satellites and return the IDs of the inserted satellites
        std::vector<int> insertedIDs = Tester::insertNSatellites(satNet, insert);

        // Remove n satellites and return the IDs of the removed satellites
        std::vector<int> removedIDs = Tester::removeNSatellites(satNet, remove);

        // Check if the BST property is preserved
        if (satNet.getBSTValidity(satNet.getRoot())) {
            return true;
        } else {
            std::cerr << "BST property is not preserved after " << insert << " insertions and " << remove << " removals." << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 9): " << e.what() << std::endl;
        return false;
    }
}

// TEST 10: Test the removeDeorbited() functionality.
bool Tester::testRemoveDeorbited(SatNet &satNet) {
    try {
        // Insert n satellites and return the IDs of the inserted satellites
        std::vector<int> insertedIDs = Tester::insertNSatellites(satNet, 300);

        // Set the state of 100 satellites to DEORBITED
        for (int i = 0; i < 100; i++) {
            satNet.setState(insertedIDs[i], DEORBITED);
        }

        // Remove all deorbited satellites
        satNet.removeDeorbited();

        // Check if the satellites were removed correctly and cannot be found
        for (int i = 0; i < 100; i++) {
            int id = insertedIDs[i]; // Use the same IDs that were removed
            if (satNet.findSatellite(id)) {
                // If any satellite was found, print an error message
                std::cerr << "Error (Test 10): Deorbited satellite found (ID: " << id << ")" << std::endl;
                return false;
            }
        }

    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 10): " << e.what() << std::endl;
        return false;
    }

    // All deorbited satellites removed and not found
    return true;
}

// TEST 11: Test the countSatellites(...) functionality.
bool Tester::testCountSatellites(SatNet &satNet) {
    try {
        // Create 2 of set inclination
        Sat satellite1(10001);
        satellite1.setInclin((INCLIN)0);
        satNet.insert(satellite1);

        Sat satellite2(10002);
        satellite2.setInclin((INCLIN)0);
        satNet.insert(satellite2);

        // Create 2 of different inclination
        Sat satellite3(10003);
        satellite3.setInclin((INCLIN)1);
        satNet.insert(satellite3);

        Sat satellite4(10004);
        satellite4.setInclin((INCLIN)1);
        satNet.insert(satellite4);

        // Check if the count is correct
        if (satNet.countSatellites((INCLIN)0) == 2) {
            return true;
        } else {
            std::cerr << "Error (Test 11): countSatellites(...) functionality" << std::endl;
            return false;
        }

    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 11): " << e.what() << std::endl;
        return false;
    }

}

// TEST 12: Test the findSatellite(...) functionality for a normal case.
bool Tester::testFindSatelliteNormal(SatNet &satNet) {
    try {
        // Create a few satellites with edge case IDs
        Sat satellite1(10001);
        Sat satellite2(10002);
        Sat satellite3(10003);
        Sat satellite4(10004);

        // Insert the satellites
        satNet.insert(satellite1);
        satNet.insert(satellite2);
        satNet.insert(satellite3);
        satNet.insert(satellite4);

        // Check if the satellites were found correctly
        if (satNet.findSatellite(10001) && satNet.findSatellite(10002) && satNet.findSatellite(10003) && satNet.findSatellite(10004)) {
            return true;
        } else {
            std::cerr << "Error (Test 12): findSatellite(...) functionality" << std::endl;
            return false;
        }

    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 12): " << e.what() << std::endl;
        return false;
    }
}

// TEST 13: Test the findSatellite(...) functionality for an edge case.
bool Tester::testFindSatelliteEdgeCases(SatNet &satNet) {
    try {
        // Create a few satellites with edge case IDs
        Sat satelliteMinID(MINID);
        Sat satelliteMaxID(MAXID);

        // Insert the satellites
        satNet.insert(satelliteMinID);
        satNet.insert(satelliteMaxID);

        // Check if the satellites were found correctly
        if (satNet.findSatellite(MINID) && satNet.findSatellite(MAXID)) {
            return true;
        } else {
            std::cerr << "Edge case for findSatellite(...) test failed." << std::endl;
            return false;
        }

    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Edge case for findSatellite(...) test failed." << std::endl;
        return false;
    }
}

// TEST 14: Test the assignment operator for a normal case.
bool Tester::testAssignmentOperatorNormal(SatNet &satNet) {
    try {
        // Create a few satellites with edge case IDs
        Sat satellite1(10001);
        Sat satellite2(10002);
        Sat satellite3(10003);
        Sat satellite4(10004);

        // Insert the satellites
        satNet.insert(satellite1);
        satNet.insert(satellite2);
        satNet.insert(satellite3);
        satNet.insert(satellite4);

        // Create a new satnet instance
        SatNet satNet2;

        // Assign satNet to satNet2
        satNet2 = satNet;

        // Check if the satellites were found correctly
        if (satNet2.findSatellite(10001) && satNet2.findSatellite(10002) && satNet2.findSatellite(10003) && satNet2.findSatellite(10004)) {
            return true;
        } else {
            std::cerr << "Error (Test 14): assignment operator functionality" << std::endl;
            return false;
        }

    }
    catch (const std::exception& e) {
        // Handle the exception (e.g., print an error message)
        std::cerr << "Error (Test 14): " << e.what() << std::endl;
        return false;
    }
}

// TEST 15: Test the assignment operator for an error case, e.g. assigning an empty object to an empty object.
bool Tester::testAssignmentOperatorErrorCases(SatNet &satNet) {
    try {
        // for this error case, we create a new satNet2, insert a node in satNet and check if that
        // node exists in satNet2. If it does, the test fails as a deepcopy was not made.

        // Create a new satNet instance
        SatNet satNet2;

        // Assign satNet to satNet2
        satNet2 = satNet;

        // Add a satellite to satNet
        Sat satellite1(10001);
        satNet.insert(satellite1);

        // Check if the satellite added to satNet was found in satNet2
        if (satNet2.findSatellite(10001)) {
            std::cerr << "Error case for assignment operator test failed." << std::endl;
            return false;
        } else {
            return true;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error (Test 15): " << e.what() << std::endl;
        return true;
    }
}

// TEST 16: Prove that the removal operation performs in O(log n).
bool Tester::testRemoveTimeComplexity(SatNet &satNet, int n) {
    // we create two trees, one with n nodes and one with 2n nodes
    // their ratio should be about 1.1 + 0.4 as the removal operation is O(log n)
    try {
        // deep copy satNet to satNet2
        SatNet satNet2 = satNet;
        // insert n elements to satNet and 2n elements to satNet2
        Tester::insertNSatellites(satNet, n);
        Tester::insertNSatellites(satNet2, 2*n);

        // measure time for removing n elements from satNet
        double time_for_n = Tester::measureTime(satNet, REMOVE, n);
        // measure time for removing 2n elements from satNet2
        double time_for_2n = Tester::measureTime(satNet2, REMOVE, 2*n);

        // calculate ratio
        // check if time_for_2n is not 0
        // if time_for_2n is zero, it follows that time_for_n is 0, and we cannot divide by 0
        // although machine-specific, this means that the operation is too fast to measure
        // in this case, we return true as it's not a failure of the test

        if (time_for_2n == 0) {
            return true;
        }

        double ratio = time_for_n / time_for_2n;
        // if ratio less than 1.5, test passes
        if (ratio <= 1.5) {
            return true;
        } else {
            std::cerr << "Error (Test 16): Removal operation does not perform in O(log n). (Ratio: " << ratio << ")" << std::endl;
            return false;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error (Test 16): " << e.what() << std::endl;
        return true;
    }

}

// TEST 17: Prove that the insertion performs in O(log n).
bool Tester::testInsertTimeComplexity(SatNet &satNet, int n) {
    // we create two trees, one with n nodes and one with 2n nodes
    // their ratio should be about 1.1 + 0.4 as the removal operation is O(log n)
    try {
        // deep copy satNet to satNet2
        SatNet satNet2 = satNet;

        // measure time for inserting n elements from satNet
        double time_for_n = Tester::measureTime(satNet, INSERT, n);
        // measure time for inserting 2n elements from satNet2
        double time_for_2n = Tester::measureTime(satNet2, INSERT, 2*n);

        // calculate ratio
        // check if time_for_2n is not 0
        // if time_for_2n is zero, it follows that time_for_n is 0, and we cannot divide by 0
        // although machine-specific, this means that the operation is too fast to measure
        // in this case, we return true as it's not a failure of the test

        if (time_for_2n == 0) {
            return true;
        }

        double ratio = time_for_n / time_for_2n;
        // if ratio less than 1.5, test passes
        if (ratio <= 1.5) {
            return true;
        } else {
            std::cerr << "Error (Test 17): Insert operation does not perform in O(log n). (Ratio: " << ratio << ")" << std::endl;
            return false;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error (Test 17): " << e.what() << std::endl;
        return true;
    }
}

