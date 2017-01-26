#include <sstream>
#include "utrie.h"


template <typename T>
string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

void toLowerCase(string &word){
    if(word.length()==1){
        if(word[0]!='I' && word[0] != 'A')
            word[0] = (char)tolower(word[0]);
    }
    if(word.length() > 1){
        int i = 0;
        while(word[i]){
            word[i] = (char)tolower(word[i]);
            i++;
        }
    }

}


//num_cnodes, num_unodes

//vector -- <char1,char2,...>
//vector -- <numRootChild, ind1RootChild, ind2RootChild, ...,numChild1,ind1Child1,ind2Child1,ind3Child1,....,numChild2,ind1Child2,ind2Child2,...> Np+N  if numChild == 0 then followed by bigramInd to parentVec of uTrie

//vector -- <unicodeChar1, unicodeChar2, unicodeChar3, ...>
//vector -- <parent1Ind, numChildParent1 parent2Ind, numChildParent2, parent3Ind, numChildParent3 ...>  --ids correspond to the unicode characters

//vector -- <p1p1p1p1,p2p2p2, ...


void writeToBinaryFile(const vector<u_char> &charArray, const vector<uint16_t> &bigrams, vector<uint8_t> &numChildArray, vector<uint16_t> &unicodeCharArray, vector<uint16_t> &unicodeParentArray,
                       vector<uint8_t> &unicodeNumChildArray, const string path){

    uint16_t sz1 = (uint16_t) charArray.size();
    uint16_t sz2 = (uint16_t) bigrams.size();
    uint16_t sz3 = (uint16_t) numChildArray.size();
    uint16_t sz4 = (uint16_t) unicodeCharArray.size();
    uint16_t sz5 = (uint16_t) unicodeParentArray.size();
    uint16_t sz6 = (uint16_t) unicodeNumChildArray.size();


    cout << "sz1: " << sz1 << ", sz2: " << sz2 << ", sz3: " << sz3 << ", sz4: " << sz4 << ", sz5: " << sz5 << ", sz6: " << sz6 << endl;


    cout << "writing to binary file" << endl;
    ofstream os (path, ios::out | ios::binary);

    os.write((char*)&sz1, 2);
    os.write((char*)&sz2, 2);
    os.write((char*)&sz3, 2);
    os.write((char*)&sz4, 2);
    //os.write((char*)&sz5, 2);
    os.write((char*)&sz6, 2);

    if(!charArray.empty()){
        for(size_t i = 0; i < charArray.size(); i++)
            os.write((char*)&charArray[i], 1);
    }

    if(!bigrams.empty())
        for(size_t i = 0; i < bigrams.size(); i++)
            os.write((char*)&bigrams[i], 2);


    if(!numChildArray.empty())
        for(size_t i = 0; i < numChildArray.size(); i++)
            os.write((char*)&numChildArray[i], 1);

    if(!unicodeCharArray.empty())
        for(size_t i = 0; i < unicodeCharArray.size(); i++)
            os.write((char*)&unicodeCharArray[i], 2);

    /*if(!unicodeParentArray.empty())
        for(size_t i = 0; i < unicodeParentArray.size(); i++)
            os.write((char*)&unicodeParentArray[i], 2);*/


    if(!unicodeNumChildArray.empty())
        for(size_t i = 0; i < unicodeNumChildArray.size(); i++)
            os.write((char*)&unicodeNumChildArray[i], 2);


    os.close();

    cout << "binary file written to " << path << endl;

}



