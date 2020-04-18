/*------------------------------------------------------------------------------------------------
|         _       _     ___                                            _ _   _      _            |
|     _ _| |_ _ _(_)___/ __| __ __ _ _ _  _ _  ___ _ _       _ __ _  _| | |_(_)_ __| |_____ __   |
|   / _` |  _| '_| / -_)__ \/ _/ _` | ' \| ' \/ -_) '_|     | '  \ || | |  _| | '_ \ / -_) \ /   |
|   \__, |\__|_| |_\___|___/\__\__,_|_||_|_||_\___|_|       |_|_|_\_,_|_|\__|_| .__/_\___/_\_\   |
|   |___/                                                                     |_|                |
|                                                                                                |
|------------------------------------------------------------------------------------------------|
|   Pedro Ribeiro ( pribeiro@dcc.fc.up.pt )    &    Andre Couto Meira ( up2014048772fc.up.pt )   |
-------------------------------------------------------------------------------------------------*/
/*
 * Graph Tree Implementation
 *
 * Last Update: 04/2020
 *
 */


#include "GraphTree.h"


GraphTreeNode::GraphTreeNode() {
    frequency = 0;
    zero = one = NULL; //left and right
}


GraphTreeNode::~GraphTreeNode() {
    if (zero != NULL) delete zero;
    if (one != NULL) delete one;
}


void GraphTreeNode::zeroFrequency() {
    frequency = 0;
    if (zero != NULL) zero->zeroFrequency();
    if (one != NULL) one->zeroFrequency();
}


void GraphTreeNode::incrementString(int pos, char *s, int f) {
    if (s[pos] == 0) {
        frequency += f;
    }
    else {
        if (s[pos] == '1') {
            if (one == NULL) one = new GraphTreeNode();
            one->incrementString(pos + 1, s, f);
        } else if (s[pos] == '0') {
            if (zero == NULL) zero = new GraphTreeNode();
            zero->incrementString(pos + 1, s, f);
        }
    }
}


void GraphTreeNode::incrementString(int pos, char *s, GraphTreeNode **final, int f) {
    if (s[pos] == 0) {
        frequency += f;
    } else {
        if (s[pos] == '1') {
            if (one == NULL) one = new GraphTreeNode();
            *final = one;
            one->incrementString(pos + 1, s, final, f);
        } else if (s[pos] == '0') {
            if (zero == NULL) zero = new GraphTreeNode();
            *final = zero;
            zero->incrementString(pos + 1, s, final, f);
        }
    }
}


void GraphTreeNode::populateMap(mapStringInt *m, int size, int pos, char *s) {
    if (zero == NULL && one == NULL) {
        s[pos] = 0;
        (*m)[s] = frequency;
    } else {
        if (zero != NULL) {
            s[pos] = '0';
            zero->populateMap(m, size, pos + 1, s);
        }
        if (one != NULL) {
            s[pos] = '1';
            one->populateMap(m, size, pos + 1, s);
        }
    }
}


double GraphTreeNode::countOccurrences() {
    double aux = frequency;
    if (zero != NULL) aux += zero->countOccurrences();
    if (one != NULL) aux += one->countOccurrences();
    return aux;
}


int GraphTreeNode::countGraphs() {
    int aux = 0;
    if (frequency > 0) aux++;
    if (zero != NULL) aux += zero->countGraphs();
    if (one != NULL) aux += one->countGraphs();
    return aux;
}


GraphTree::GraphTree() {
    root = new GraphTreeNode();
}


GraphTree::~GraphTree() {
    if (root != NULL)
        delete root;
}


void GraphTree::zeroFrequency() {
    root->zeroFrequency();
    occurrences = 0;
    leafs = 0;
}


void GraphTree::incrementString(char *s) {
    root->incrementString(0, s, 1);
}


void GraphTree::incrementString(char *s, int f) {
    root->incrementString(0, s, f);
}


void GraphTree::addSubstring(GraphTreeNode *initial, char *s, GraphTreeNode **final, int f) {
    initial->incrementString(0, s, final, f);
}


double GraphTree::countOccurrences() {
    return root->countOccurrences();
}


int GraphTree::countGraphs() {
    return root->countGraphs();
}


void GraphTree::populateMap(mapStringInt *m, int maxsize) {
    char s[maxsize * maxsize + 1];
    root->populateMap(m, maxsize, 0, s);
}
