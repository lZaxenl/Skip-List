#include <iostream>
#include <stdio.h>
#include <tuple>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;

#include "sha1.h"

#define tuple tuple<int, int, bool, string>
int hash_number = 1;

class skipList {
    private:
        struct Node {
            int rank;  // how many data nodes the current node can reach
            int level; // highest level of data node
            int data;
            string label; // hash value for every node
            Node *rgt; // rgt points to the next data. This varies depending on the level
            Node *dwn; // dwn points to the nodes next level down in the column.
            Node *prev; // only used during insertion to set new hash values
            Node(int value, int lvl) {
                rgt = NULL;
                dwn = NULL;
                prev = NULL;
                rank = 1;
                data = value;
                level = lvl;

                stringstream ss;
                ss << hash_number;
                if(level > 0) { label = "m"; label += ss.str(); ++hash_number;}
                else { label = "b"; label += ss.str(); ++hash_number;}
            }
        };

        int MAXLVL;// Maximum level for this skip list
        // P is the fraction of the nodes with level
        // i pointers also having level i+1 pointers
        float P;
        string Tau; // represents Tau of block_i
        Node *root; // pointer to current node
        Node **envPtr; // environmental pointer

    public:
        // constructor
        skipList(int MAX, float P) {
            this->MAXLVL = MAX;
            this->P = P;
            root = new Node(-1, MAXLVL);
            root->rank = 0;
            root->label = "root";
            *envPtr = root;
            // The first node in the skip list is a 
            // sentinal node that holds no data
            for(int i = MAXLVL-1; i >= 0; --i) {
                (*envPtr)->dwn = new Node(-1, i);
                *envPtr = (*envPtr)->dwn;
                (*envPtr)->rank = 0;
                (*envPtr)->label = "root";
            }
            envPtr = NULL;
        }
        // delete skip list
        void delete_skipList() {
            del(root);
        }
        void del(Node *N) {
            if(N->rgt) {
                Node *R = N->rgt;
                del(R);
            }  
            if(N->dwn) {
                Node *D = N->dwn;
                del(D);
            }
            N->rgt = NULL;
            N->dwn = NULL;
            //printf("DELETE -> data: %d level: %d\n", N->data, N->level);
            delete N;
        }

        // create random level for node
        int randomLevel() {
            float r = (float)rand()/RAND_MAX;
            int lvl = 0;
            while (r < P && lvl < MAXLVL) {
                lvl++;
                r = (float)rand()/RAND_MAX;
            }
            return lvl;
        }

