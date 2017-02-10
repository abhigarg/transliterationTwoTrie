#include <sstream>
#include <cstring>
#include "utrie.h"


template <typename T>
string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

int toLowerCase(string &word){
    string chk_word = word;
    int count = 0;
    if(word.length()==1){
        if(word[0]!='I' && word[0] != 'A')
            word[0] = (char)tolower(word[0]);
    }

    if(word.length() > 1){
        int i = 0;
        while(word[i]){
            word[i] = (char)tolower(word[i]);
            if(chk_word[i] != word[i])
                count++;
            i++;

        }
    }

    return count;
}


vector<string> testWords, testOutputs, checkedWords;


void readBinaryFile(const string &path, vector<u_char> &charArray, vector<uint16_t> &bigrams, vector<uint8_t> &numChildArray, vector<uint16_t> &unicodeCharArray, vector<uint16_t> &unicodeParentArray){

    printf("reading from file: %s\n", path.c_str());// << endl;
    ifstream is (path, ios::in | ios::binary);

    //check if binary file not found
    if(is.good())
        printf("trie binary file found\n");
    else{
        printf("trie binary file not found\n");
        return;
    }

    uint16_t sz1;

    vector<uint16_t> sz;

    for(size_t i = 0; i < 5; i++){

        if(!is.eof()){
            is.read((char*)&sz1, 2);
        }
        else{
            printf("unexpected eof() reached\n");
            return;
        }

        printf("sz%d: %d\n", (int) i, (int)sz1);

        sz.push_back(sz1);

    }

    //read charArray
    for(size_t i = 0; i < sz[0]; i++){

        u_char c;
        is.read((char*)&c, 1);

        charArray.push_back(c);

        if(is.eof()){
            printf("Unexpected eof reached\n");
            return;
        }
    }


    //read bigrams
    for(size_t i = 0; i < sz[1]; i++){

        uint16_t b;
        is.read((char*)&b, 2);

        bigrams.push_back(b);

        if(is.eof()){
            printf("Unexpected eof reached\n");
            return;
        }
    }


    //read numChildArray
    for(size_t i = 0; i < sz[2]; i++){

        uint8_t nC;
        is.read((char*)&nC, 1);

        numChildArray.push_back(nC);

        if(is.eof()){
            printf("Unexpected eof reached\n");
            return;
        }
    }


    //read unicodeCharArray
    for(size_t i = 0; i < sz[3]; i++){

        uint16_t u;
        is.read((char*)&u, 2);

        unicodeCharArray.push_back(u);

        if(is.eof()){
            printf("Unexpected eof reached\n");
            return;
        }
    }

    //read unicodeParentArray
    for(size_t i = 0; i < sz[4]; i++){

        uint16_t p;
        is.read((char*)&p, 2);

        unicodeParentArray.push_back(p);

        if(is.eof()){
            printf("Unexpected eof reached\n");
            return;
        }
    }


    is.close();

    cout << "size of charArray: " << charArray.size() << endl;

    cout << "size of bigrams: " << bigrams.size() << endl;

    cout << "size of numChildArray : " << numChildArray.size() << endl;

    cout << "size of unicodeCharArray: " << unicodeCharArray.size() << endl;

    cout << "size of unicodeParentArray: " << unicodeParentArray.size() << endl;

}





//num_cnodes, num_unodes

//vector -- <char1,char2,...>
//vector -- <numRootChild, ind1RootChild, ind2RootChild, ...,numChild1,ind1Child1,ind2Child1,ind3Child1,....,numChild2,ind1Child2,ind2Child2,...> Np+N  if numChild == 0 then followed by bigramInd to parentVec of uTrie

//vector -- <unicodeChar1, unicodeChar2, unicodeChar3, ...>
//vector -- <parent1Ind, numChildParent1 parent2Ind, numChildParent2, parent3Ind, numChildParent3 ...>  --ids correspond to the unicode characters

//vector -- <p1p1p1p1,p2p2p2, ...


