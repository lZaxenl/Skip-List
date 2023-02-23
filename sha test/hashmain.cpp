#include <iostream>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

#include "sha1.h"

string hashing(int lvl, int rank, string str1 = "0", string str2 = "0") {
    SHA1 sha1;
    stringstream ss1,ss2;
    ss1 << lvl;
    ss2 << rank;
    return sha1(sha1(str2) + sha1(ss1.str()) + sha1(ss2.str()) + sha1(str1));
}


int main() {
    cout << "\ngamma_1: ";
    cout << hashing(0, 2, 
    "44C9C163639E35E85C10E9666D1C8BB99187C0EC", 
    "AC361E02A20D2A2552DBF8949DB295680D128961") << endl;

    cout << "gamma_2: ";
    cout << hashing(3, 2, 
    "463A643E7CC7255D7DFE62C33BAA366251A61FA5", 
    "2BDAA335F4C25FC5D08CE19CAA454EB7FB64D3E8") << endl << endl;


    return 0;
}