void serialize(const Trie &trie, const UTrie &utrie, vector<u_char> &charArray, vector<uint16_t> &bigrams, vector<uint8_t> &numChildArray, vector<uint16_t> &unicodeCharArray, vector<uint16_t> &unicodeParentArray,
               vector<uint8_t> &unicodeNumChildArray)
{

    vector<shared_ptr<cNode>> allCnodes;
    //vector<uint16_t> numCNodesForEachLevel;

    vector<shared_ptr<uNode>> allUnodes;
    //vector<uint16_t> numUNodesForEachLevel;


    //preparing unicode trie for serialization

    shared_ptr<uNode> unode = utrie.root;

    //numUNodesForEachLevel.push_back((uint16_t)unode->children.size());

    vector<shared_ptr<uNode>> levelUNodes;

    levelUNodes = unode->children;

    //fill unicode root node values in unicode char array

    //set parent as 0 for all children of root
    unicodeNumChildArray.push_back((uint8_t )levelUNodes.size());

    cout << "number of children: " << endl;

    cout << levelUNodes.size() << ", ";

    while(!levelUNodes.empty()){

        vector<shared_ptr<uNode>> tempUnodes;

        for(size_t i = 0; i < levelUNodes.size(); i++){
            allUnodes.push_back(levelUNodes[i]);

            unicodeCharArray.push_back(levelUNodes[i]->value);

            //children of nodes at current level

            if(!levelUNodes[i]->children.empty()) {

                //unicodeParentArray.push_back((uint16_t)unicodeCharArray.size());
                unicodeNumChildArray.push_back((uint8_t )levelUNodes[i]->children.size());

                cout << levelUNodes[i]->children.size() << ", ";

                for (size_t j = 0; j < levelUNodes[i]->children.size(); j++) {
                    tempUnodes.push_back(levelUNodes[i]->children[j]);
                }
            }
            else {
                unicodeNumChildArray.push_back(0);
                cout << 0 << ", ";
            }
        }

        //cout << "size of tempUnodes: " << tempUnodes.size() << endl;

        levelUNodes.clear();
        levelUNodes = tempUnodes;
        tempUnodes.clear();

        //cout << "size of levelUnodes: " << levelUNodes.size() << endl;

    }

    cout << endl << "parent ids: " << endl;

    uint16_t parentInd = 0;


    for(size_t i = 0; i < unicodeNumChildArray.size(); i++){

        if(unicodeNumChildArray[parentInd] == 0){
            parentInd++;
            continue;
        }

        for(size_t j = 0; j < unicodeNumChildArray[parentInd]; j++){
            unicodeParentArray.push_back(parentInd);
            cout << parentInd << ", ";
        }

        parentInd++;

    }

    cout << endl;

    /*for(size_t i = 0; i < unicodeCharArray.size(); i++) {

        if(unicodeNumChildArray[parentInd] == 0){
            parentInd++;
            continue;
        }

        for(size_t j = 0; j < unicodeNumChildArray[parentInd]; j++){
            unicodeParentArray.push_back(parentInd);
            cout << parentInd << ", ";
        }

        i += unicodeNumChildArray[parentInd]-1;

        parentInd++;


    }*/

    cout << endl;

    cout << "size of unicode char array: " << unicodeCharArray.size() << endl;

    cout << "size of unicode num children array: " << unicodeNumChildArray.size() << endl;

    cout << "size of unicode parent array: " << unicodeParentArray.size() << endl;


    //preparing char trie for serialization
    shared_ptr<cNode> cnode = trie.root;

    vector<shared_ptr<cNode>> levelCNodes;

    levelCNodes = cnode->children;

    numChildArray.push_back((uint8_t) levelCNodes.size());


    cout << "bigrams: " << endl;

    while(!levelCNodes.empty()){

        vector<shared_ptr<cNode>> tempCnodes;

        //cout << "size of levelCnodes: " << levelCNodes.size() << endl;

        for(size_t i = 0; i < levelCNodes.size(); i++){
            allCnodes.push_back(levelCNodes[i]);

            charArray.push_back(levelCNodes[i]->value);

            if(!levelCNodes[i]->children.empty()){

                numChildArray.push_back((uint8_t)levelCNodes[i]->children.size());
                bigrams.push_back(0);

                cout << 0 << ", ";

                for(size_t j = 0; j < levelCNodes[i]->children.size(); j++)
                    tempCnodes.push_back(levelCNodes[i]->children[j]);
            }
            else{
                numChildArray.push_back(0);

                uint16_t bigram = (uint16_t)(find(allUnodes.begin(), allUnodes.end(), levelCNodes[i]->bigram)-allUnodes.begin());

                bigrams.push_back(bigram);

                cout << bigram << ", ";
            }

        }


        //cout << "size of tempCnodes: " << tempCnodes.size() << endl;

        levelCNodes.clear();
        levelCNodes = tempCnodes;
        //tempCnodes.clear();

    }

    cout << endl;
    /*cout << "size of bigrams: " << bigrams.size() << endl;

    for(size_t i = 0; i < bigrams.size(); i++)
        cout << "bigram - " << i << " : " << bigrams[i] << endl;*/


}


