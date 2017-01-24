//
// Created by abu on 23/1/17.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "utf8.h"

#ifndef TESTUNICODE_UTRIE_H
#define TESTUNICODE_UTRIE_H

#endif //TESTUNICODE_UTRIE_H


using namespace std;

bool isDebug = true;

void LOG(char msg){
    if(isDebug)
        cout << msg << endl;
}

void LOG(string msg){
    if(isDebug)
        cout << msg << endl;
}


class uNode{
public:
    uint16_t value;  // character value of node   --> N x 2 bytes
    vector<shared_ptr<uNode>> children; //character nodes connected to this character node  --> N x 16 bytes
    shared_ptr<uNode> parent; //parent node   --> 1 shared_ptr  total 18609x16 bytes

    uNode(){
        value = 0;
        parent = nullptr;
    };
};


//size of UTrie -- 850KB

class UTrie {
public:

    shared_ptr<uNode> root;

    UTrie() {

        LOG("unicode trie initiated");
        root = make_shared<uNode>();

    };

    void getSetWordNode(const string word, bool &isWordExist, shared_ptr<uNode> &wordNode);
    bool isEmpty();
    void getSizeOf();
    //void getWord(std::shared_ptr<uNode> unode, string &word);
};


class cNode{
public:
    u_char value;  // character value of node  --> N bytes
    vector<shared_ptr<cNode>> children; //character nodes connected to this character node  --> N x 16 bytes
    shared_ptr<uNode> bigram;  //<word1>, <word2>, .. --> 1 shared_ptr each -- N x 16 bytes
    //shared_ptr<cNode> parent; //parent node

    cNode(){
        value = 0;
        //parent = nullptr;
        bigram = nullptr;
    };
};

//size of Trie -- N x 9 (N = total number of nodes) -- 427KB
class Trie {
public:

    shared_ptr<cNode> root;

    Trie() {

        LOG("trie initiated");
        root = make_shared<cNode>();

    };

    void getSetWordNode(const string word, bool &isWordExist, shared_ptr<cNode> &wordNode);

    bool getBigram(const string head, string &result);

    bool isEmpty();

    //void getSizeOf();

};


//reverse traverse tree to find word represented by a leaf node
void getWord(shared_ptr<uNode> unode, string &word){

    LOG("getting word from node");

    word = "";

    vector<uint16_t> utf16Vec;

    while(unode->parent!= nullptr){
        utf16Vec.push_back(unode->value);
        unode = unode->parent;
    }

    reverse(utf16Vec.begin(), utf16Vec.end());

    utf8::utf16to8(utf16Vec.begin(), utf16Vec.end(), back_inserter(word));

}

bool UTrie::isEmpty() {

    return this->root->children.empty();

}


void UTrie::getSizeOf(){

    vector<size_t > nChildren;

    shared_ptr<uNode> unode = this->root;

    nChildren.push_back(unode->children.size());

    vector<shared_ptr<uNode>> unodes;

    //cout << "capacity of vector: " << unodes.capacity() << endl;

    unodes = unode->children;

    /*cout << "size of shared ptr: " << sizeof(shared_ptr<uNode>) << endl;

    cout << "capacity of vector: " << unodes.capacity() << endl;

    cout << "size of vector of shared ptrs of uNodes: " << sizeof(unodes) + sizeof(shared_ptr<uNode>) * unodes.capacity() << endl;

    cout << "number of children of root: " << unodes.size() << endl;*/

    uint16_t totChildren = (uint16_t)unode->children.size();

    uint16_t totLeaves = 0;

    size_t szChildVec = sizeof(unodes) + sizeof(shared_ptr<uNode>) * unodes.capacity();

    cout << "size of shared ptr: " << sizeof(shared_ptr<uNode>) << endl;

    cout << "size of value: " << sizeof(this->root->value) << endl;

    uNode tempNode;

    cout << "size of normal pointer: " << sizeof(&tempNode) << endl;

    //cout << "size of root node: " << sizeof(this->root) << endl;

    while(!unodes.empty()){

        vector<shared_ptr<uNode>> tempVec;

        for(size_t i = 0; i < unodes.size(); i++){
            if(!unodes[i]->children.empty())
                for(size_t j = 0; j < unodes[i]->children.size(); j++)
                    tempVec.push_back(unodes[i]->children[j]);

            else
                totLeaves += 1;

        }

        nChildren.push_back(tempVec.size());

        totChildren += (uint16_t)(tempVec.size());

        unodes.clear();
        unodes = tempVec;
    }


    cout << "number of children of unodes: " << totChildren << endl;

    cout << "total number of leaf nodes: " << totLeaves << endl;

    cout << "total number of parent nodes: " << totChildren - totLeaves << endl;

}



