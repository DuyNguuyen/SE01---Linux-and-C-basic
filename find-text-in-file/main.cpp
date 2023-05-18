/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: home
 *
 * Created on March 28, 2023, 12:17 PM
 */

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

#define BUFFSIZE 10

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
   
    int a;
    
    if (argc != 3){
        cout << "Error! Invalid arguments";
        return 1;
    }
    
    ifstream path(argv[1]);
    string pattern = argv[2];
    
    if (path.fail()){
        cout << "Error! Can not open file path: " << argv[1] << "\n";
        return 1;
    }
    
    //The number of last characters will be store
    int storeLength = pattern.size() - 1;
    //buffer to store data from file
    char* buffer = new char[BUFFSIZE];
    //buffer to store last n characters of the block
    string storedBuffer;
    //number of pattern matching
    int anyMatch = 0;
    int currentLine = 0;
    //variable to store length of block have no "\n" at last
    int pvPatPos = 0;
    
    while (path.readsome(buffer, BUFFSIZE)) {
        string textFile = storedBuffer;
        textFile += buffer;
        
        int eolPos = 0;
        int pvEolPos = 0;

        //Split text before "\n" in buffer then process
        while((eolPos = textFile.find("\n", pvEolPos)) != string::npos){
            string textLine = textFile.substr(pvEolPos, eolPos - pvEolPos);
            pvEolPos = eolPos + 1;
            
            int patPos = -1;
            do {
                patPos = textLine.find(pattern, patPos + 1);
                if (patPos != string::npos){
                    cout << "Line " << currentLine + 1 << ", character at " << patPos + pvPatPos + 1 << "\n";
                    anyMatch++;
                }
            } while (patPos != string::npos);
            
            pvPatPos = 0;
            currentLine++;
        }
        
        //Process the block of text that have no "\n" at the end
        string incompLine = textFile.substr(pvEolPos);
        int patPos = -1;
        do {
                patPos = incompLine.find(pattern, patPos + 1);
                if (patPos != string::npos){
                    cout << "Line " << currentLine + 1 << ", character at " << patPos + pvPatPos + 1 << "\n";
                    
                    anyMatch++;
                }
        } while (incompLine.find(pattern, patPos + 1) != string::npos);
        
        //Store the last n character of the block
        if (incompLine.size() >= storeLength){
            storedBuffer = incompLine.substr(incompLine.size() - storeLength);
            pvPatPos += incompLine.length() - storeLength;
        } else {
            storedBuffer = incompLine;
            pvPatPos += incompLine.length();
        }
        
        memset(buffer, 0, BUFFSIZE);
    }

    
    if (!anyMatch)
    {
        cout << "Can not find any occurrences of pattern \"" << pattern << "\" in file \"" << argv[1] << "\"\n";
    } else {
        cout << "Found " << anyMatch << " occurrences of pattern \"" << pattern << "\" in file \"" << argv[1] << "\"\n";
    }
    
    delete buffer;
    path.close();
    return 0;
}