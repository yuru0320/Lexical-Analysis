//  main.cpp
//  hw1
//  Created by Ruby on 2021/10/20.
#include <iostream> // cout, endl
#include <fstream>    // open, is_open, close, write, ignore
#include <string>    // string, find_last_of, substr, clear
#include <cstdlib>    // atoi, system
#include <iomanip>    // setw
#include <cstring>    // strcpy
#include <vector>     // vector, push_back
#include <time.h>
#include <stdio.h>
//#include <array>
using namespace std;
typedef struct {
    string instruction ;
    int loc ;
    int table ;
} Tabledata;

class LexicalAnalysis{
    public:
    fstream outFile;
    vector <Tabledata> tData1, tData2, tData3, tData4, fData ;
    Tabledata tData5[100],tData6[100],tData7[100]; // symbol,integer,string
    bool isStr, integer;
    bool ReadFile();
    void ReadTable();
    bool FindTable(string);
    void BuildTable(string);
    bool IsDig(string);
    int Hash(string);
    void initial();
    bool checkTable4(char, string);
};
// ==================== MAIN() ====================
int main(void) {
    LexicalAnalysis LAItem;
    int command = 0;
    do {
        cout << endl << "************     HW1     ************";
        cout << endl << "* 0. QUIT                           *";
        cout << endl << "* 1. LexicalAnalysis                *";
        cout << endl << "*************************************";
        cout << endl << "Input a choice(0, 1): ";
        cin >> command; // get a command;
        switch (command) {
        case 1:
                LAItem.initial();
                LAItem.ReadTable();
                LAItem.ReadFile();
                break;
        case 0: cout << "Quit the program !" << endl; break;
        default: cout << endl << "Command does not exist!" << endl;
        } // end switch( command )
    } while (command != 0);
    return 0;
} // end main()

// ====================⇡MAIN()⇡====================
void LexicalAnalysis::initial(){
    isStr = false;
    integer = false;
    tData1.clear();
    tData2.clear();
    tData3.clear();
    tData4.clear();
    fData.clear();
    for(int i = 0; i < 100 ; i++){
        tData5[i].instruction = "";
        tData6[i].instruction = "";
        tData7[i].instruction = "";
    }
}

