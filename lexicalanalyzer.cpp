#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

void gettoken(string tok, map <string, int> &mu){
    if (mu.find(tok) == mu.end()){
        mu[tok] = 1;
    }
    else{
        mu[tok] += 1;
    }
}
void getid(string id, map <string, int> &m){
    if (m.find(id) == m.end()){
        m[id] = 1;
    }
    else{
        m[id] += 1;
    }
}

int main(int argc, char* argv[]){

    bool dFlags[] = {false, false, false, false}; //([0] = "v", [1] = "stats", [2] = "sum", [3] = file found) - set of flags
    bool empty1 = false;
    ifstream fr;
    int lCount = 0;
    int tCount = 0;
    int iCount = 0;
    string frequent = "";

    map <string, int> tm;
    map <string, int> tmuk;
    //dFlags[1] = true;
    //dFlags[0] = true;
    //dFlags[2] = true;

    for (int i = 1; i < argc; i++){
        string arg = argv[i];
        //cout << "test --- " << arg << endl;

        if (arg.substr(0, 1).compare("-") == 0){ //Checks what functions are being called
            int len = arg.length();
            if (len == 2 && arg.substr(1).compare("v") == 0){
                dFlags[0] = true;
            }
            else if (len == 6 && arg.substr(1).compare("stats") == 0){
                dFlags[1] = true;
            }
            else if (len == 4 && arg.substr(1).compare("sum") == 0){
                dFlags[2] = true;
            }
            else {
                cout << "Invalid argument " << arg << endl;
                return 0;
            }
        }

        else { //For anything without the - preceeding it (filenames)
            if (dFlags[3]){
                cout << "Too many file names" << endl;
                return 0;
            }
            fr.open(arg);
            if (fr.is_open()){
                dFlags[3] = true;
            }
            else {
                cout << "Could not open " << arg << endl;
                return 0;
            }
        }
    }
    if (fr.peek() == ifstream::traits_type::eof() && dFlags[3]){
        if (dFlags[1]){ //if there was a -stats function
            cout << "Total IDs: " << iCount << endl;
        }
        if (dFlags[2]){ //if there was a -sum function
            cout << "Total Lines: " << lCount << endl;
            cout << "Total tokens: " << tCount << endl;
        }
        return 0;
    }
    char c;
    bool commented = false;
    bool stringed = false;
    bool special = false;
    bool finalize = false;
    string temp = "";

    while (fr.get(c)){
        //cout << "test --- " << c << " --- " << temp << endl;
        finalize = false;
        if (!commented && !stringed){
            switch (c){ // Handles special single-character tokens
                case '+' :
                    if (dFlags[0]) { cout << "PLUS" << endl; }
                    gettoken("PLUS", tmuk);
                    special = true;
                    tCount++;
                    break;
                case '-' :
                    if (dFlags[0]) { cout << "MINUS" << endl; }
                    gettoken("MINUS", tmuk);
                    special = true;
                    tCount++;
                    break;
                case '*' :
                    if (dFlags[0]) { cout << "STAR" << endl; }
                    gettoken("STAR", tmuk);
                    special = true;
                    tCount++;
                    break;
                case ',' :
                    if (dFlags[0]) { cout << "COMMA" << endl; }
                    gettoken("COMMA", tmuk);
                    special = true;
                    tCount++;
                    break;
                case '{' :
                    if (dFlags[0]) { cout << "LBR" << endl; }
                    gettoken("LBR", tmuk);
                    special = true;
                    tCount++;
                    break;
                case '}' :
                    if (dFlags[0]) { cout << "RBR" << endl; }
                    gettoken("RBR", tmuk);
                    special = true;
                    tCount++;
                    break;
                case '[' :
                    if (dFlags[0]) { cout << "LSQ" << endl; }
                    gettoken("LSQ", tmuk);
                    special = true;
                    tCount++;
                    break;
                case ']' :
                    if (dFlags[0]) { cout << "RSQ" << endl; }
                    gettoken("RSQ", tmuk);
                    special = true;
                    tCount++;
                    break;
                case '(' :
                    if (dFlags[0]) {  cout << "LPAREN" << endl; }
                    gettoken("LPAREN", tmuk);
                    special = true;
                    tCount++;
                    break;
                case ')' :
                    if (dFlags[0]) {  cout << "RPAREN" << endl; }
                    gettoken("RPAREN", tmuk);
                    special = true;
                    tCount++;
                    break;
                case ';' :
                    if (dFlags[0]) {  cout << "SC" << endl; }
                    gettoken("SC", tmuk);
                    special = true;
                    tCount++;
                    break;
                default:
                    break;
            }
        }
        if (c == '.') {
            if (temp.empty()){
                cout << "Error on line " << lCount+1 << " (" << c << ")" << endl;
                temp = "";
                return 0;
            }
        }
        if (special) { // Loops back up if it has printed one of the single-character tokens
            special = false;
            finalize = true;
        }
        else if (c == '#'){ // Checks for conditionals regarding a comment
            char t = fr.peek();
            if (!stringed){
                commented = true;
                continue;
            }
            else if (stringed){
                temp += c;
                continue;
            }
        }
        else if (c == '"'){ // Checks for conditionals regarding a STRING token
            if (!commented && !stringed){ // Sets up the conditionals for the STRING token if not done already
                temp = "";
                stringed = true;
                temp += "\"";
            }
            else if (stringed){ // Exits out of the STRING token and prints it
                stringed = false;
                temp += "\"";
                if (dFlags[0]){ //if there was a -v function
                    cout << "STRING (" << temp << ")" << endl;
                }
                temp = "";
            }
        }
        else if (c == '\n'){ // Checks if its end of line
            lCount++;
            if (commented){
                commented = false;
                temp = "";
                continue;
            }
            else if (stringed){
                stringed = false;
                cout << "Error on line " << lCount << " (" << temp << ")" << endl;
                temp = "";
                return 0;
            }
            else {
                finalize = true;
            }
        }
        else { // If not any special condition character, it adds it to temp for analysis later on
            if (!isspace(c) && !commented){
                temp += c;
            }
            else if (isspace(c) && stringed){
                temp += c;
            }
        }

        if ((isspace(c) && !stringed) || finalize){ // Checks if a token is ready to be analyzed
            if (!temp.empty()){
                bool id = false;
                bool intgr = false;
                bool flt = false;

                for (int i = 0; i < temp.length(); i++){
                    char t = temp[i];
                    if (isdigit(t)){
                        if (!flt){
                            intgr = true;
                        }
                    }
                    else if (!isdigit(t)){
                        if (t == '.'){
                            flt = true;
                            intgr = false;
                            continue;
                        }
                        else if (t == '\n'){
                            temp.erase(i);
                            continue;
                        }
                        id = true;
                        flt = false;
                        intgr = false;
                    }
                }
                if (id){
                    iCount++;
                    tCount++;
                    if (dFlags[0]){
                        cout << "ID (" << temp << ")" << endl;
                    }
                    getid(temp, tm);
                    gettoken("ID", tmuk);
                    //cout << temp << endl;
                    temp = "";
                }
                else if (flt) {
                    tCount++;
                    if (dFlags[0]){
                        cout << "FCONST (" << temp << ")" << endl;
                    }
                    gettoken("FCONST", tmuk);
                    temp = "";
                }
                else if (intgr) {
                    tCount++;
                    if (dFlags[0]){
                        cout << "ICONST (" << temp << ")" << endl;
                    }
                    gettoken("ICONST", tmuk);
                    temp = "";
                }
                finalize = false;
                continue;
            }
        }
    }
    if (!dFlags[3]){
        string lineF;
        while (getline(cin, lineF)){
            lCount++;
            lineF += '\n'
            for (int i = 0; i < lineF.length(); i++){
                c = lineF[i];
                finalize = false;
                if (!commented && !stringed){
                    switch (c){ // Handles special single-character tokens
                        case '+' :
                            if (dFlags[0]) { cout << "PLUS" << endl; }
                            gettoken("PLUS", tmuk);
                            special = true;
                            tCount++;
                            break;
                        case '-' :
                            if (dFlags[0]) { cout << "MINUS" << endl; }
                            gettoken("MINUS", tmuk);
                            special = true;
                            tCount++;
                            break;
                        case '*' :
                            if (dFlags[0]) { cout << "STAR" << endl; }
                            gettoken("STAR", tmuk);
                            special = true;
                            tCount++;
                            break;
                        case ',' :
                            if (dFlags[0]) { cout << "COMMA" << endl; }
                            gettoken("COMMA", tmuk);
                            special = true;
                            tCount++;
                            break;
                        case '{' :
                            if (dFlags[0]) { cout << "LBR" << endl; }
                            gettoken("LBR", tmuk);
                            special = true;
                            tCount++;
                            break;
                        case '}' :
                            if (dFlags[0]) { cout << "RBR" << endl; }
                            gettoken("RBR", tmuk);
                            special = true;
                            tCount++;
                            break;
                        case '[' :
                            if (dFlags[0]) { cout << "LSQ" << endl; }
                            gettoken("LSQ", tmuk);
                            special = true;
                            tCount++;
                            break;
                        case ']' :
                            if (dFlags[0]) { cout << "RSQ" << endl; }
                            gettoken("RSQ", tmuk);
                            special = true;
                            tCount++;
                            break;
                        case '(' :
                            if (dFlags[0]) {  cout << "LPAREN" << endl; }
                            gettoken("LPAREN", tmuk);
                            special = true;
                            tCount++;
                            break;
                        case ')' :
                            if (dFlags[0]) {  cout << "RPAREN" << endl; }
                            gettoken("RPAREN", tmuk);
                            special = true;
                            tCount++;
                            break;
                        case ';' :
                            if (dFlags[0]) {  cout << "SC" << endl; }
                            gettoken("SC", tmuk);
                            special = true;
                            tCount++;
                            break;
                        default:
                            break;
                    }
                }
                if (c == '.') {
                    if (temp.empty()){
                        cout << "Error on line " << lCount+1 << " (" << c << ")" << endl;
                        temp = "";
                        return 0;
                    }
                }
                if (special) { // Loops back up if it has printed one of the single-character tokens
                    special = false;
                    finalize = true;
                }
                else if (c == '#'){ // Checks for conditionals regarding a comment
                    char t = fr.peek();
                    if (!stringed){
                        commented = true;
                        continue;
                    }
                    else if (stringed){
                        temp += c;
                        continue;
                    }
                }
                else if (c == '"'){ // Checks for conditionals regarding a STRING token
                    if (!commented && !stringed){ // Sets up the conditionals for the STRING token if not done already
                        temp = "";
                        stringed = true;
                        temp += "\"";
                    }
                    else if (stringed){ // Exits out of the STRING token and prints it
                        stringed = false;
                        temp += "\"";
                        if (dFlags[0]){ //if there was a -v function
                            cout << "STRING (" << temp << ")" << endl;
                        }
                        temp = "";
                    }
                }
                else if (c == '\n'){ // Checks if its end of line
                    lCount++;
                    if (commented){
                        commented = false;
                        temp = "";
                        continue;
                    }
                    else if (stringed){
                        stringed = false;
                        cout << "Error on line " << lCount << " (" << temp << ")" << endl;
                        temp = "";
                        return 0;
                    }
                    else {
                        finalize = true;
                    }
                }
                else { // If not any special condition character, it adds it to temp for analysis later on
                    if (!isspace(c) && !commented){
                        temp += c;
                    }
                    else if (isspace(c) && stringed){
                        temp += c;
                    }
                }

                if ((isspace(c) && !stringed) || finalize){ // Checks if a token is ready to be analyzed
                    if (!temp.empty()){
                        bool id = false;
                        bool intgr = false;
                        bool flt = false;

                        for (int i = 0; i < temp.length(); i++){
                            char t = temp[i];
                            if (isdigit(t)){
                                if (!flt){
                                    intgr = true;
                                }
                            }
                            else if (!isdigit(t)){
                                if (t == '.'){
                                    flt = true;
                                    intgr = false;
                                    continue;
                                }
                                else if (t == '\n'){
                                    temp.erase(i);
                                    continue;
                                }
                                id = true;
                                flt = false;
                                intgr = false;
                            }
                        }
                        if (id){
                            iCount++;
                            tCount++;
                            if (dFlags[0]){
                                cout << "ID (" << temp << ")" << endl;
                            }
                            getid(temp, tm);
                            gettoken("ID", tmuk);
                            //cout << temp << endl;
                            temp = "";
                        }
                        else if (flt) {
                            tCount++;
                            if (dFlags[0]){
                                cout << "FCONST (" << temp << ")" << endl;
                            }
                            gettoken("FCONST", tmuk);
                            temp = "";
                        }
                        else if (intgr) {
                            tCount++;
                            if (dFlags[0]){
                                cout << "ICONST (" << temp << ")" << endl;
                            }
                            gettoken("ICONST", tmuk);
                            temp = "";
                        }
                        finalize = false;
                        continue;
                    }
                }
            }
        }
    }

    if (dFlags[1]){ //if there was a -stats function
        cout << "Total IDs: " << iCount << endl;
        if (iCount != 0){
            cout << "List of IDs: ";
            for( map<string, int>::iterator it = tm.begin(); it != tm.end(); ++it){
                cout << it->first;
                if (it->first != tm.rbegin()->first){
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }
    if (dFlags[2]){ //if there was a -sum function
        cout << "Total Lines: " << lCount << endl;
        cout << "Total tokens: " << tCount << endl;
        if (tCount != 0){
            int max = 0;
            int fcheck = -1;
            for( map<string, int>::iterator it = tmuk.begin(); it != tmuk.end(); ++it){ //sets max
                if (it->second >= max){
                    max = it->second;
                }
            }
            for( map<string, int>::reverse_iterator it = tmuk.rbegin(); it != tmuk.rend(); ++it){ //makes frequent
                if (it->second >= max){
                    fcheck++;
                    if (fcheck > 0){
                        frequent += ", ";
                    }
                    frequent += it->first;
                }
            }
            cout << "Most frequently used tokens: " << frequent << endl;
        }
    }
}
