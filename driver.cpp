#include "satnet.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

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
    double sampleTimeMeasurement(SatNet & aNet, int tempArray[], int arraySize);
    bool testInsertion();
    bool testEdgeInsertion();
    bool testInsertionErrorCase();
    bool testTreeBalance();
    bool testBSTProperty();
    bool testRemoveNormalCase();
    bool testRemoveEdgeCase();
    bool testBalancingAfterRemovals();
    bool testInsertPerformance();
    bool testBSTPropertyAfterRemovals();
    bool testDeorbited();
    bool testCountSatellites();
    bool testFindSatelliteNormalCase();
    bool testAssignmentOperatorErrorCase();

    bool testFindSatelliteErrorCase();
    bool testSetState();

    bool testRemovalPerformance();
};

bool isTreeBalanced(Sat* node) {
    if (node == nullptr) {
        return true;
    }

    int leftHeight = (node->getLeft() != nullptr) ? node->getLeft()->getHeight() : 0;
    int rightHeight = (node->getRight() != nullptr) ? node->getRight()->getHeight() : 0;
    int balance = leftHeight - rightHeight;

    if (abs(balance) > 1) {
        return false; // Tree is not balanced
    }

    return isTreeBalanced(node->getLeft()) && isTreeBalanced(node->getRight());
}

bool isBST(Sat* node) {
    if (node == nullptr) {
        return true;
    }

    if (node->getLeft() != nullptr && node->getLeft()->getID() > node->getID()) {
        return false; // BST property not preserved on the left
    }

    if (node->getRight() != nullptr && node->getRight()->getID() < node->getID()) {
        return false; // BST property not preserved on the right
    }

    return isBST(node->getLeft()) && isBST(node->getRight());
}

int main(){
    Tester tester;

    cout << "insert test: " << endl;
    cout << tester.testInsertion() << endl;
    cout << tester.testEdgeInsertion() << endl;
    cout << tester.testInsertionErrorCase() << endl;
    cout << tester.testTreeBalance() << endl;
    cout << tester.testBSTProperty() << endl;
    cout << endl;


    cout << "remove test: " << endl;
   cout << tester.testRemoveNormalCase() << endl;
    cout << tester.testRemoveEdgeCase() << endl;
    cout << tester.testBalancingAfterRemovals() << endl;
    cout << tester.testBSTPropertyAfterRemovals() << endl;
    cout << tester.testRemovalPerformance() << endl;

    cout << endl;

    cout << "Count test" << endl;
    cout << tester.testCountSatellites() << endl;
    cout << endl;

    cout << "Find test: " << endl;
    cout << tester.testFindSatelliteNormalCase() << endl;
    cout << tester.testFindSatelliteErrorCase() << endl;

    cout << "setState test" << endl;
    cout << tester.testSetState() << endl;
    cout << endl;
    cout << "Time Complexity Test: " << endl;
    cout << tester.testInsertPerformance() << endl;

    cout << "Assignment Operator Test" << endl;
    cout << tester.testAssignmentOperatorErrorCase() << endl;
    return 0;
}