void writeToBinaryFile(const vector<u_char> &charArray, const vector<uint16_t> &bigrams, const vector<uint8_t> &numChildArray, const vector<uint16_t> &unicodeCharArray, const vector<uint16_t> &unicodeParentArray,
                       const string path){

    uint16_t sz1 = (uint16_t) charArray.size();
    uint16_t sz2 = (uint16_t) bigrams.size();
    uint16_t sz3 = (uint16_t) numChildArray.size();
    uint16_t sz4 = (uint16_t) unicodeCharArray.size();
    uint16_t sz5 = (uint16_t) unicodeParentArray.size();


    cout << "sz1: " << sz1 << ", sz2: " << sz2 << ", sz3: " << sz3 << ", sz4: " << sz4 << ", sz5: " << sz5 << endl;


    cout << "writing to binary file" << endl;
    ofstream os (path, ios::out | ios::binary);

    os.write((char*)&sz1, 2);
    os.write((char*)&sz2, 2);
    os.write((char*)&sz3, 2);
    os.write((char*)&sz4, 2);
    os.write((char*)&sz5, 2);

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

    if(!unicodeParentArray.empty())
        for(size_t i = 0; i < unicodeParentArray.size(); i++)
            os.write((char*)&unicodeParentArray[i], 2);


    os.close();

    cout << "binary file written to " << path << endl;

}



void serialize(const Trie &trie, const UTrie &utrie, vector<u_char> &charArray, vector<uint16_t> &bigrams, vector<uint8_t> &numChildArray, vector<uint16_t> &unicodeCharArray, vector<uint16_t> &unicodeParentArray,
               vector<uint8_t> &unicodeNumChildArray)
{

    vector<shared_ptr<cNode>> allCnodes;

    vector<shared_ptr<uNode>> allUnodes;


    //preparing unicode trie for serialization

    shared_ptr<uNode> unode = utrie.root;


    vector<shared_ptr<uNode>> levelUNodes;

    levelUNodes = unode->children;

    //fill unicode root node values in unicode char array

    //set parent as 0 for all children of root
    unicodeNumChildArray.push_back((uint8_t )levelUNodes.size());

    while(!levelUNodes.empty()){

        vector<shared_ptr<uNode>> tempUnodes;

        for(size_t i = 0; i < levelUNodes.size(); i++){
            allUnodes.push_back(levelUNodes[i]);

            unicodeCharArray.push_back(levelUNodes[i]->value);

            //children of nodes at current level

            if(!levelUNodes[i]->children.empty()) {

                unicodeNumChildArray.push_back((uint8_t )levelUNodes[i]->children.size());


                for (size_t j = 0; j < levelUNodes[i]->children.size(); j++) {
                    tempUnodes.push_back(levelUNodes[i]->children[j]);
                }
            }
            else {
                unicodeNumChildArray.push_back(0);

            }
        }


        levelUNodes.clear();
        levelUNodes = tempUnodes;
        tempUnodes.clear();


    }


    uint16_t parentInd = 0;


    for(size_t i = 0; i < unicodeNumChildArray.size(); i++){

        if(unicodeNumChildArray[parentInd] == 0){
            parentInd++;
            continue;
        }

        for(size_t j = 0; j < unicodeNumChildArray[parentInd]; j++){
            unicodeParentArray.push_back(parentInd);
        }

        parentInd++;

    }

    cout << endl;

    cout << endl;

    cout << "size of unicode char array: " << unicodeCharArray.size() << endl;

    cout << "size of unicode num children array: " << unicodeNumChildArray.size() << endl;

    cout << "size of unicode parent array: " << unicodeParentArray.size() << endl;


    //preparing char trie for serialization
    shared_ptr<cNode> cnode = trie.root;

    vector<shared_ptr<cNode>> levelCNodes;

    levelCNodes = cnode->children;

    numChildArray.push_back((uint8_t) levelCNodes.size());


    while(!levelCNodes.empty()){

        vector<shared_ptr<cNode>> tempCnodes;

        for(size_t i = 0; i < levelCNodes.size(); i++){
            allCnodes.push_back(levelCNodes[i]);

            charArray.push_back(levelCNodes[i]->value);

            if(!levelCNodes[i]->children.empty()){

                numChildArray.push_back((uint8_t)levelCNodes[i]->children.size());

                if(levelCNodes[i]->bigram == nullptr)
                    bigrams.push_back(65535);
                else{
                    uint16_t bigram = (uint16_t)(find(allUnodes.begin(), allUnodes.end(), levelCNodes[i]->bigram)-allUnodes.begin());

                    bigrams.push_back(bigram);
                }


                for(size_t j = 0; j < levelCNodes[i]->children.size(); j++)
                    tempCnodes.push_back(levelCNodes[i]->children[j]);
            }
            else{
                numChildArray.push_back(0);

                uint16_t bigram = (uint16_t)(find(allUnodes.begin(), allUnodes.end(), levelCNodes[i]->bigram)-allUnodes.begin());

                bigrams.push_back(bigram);

            }

        }

        levelCNodes.clear();
        levelCNodes = tempCnodes;

    }

    cout << endl;

}


