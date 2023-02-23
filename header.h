#include <iostream>
#include <stdio.h>
#include <tuple>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;

#include "sha1.h"

#define tuple tuple<int, int, bool, string>
unsigned long long int hash_number = 1;

class skipList {
    private:
        struct Node {
            int rank;  // how many data nodes the current node can reach
            int level; // highest level of data node
            int data;
            string label; // hash value for every node
            Node *rgt; // rgt points to the next data. This varies depending on the level
            Node *dwn; // dwn points to the nodes next level down in the column.
            Node *prev; // used during insert/delete to reset values: hash, rank, etc.
            Node(int value, int lvl) {
                rgt = NULL;
                dwn = NULL;
                prev = NULL;
                rank = 1;
                data = value;
                level = lvl;
                label = "0";
            }
        };

        int MAXLVL;// Maximum level for this skip list
        // P is the fraction of the nodes with level
        // i pointers also having level i+1 pointers
        float P;
        string Tau; // represents Tau of block_i
        Node *root; // pointer to current node
        Node **envPtr; // environmental pointer


        //******************** Get Hash Function ***********************
        string hashing(int lvl, int rank, string str1 = "0", string str2 = "0") {
            SHA1 sha1;
            stringstream ss1,ss2;
            ss1 << lvl;
            ss2 << rank;
            return sha1(sha1(str2) + sha1(ss1.str()) + sha1(ss2.str()) + sha1(str1));
        }
        // return value for Tau
        
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
                (*envPtr)->label = hashing(0, 0, "root", "0");
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
            N->prev = NULL;
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
        Node* createNode(int value, /* DELETE WHEN DONE */int lvl = -1) {
            if(lvl == -1)
                int lvl = randomLevel();
            Node *n = new Node(value, lvl);
            Node* temp = n;
            // create a linked list coloumn of length 'lvl'
            for(int i = lvl-1; i >=0; --i) {
                temp->dwn = new Node(value, i);
                temp = temp->dwn;
            }
            return n;
        }
        string getT() { 
            return Tau; 
        }
        string getRL() { 
            return root->label; 
        }
//********************* INSERT NEW NODE **************************************************************
        void insertElement(int value, int block_i, /* DELETE WHEN DONE */ int lvl) {
            Node *N = createNode(value, /* DELETE WHEN DONE */ lvl); // New node being inserted
            Node *C = root;         // Current Node from root
            Node *hold_C = NULL;
            Node *hold_N = NULL;
            int temp = N->level;    // Holds the highest level for new node
            int rank = root->rank;  // Used to find block_i

            printf("***** Insert New Element: %d at block %d *****\n", N->data, block_i);
            while(C) {
                if(C->level == temp) {
                    if(C->rgt && ((rank - C->rgt->rank + 1) >= block_i || (rank - C->rgt->rank) == block_i)) {
                        rank -= C->rgt->rank;

                        Node* temp = C->rgt; // Swap right pointers
                        C->rgt = N;
                        N->rgt = temp;

                        if(C->level > 0) {
                            C->dwn->prev = C;
                            N->dwn->prev = N;
                        }
                        
                        if(!C->dwn) { hold_C = C; hold_N = N; }
                        N = N->dwn; // move to N's next level down
                        C = C->dwn; // move to C's next node down
                        
                    }
                    else if(C->rgt && (rank - C->rgt->rank) < block_i) {
                        C->rgt->prev = C;
                        C = C->rgt;
                    }
                    else {
                        C->rgt = N;

                        if(C->level > 0) {
                            C->dwn->prev = C;
                            N->dwn->prev = N;
                        }
                        
                        if(!C->dwn) { hold_C = C; hold_N = N; }
                        N = N->dwn; // move to N's next level down
                        C = C->dwn; // move to C's next node down
                    }
                }
                else if(C->level > temp) {
                    if(C->rgt && (rank - C->rgt->rank) >= block_i) {
                        rank -= C->rgt->rank;
                        C->dwn->prev = C;
                        C = C->dwn;
                    }
                    else if(C->rgt && (rank - C->rgt->rank + 1) < block_i) {
                        C->rgt->prev = C;
                        C = C->rgt;
                    }
                    else {
                        if(C->rgt)
                            rank -= C->rgt->rank;

                        C->dwn->prev = C;
                        C = C->dwn;
                    }
                }
                else if(C->level < temp) {
                    if(C->rgt && (rank - C->rgt->rank + 1) >= block_i) {
                        rank -= C->rgt->rank;
                        N->rgt = C->rgt;
                        C->rank -= C->rgt->rank;
                        C->rgt = NULL;

                        if(C->level > 0) {
                            C->dwn->prev = C;
                            N->dwn->prev = N;
                        }

                        if(!C->dwn) { hold_C = C; hold_N = N; }
                        N = N->dwn; // move to N's next level down
                        C = C->dwn; // move to C's next node down
                    }
                    else if(C->rgt && (rank - C->rgt->rank + 1) < block_i) {
                        C->rgt->prev = C;
                        C = C->rgt;
                    }
                    else {
                        if(C->level > 0) {
                            C->dwn->prev = C;
                            N->dwn->prev = N;
                        }

                        if(!C->dwn) { hold_C = C; hold_N = N; }
                        N = N->dwn; // move to N's next level down
                        C = C->dwn; // move to C's next node down
                    }
                }
            }
            // reset hash values along traversed path
            setHashandRank(hold_N, hold_C);
        }
//********************* INSERT NEW NODE **************************************************************
        void deleteElement(int block_i) {
            Node *curr = root;          // Current Node from root
            Node *dele = NULL;          // traverses the node to be delete
            Node *hold_curr = NULL;     // gets passed to setHashandRank() function
            Node *top = NULL;           // holds top of node to be deleted
            int rank = root->rank;      // used to find block_i
            bool block_i_found = false;

            printf("***** Delete Block number: %d\n", block_i);

            if(rank < block_i) {
                block_i_found = false;
            }
            else {
                while(curr) {
                    if(curr->rgt && (rank - curr->rgt->rank + 1) == block_i) {
                        dele = curr->rgt;
                        curr->rgt = NULL;
                        top = dele; // used to delete element at end of function
                            
                        while(curr) {
                            if(curr->rgt) {
                                curr->rgt->prev = curr;
                                curr = curr->rgt;
                            }
                            else if(!curr->rgt && dele->rgt && curr->level == dele->level) {
                                curr->rgt = dele->rgt;
                                dele->rgt = NULL;
                                
                                if(curr->level > 0) { curr->dwn->prev = curr; }
                                if(!curr->dwn) { hold_curr = curr; }

                                dele = dele->dwn;
                                curr = curr->dwn;
                            }
                            else {
                                if(curr->level > 0) { curr->dwn->prev = curr; }
                                if(!curr->dwn) { hold_curr = curr; }

                                dele = dele->dwn;
                                curr = curr->dwn;
                            }
                        }

                        block_i_found = true; 
                        break;
                    }
                    else if(curr->rgt && (rank - curr->rgt->rank + 1) < block_i ){
                        if(curr->rgt) { curr->rgt->prev = curr; }
                        curr = curr->rgt;
                    }
                    else if(curr->rgt && (rank - curr->rgt->rank + 1) > block_i) {
                        rank -= curr->rgt->rank;
                        if(curr->dwn) { curr->dwn->prev = curr; }
                        curr = curr->dwn;
                    }
                    else {
                        if(curr->dwn) { curr->dwn->prev = curr; }
                        curr = curr->dwn;
                    }
                }
                // set new hash and rank values in skip list
                setHashandRank(NULL, hold_curr);
                // delete block_i from memory
                del(top);
            }
            if(block_i_found)
                printf("Block %d has been Deleted!\n\n", block_i);
            else
                printf("Block %d does not exist!\n\n", block_i);
        }
//************************ HASH ***********************************************************
// reset hash values along traversed path
        void setHashandRank(Node *hold_N, Node *hold_C) {
            stringstream ss;
            while(hold_N) {
                string fd = "0";
                string fr = "0";
                if(hold_N->level > 0) { 
                    if(hold_N->dwn) { fd = hold_N->dwn->label; }
                    if(hold_N->rgt) { fr = hold_N->rgt->label; }
                    if(hold_N->rgt)
                        hold_N->rank = hold_N->rgt->rank + hold_N->dwn->rank;
                    else
                        hold_N->rank = hold_N->dwn->rank;
                    cout << " level: " << hold_N->level << endl;
                    cout << "  rank: " << hold_N->rank << endl;
                    cout << "f(dwn): " << fd << endl;
                    cout << "f(rgt): " << fr << endl;
                    hold_N->label = hashing(hold_N->level, hold_N->rank, fd, fr);
                    cout << "label: " << hold_N->label << endl << endl;
                }
                else if(hold_N->level == 0) {
                    ss << hash_number; // This will be changed to the metadata eventually
                    ++hash_number;
                    if(hold_N->rgt) { fr = hold_N->rgt->label; }
                    if(hold_N->rgt)
                        hold_N->rank = hold_N->rgt->rank + 1;
                    else
                        hold_N->rank = 1;
                    cout << " level: " << hold_N->level << endl;
                    cout << "  rank: " << hold_N->rank << endl;
                    cout << "f(dwn): " << ss.str() << endl;
                    cout << "f(rgt): " << fr << endl;
                    hold_N->label = hashing(hold_N->level, hold_N->rank, ss.str(), fr);
                    cout << "label: " << hold_N->label << endl << endl;
                    ss.clear();
                }
                Node *tmp = hold_N;
                hold_N = hold_N->prev;
                tmp->prev = NULL;
                tmp = NULL;
            }
            
            while(hold_C) {
                string fd = "0";
                string fr = "0";
                if(hold_C->level > 0) {
                    if(hold_C->dwn) { fd = hold_C->dwn->label; }
                    if(hold_C->rgt) { fr = hold_C->rgt->label; }
                    
                    if(hold_C->rgt) {
                        hold_C->rank = hold_C->rgt->rank + hold_C->dwn->rank;
                    } 
                    else {
                        hold_C->rank = hold_C->dwn->rank;
                    }
                    cout << " level: " << hold_C->level << endl;
                    cout << "  rank: " << hold_C->rank << endl;
                    cout << "f(dwn): " << fd << endl;
                    cout << "f(rgt): " << fr << endl;
                    hold_C->label = hashing(hold_C->level, hold_C->rank, fd, fr);
                    cout << "label: " << hold_C->label << endl << endl;
                }
                else if(hold_C->level == 0) {
                    if(hold_C->rgt) {
                        if(hold_C->data != -1) {
                            hold_C->rank = hold_C->rgt->rank + 1;
                        }
                        else {
                            hold_C->rank = hold_C->rgt->rank;
                            cout << " level: " << hold_C->level << endl;
                            cout << "  rank: " << hold_C->rank << endl;
                            cout << "f(dwn): " << fd << endl;
                            cout << "f(rgt): " << fr << endl;
                            hold_C->label = hashing(hold_C->level, hold_C->rank, fd, hold_C->rgt->label);
                            cout << "label: " << hold_C->label << endl << endl;
                        }
                    }
                    else {
                        if(hold_C->data != -1)
                            hold_C->rank = 1;
                        else
                            hold_C->rank = 0;
                    }
                }
                // printf("data: %d level: %d rank: %d ", hold_C->data, hold_C->level, hold_C->rank);
                // cout << "label: " << hold_C->label << endl;
                Node *tmp = hold_C;
                hold_C = hold_C->prev;
                tmp->prev = NULL;
                tmp = NULL;
            }
        }
        
//************************ PRINT ***********************************************************
// Print the path to get to every data node
        void print() {
            int size = root->rank;
            // cout << "size: " << size << endl;
            for(int i = 1; i <= size; ++i) {
                search(i);
                // printf("Hash Number: %d", hash_number);
                
            }
        }
//************************** SEARCH *********************************************************
        // search for a specific data node
        void search(int block_i) {
            Node* curr = root;
            int start_rank = root->rank;
            bool block_i_found = false;
            
            printf("Searching for block: %d\n", block_i);
            // printf("  from: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
            printf("  from: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
            cout << "label: " << curr->label << endl;
            /* POSSIBLE PARAMETERS TO USE */
            if(start_rank < block_i) {
                block_i_found = false;
            }
            else {
                while(curr) {
                    // if(curr->rgt)
                    //     printf("     start_rank: %d, C->rgt->rank: %d\n", start_rank, curr->rgt->rank);
                    if(curr->rgt && (start_rank - curr->rgt->rank + 1) == block_i) {
                        start_rank = block_i;
                        curr = curr->rgt;
                        // printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                        printf("    to: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
                        cout << "label: " << curr->label << endl;
                        while(curr->level > 0) {
                            curr = curr->dwn;
                            // printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                            printf("    to: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
                            cout << "label: " << curr->label << endl;
                        }
                        block_i_found = true; 
                        // cout << "label:" << curr->label << endl;
                        break;
                    }
                    else if(curr->rgt && (start_rank - curr->rgt->rank) >= block_i ){
                        start_rank -= curr->rgt->rank;
                        curr = curr->dwn;
                        // printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                        printf("    to: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
                        cout << "label: " << curr->label << endl;
                    }
                    else if(curr->rgt && (start_rank - curr->rgt->rank) < block_i ){
                        curr = curr->rgt;
                        // printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                        printf("    to: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
                        cout << "label: " << curr->label << endl;
                    }
                    else {
                        curr = curr->dwn;
                        // printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                        printf("    to: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
                        cout << "label: " << curr->label << endl;
                    }
                }
            }
            if(block_i_found)
                printf("Block %d, Data: %d has been found!\n\n", block_i, curr->data);
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
    Tuple_List.clear();
    int l;      // l(v) = level of node 'v'
    int q;      // q(v) = rank of successor of node 'v'
    bool d;     // d(v) = if previous node is to the right (true) or below (false) 'v'
    string g;   // g(v) = label of successor of node 'v'
    Node* curr = root;
    int start_rank = root->rank;
    bool block_i_found = false;

    printf("Searching for block: %d\n", block_i);
    printf("  from: %d level: %d rank: %d", curr->data, curr->level, curr->rank);
    cout << "label: " << curr->label << endl;
    if(start_rank < block_i) {
        block_i_found = false;
    }
    else {
        while(curr) {
            tuple A; // temporary tuple to gather data
            l = curr->level; // l = current level of node 'v'
            if(curr->rgt && (start_rank - curr->rgt->rank + 1) == block_i) {
                start_rank = block_i;
                d = true;           // previous node is right of node 'v'
                if(curr->dwn) {
                    q = curr->dwn->rank;    // rank of successor node
                    g = curr->dwn->label;   // label of successor node 'v'
                }
                else {
                    q = 1;       // we're at level 0, so q = 1
                    g = "0";     // we're at level 0, so g = "0"
                }

                curr = curr->rgt;
                //printf("1    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                cout << "label: " << curr->label << endl;
                cout << "g: " << g << endl;
                cout << "q: " << q << endl;
                cout << "d: " << d << endl;
                cout << "l: " << l << endl << endl;
                printf("1    to: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
                cout << "label: " << curr->label << endl;

                A = make_tuple(l, q, d, g);     // make a tuple
                Tuple_List.push_front(A);        // add tuple to front of list

                while(curr) {
                    if(curr->rgt) {
                        q = curr->rgt->rank; // if successor, then q = successor rank
                        g = curr->rgt->label; // if successor, then g = successor label
                    } 
                    else {
                        q = 0;  // if no successor, then q = 0
                        g = "0"; // if no successor, then g = "0"
                    }
                        
                    l = curr->level;        // l = current level of node 'v'
                    d = false;              // previous node is below node 'v'
                    
                    if(curr->level == 0) { 
                        Tau = curr->label;
                        q = 0;
                        g = "0";
                    }

                    cout << "g: " << g << endl;
                    cout << "q: " << q << endl;
                    cout << "d: " << d << endl;
                    cout << "l: " << l << endl << endl;
                    curr = curr->dwn;
                    if(curr) {
                        //printf("2    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                        printf("2    to: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
                        cout << "label: " << curr->label << endl;
                    }
                    
                    
                    A = make_tuple(l, q, d, g);     // make a tuple
                    Tuple_List.push_front(A);        // add tuple to front of list
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
                if(curr) {
                    //printf("3    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                    printf("3    to: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
                    cout << "label: " << curr->label << endl;
                }
            }
            else if(curr->rgt && (start_rank - curr->rgt->rank) < block_i ){
                d = true;               // previous node is right of node 'v'
                q = curr->dwn->rank;    // q = rank of successor node
                g = curr->dwn->label;   // g = label of successor node
                curr = curr->rgt;
                if(curr) {
                    //printf("4    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                    printf("4    to: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
                    cout << "label: " << curr->label << endl;
                }  
            }
            else {
                d = false;          // previous node is below node 'v'
                q = 0;             // if no successor, then q = 0
                g = "0"; //curr->label;    // if no successor, then g = "0"
                curr = curr->dwn;
                if(curr) {
                    //printf("5    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
                    printf("5    to: %d level: %d rank: %d, ", curr->data, curr->level, curr->rank);
                    cout << "label: " << curr->label << endl;
                }      
            }
            cout << "g: " << g << endl;
            cout << "q: " << q << endl;
            cout << "d: " << d << endl;
            cout << "l: " << l << endl << endl;
            cout << "***************" << endl;
            
            A = make_tuple(l, q, d, g);    // make a tuple
            Tuple_List.push_front(A);       // add tuple to front of list
        }
    }

    /* Output message if block_i was found */
    if(block_i_found)
        printf("Block %d has been found!\n\n", block_i);
    else
        printf("Block %d has not been found!\n\n", block_i);

            // // Using Tuples: A(v) = (l(v), q(v), d(v), g(v))
            // list <tuple> Tuple_List;
            // Tuple_List.clear();
            // int l;      // l(v) = level of node 'v'
            // int q;      // q(v) = rank of successor of node 'v'
            // bool d;     // d(v) = if previous node is to the right (true) or below (false) 'v'
            // string g;   // g(v) = label of successor of node 'v'
            // Node* curr = root;
            // int start_rank = root->rank;
            // bool block_i_found = false;

            // if(start_rank < block_i) {
            //     block_i_found = false;
            // }
            // else {
            //     while(curr) {
            //         tuple A; // temporary tuple to gather data
            //         l = curr->level; // l = current level of node 'v'
            //         if(curr->rgt && (start_rank - curr->rgt->rank + 1) == block_i) {
            //             start_rank = block_i;
            //             d = true;           // previous node is right of node 'v'
            //             if(curr->dwn) {
            //                 q = curr->dwn->rank;    // rank of successor node
            //                 g = curr->dwn->label;   // label of successor node 'v'
            //             }
            //             else {
            //                 q = 1;       // we're at level 0, so q = 1
            //                 g = "0";     // we're at level 0, so g = "0"
            //             }

            //             curr = curr->rgt;
            //             A = make_tuple(l, q, d, g);     // make a tuple
            //             Tuple_List.push_front(A);        // add tuple to front of list

            //             while(curr) {
            //                 if(curr->rgt) {
            //                     q = curr->rgt->rank; // if successor, then q = successor rank
            //                     g = curr->rgt->label; // if successor, then g = successor label
            //                 } 
            //                 else {
            //                     q = 0;  // if no successor, then q = 0
            //                     g = "0"; // if no successor, then g = "0"
            //                 }
                                
            //                 l = curr->level;        // l = current level of node 'v'
            //                 d = false;              // previous node is below node 'v'
                            
            //                 if(curr->level == 0) { 
            //                     Tau = curr->label;
            //                     q = 0;
            //                     g = "0";
            //                 }

            //                 curr = curr->dwn;         
            //                 A = make_tuple(l, q, d, g);     // make a tuple
            //                 Tuple_List.push_front(A);        // add tuple to front of list
            //             }
            //             block_i_found = true;
            //             break;
            //         }
            //         else if(curr->rgt && (start_rank - curr->rgt->rank) >= block_i ){
            //             start_rank -= curr->rgt->rank;
            //             d = false;              // previous node is below node
            //             q = curr->rgt->rank;    // q = rank of successor node
            //             g = curr->rgt->label;   // g = label of successor node
            //             curr = curr->dwn;
            //         }
            //         else if(curr->rgt && (start_rank - curr->rgt->rank) < block_i ){
            //             d = true;               // previous node is right of node 'v'
            //             q = curr->dwn->rank;    // q = rank of successor node
            //             g = curr->dwn->label;   // g = label of successor node
            //             curr = curr->rgt;
            //         }
            //         else {
            //             d = false;         // previous node is below node 'v'
            //             q = 0;             // if no successor, then q = 0
            //             g = "0";           // if no successor, then g = "0"
            //             curr = curr->dwn;    
            //         }
            //         A = make_tuple(l, q, d, g);    // make a tuple
            //         Tuple_List.push_front(A);       // add tuple to front of list
            //     }
            // }
        
            return Tuple_List;
        }
//********************************* Algorithm 2 **************************************************
//********************************* VERIFY *******************************************************
// THIS ALGORITHM IS ON THE CLIENTS SIDE
        bool verify(int block_i, string Mc, string Tau, list <tuple>& Tuple_List) {
            int lambda = 0;     // get<0>(A), lambda (λ) = l(v), the level of vj
    int rho = 1;        // get<1>(A), rho (ρ) = r(v), rank of successor of node 'v'
    bool delta = 0;     // get<2>(A), delta (δ), indicates whether the previous node v(j-1) is to the right or below vj
    string gamma = Tau; // get<3>(A), label of successor of node 'v'
    int xi = 0;         // xi (ξ) = sum of all node ranks to the right that are not on the path

    list <tuple>::iterator it;  // initialize an iterator to traverse the list
    it = Tuple_List.begin();
    // tuple A = *it;              // holds value of previous 'it'
    ++it;                          // start loop at 2nd element in list
    
    /* tuple A is as element 'j' */
    /* tuple B is as element 'j-1' */
    /* A(v) = (l(v) level, q(v) rank, d(v) rgt(1)/dwn(0), g(v) label) */ 
    while(it != Tuple_List.end()) {
        tuple A = *it; // holds value of current 'it'
        lambda = get<0>(A);
        delta = get<2>(A);
        printf("rho = %d + %d\n", rho, get<1>(A));
        rho = rho + get<1>(A);

        cout << "lambda: " << lambda << endl;
        cout << "rho: " << rho << endl;
        cout << "delta: " << delta << endl;

        // f(v) = h( l(v), r(v), f(dwn(v)), f(rgt(v)) )
        if(delta) {
            cout << "if:\n";
            cout << "get<3>(A), g: " << get<3>(A) << endl;
            cout << "  curr gamma: " << gamma << endl;
            gamma = hashing(lambda, rho, get<3>(A), gamma);
            xi = xi;
            cout << "  after hash: " << gamma << endl;
        }
        else {
            cout << "else:\n";
            cout << "get<3>(A), g: " << get<3>(A) << endl;
            cout << "  curr gamma: " << gamma << endl;
            gamma = hashing(lambda, rho, gamma, get<3>(A));
            xi = xi + get<1>(A);
            cout << "xi = xi + get<1>(A) = " << xi << endl;
            cout << "  after hash: " << gamma << endl;
        }
        cout << endl;
        // B = *it;
        ++it;
    }
    cout << "xi = " << xi << endl;
    cout << "block_i = " << block_i << endl;
    cout << "gamma = " << gamma << endl;
    cout << "Mc = " << Mc << endl;
    cout << "rho = " << rho << endl << endl;
    bool result = true;
    if(gamma != Mc) {
        cout << "gamma != Mc" << endl;
        result = false;
    }
    if(rho - xi != block_i) {
        cout << "rho - xi != block_i, " << rho - xi << endl;
        result = false;
    }
    if(gamma == Mc && rho - xi == block_i) {
        cout << "gamma == Mc && rho - xi == block_i" << endl;
        result = true;
    }
            // int lambda = 0;     // get<0>(A), lambda (λ) = l(v), the level of vj
            // int rho = 1;        // get<1>(A), rho (ρ) = r(v), rank of successor of node 'v'
            // bool delta = 0;     // get<2>(A), delta (δ), indicates whether the previous node v(j-1) is to the right or below vj
            // string gamma = Tau; // get<3>(A), label of successor of node 'v'
            // int xi = 0;         // xi (ξ) = sum of all node ranks to the right that are not on the path

            // list <tuple>::iterator it;  // initialize an iterator to traverse the list
            // it = Tuple_List.begin();
            // ++it;                       // start loop at 2nd element in list
            
            // int block = 1; cout << endl;
            // /* A(v) = (l(v) level, q(v) rank, d(v) rgt(1)/dwn(0), g(v) label) */ 
            // while(it != Tuple_List.end()) {
            //     tuple A = *it; // holds value of current 'it'
            //     lambda = get<0>(A);
            //     delta = get<2>(A);
            //     rho = rho + get<1>(A);
                
            //     // f(v) = h( l(v), r(v), f(dwn(v)), f(rgt(v)) )
            //     if(delta) {
            //         gamma = hashing(lambda, rho, get<3>(A), gamma);
            //         xi = xi;
            //     }
            //     else {
            //         gamma = hashing(lambda, rho, gamma, get<3>(A));
            //         xi = xi + get<1>(A);
            //     }
            //     ++it;
            // }
            // bool result = true;
            // if(gamma != Mc) {
            //     result = false;
            // }
            // if(rho - xi != block_i) {
            //     result = false;
            // }
            // if(gamma == Mc && rho - xi == block_i) {
            //     cout << "gamma == Mc && rho - xi == block_i" << endl;
            //     result = true;
            // }
            return result;
        }

//********************************* Algorithm 3 **************************************************
//******************************* PERFORM UPDATE *************************************************
// possible updates include: insertion, deleteion, & modification of block_i
        /*
            update == i, then insertion
            update == d, then deletion
            update == m, then modification

            For insertion or modification:
            1) Create a temporary skip list to copy the current one, peform an insertion of block_i 
            2) use atRank(i) to get T of block_i and its proof A'
            For deletion:
            2) use atRank(i-1) to get T of block_i and its proof A'
            3) the client uses verify() to compute what would be the label of the start node
            after the update.
            4) the server performs the update on the skip list
        */
//******************************* COMPUTE LABEL *************************************************
// computes what would be the label ofthe start node after an insertion
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
        cout << "max height: " << maxHeight << endl;
        cout << "   lambda1: " << lambda1 << ", lambda2: " << lambda2 << endl;
        cout << "      rho1: " << rho1 << ", rho2: " << rho2 <<endl;
        cout << " get<1>(A): " << get<1>(A) << endl;
        cout << " get<3>(A): " << get<3>(A) << endl;
        cout << "   Bgamma1: " << gamma1 << endl;
        cout << "   Bgamma2: " << gamma2 << endl;
        if(lambda1 < maxHeight) {
            printf("part 1\n");
            if(lambda1 > lambda2) {
                printf(" 1.1\n");
                gamma2 = hashing(lambda2, rho2, gamma2, "0"); // normal hashing for gamma2
                ++lambda2;
            }
            else if(delta1) {// vj previous node is right
                printf(" 1.2\n");
                gamma1 = hashing(lambda1, rho1, get<3>(A), gamma1); // gamma1 gets rehashed
            }
            else if(!delta1) { // vj previous node is down
                printf(" 1.3\n");
                if((get<1>(A)) == 0) { // no right pointer
                    printf("  1.3.1\n");
                    if(lambda1 != 0) {
                        gamma2 = hashing(lambda2, rho2, gamma2, "0"); // normal hashing for gamma2
                        gamma1 = hashing(lambda1, rho1, gamma1, "0");// gamma1 gets rehashed
                    }
                    ++lambda2;
                } 
                else { // right pointer
                    printf("  1.3.2\n");
                    rho2 = rho2 + get<1>(A); // incerase the rank for new hash label
                    gamma2 = hashing(lambda2, rho2, gamma2, get<3>(A)); // gamma2 gets hashed with gamma1
                    ++lambda2;
                    rho1 = rho1 - get<1>(A); // reduce the rank for new hash label
                    gamma1 = hashing(lambda1, rho1, gamma1, "0"); // gamma1 loses right pointer and gets rehashed
                }
            }
        }
        else if(lambda1 >= maxHeight) {
            printf("part 2\n");
            if(mergePoint) {
                printf(" 2.1\n");
                if(delta1) { // vj previous node is right
                    printf("  2.1.1\n");
                    gamma1 = hashing(lambda1, rho1, get<3>(A), gamma1); // gamma1 gets rehashed
                    ++lambda1;
                }
                else { // vj previous node is down
                    printf("  2.1.2\n");
                    gamma1 = hashing(lambda1, rho1, gamma1, get<3>(A));// gamma1 gets rehashed
                    ++lambda1;
                }
            }
            else {
                printf(" 2.2\n");
                if(delta1) { // vj previous node is right
                    printf("  2.2.1\n");
                    rho2 = rho2 + get<1>(A); // incerase the rank for new hash label
                    gamma2 = hashing(lambda2, rho2, gamma2, get<3>(A)); // gamma2 gets hashed with gamma1
                    gamma1 = hashing(lambda1, rho1, gamma1, gamma2); // gamma1 gets rehashed
                    ++lambda1;
                }
                else { // vj previous node is down
                    printf("  2.2.2\n");
                    rho2 = rho2 + get<1>(A); // incerase the rank for new hash label
                    cout << "****************************************************" << endl;
                cout << "lambda2: " << lambda2 << endl;
                cout << "rho2: " << rho2 << endl;
                cout << "get<3>(A): " << get<3>(A) << endl;
                cout << "gamma2: " << gamma2 << endl << endl;
                    gamma2 = hashing(lambda2, rho2, gamma2, get<3>(A)); // normal hashing for gamma2
                cout << "gamma2: " << gamma2 << endl;
                cout << "****************************************************" << endl;
                    rho1 = rho1 + rho2;
                cout << "lambda1: " << lambda1 << endl;
                cout << "rho1: " << rho1 << endl;
                cout << "gamma1: " << gamma1 << endl;
                cout << "gamma2: " << gamma2 << endl << endl;
                    gamma1 = hashing(lambda1, rho1, gamma1, gamma2);// gamma1 gets rehashed
                cout << "gamma1: " << gamma1 << endl;
                cout << "****************************************************" << endl;
                    ++lambda1;
                }
                mergePoint = true; // merge point was found
            }
        }
        cout << "   Agamma1: " << gamma1 << endl;
        cout << "   Agamma2: " << gamma2 << endl << endl;
        ++it;
    }
            // int lambda1 = 0;     // get<0>(A), lambda (λ) = l(v), the level of vj
            // int rho1 = 1;        // get<1>(A), rho (ρ) = r(v), rank of successor of node 'v'
            // // get<2>(A), delta (δ), if previous node v(j-1) is to the right (true) or below (false) v(j)
            // bool delta1 = 0;     
            // string gamma1 = Tau; // get<3>(A), label of successor of node 'v'
            // bool mergePoint = false; // determines if we found the point that new data block merges into list

            // // values for new data block at lvl == 0
            // int lambda2 = 0;
            // int rho2 = 1;
            // bool delta2 = 0;
            // stringstream ss;
            // ss << hash_number; // This will be changed to the metadata eventually
            // string gamma2 = hashing(lambda2, rho2, ss.str(), "0");     

            // list <tuple>::iterator it;  // initialize an iterator to traverse the list
            // it = Tuple_List.begin();
            
            // /* A(v) = (l(v) level, q(v) rank, d(v) rgt(1)/dwn(0), g(v) label) */ 
            // while(it != Tuple_List.end()) {
            //     tuple A = *it; // holds value of current 'it'
            //     lambda1 = get<0>(A);
            //     delta1 = get<2>(A);
            //     rho1 = rho1 + get<1>(A);

            //     if(lambda1 < maxHeight) {
            //         if(lambda1 > lambda2) {
            //             gamma2 = hashing(lambda2, rho2, gamma2, "0"); // normal hashing for gamma2
            //             ++lambda2;
            //         }
            //         else if(delta1) {// vj previous node is right
            //             gamma1 = hashing(lambda1, rho1, get<3>(A), gamma1); // gamma1 gets rehashed
            //         }
            //         else if(!delta1) { // vj previous node is down
            //             if((get<1>(A)) == 0) { // no right pointer
            //                 if(lambda1 != 0) {
            //                     gamma2 = hashing(lambda2, rho2, gamma2, "0"); // normal hashing for gamma2
            //                     gamma1 = hashing(lambda1, rho1, gamma1, "0");// gamma1 gets rehashed
            //                 }
            //                 ++lambda2;
            //             } 
            //             else { // right pointer
            //                 rho2 = rho2 + get<1>(A); // incerase the rank for new hash label
            //                 gamma2 = hashing(lambda2, rho2, gamma2, get<3>(A)); // gamma2 gets hashed with gamma1
            //                 ++lambda2;
            //                 rho1 = rho1 - get<1>(A); // reduce the rank for new hash label
            //                 gamma1 = hashing(lambda1, rho1, gamma1, "0"); // gamma1 loses right pointer and gets rehashed
            //             }
            //         }
            //     }
            //     else if(lambda1 >= maxHeight) {
            //         if(mergePoint) {
            //             if(delta1) { // vj previous node is right
            //                 ++lambda1;
            //                 gamma1 = hashing(lambda1, rho1, get<3>(A), gamma1); // gamma1 gets rehashed
            //             }
            //             else { // vj previous node is down
            //                 ++lambda1;
            //                 gamma1 = hashing(lambda1, rho1, gamma1, get<3>(A));// gamma1 gets rehashed
            //             }
            //         }
            //         else {
            //             if(delta1) { // vj previous node is right
            //                 rho2 = rho2 + get<1>(A); // incerase the rank for new hash label
            //                 gamma2 = hashing(lambda2, rho2, gamma2, get<3>(A)); // gamma2 gets hashed with gamma1
            //                 gamma1 = hashing(lambda1, rho1, gamma1, gamma2); // gamma1 gets rehashed
            //                 ++lambda1;
            //             }
            //             else { // vj previous node is down
            //                 rho2 = rho2 + get<1>(A); // incerase the rank for new hash label
            //                 gamma2 = hashing(lambda2, rho2, gamma2, get<3>(A)); // normal hashing for gamma2
            //                 ++rho1;
            //                 gamma1 = hashing(lambda1, rho1, gamma1, gamma2);// gamma1 gets rehashed
            //                 ++lambda1;
            //             }
            //             mergePoint = true; // merge point was found
            //         }
            //     }
            //     ++it;
            // }
            return gamma1;
        }
        
        string root_label() {
            return root->label;
        }

};