/*void Trie::getSizeOf(){

    vector<size_t > nChildren;

    shared_ptr<cNode> cnode = this->root;

    nChildren.push_back(cnode->children.size());

    vector<shared_ptr<cNode>> cnodes;

    cnodes = cnode->children;

    uint16_t totChildren = (uint16_t)cnode->children.size();

    uint16_t totLeaves = 0;

    while(!cnodes.empty()){

        vector<shared_ptr<cNode>> tempVec;

        size_t nChild;

        for(size_t i = 0; i < cnodes.size(); i++){
            if(!cnodes[i]->children.empty())
                for(size_t j = 0; j < cnodes[i]->children.size(); j++)
                    tempVec.push_back(cnodes[i]->children[j]);

            else
                totLeaves += 1;

        }

        nChildren.push_back(tempVec.size());

        totChildren += (uint16_t)(tempVec.size());

        cnodes.clear();
        cnodes = tempVec;
    }


    cout << "number of children of unodes: " << totChildren << endl;

    cout << "total number of leaf nodes: " << totLeaves << endl;

    cout << "total number of parent nodes: " << totChildren - totLeaves << endl;

}*/



int numUNodes = 0;

void UTrie::getSetWordNode(const string word, bool &isWordExist, shared_ptr<uNode> &currNode){
    isWordExist = true;
    currNode = this->root;

    string tempWord = word;

    vector<uint16_t> utf16Vec;
    string::iterator end_it = utf8::find_invalid(tempWord.begin(), tempWord.end());
    utf8::utf8to16(tempWord.begin(), end_it, back_inserter(utf16Vec));

    for(int i = 0; i < utf16Vec.size(); i++){

        //u_char c = (u_char)word[i];

        if(currNode->children.empty()) {//no child node exist --> add child node

            shared_ptr<uNode> unode = make_shared<uNode>();
            unode->value = utf16Vec[i];

            unode->parent = currNode;
            currNode->children.push_back(unode);
            currNode = unode;
            isWordExist = false;

            numUNodes++;

            continue;
        }
        else{

            uint8_t count = 0;

            if(currNode->children.size() > 0) {
                for (int j = 0; j < currNode->children.size(); j++) {
                    if (currNode->children[j]->value == utf16Vec[i]) {
                        break;
                    }
                    count++;
                }

                if(count == currNode->children.size()) {//value does not exist in children --> add child node for value

                    shared_ptr<uNode> unode = make_shared<uNode>();
                    unode->value = utf16Vec[i];
                    currNode->children.push_back(unode);
                    unode->parent = currNode;
                    currNode = unode;
                    isWordExist = false;
                    numUNodes++;
                    continue;
                }
                else {
                    currNode = currNode->children[count];
                    continue;
                }
            }
            else{  //curr node has no child --> add child node with new value

                shared_ptr<uNode> unode = make_shared<uNode>();
                unode->value = utf16Vec[i];
                currNode->children.push_back(unode);
                unode->parent = currNode;
                currNode = unode;
                isWordExist = false;
                numUNodes++;
                continue;
            }
        }
    }

}


bool Trie::isEmpty() {

    return this->root->children.empty();

}


int numCNodes = 0;

void Trie::getSetWordNode(const string word, bool &isWordExist, shared_ptr<cNode> &currNode){
    isWordExist = true;
    currNode = this->root;

    for(int i = 0; i < word.length(); i++){

        u_char c = (u_char)word[i];

        if(currNode->children.empty()) {//no child node exist --> add child node

            shared_ptr<cNode> cnode = make_shared<cNode>();
            cnode->value = c;

            //cnode->parent = currNode;
            currNode->children.push_back(cnode);
            currNode = cnode;
            isWordExist = false;

            numCNodes++;

            continue;
        }
        else{

            uint8_t count = 0;

            if(currNode->children.size() > 0) {
                for (int j = 0; j < currNode->children.size(); j++) {
                    if (currNode->children[j]->value == c) {
                        break;
                    }
                    count++;
                }

                if(count == currNode->children.size()) {//value does not exist in children --> add child node for value

                    shared_ptr<cNode> cnode = make_shared<cNode>();
                    cnode->value = c;
                    currNode->children.push_back(cnode);
                    //cnode->parent = currNode;
                    currNode = cnode;
                    isWordExist = false;
                    numCNodes++;
                    continue;
                }
                else {
                    currNode = currNode->children[count];
                    continue;
                }
            }
            else{
                shared_ptr<cNode> cnode = make_shared<cNode>();
                cnode->value = (u_char)c;
                currNode->children.push_back(cnode);
                //cnode->parent = currNode;
                currNode = cnode;
                isWordExist = false;
                numCNodes++;
                continue;
            }
        }
    }

}

bool Trie::getBigram(const string head, string &result) {
    bool isHeadExist;

    shared_ptr<cNode> headNode = this->root;
    getSetWordNode(head, isHeadExist, headNode);

    LOG("getBigram() called for word: " + head);

    if(isHeadExist){
        if(headNode->bigram != nullptr)
            getWord(headNode->bigram, result);
        else
            LOG("no bigrams found for the head: " + head);
    }
    else{
        LOG( head + " does not exist in trie .. head is added to trie");
    }

    return !result.empty();

}