bool checkWordExist(const string &word, const vector<uint8_t> &numChildArray, const vector<u_char> &charArray, const vector<uint16_t> &bigrams, uint16_t &unicodeInd){
    uint16_t off1 = 0;
    uint16_t off2 = (uint16_t)numChildArray[0];
    uint16_t j = 0;
    size_t j2 = 1;


    bool wordExist = true;

    for(size_t i = 0; i < word.size(); i++){

        j = (uint16_t) (find(charArray.begin() + off1, charArray.begin() + off2, word[i]) - charArray.begin());

        if(j >= off2) {
            cout << "word does not exist" << endl;
            wordExist = false;
            return wordExist;
        }

        for(size_t k = j2; k < j+1; k++)
            off2 += numChildArray[k];

        j2 = (size_t)j + 2;

        off1 = off2;

        off2 += numChildArray[j+1];

    }

    unicodeInd = bigrams[j];  //index of unicode char in unicodeCharArray

    if(unicodeInd == 65535)
        wordExist = false;

    return wordExist;

}


void getTransliteration(const string &word, const vector<uint8_t> &numChildArray, const vector<u_char> &charArray, const vector<uint16_t> &bigrams, const vector<uint16_t> &unicodeCharArray,
                        const vector<uint16_t> &unicodeParentArray, string &output){

    output = "";

    uint16_t unicodeInd = 0;

    if(!checkWordExist(word, numChildArray, charArray, bigrams, unicodeInd)){
        cout << "word does not exist in list" << endl;
        return;
    }

    if(unicodeInd == 0){
        cout << "word does not exist in list" << endl;
        return;
    }

    vector<uint16_t> utf16Vec;

    utf16Vec.push_back(unicodeCharArray[unicodeInd]);

    while(1){

        unicodeInd = unicodeParentArray[unicodeInd];

        if(unicodeInd == 0)
            break;

        unicodeInd = unicodeInd-1;

        utf16Vec.push_back(unicodeCharArray[unicodeInd]);

    }

    reverse(utf16Vec.begin(), utf16Vec.end());

    utf8::utf16to8(utf16Vec.begin(), utf16Vec.end(), back_inserter(output));

}



void testDryRun(const vector<u_char> &charArray, const vector<uint16_t> &bigrams, const vector<uint8_t> &numChildArray, const vector<uint16_t> &unicodeCharArray, const vector<uint16_t> &unicodeParentArray) {

    cout << endl << "*****************Dry-Run**************" << endl << endl;


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
            string output;
            getTransliteration(curr_input, numChildArray, charArray, bigrams, unicodeCharArray, unicodeParentArray, output);
            cout << "transliteration: " + output << endl;
        }
    }

}