void testDryRun(const vector<u_char> &charArray, const vector<uint16_t> &bigrams, const vector<uint8_t> &numChildArray, const vector<uint16_t> &unicodeCharArray, const vector<uint16_t> &unicodeParentArray) {

    cout << endl << "*****************Dry-Run**************" << endl << endl;

    //shared_ptr<uNode> unode;

    vector<string> user_input;
    isDebug = false;

    int count = 0;

    while (1) {
        string curr_input = "";
        cout << "user: ";
        getline(cin, curr_input);

        if (curr_input.length() == 0) {

            //break if no input is given for 3 consecutive times
            if (count >= 3)
                break;
            count++;
            user_input.clear();
        }

        else {  //if input is non-empty
            count = 0;
            //toLowerCase(curr_input);

            cout << "user input: " << curr_input << endl;

            string output = "";

            uint16_t off1 = 0;
            uint16_t off2 = (uint16_t)numChildArray[0];
            uint16_t j = 0;


/*            cout << "char trie num children: " << endl;

            for(size_t i = 0; i < numChildArray.size(); i++){

                cout << (int)numChildArray[i] << ", ";
            }

            cout << endl;

            cout << "char array: " << endl;

            for(size_t i = 0; i < charArray.size(); i++){

                cout << charArray[i] << ", ";
            }

            cout << endl;*/

            for(size_t i = 0; i < curr_input.size(); i++){

                j = (uint16_t) (find(charArray.begin() + off1, charArray.begin() + off2, curr_input[i]) - charArray.begin());

                if(j > off2)
                    cout << "word does not exist" << endl;

                //cout << "j: " << j << " char: " << charArray[j] << endl;

                off2 = 0;

                for(size_t k = 0; k < j+1; k++)
                    off2 += numChildArray[k];

                off1 = off2;

                off2 += numChildArray[j+1];

                //cout << "off1: " << off1 << endl;
                //cout << "off2: " << off2 << endl;

            }

            //cout << "number of children of last char: " << (int)numChildArray[j] << endl;

            //cout << "off2: " << off2 << endl;

            //find bigram
            uint16_t unicodeInd = 0;

            /*for(size_t i = 1; i < j+1; i++){
                if(numChildArray[i]==0)
                    bigramInd++;
            }*/

            //cout << "bigramInd: " << bigramInd << endl;

            vector<uint16_t> utf16Vec;

            unicodeInd = bigrams[j];  //index of unicode char in unicodeCharArray

            utf16Vec.push_back(unicodeCharArray[unicodeInd]);

            cout << "bigram: " << unicodeInd << endl;

            utf8::utf16to8(utf16Vec.begin(), utf16Vec.end(), back_inserter(output));

            cout << "output: " << output << endl;

            output = "";

            //cout << "bigram: " << unicodeInd << endl;

            while(1){

                unicodeInd = unicodeParentArray[unicodeInd];

                cout << "unicodeInd: " << unicodeInd-1 << endl;

                if(unicodeInd == 0)
                    break;

                unicodeInd = unicodeInd-1;

                utf16Vec.push_back(unicodeCharArray[unicodeInd]);

                utf8::utf16to8(utf16Vec.begin(), utf16Vec.end(), back_inserter(output));

                cout << "output: " << output << endl;

                output = "";

            }


            uint16_t parentInd, numChildInd = 0, sum = 0;

            /*while(1){

                for(size_t i = 0; i < unicodeNumChildArray.size(); i++){

                    *//*if(sum >= bigramInd)
                        break;*//*

                    sum += unicodeNumChildArray[i];
                    numChildInd = (uint16_t) i;

                    if(sum < bigramInd){

                        cout << "sum: " << sum << endl;
                        cout << "numChildInd: " << numChildInd << endl;

                    }
                    else {
                        //numChildInd = (uint16_t) i;
                        break;
                    }
                }

                parentInd = numChildInd;

                bigramInd = parentInd;

                utf16Vec.push_back(unicodeCharArray[parentInd]);

                sum = 0;

                if(numChildInd == 0)
                    break;


                utf8::utf16to8(utf16Vec.begin(), utf16Vec.end(), back_inserter(output));

                cout << "output: " << output << endl;

                output = "";

                cout << "bigram: " << bigramInd << endl;
            }*/

            reverse(utf16Vec.begin(), utf16Vec.end());

            utf8::utf16to8(utf16Vec.begin(), utf16Vec.end(), back_inserter(output));

            //t.getBigram(curr_input, output);

            cout << "transliteration: " + output << endl;
        }
    }

}