bool Tester::testInsertion() {
    Random idGen(MINID, MAXID);
    Random inclinGen(0, 3);
    Random altGen(0, 3);
    SatNet network;

    int teamSize = 10;
    int tempID = 0;
    int ID = 0;
    int tempArray[teamSize];

    for (int i = 0; i < teamSize; i++) {
        ID = idGen.getRandNum();
        tempArray[i] = ID;
        if (i == teamSize / 2) tempID = ID;
        Sat satellite(ID,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        network.insert(satellite);
    }

    // checking if the satellite with tempID is removed
    network.remove(tempID);

    // seeing if all other inserted satellites are present
    for (int i = 0; i < teamSize; i++) {
        if (i != teamSize / 2) {
            if (!network.findSatellite(tempArray[i])) {
                return false; // Some inserted satellites are missing
            }
        }
    }

    return true;
}

bool Tester::testEdgeInsertion() {
    Random idGen(MINID, MAXID);
    Random inclinGen(0, 3);
    Random altGen(0, 3);
    SatNet network1;

    int size = 1000;
    int tempIDs[1001] = {0};
    int id = 0;
    for (int i = 0; i < size; i++) {
        id = idGen.getRandNum();
        tempIDs[i] = id;
        Sat satellite(id,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        network1.insert(satellite);
    }

    for (int i = 0; i < size; i++) {
        if (!network1.findSatellite(tempIDs[i])) {
            return false; // Some inserted satellites are missing
        }
    }

    return true;
}

bool Tester::testInsertionErrorCase() {
    SatNet network;

    Sat satellite1(12345, MI208, I48, ACTIVE);
    Sat satellite2(12345, MI215, I48, DEORBITED);

    network.insert(satellite1);
    network.insert(satellite2);

    if (network.countSatellites(I48) != 1) {
        return false; // The test failed because duplicate ID was inserted
    }

    return true;
}

bool Tester::testTreeBalance() {
    SatNet network;
    const int numInsertions = 300;

    for (int i = 0; i < numInsertions; i++) {
        Sat satellite(i, MI208, I48, ACTIVE);
        network.insert(satellite);
    }

    // Check if the tree is balanced
    bool balanced = isTreeBalanced(network.m_root);

    return balanced;
}

bool Tester::testBSTProperty() {
    SatNet network;
    const int numInsertions = 300;

    for (int i = 0; i < numInsertions; i++) {
        Sat satellite(i, MI208, I48, ACTIVE);
        network.insert(satellite);
    }

    // Check if the BST property is preserved
    bool bstPreserved = isBST(network.m_root);

    return bstPreserved;
}


bool Tester::testRemoveNormalCase() {
    SatNet network;

    Sat satellite1(1001, MI208, I48, ACTIVE);
    Sat satellite2(1002, MI215, I53, DEORBITED);
    Sat satellite3(1003, MI340, I70, ACTIVE);

    network.insert(satellite1);
    network.insert(satellite2);
    network.insert(satellite3);

    int idToRemove = 1002;
    network.remove(idToRemove);

    if (network.findSatellite(idToRemove)) {
        return false; // the satellite removal failed
    }

    return true;
}

bool Tester::testRemoveEdgeCase() {
    SatNet network;

    Sat satellite1(1001, MI208, I48, ACTIVE);
    Sat satellite2(1002, MI215, I53, DEORBITED);
    Sat satellite3(1003, MI340, I70, ACTIVE);

    network.insert(satellite1);
    network.insert(satellite2);
    network.insert(satellite3);

    // remove  root node
    int idToRemove = 1001;
    network.remove(idToRemove);

    // check if the root node was removed and is no longer in the tree
    if (network.findSatellite(idToRemove)) {
        return false;
    }

    return true;
}


bool Tester::testBalancingAfterRemovals() {

    SatNet satnet;
    const int numRemovals = 150;

    Random idGen(MINID, MAXID);
    Random inclinGen(0, 3);
    Random altGen(0, 3);
    SatNet network;

    int netSize = 300;


    for (int i = 0; i < netSize; i++) {
        Sat satellite(i,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        network.insert(satellite);
    }

    for (int i = 1; i <= numRemovals; i++) {
        int idToRemove = i * 2;
        satnet.remove(idToRemove);
    }

    // Check if the tree remains balanced
    bool isBalanced = isTreeBalanced(satnet.m_root);
    return isBalanced;
}
bool Tester::testBSTPropertyAfterRemovals() {
    SatNet satnet;
    const int numRemovals = 150;

    // Insert 300 nodes into the AVL tree

    Random idGen(MINID, MAXID);
    Random inclinGen(0, 3);
    Random altGen(0, 3);
    SatNet network;

    int netSize = 300;

    for (int i = 0; i < netSize; i++) {
        Sat satellite(i,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        network.insert(satellite);
    }

    // remove 150 nodes from the tree
    for (int i = 1; i <= numRemovals; i++) {
        int idToRemove = i * 2; // remove every second node
        satnet.remove(idToRemove);
    }

    //checkin bst property
    bool isBst= isBST(satnet.m_root);
    return isBst;
}

bool Tester::testCountSatellites() {
    SatNet satnet;

    Sat sat1(10001, MI208, I48, ACTIVE);
    Sat sat2(10002, MI215, I53, ACTIVE);
    Sat sat3(10003, MI340, I70, DEORBITED);
    Sat sat4(10004, MI350, I97, DEORBITED);

    satnet.insert(sat1);
    satnet.insert(sat2);
    satnet.insert(sat3);
    satnet.insert(sat4);

    int countI48 = satnet.countSatellites(I48);

    int countI53 = satnet.countSatellites(I53);

    // Check if the counts are correct
    if (countI48 == 1 && countI53 == 1) {
        return true;
    }

    return false;
}

bool Tester::testFindSatelliteNormalCase() {
    SatNet satnet;

    // Insert satellites
    Sat satellite1(1001, MI208, I48, ACTIVE);
    Sat satellite2(1002, MI215, I53, DEORBITED);
    Sat satellite3(1003, MI340, I70, ACTIVE);
    satnet.insert(satellite1);
    satnet.insert(satellite2);
    satnet.insert(satellite3);

    bool found1 = satnet.findSatellite(1001);
    bool found2 = satnet.findSatellite(1002);
    bool found3 = satnet.findSatellite(1003);

    // Check if the satellites were found
    if (found1 && found2 && found3) {
        return true;
    }

    return false;
}

bool Tester::testFindSatelliteErrorCase() {
    SatNet network;

    Sat satellite1(1001, MI208, I48, ACTIVE);
    Sat satellite2(1002, MI215, I53, DEORBITED);
    network.insert(satellite1);
    network.insert(satellite2);

    int idToSearch = 999; // An ID that is not in the tree

    if (network.findSatellite(idToSearch)) {
        return false;
    }

    return true;
}

bool Tester::testAssignmentOperatorErrorCase() {
    SatNet emptyNetwork1;
    SatNet emptyNetwork2;

    // assigning an empty object to another empty object
    emptyNetwork1 = emptyNetwork2;

    //  both should remain empty
    if (emptyNetwork1.countSatellites(I48) != 0 || emptyNetwork2.countSatellites(I48) != 0 ||
        emptyNetwork1.countSatellites(I53) != 0 || emptyNetwork2.countSatellites(I53) != 0 ||
        emptyNetwork1.countSatellites(I70) != 0 || emptyNetwork2.countSatellites(I70) != 0 ||
        emptyNetwork1.countSatellites(I97) != 0 || emptyNetwork2.countSatellites(I97) != 0) {
        return false;
    }

    return true;
}

bool Tester::testSetState() {

    SatNet satnet;

    Sat sat1(1001, MI208, I48, ACTIVE);
    Sat sat2(1002, MI215, I53, ACTIVE);
    Sat sat3(1003, MI340, I70, ACTIVE);

    satnet.insert(sat1);
    satnet.insert(sat2);
    satnet.insert(sat3);

    int idToChange = 1002;
    STATE newState = DEORBITED;

    bool stateUpdated = satnet.setState(idToChange, newState);
    cout << satnet.setState(idToChange, newState) << endl;
    // Check if the state is updated
    if (!stateUpdated) {
        return false;
    }


    // Check if setting the state for a non-existing ID returns false
    int nonID = 9999;
    bool nonExistingStateUpdated = satnet.setState(nonID, newState);
    if (nonExistingStateUpdated) {
        return false; // setState should return false for non-existing ID
    }

    return true;
}

bool Tester::testRemovalPerformance() {
    clock_t start;
    double T1, T2, expectedRatio;

    start = clock();
    T1 = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;

    start = clock();
    T2 = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;

    expectedRatio = log(2.0 * 2000) / log(1000); // Theoretical expected ratio

    return ( expectedRatio - 0.4 < T2 / (2 * T1) < expectedRatio + 0.4);
}


bool Tester :: testInsertPerformance() {
    const int treeSize1000 = 1000;
    const int treeSize2000 = 2000;

    SatNet aNet1000;
    SatNet aNet2000;

    int idArray1000[treeSize1000];
    int idArray2000[treeSize2000];

    // Generate random IDs for satellites
    for (int i = 0; i < treeSize2000; ++i) {
        idArray2000[i] = rand() % (MAXID - MINID + 1) + MINID;
        if (i < treeSize1000) {
            idArray1000[i] = rand() % (MAXID - MINID + 1) + MINID;
        }
    }

    clock_t start, stop;
    double T1, T2;

    // Start the timer for tree of 1000 nodes
    start = clock();

    for (int i = 0; i < treeSize1000; ++i) {
        aNet1000.insert(Sat(idArray1000[i]));
    }

    stop = clock();

    // Calculate time of 1000 nodes
    T1 = static_cast<double>(stop - start) / CLOCKS_PER_SEC;

    // Start the timer for tree of 2000 nodes
    start = clock();

    for (int i = 0; i < treeSize2000; ++i) {
        aNet2000.insert(Sat(idArray2000[i]));
    }

    stop = clock();

    // Calculate time of 2000 nodes
    T2 = static_cast<double>(stop - start) / CLOCKS_PER_SEC;

    // Expected theoretical ratio
    double expectedRatio = log(2.0 * treeSize2000) / log(treeSize1000);

    double acceptableRange = 0.4;

    return ((expectedRatio - acceptableRange) < T2 / (2 * T1) < (expectedRatio + acceptableRange));

}
