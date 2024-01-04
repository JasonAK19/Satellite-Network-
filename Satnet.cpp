//
// Created by Jason on 10/9/2023.
//

#include "satnet.h"
#include <stack>
SatNet::SatNet(){
    m_root = nullptr;
}

SatNet::~SatNet(){
clear();
}

int SatNet:: calculateBalance(Sat* node) {
    if (node == nullptr) {
        return 0;
    }

    int leftHeight = (node->getLeft() != nullptr) ? node->getLeft()->getHeight() : 0;
    int rightHeight = (node->getRight() != nullptr) ? node->getRight()->getHeight() : 0;

    return leftHeight - rightHeight;
}

Sat* SatNet::rotateRight(Sat* y) {
    Sat* x = y->getLeft();
    Sat* T2 = x->getRight();

    // rotation
    x->setRight(y);
    y->setLeft(T2);

    // Update heights

    int leftHeight = (y->getLeft() != nullptr) ? y->getLeft()->getHeight() : 0;
    int rightHeight = (y->getRight() != nullptr) ? y->getRight()->getHeight() : 0;
    y->setHeight(1 + max(leftHeight, rightHeight));

    leftHeight = (x->getLeft() != nullptr) ? x->getLeft()->getHeight() : 0;
    rightHeight = (x->getRight() != nullptr) ? x->getRight()->getHeight() : 0;
    x->setHeight(1 + max(leftHeight, rightHeight));

    return x;
}

Sat* SatNet::rotateLeft(Sat* x) {
    Sat* y = x->getRight();
    Sat* T2 = y->getLeft();

    // rotation
    y->setLeft(x);
    x->setRight(T2);

    // Update heights

    int leftHeight = (x->getLeft() != nullptr) ? x->getLeft()->getHeight() : 0;
    int rightHeight = (x->getRight() != nullptr) ? x->getRight()->getHeight() : 0;
    x->setHeight(1 + max(leftHeight, rightHeight));


    leftHeight = (y->getLeft() != nullptr) ? y->getLeft()->getHeight() : 0;
    rightHeight = (y->getRight() != nullptr) ? y->getRight()->getHeight() : 0;
    y->setHeight(1 + max(leftHeight, rightHeight));

    return y;
}

Sat* SatNet::insertHelper(Sat* node, const Sat& satellite) {
    if (node == nullptr) {
        Sat* newNode = new Sat(satellite);
        newNode->setHeight(1);
        return newNode;
    }

    if (satellite.getID() < node->getID()) {
        if (node->getLeft() == nullptr) {
            node->setLeft(new Sat(satellite));
        } else {
            node->setLeft(insertHelper(node->getLeft(), satellite));
        }
    } else if (satellite.getID() > node->getID()) {
        if (node->getRight() == nullptr) {
            node->setRight(new Sat(satellite));
        } else {
            node->setRight(insertHelper(node->getRight(), satellite));
        }
    } else {

        return node;
    }

    // Update height and balance factor
    int leftHeight = (node->getLeft() != nullptr) ? node->getLeft()->getHeight() : 0;
    int rightHeight = (node->getRight() != nullptr) ? node->getRight()->getHeight() : 0;
    node->setHeight(1 + max(leftHeight, rightHeight));

    int balance = leftHeight - rightHeight;

    // Perform rotations
    if (balance > 1) {
        if (satellite.getID() < node->getLeft()->getID()) {
            return rotateRight(node);
        } else {
            node->setLeft(rotateLeft(node->getLeft()));
            return rotateRight(node);
        }
    }
    if (balance < -1) {
        if (satellite.getID() > node->getRight()->getID()) {
            return rotateLeft(node);
        } else {
            node->setRight(rotateRight(node->getRight()));
            return rotateLeft(node);
        }
    }

    return node;
}

void SatNet::insert(const Sat& satellite){
    m_root = insertHelper(m_root, satellite);
}


void SatNet::clearHelper(Sat *node) {
    if (node != nullptr) {
        clearHelper(node->m_left);
        clearHelper(node->m_right);
        delete node;
    }
}
void SatNet::clear(){
    clearHelper(m_root);
    m_root = nullptr;
}

Sat* SatNet::findMin(Sat* node) {
    Sat* current = node;
    while (current->getLeft() != nullptr) {
        current = current->getLeft();
    }
    return current;
}

