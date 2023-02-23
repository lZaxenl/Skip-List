#include <iostream>
#include <string.h>
using namespace std;
#define tuple tuple<int, int, bool, string>

string computeLabel(string Tau, list <tuple>& Tuple_List, int maxHeight/*highest level of new data block*/) {
    int lambda1 = 0;     // get<0>(A), lambda (λ) = l(v), the level of vj
    int rho1 = 1;        // get<1>(A), rho (ρ) = r(v), rank of successor of node 'v'
    // get<2>(A), delta (δ), if previous node v(j-1) is to the right (true) or below (false) v(j)
    bool delta1 = 0;     
    string gamma1 = Tau; // get<3>(A), label of successor of node 'v'
    bool mergePoint = false; // determines if we found the point that new data block merges into list

    // values for new data block at lvl == 0
    int lambda2 = 0;
    int rho2 = 1;
    bool delta2 = 0;
    stringstream ss;
    ss << hash_number; // This will be changed to the metadata eventually
    string gamma2 = hashing(lambda2, rho2, ss.str(), "0");     

    list <tuple>::iterator it;  // initialize an iterator to traverse the list
    it = Tuple_List.begin();
    
    /* A(v) = (l(v) level, q(v) rank, d(v) rgt(1)/dwn(0), g(v) label) */ 
    while(it != Tuple_List.end()) {
        tuple A = *it; // holds value of current 'it'
        lambda1 = get<0>(A);
        delta1 = get<2>(A);
        rho1 = rho1 + get<1>(A);

        if(lambda1 < maxHeight) {
            if(lambda1 > lambda2) {
                gamma2 = hashing(lambda2, rho2, gamma2, "0"); // normal hashing for gamma2
                ++lambda2;
            }
            else if(delta1) {// vj previous node is right
                gamma1 = hashing(lambda1, rho1, get<3>(A), gamma1); // gamma1 gets rehashed
            }
            else(!delta1) { // vj previous node is down
                if((rho1 - get<1>(A)) == rho1) { // no right pointer
                    gamma2 = hashing(lambda2, rho2, gamma2, "0"); // normal hashing for gamma2
                    ++lambda2;
                    gamma1 = hashing(lambda1, rho1, gamma1, get<3>(A));// gamma1 gets rehashed
                } 
                else { // right pointer
                    rho2 = rho2 + get<1>(A); // incerase the rank for new hash label
                    gamma2 = hashing(lambda2, rho2, gamma2, get<3>(A)); // gamma2 gets hashed with gamma1
                    ++lambda2;
                    rho1 = rho1 - get<1>(A); // reduce the rank for new hash label
                    gamma1 = hashing(lambda1, rho1, gamma1, "0"); // gamma1 loses right pointer and gets rehashed
                }
            }
        }
        else if(lambda1 >= maxHeight) {
            if(mergePoint) {
                if(delta1) { // vj previous node is right
                    ++lambda1;
                    gamma1 = hashing(lambda1, rho1, get<3>(A), gamma1); // gamma1 gets rehashed
                }
                else { // vj previous node is down
                    ++lambda1;
                    gamma1 = hashing(lambda1, rho1, gamma1, get<3>(A));// gamma1 gets rehashed
                }
            }
            else {
                if(delta1) { // vj previous node is right
                    rho2 = rho2 + get<1>(A); // incerase the rank for new hash label
                    gamma2 = hashing(lambda2, rho2, gamma2, get<3>(A)); // gamma2 gets hashed with gamma1
                    ++lambda1;
                    gamma1 = hashing(lambda1, rho1, gamma1, gamma2); // gamma1 gets rehashed
                }
                else { // vj previous node is down
                    rho2 = rho2 + get<1>(A); // incerase the rank for new hash label
                    gamma2 = hashing(lambda2, rho2, gamma2, get<3>(A)); // normal hashing for gamma2
                    ++lambda1;
                    gamma1 = hashing(lambda1, rho1, gamma1, gamma2);// gamma1 gets rehashed
                }
                mergePoint = true; // merge point was found
            }
        }
        ++it;
    }
    return gamma1;
}

    