void testDryRun(Trie &t) {

    cout << endl << "*****************Dry-Run**************" << endl << endl;

    //shared_ptr<uNode> unode;

    vector<string> user_input;
    isDebug = false;

    int count = 0;

    while (1) {
        string curr_input = "";
        cout << "user: ";
        getline(cin, curr_input);

        if (curr_input.length() == 0) {

            //break if no input is given for 3 consecutive times
            if (count >= 3)
                break;
            count++;
            user_input.clear();
        }

        else {  //if input is non-empty
            count = 0;
            //toLowerCase(curr_input);

            LOG("user input: " + curr_input);

            string output;

            t.getBigram(curr_input, output);

            cout << "transliteration: " + output << endl;
        }
    }

}

void createTries(const string fileName, Trie &trie, UTrie &utrie){

    ifstream fs8(fileName.c_str());
    if (!fs8.is_open()) {
        cout << "Could not open " << fileName << endl;
        return;
    }

    string linestr;

    unsigned line_count = 1;

   /* Trie trie;
    UTrie utrie;*/

    shared_ptr<cNode> cnode;
    shared_ptr<uNode> unode;

    bool isWordExist;

    while (getline(fs8, linestr)) {

        cout << "linestr: " << linestr << endl;

        cout << "length of linestr: " << linestr.size() << endl;

        //string linestr(line);

        /*if (line_count < 3) {
            line_count++;
            continue;
        }*/

        if(line_count == 1) {
            linestr = linestr.substr(3, linestr.size());
            line_count++;
            cout << "new line: " << linestr << endl;
        }

        /*cout << "size of linestr: " << linestr.size() << endl;
        cout << "linestr: " << linestr << endl;*/

        size_t delim_pos = linestr.find_first_of(" ");

        //cout << "delim pos: " << delim_pos << endl;

        if (delim_pos == string::npos)
            continue;

        string word1 = linestr.substr(0, delim_pos);

        string word2 = linestr.substr(delim_pos + 1, linestr.size());

        LOG("word1: " + word1);
        LOG("word2: " + word2);

        string temp = "";

        /*for(int i = 2; i < word2.size(); i++){
            temp += word2[i];
            cout << "i: " << i << " " << temp << endl;
        }*/

        trie.getSetWordNode(word1, isWordExist, cnode);
        utrie.getSetWordNode(word2, isWordExist, unode);

        //LOG("number of cnodes: " + to_string(numCNodes));
        //LOG("number of unodes: " + to_string(numUNodes));

        cnode->bigram = unode;
    }



    utrie.getSizeOf();

}


int main() {
    cout << "Creating tries from file..." << std::endl;

    string fileName = "en_hi_without_duplicates.txt";// "test.txt";//"en-hi-test-data.txt";  //

    Trie trie;
    UTrie utrie;

    createTries(fileName, trie, utrie);

    //cout << "size of trie: " << sizeof(trie) << endl;

    //cout << "size of unicode trie: " << sizeof(utrie) << endl;

    if(trie.isEmpty() || utrie.isEmpty()){
        cout << "empty tries";

        return -1;
    }

    vector<u_char> charArray;
    vector<uint16_t> bigrams;
    vector<uint8_t> numChildArray;
    vector<uint16_t> unicodeCharArray;
    vector<uint16_t> unicodeParentArray;
    vector<uint8_t> unicodeNumChildArray;

    serialize(trie, utrie, charArray, bigrams, numChildArray, unicodeCharArray, unicodeParentArray, unicodeNumChildArray);

    string path = "transliteration_en_hi.bin";

    writeToBinaryFile(charArray, bigrams, numChildArray, unicodeCharArray, unicodeParentArray, unicodeNumChildArray, path);

    //testDryRun(trie);

    testDryRun(charArray, bigrams, numChildArray, unicodeCharArray, unicodeParentArray);  //, unicodeNumChildArray

    return 0;
}


/*
cout << "word2 char by char: " << endl;

// Convert it to utf-16
vector<uint16_t> utf16Vec;

string::iterator end_it = utf8::find_invalid(word2.begin(), word2.end());
if (end_it != word2.end()) {
//cout << "Invalid UTF-8 encoding detected at line " << line_count << "\n";
cout << "This part is fine: " << string(word2.begin(), end_it) << "\n";
}

cout << "word2: " << word2 << endl;

utf8::utf8to16(word2.begin(), end_it, back_inserter(utf16Vec));

// And back to utf-8
string utf8line;
utf8::utf16to8(utf16Vec.begin(), utf16Vec.end(), back_inserter(utf8line));

cout << "utf8 word: " << utf8line << endl;*/