bool LexicalAnalysis::ReadFile(){
    fstream File; // input file handle
    string fileName;
    string ins = "";
    bool inTable = false, table4 = false, isTable4 = false;
    char ch;
    int j = 0;
    bool typeC = false, typeX  = false , first = true;
    string dataLine = "" ;
    Tabledata temp;
    do {
        cout << endl << "Input a file name ( input...( [0]:quit )): ";
        cin >> fileName;
        if (!fileName.compare("0")) // No file to open, so quit
            return false;
        File.open(("SIC_" + fileName + ".txt").c_str(), fstream::in); // open a txt file
        if (File.is_open())
            break; // successfully open a file, so leave the loop
        else {
            cout << endl << "******* SIC_" + fileName + ".txt  IS NOT EXIST!! *******" << endl ;
        } // else
    } while (true); //end while1

    if (File.is_open()) {
        outFile.open("SIC_output.txt", fstream::out);
        while ( true ) {
            File >> noskipws >> ch ;
            dataLine = dataLine + ch;
            if(ch == '\n' && !File.eof() && !first) {
                first = false;
                cout << "\r\n"  ;
                outFile << "\r\n" ;
            }
            if(ch == 'C'|| ch == 'X' || ch == '\''){
                if( ch == 'C' || ch == '\'' ) typeC = true;
                else if(ch == 'X') typeX = true;
                ins = ins + ch;
                if(ch != '\'') {
                    File >> noskipws >> ch ;
                    dataLine = dataLine + ch;
                }
                if(ch == '\'' ){
                    table4 = true;
                    ins ="";
                    ins.insert(0, 1, ch);
                    inTable = FindTable(ins); //存‘
                    File >> noskipws >> ch ;
                    dataLine = dataLine + ch;
                    ins = "";
                    if(ch == '\''){
                        ins.insert(0, 1, ch);
                        inTable = FindTable(ins); //存‘
                        ins = "";
                    }
                    else{
                        while( ch != '\'' ){ // 處理EOF
                            ins = ins+ch;
                            File >> noskipws >> ch ;
                            dataLine = dataLine + ch;
                            if(ch == '\n' || ch == '\r') {
                                break;
                            }
                        }
                        if(typeC) isStr = true;
                        else integer = true;
                        BuildTable(ins);
                        ins ="";
                        if(ch != '\n' || ch != '\r'){
                            ins.insert(0, 1, ch); // char to string 把‘存回ins遇到空白再處理
                            inTable = FindTable(ins);
                            ins ="";
                        }
                        if(ch == '\n' ) {
                            first = false ;
                            cout << dataLine;
                            outFile << dataLine ;
                            dataLine = "" ;
                            while( j < fData.size() ){
                                outFile << "(" << fData[j].table << "," << fData[j].loc << ")";
                                cout  << "(" << fData[j].table << "," << fData[j].loc << ")";
                                j++ ;
                            }
                            //if(!File.eof())outFile << "\r\n";
                            //if(!File.eof())cout << "\n";
                        }
                    }
                }
            }
            if(!table4) isTable4 =  checkTable4(ch, ins); // 判斷是否為delimiter
            if(isTable4) ins = "" ;
            if(!table4 && !isTable4){
                if(ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {
                    inTable = FindTable(ins);
                    if(inTable == false) BuildTable(ins);
                    ins = "";
                    if(ch == '\n'  ){ //換行及寫入資料
                        first = false ;
                        cout << dataLine ;
                        outFile << dataLine ;
                        dataLine = "" ;
                        for( j = j; j < fData.size() ; j++ ){
                            outFile << "(" << fData[j].table << "," << fData[j].loc << ")";
                            cout  << "(" << fData[j].table << "," << fData[j].loc << ")";
                        }
                    }
                }
                else ins = ins + ch;
            }
            typeC = false;
            typeX = false;
            table4 = false;
            if(File.eof()) break;
        } //end while
        File.close(); // close input file
        outFile.close();
        return true;
    } // end if
    else
        return false;
} // end readfile

void LexicalAnalysis::ReadTable(){
    fstream TableFile; // input file handle
    string tableinput;
    Tabledata temp;
    int location = 1 ;
    TableFile.open("Table1.table", fstream::in); // open a table file
    if(TableFile.is_open()) {
        while ( true ) {
            TableFile >> tableinput ;
            temp.instruction = tableinput;
            temp.loc =location;
            location ++ ;
            temp.table = 1;
            tData1.push_back(temp);
            if(TableFile.eof()) break;
        } //end while
        TableFile.close(); // close input file
    } // end if
    TableFile.open("Table2.table", fstream::in); // open a table file
    location = 1;
    if(TableFile.is_open()) {
        while ( true ) {
            TableFile >> tableinput ;
            temp.instruction = tableinput;
            temp.loc =location;
            location ++ ;
            temp.table = 2;
            tData2.push_back(temp);
            if(TableFile.eof()) break;
        } //end while
        TableFile.close(); // close input file
    } // end if
    TableFile.open("Table3.table", fstream::in); // open a table file
    location = 1;
    if(TableFile.is_open()) {
        while ( true ) {
            TableFile >> tableinput ;
            temp.instruction = tableinput;
            temp.loc =location;
            location ++ ;
            temp.table = 3;
            tData3.push_back(temp);
            if(TableFile.eof()) break;
        } //end while
        TableFile.close(); // close input file
    } // end if
    TableFile.open("Table4.table", fstream::in); // open a table file
    location = 1;
    if(TableFile.is_open()) {
        while ( true ) {
            TableFile >> tableinput ;
            temp.instruction = tableinput;
            temp.loc =location;
            location ++ ;
            temp.table = 4;
            tData4.push_back(temp);
            if(TableFile.eof()) break;
        } //end while
        TableFile.close(); // close input file
    } // end if
}

bool LexicalAnalysis::FindTable(string ins){
    Tabledata temp;
    string instemp  = ins ;
    string instemp2  = ins ;
    if(ins != ""){
        for(int j = 0 ; j <ins.length() ;j++){ //小寫轉大寫
            if( ins[j] >= 'a' && ins[j] <= 'z' ) instemp[j] = ins[j] - 32;
        }
        for(int j = 0 ; j < ins.length();j++){ //大寫轉小寫
           if( ins[j] >= 'A' && ins[j] <= 'Z' ) instemp2[j] = ins[j] + 32;
        }
    }
    
    if(ins != ""){
        for(int i = 0 ;i < tData1.size(); i++ ){
            if(instemp2 == tData1[i].instruction || instemp == tData1[i].instruction ){
                temp.instruction = tData1[i].instruction;
                temp.loc = tData1[i].loc;
                temp.table = tData1[i].table;
                fData.push_back(temp);
                return true;
            }
        }
        for(int i = 0 ;i < tData2.size(); i++ ){
            if(instemp2 == tData2[i].instruction || instemp == tData2[i].instruction){
                temp.instruction = tData2[i].instruction;
                temp.loc = tData2[i].loc;
                temp.table = tData2[i].table;
                fData.push_back(temp);
                return true;
            }
        }
        for(int i = 0 ;i < tData3.size(); i++ ){
            if(instemp2 == tData3[i].instruction || instemp == tData3[i].instruction ){
                temp.instruction = tData3[i].instruction;
                temp.loc = tData3[i].loc;
                temp.table = tData3[i].table;
                fData.push_back(temp);
                return true;
            }
        }
        for(int i = 0 ;i < tData4.size(); i++ ){
            if(ins == tData4[i].instruction ){
                temp.instruction = tData4[i].instruction;
                temp.loc = tData4[i].loc;
                temp.table = tData4[i].table;
                fData.push_back(temp);
                return true;
            }
        }
        return false;
    }
    return true;
}

void LexicalAnalysis::BuildTable(string ins){
    Tabledata temp;
    int hashLoc = 0;
    hashLoc = Hash(ins) ;
    if (IsDig(ins) || integer ){ // integer
        while(tData6[hashLoc].instruction != "" && tData6[hashLoc].instruction != ins){
            hashLoc ++;
        }
        tData6[hashLoc].instruction = ins;
        tData6[hashLoc].loc = hashLoc;
        tData6[hashLoc].table = 6;
        fData.push_back(tData6[hashLoc]);
        integer = false;
    }
    else if(isStr){ // String
        while(tData7[hashLoc].instruction != "" && tData7[hashLoc].instruction != ins){
            hashLoc ++;
        }
        tData7[hashLoc].instruction = ins;
        tData7[hashLoc].loc = hashLoc;
        tData7[hashLoc].table = 7;
        fData.push_back(tData7[hashLoc]);
        isStr = false;
    }
    else { //5
        while(tData5[hashLoc].instruction != "" && tData5[hashLoc].instruction != ins){
            hashLoc ++;
        }
        tData5[hashLoc].instruction = ins;
        tData5[hashLoc].loc = hashLoc;
        tData5[hashLoc].table = 5;
        fData.push_back(tData5[hashLoc]);
    }
}

bool LexicalAnalysis::IsDig(string ins){
    bool dig = false;
    for(int i = 0; i < ins.length() ; i++ ){
        if((ins[i] <= '9' && ins[i] >= '0') ||  ins[0] == 'X' ) dig = true;
        else return false;
    }
    if(dig) return true;
    else return false;
}

int LexicalAnalysis::Hash(string ins) {
    int loc = 0, i = 0, sum = 0 ;
    while(i < ins.length()){
        sum = sum + (int)ins[i];
        i++;
    }
    loc = sum % 100;
    return loc ;
}

bool LexicalAnalysis::checkTable4(char ch, string ins){
    bool inTable = false;
    Tabledata temp;
    for(int j = 0; j < tData4.size(); j++){
        string str = tData4[j].instruction;
        if( ch == str[0]){ //delimiter
            if(ins != ""){
                inTable = FindTable(ins);
                if(inTable == false) BuildTable(ins);
            }
            temp.instruction = tData4[j].instruction; //'
            temp.loc = tData4[j].loc;
            temp.table = tData4[j].table;
            fData.push_back(temp);
            return true;
        }
    }
    return false;
}
