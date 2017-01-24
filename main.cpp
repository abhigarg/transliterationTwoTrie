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

        //string linestr(line);

        if (line_count < 3) {
            line_count++;
            continue;
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

        trie.getSetWordNode(word1, isWordExist, cnode);
        utrie.getSetWordNode(word2, isWordExist, unode);

        cnode->bigram = unode;
    }

    LOG("number of cnodes: " + to_string(numCNodes));
    LOG("number of unodes: " + to_string(numUNodes));

    utrie.getSizeOf();

}


int main() {
    cout << "Creating tries from file..." << std::endl;

    string fileName = "en_hi_without_duplicates.txt";// "en-hi-test-data.txt";  //

    Trie trie;
    UTrie utrie;

    createTries(fileName, trie, utrie);


    cout << "size of trie: " << sizeof(trie) << endl;

    cout << "size of unicode trie: " << sizeof(utrie) << endl;

    if(trie.isEmpty() || utrie.isEmpty()){
        cout << "empty tries";

        return -1;
    }

    testDryRun(trie);

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