Sat* SatNet::removeHelper(Sat* node, int id) {
    if (node == nullptr) {
        return node;
    }

    if (id < node->getID()) {
        node->setLeft(removeHelper(node->getLeft(), id));
    }
    else if (id > node->getID()) {
        node->setRight(removeHelper(node->getRight(), id));
    }
    else {
        //  only one child or no child
        if (node->getLeft() == nullptr || node->getRight() == nullptr) {
            Sat* temp = (node->getLeft() != nullptr) ? node->getLeft() : node->getRight();

            // No child
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else { // One child
                *node = *temp;
            }

            delete temp;
        } else {
            Sat* temp = findMin(node->getRight());

            *node = *temp;

            node->setRight(removeHelper(node->getRight(), temp->getID()));
        }
    }

    if (node == nullptr) {
        return node;
    }

    // Update height of current node
    int leftHeight = (node->getLeft() != nullptr) ? node->getLeft()->getHeight() : 0;
    int rightHeight = (node->getRight() != nullptr) ? node->getRight()->getHeight() : 0;
    node->setHeight(1 + max(leftHeight, rightHeight));

    // Calculate balance factor
    int balance = calculateBalance(node);

    // rotations to rebalance the tree
    if (balance > 1) {
        if (calculateBalance(node->getLeft()) >= 0) {
            return rotateRight(node);
        } else {
            node->setLeft(rotateLeft(node->getLeft()));
            return rotateRight(node);
        }
    }
    if (balance < -1) {
        if (calculateBalance(node->getRight()) <= 0) {
            return rotateLeft(node);
        } else {
            node->setRight(rotateRight(node->getRight()));
            return rotateLeft(node);
        }
    }

    return node;
}

void SatNet::remove(int id){
    m_root = removeHelper(m_root, id);
}
void SatNet::dumpTree() const {
    dump(m_root);
}

void SatNet::dump(Sat* satellite) const{
    if (satellite != nullptr){
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}

void SatNet::listSatellitesHelper(Sat* node) const {
    if (node != nullptr) {
        listSatellitesHelper(node->getLeft());

        cout << node->getID() << ": " << node->getStateStr() << ": " << node->getInclinStr() << ": " << node->getAltStr() << endl;

        listSatellitesHelper(node->getRight());
    }
}
void SatNet::listSatellites() const {
    listSatellitesHelper(m_root);
}

bool SatNet::setState(int id, STATE state){
    return setStateHelper(m_root, id, state);
}

bool SatNet::setStateHelper(Sat* node, int id, STATE state) {
    if (node == nullptr) {
        return false;
    }

    if (id < node->getID()) {
        // search left subtree
        return setStateHelper(node->getLeft(), id, state);
    } else if (id > node->getID()) {
        // search right subtree
        return setStateHelper(node->getRight(), id, state);
    } else {
        node->setState(state);
        return true;
    }
}
void SatNet::removeDeorbited() {
    removeDeorbitedHelper(m_root);
}

void SatNet::removeDeorbitedHelper(Sat*& node) {
    if (node == nullptr) {
        return;
    }

    removeDeorbitedHelper(node->m_left);
    removeDeorbitedHelper(node->m_right);

    if (node->getState() == DEORBITED) {
        node = removeHelper(node, node->getID());
    }
}

bool SatNet::findSatelliteHelper(Sat* node, int id) const {
    if (node == nullptr) {
        return false;
    }
    if (id == node->getID()) {
        return true;
    }
    if (id < node->getID()) {
        return findSatelliteHelper(node->getLeft(), id);
    } else {
        return findSatelliteHelper(node->getRight(), id);
    }
}
bool SatNet::findSatellite(int id) const {
    return findSatelliteHelper(m_root, id);
}

Sat* SatNet::deepCopy(const Sat* node) {
    if (node == nullptr) {
        return nullptr;
    }

    Sat* newSat = new Sat(*node); // Copy the current node

    newSat->setLeft(deepCopy(node->getLeft()));
    newSat->setRight(deepCopy(node->getRight()));

    return newSat;
}

const SatNet & SatNet::operator=(const SatNet & rhs){
    if (this == &rhs) {
        return *this;
    }

    // clear to avoid memory leaks
    clear();

    // Perform a deep copy of the rhs tree
    m_root = deepCopy(rhs.m_root);

    return *this;
}

int SatNet::countSatellitesHelper(Sat* node, INCLIN degree) const {
    if (node == nullptr) {
        return 0;
    }

    int count = 0;

    if (node->getInclin() == degree) {
        //  increment count
        count++;
    }

    // count satellites in subtrees
    count += countSatellitesHelper(node->getLeft(), degree);
    count += countSatellitesHelper(node->getRight(), degree);

    return count;
}
int SatNet::countSatellites(INCLIN degree) const{
    return countSatellitesHelper(m_root, degree);

}