        // create a new node
        Node* createNode(int value, /* DELETE WHEN DONE */int lvl) {
            //int lvl = randomLevel();
            Node *n = new Node(value, lvl);
            Node* temp = n;
            // create a linked list coloumn of length 'lvl'
            for(int i = lvl-1; i >=0; --i) {
                temp->dwn = new Node(value, i);
                temp = temp->dwn;
            }
            //printf("new node: level = %d, \n\t  data = %d\n", n->level, n->data);
            return n;
        }
        string getT() { 
            return Tau; 
        }

//********************* INSERT NEW NODE **************************************************************
        void insertElement(int value, int block_i, /* DELETE WHEN DONE */ int lvl) {
            Node *N = createNode(value, /* DELETE WHEN DONE */ lvl); // New node being inserted
            Node *C = root;         // Current Node from root
            Node *top = N;          // holds top of tower 'N'
            int temp = N->level;    // Holds the highest level for new node
            int rank = root->rank;  // Used to find block_i
            int cnt = 0;            // used to adjust ranks after insertion
            pointer *path;

            printf("***** Insert New Element: %d *****\n", N->data);
            while((C || N)) {
                if(C->level == temp) {
                    if(C->rgt && ((rank - C->rgt->rank + 1) == block_i || (rank - C->rgt->rank) == block_i)) {
                        Node* temp = C->rgt; // Swap right pointers
                        C->rgt = N;
                        N->rgt = temp;
                        ++C->rank;

                        if(N->rgt != NULL)
                            N->rank += N->rgt->rank; // Increment rank by value of right pointers rank
                        rank -= C->rgt->rank;
            if(N->dwn) { N->dwn->prev = N; }
            if(C->dwn) { C->dwn->prev = C; }
                        N = N->dwn; // move to N's next level down
                        C = C->dwn; // move to C's next node down
                    }
                    else if(C->rgt && (rank - C->rgt->rank) < block_i) {
                        cnt += (C->rank - C->rgt->rank);
                        ++C->rank;
            if(C->rgt) { C->rgt->prev = C; }
                        C = C->rgt;
                    }
                    else {
                        C->rgt = N;
                        ++C->rank;
            if(N->dwn) { N->dwn->prev = N; }
            if(C->dwn) { C->dwn->prev = C; }
                        N = N->dwn; // move to N's next level down
                        C = C->dwn; // move to next node down
                    }
                }
                else if(C->level > temp) {
                    if(C->rgt && (rank - C->rgt->rank) >= block_i) {
                        rank -= C->rgt->rank;
                        ++C->rank;
            if(C->dwn) { C->dwn->prev = C; }
                        C = C->dwn;
                    }
                    else if(C->rgt && (rank - C->rgt->rank) < block_i && (rank - C->rgt->rank) > 0) {
                        cnt += (C->rank - C->rgt->rank);
                        ++C->rank;
            if(C->rgt) { C->rgt->prev = C; }
                        C = C->rgt;
                    }
                    else {
                        ++C->rank;
            if(C->dwn) { C->dwn->prev = C; }
                        C = C->dwn;
                    }
                }
                else if(C->level < temp) {
                    if(C->rgt && (rank - C->rgt->rank + 1) == block_i) {
                        N->rgt = C->rgt;
                        C->rank -= C->rgt->rank; // Decrement rank by value of right pointers rank
                        N->rank += N->rgt->rank; // Increment rank by value of right pointers rank
                        C->rgt = NULL;

                        /* call function in increment entire tower of 'N' */
                        Node *incr = top;
                        while(incr->level > C->level) {
                            incr->rank += N->rgt->rank;
                            incr = incr->dwn;
                        }
                        incr = NULL;
                        delete incr;
            if(N->dwn) { N->dwn->prev = N; }
            if(C->dwn) { C->dwn->prev = C; }
                        N = N->dwn; // move to N's next level down
                        C = C->dwn; // move to C's next node down
                    }
                    else if(C->rgt) {
                        if(C->rgt && (rank - C->rgt->rank + cnt) < block_i) {
                            C->rank -= cnt;
                        }
            if(C->rgt) { C->rgt->prev = C; }
                        C = C->rgt; // move to C's next node right
                    }
                    else {
                        if(C->rgt && (rank - C->rgt->rank + cnt) < block_i) {
                            C->rank -= cnt;
                        }
            if(N->dwn) { N->dwn->prev = N; }
            if(C->dwn) { C->dwn->prev = C; }
                        N = N->dwn; // move to N's next level down
                        C = C->dwn; // move to C's next node down
                    }
                }
            }
            printf("\npress ENTER to continue\n");
            cin.get();
            while(N) {
                string x, fd, fr;
                int l, r;
                if(N->level > 0) {
                    if(!N->rgt) { fr = "0"; }
                    else { fr = N->rgt->label; }
                    if(!N->dwn) { fd = "0"; }
                    else { fd = N->dwn->label; }
                    N->label = hash(N->level, N->rank, fd, fr);
                }
                else {
                    if(!N->rgt) { fr = "0"; }
                    else { fr = N->rgt->label; }
                    N->label = hash(N->level, N->rank, block_i, fr);
                }
                temp = N->prev;
                N->prev = NULL;
                N = temp;
            }
            N = NULL;
            printf("\npress ENTER to continue\n");
            cin.get();
            while(C) {
                string x, fd, fr;
                int l, r;
                if(C->level > 0) {
                    if(!C->rgt) { fr = "0"; }
                    else { fr = C->rgt->label; }
                    if(!C->dwn) { fd = "0"; }
                    else { fd = C->dwn->label; }
                    C->label = hash(C->level, C->rank, fd, fr);
                }
                else {
                    if(!C->rgt) { fr = "0"; }
                    else { fr = C->rgt->label; }
                    C->label = hash(C->level, C->rank, C->label, fr);
                }
                temp = C->prev;
                C->prev = NULL;
                C = temp;
            }
            C = NULL;



            if(N)
                printf("true\n");
            // printf("\npress ENTER to continue\n");
            // cin.get();
        }
        
//************************ PRINT ***********************************************************
// Print the path to get to every data node
        void print() {
            int size = root->rank;
            for(int i = 1; i <= size; ++i) {
                search(i);
            }
        }
//************************** SEARCH *********************************************************
        // search for a specific data node
        void search(int block_i) {
            Node* curr = root;
            int start_rank = root->rank;
            bool block_i_found = false;

            printf("Searching for block: %d\n", block_i);
            printf("  from: %d level: %d rank: %d\n", curr->data, curr->level, curr->rank);

            /* POSSIBLE PARAMETERS TO USE */
            if(start_rank < block_i) {
                block_i_found = false;
            }
            else {
                while(curr) {    
                    if(curr->rgt && (start_rank - curr->rgt->rank + 1) == block_i) {
                        start_rank = block_i;
                        curr = curr->rgt;
                        printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                        while(curr->level > 0) {
                            curr = curr->dwn;
                            printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                        }
                        block_i_found = true; 
                        break;
                    }
                    else if(curr->rgt && (start_rank - curr->rgt->rank) >= block_i ){
                        start_rank -= curr->rgt->rank;
                        curr = curr->dwn;
                        printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                    }
                    else if(curr->rgt && (start_rank - curr->rgt->rank) < block_i ){
                        curr = curr->rgt;
                        printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                    }
                    else {
                        curr = curr->dwn;
                        printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                    }
                }
            }
            if(block_i_found)
                printf("Block %d has been found!\n\n", block_i);
            else
                printf("Block %d has not been found!\n\n", block_i);
            
        }
//********************************* Algorithm 1: atRank() **************************************************
// THIS ALGORITHM IS ON THE SERVER SIDE *** ONCE COMPLETE IT SAVES THE TUPLE AND TAU VALUES
// Client wants to verify the integrity of block 'i'
        list <tuple> atRank(int block_i) {
            /*  TUPLES
                get<j>(A) - used to access the tuple values and modify them. The index 'j'
                            refers to each individual data for each tuple: 
                A = make_tuple(int, int, bool, string);
                tup1.swap(tup2) - swaps the elements of 2 different tuples
            */
            // Using Tuples: A(v) = (l(v), q(v), d(v), g(v))
            list <tuple> Tuple_List;
            int l;      // l(v) = level of node 'v'
            int q;      // q(v) = rank of successor of node 'v'
            bool d;     // d(v) = if previous node is to the right (true) or below (false) 'v'
            string g;   // g(v) = label of successor of node 'v'
            Node* curr = root;
            int start_rank = root->rank;
            bool block_i_found = false;

            printf("Searching for block: %d\n", block_i);
            printf("  from: %d level: %d rank: %d\n", curr->data, curr->level, curr->rank);

            if(start_rank < block_i) {
                block_i_found = false;
            }
            else {
                while(curr) {    
                    tuple A; // temporary tuple to gather data
                    l = curr->level; // l = current level of node 'v'
                    if(curr->rgt && (start_rank - curr->rgt->rank + 1) == block_i) {
                        start_rank = block_i;
                        d = true;               // previous node is right of node 'v'
                        if(curr->dwn) {
                            q = curr->dwn->rank;    // rank of successor node
                            g = curr->dwn->label;   // label of successor node 'v'
                        }
                        else {
                            q = 1;              // if level = 0, then q = 1
                            g = curr->label;    // if level = 0, then g = current label
                        }

                        A = make_tuple(l, q, d, g);     // make a tuple
                        Tuple_List.push_back(A);        // add tuple to front of list

                        curr = curr->rgt;
                        printf("1    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                        while(curr) {
                            if(curr->rgt)
                                q = curr->rgt->rank;// if no successor, q = current rank
                            else
                                q = curr->rank;
                            l = curr->level;        // l = current level of node 'v'
                            d = false;              // previous node is below node 'v'
                            g = curr->label;        // if no successor, then g = current label
                            if(curr->level == 0) { 
                                Tau = g; 
                                g = "0";
                            }

                            A = make_tuple(l, q, d, g);     // make a tuple
                            Tuple_List.push_back(A);        // add tuple to front of list

                            curr = curr->dwn;
                            if(curr)
                                printf("2    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                        }
                        block_i_found = true; 
                        break;
                    }
                    else if(curr->rgt && (start_rank - curr->rgt->rank) >= block_i ){
                        start_rank -= curr->rgt->rank;
                        d = false;              // previous node is below node
                        q = curr->rgt->rank;    // q = rank of successor node
                        g = curr->rgt->label;   // g = label of successor node
                        curr = curr->dwn;
                        if(curr)
                            printf("3    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                    }
                    else if(curr->rgt && (start_rank - curr->rgt->rank) < block_i ){
                        d = true;               // previous node is right of node 'v'
                        q = curr->dwn->rank;    // q = rank of successor node
                        g = curr->dwn->label;   // g = label of successor node
                        curr = curr->rgt;
                        if(curr)
                            printf("4    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                    }
                    else {
                        d = false;          // previous node is below node 'v'
                        q = curr->rank;     // if no successor, then q = current rank
                        g = curr->label;    // if no successor, then g = current label
                        curr = curr->dwn;
                        if(curr)
                            printf("5    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                    }
                    A = make_tuple(l, q, d, g);    // make a tuple
                    Tuple_List.push_back(A);       // add tuple to front of list
                }
            }

            /* Output message if block_i was found */
            if(block_i_found)
                printf("Block %d has been found!\n\n", block_i);
            else
                printf("Block %d has not been found!\n\n", block_i);

            return Tuple_List;
        }  
};

//********************************* Algorithm 2 **************************************************
// THIS ALGORITHM IS ON THE CLIENTS SIDE
/* 
    int lambda (λ) = l(v), the level of vj
    int rho (ρ) = r(v), the rank of vj
    bool delta (δ), indicates whether the previous node v(j-1) is to the right or below vj
    hash_value gamma (γ) = f(v), the label of vj
    int xi (ξ) = the sum of the ranks of all the nodes that are to the right of the nodes of
            the path seen so far, but are no on the path

    If T is the correct representation of block i and sequence pi of length k is the correct 
    proof for T, then the following properties hold for the values computed in iteration k 
    of the for-loop of Algorithm 2:

    1) Value ρk is equal to the number of nodes at the bottom level of the skip list, that is,
    the number n of blocks of the file;
    2) Value ξk is equal to n− i; and
    3) Value γk is equal to the label of the start node of the skip list.

*************************************
    For now :
    We will work with block_i = 7 

    Metadata for block_7 => Mc = b24
    Tau for block_7 => T = b24
*/
bool verify(int block_i, string Mc, string Tau, list <tuple>& Tuple_List) {
    //printf("error 1\n");
    int lambda = 0; 
    int rho = 1;
    int xi = 0;
    string gamma = Tau;

    list <tuple>::iterator it;  // initialize an iterator to traverse the list
    it = Tuple_List.begin();
    tuple B = *it;              // holds value of previous 'it'
    ++it;                       // start loop at 2nd element in list

    while(it != Tuple_List.end()) {
        tuple A = *it; // holds value of current 'it'
        // Using Tuples: A(v) = (l(v), q(v), d(v), g(v))
        lambda = get<0>(A);
        rho = get<1>(B) + get<1>(A);
        if(get<2>(A)) {
            gamma = get<3>(A);
            xi = get<1>(B);
        }
        else {
            gamma = get<3>(A);
            xi = get<1>(B) + get<1>(A);
        }
        //cout << gamma << endl;
        B = *it;
        ++it;
    }
    //printf("error 3\n");
    bool result = true;
    if(gamma != Mc) 
        result = false;
    else if(rho - xi != block_i)
        result = false;
    else if(gamma == Mc && rho - xi == block_i)
        result = true;
    
    //printf("error 4\n");
    return result;
}

//******************** Verify the integrity of a data block ***********************
    // algorithm 1: atRank() happens in skipList.h
    // algorithm 2: verify() happens in skipList.h
    // getT() happens in skipList.h
bool verify_block_i_integrity(skipList skip_list, string client_Mc, int block_i) {
    list <tuple> Tuple_List = skip_list.atRank(block_i); // call server to run atRank function
    string Tau = skip_list.getT();  // get label that server side says will match clients metadata

    // verify data received from server side on clients side
    bool verify_data = verify(block_i, client_Mc, Tau, Tuple_List);
    return verify_data;
}

//******************** Get Hash Function ***********************
int letitgo = 1;
string hash(int lvl, int rank, string str1 = "0", string str2 = "0") {
    SHA1 sha1;
    stringstream ss1,ss2;
    ss1 << lvl;
    ss2 << rank;
    string here = sha1(sha1(str2) + sha1(ss1.str()) + sha1(ss2.str()) + sha1(str1));
    cout << letitgo << ": " << here << endl;
    
    return here;
}