void testDryRun(Trie &t) {

    cout << endl << "*****************Dry-Run**************" << endl << endl;

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

void createTries(const string &fileName, Trie &trie, UTrie &utrie, vector<u_char> &charArray, vector<uint16_t> &bigrams, vector<uint8_t> &numChildArray, vector<uint16_t> &unicodeCharArray,
                 vector<uint16_t> &unicodeParentArray, vector<uint8_t> &unicodeNumChildArray, const string &path){

    ifstream fs8(fileName.c_str());
    if (!fs8.is_open()) {
        cout << "Could not open " << fileName << endl;
        return;
    }

    string linestr;

    shared_ptr<cNode> cnode;
    shared_ptr<uNode> unode;

    bool isWordExist;

    while (getline(fs8, linestr)) {

        size_t delim_pos = linestr.find_first_of(",");

        if (delim_pos == string::npos)
            continue;

        string word1 = linestr.substr(0, delim_pos);

        //skip if word length is 1
        if(word1.length() <= 1)
            continue;

        //convert to lower case
        if(toLowerCase(word1) > 1)
            continue; //skip if more than one capital character is found

        //filter duplicates -- only first instance is considered
        if( find(testWords.begin(), testWords.end(), word1)-testWords.begin() < testWords.size())
            continue;

        testWords.push_back(word1);

        string word2 = linestr.substr(delim_pos + 1, linestr.size());

        testOutputs.push_back(word2);

        //LOG("word1: " + word1);
        //LOG("word2: " + word2);

        string temp = "";

        trie.getSetWordNode(word1, isWordExist, cnode);
        utrie.getSetWordNode(word2, isWordExist, unode);


        cnode->bigram = unode;
    }

    if(trie.isEmpty() || utrie.isEmpty()){
        cout << "empty tries";
        return;
    }


    serialize(trie, utrie, charArray, bigrams, numChildArray, unicodeCharArray, unicodeParentArray, unicodeNumChildArray);

    writeToBinaryFile(charArray, bigrams, numChildArray, unicodeCharArray, unicodeParentArray, path);


}



void testTranscription(const vector<u_char> &charArray, const vector<uint16_t> &bigrams, const vector<uint8_t> &numChildArray, const vector<uint16_t> &unicodeCharArray, const vector<uint16_t> &unicodeParentArray){


    int countMismatches = 0;

    for(size_t i = 0; i < testWords.size(); i++){

        string output;

        string input = testWords[i];

        toLowerCase(input);

        //filter duplicates -- only first instance is considered
        if( find(checkedWords.begin(), checkedWords.end(), input)-checkedWords.begin() < checkedWords.size())
            continue;

        checkedWords.push_back(input);

        getTransliteration(input, numChildArray, charArray, bigrams, unicodeCharArray, unicodeParentArray, output);

        string trueOutput = testOutputs[i];

        if(strcmp(output.c_str(), trueOutput.c_str())!=0) {
            countMismatches++;

            cout << "mismatch found for input: " << input << endl << "true output: " << trueOutput << endl << "output found: " << output << endl;
        }

    }

    cout << "total number of words: " << testWords.size() << endl;

    cout << "total number of mismatches: " << countMismatches << endl;


}



void testTranscription2(const string &path, const vector<u_char> &charArray, const vector<uint16_t> &bigrams, const vector<uint8_t> &numChildArray, const vector<uint16_t> &unicodeCharArray,
                        const vector<uint16_t> &unicodeParentArray){

    ifstream fs8(path.c_str());
    if (!fs8.is_open()) {
        cout << "Could not open " << path << endl;
        return;
    }

    string linestr;

    int countMatches = 0;

    vector<string> allWords;

    while (getline(fs8, linestr)) {

        size_t prev_pos = 0;

        size_t delim_pos = linestr.find_first_of(" ", prev_pos);

        while (delim_pos != string::npos){

            string word = linestr.substr(prev_pos, delim_pos);

            allWords.push_back(word);

            if(find(testWords.begin(), testWords.end(), word) - testWords.begin() < testWords.size()) {

                cout << word << endl;

                countMatches++;
            }

            prev_pos = delim_pos+1;

            delim_pos = linestr.find_first_of(" ", prev_pos);
        }
    }

    cout << "total number of words in corpus: " << allWords.size() << endl;

    cout << "number of matches: " << countMatches << endl;

}


int main() {
    cout << "Creating tries from file..." << std::endl;

    string fileName = "/media/DATA/fromUbuntu/Bobble/keyboard/transliteration/direct_mappings/hinglish_hindi_10Feb_filtered.csv";//"en_hi_without_duplicates_1.txt";  //"test.txt";//"";//"Bengalish_Bengoli.txt";//

    Trie trie;
    UTrie utrie;

    vector<u_char> charArray;
    vector<uint16_t> bigrams;
    vector<uint8_t> numChildArray;
    vector<uint16_t> unicodeCharArray;
    vector<uint16_t> unicodeParentArray;
    vector<uint8_t> unicodeNumChildArray;

    string binaryFilePath = "/media/DATA/fromUbuntu/Bobble/keyboard/transliteration/direct_mappings/hinglish_hindi_10Feb_filtered.bin";

    createTries(fileName, trie, utrie, charArray, bigrams, numChildArray, unicodeCharArray, unicodeParentArray, unicodeNumChildArray, binaryFilePath);

    readBinaryFile(binaryFilePath, charArray, bigrams, numChildArray, unicodeCharArray, unicodeParentArray);

    // test manually using tries
    //testDryRun(trie);

    // test manually using vectors from binary file or serialized vectors
    //testDryRun(charArray, bigrams, numChildArray, unicodeCharArray, unicodeParentArray);  //, unicodeNumChildArray

    //test against all words in mappings
    testTranscription(charArray, bigrams, numChildArray, unicodeCharArray, unicodeParentArray);


    //test against corpus
    //string testFile = "hinglish_corpus3.txt";
    //testTranscription2(testFile, charArray, bigrams, numChildArray, unicodeCharArray, unicodeParentArray);


    return 0;
}

