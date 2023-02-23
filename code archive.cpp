/*  ORDER OF ALGORITHMS IN THIS FILE

    insert()        line 15
    delete()        line 576
    search()        line 849
    atRank()        line 945
    print()         line 1252
    verify()        line 1284
    setHashandRank()line 1372
    computeLabel()  line 1440
*/




/********************************************************************************/
/************************* INSERT ALGORITHM *************************************/
//******** TRY #1 *****
if(C->rgt && C->rgt->data < N->data && C->rgt->level >= temp) {// part 1
    ++C->rank;
    C = C->rgt;
}
/*
    If a node points to another node to the right, it should
    only be pointed to the top node of that column. This 
    prevents a column from pointing to another column more
    than once.
*/
else if(C->level == temp) {// part 2
    Node* temp = C->rgt; // Swap right pointers
    C->rgt = N;
    N->rgt = temp;
    ++C->rank;

    if(N->rgt != NULL)
        N->rank += N->rgt->rank; // Increment rank by value of right pointers rank
    N = N->dwn; // move to N's next level down
    C = C->dwn; // move to next node down
}
else if(C->level == N->level && N->level < temp) {// part 3
    if(C->rgt && C->rgt->level >= temp) {
        N->rgt = C->rgt;
        C->rank -= C->rgt->rank; // Decrement rank by value of right pointers rank
        N->rank += N->rgt->rank; // Increment rank by value of right pointers rank
        C->rgt = NULL;
    }
    N = N->dwn; // move to N's next level down
    C = C->dwn; // move to next node down
}
// part 4
else { // If nothing needs to happen, ++rank and move down a node
    ++C->rank;
    C = C->dwn; // move to next node down
}
// C = C->dwn; // move to next node down

//******** TRY #2 *****
Node *C = root; // Current Node from root
Node *N = createNode(value, /* DELETE WHEN DONE */ lvl); // New node being inserted
Node *top = N; // holds top of tower 'N'
int temp = N->level; // Holds the highest level for new node
int rank = root->rank;
int cnt = 0;

printf("***** Insert New Element: %d *****\n", N->data);
while((C || N)) {
    // printf("N-data: %d, N-level: %d, block_i: %d\n", N->data, N->level, block_i);
    // printf("   C-data: %d, C-level: %d, C-rank: %d\n", C->data, C->level, C->rank);
    // printf("   rank = %d,  count = %d\n", rank, cnt);
    if(C->level == temp) {// part 1
        cout << "part 1,\n";
        if(C->rgt && ((rank - C->rgt->rank + 1) >= block_i || (rank - C->rgt->rank) >= block_i)) {
            cout << "  Insert 'N': 1, then go down*************************" << endl;
            Node* temp = C->rgt; // Swap right pointers
            C->rgt = N;
            N->rgt = temp;
            ++C->rank;

            if(N->rgt != NULL)
                N->rank += N->rgt->rank; // Increment rank by value of right pointers rank
            rank -= C->rgt->rank;
            N = N->dwn; // move to N's next level down
            C = C->dwn; // move to next node down
        }
        else if(C->rgt && (rank - C->rgt->rank) < block_i) {
            cout << "  Go right: 1" << endl;
            cnt += (C->rank - C->rgt->rank);
            ++C->rank;
            C = C->rgt;
        }
        else {
            cout << "  Insert 'N': 2, then go down************************" << endl;
            C->rgt = N;
            ++C->rank;
            N = N->dwn; // move to N's next level down
            C = C->dwn; // move to next node down
        }
    }
    else if(C->level > temp) {// part 2
        cout << "part 2,\n";
        if(C->rgt && (rank - C->rgt->rank) >= block_i) {
            cout << "  Go down: 1" << endl;
            rank -= C->rgt->rank;
            ++C->rank;
            C = C->dwn;
        }
        else if(C->rgt && (rank - C->rgt->rank) < block_i && (rank - C->rgt->rank) != 0) {
            cout << "  Go right: 1" << endl;
            cnt += (C->rank - C->rgt->rank);
            ++C->rank;
            C = C->rgt;
        }
        else {
            cout << "  Go down: 2" << endl;
            ++C->rank;
            C = C->dwn;
        }
    }
    else if(C->level < temp) {// part 3
        cout << "part 3,\n";
        if(C->rgt && (rank - C->rgt->rank + 1) == block_i) {
            cout << "  Go down: 1" << endl;
            N->rgt = C->rgt;
            C->rank -= C->rgt->rank; // Decrement rank by value of right pointers rank
            N->rank += N->rgt->rank; // Increment rank by value of right pointers rank
            C->rgt = NULL;

            // /* call function in increment entire tower of 'N' */
            Node *incr = top;
            while(incr->level > C->level) {
                incr->rank += N->rgt->rank;
                incr = incr->dwn;
            }
            incr = NULL;
            delete incr;

            N = N->dwn; // move to N's next level down
            C = C->dwn; // move to next node down
        }
        else if(C->rgt) {
            cout << "  Go right: 1" << endl;
            if(C->rgt && (rank - C->rgt->rank + cnt) < block_i) {
                printf("   C->rgt true\n");
                C->rank -= cnt;
            }
            // if(C->rank -= cnt > 0){
            //     printf("*** 1 if ************************************\n");
            //     C->rank -= cnt;
            // }
            // else {
            //     printf("*** 1 else ************************************\n");
            //     C->rank = 1;
            // }
                
            C = C->rgt;
        }
        else {
            cout << "  Go down: 2" << endl;
            if(C->rgt && (rank - C->rgt->rank + cnt) < block_i) {
                printf("   C->rgt true\n");
                C->rank -= cnt;
            }
            // if(C->rank -= cnt > 0){
            //     printf("*** 2 if ************************************\n");
            //     C->rank -= cnt;
            // }
            // else {
            //     printf("*** 2 else ************************************\n");
            //     C->rank = 1;
            // }
            C = C->dwn;
            N = N->dwn; // move to N's next level down
        }
    }
    // cout << endl;
}
// cout << endl;

//******** TRY #3 *****
void insertElement(int value, int block_i, /* DELETE WHEN DONE */ int lvl) {
    Node *N = createNode(value, /* DELETE WHEN DONE */ lvl); // New node being inserted
    Node *C = root;         // Current Node from root
    Node *top = N;          // holds top of tower 'N'
    Node *hold_C = NULL;
    Node *hold_N = NULL;
    int temp = N->level;    // Holds the highest level for new node
    int rank = root->rank;  // Used to find block_i
    int cnt = 0;            // used to adjust ranks after insertion
    
    printf("***** Insert New Element: %d at block %d *****\n", N->data, block_i);
    printf("    N-data: %d N-level: %d N-rank: %d, curr_rank: %d\n", N->data, N->level, N->rank, rank);
    printf("    C-data: %d C-level: %d C-rank: %d\n", C->data, C->level, C->rank);
    while(C) {
        if(C->level == temp) {
            cout << "part 1,\n";
            if(C->rgt && ((rank - C->rgt->rank + 1) >= block_i || (rank - C->rgt->rank) == block_i)) {
                cout << rank << ", " << (rank - C->rgt->rank + 1) << endl;
                cout << rank << ", " << (rank - C->rgt->rank) << endl;
                Node* temp = C->rgt; // Swap right pointers
                C->rgt = N;
                N->rgt = temp;
                ++C->rank;
                cout << "  Insert 'N': 1, then go down*************************" << endl;

                // NEW STUFF
                if(C->level > 0) {
                    C->dwn->prev = C;
                    N->dwn->prev = N;
                }
                // NEW STUFF
                
                if(N->rgt != NULL)
                    N->rank += N->rgt->rank; // Increment rank by value of right pointers rank
                rank -= C->rgt->rank;

                if(!C->dwn) { hold_C = C; hold_N = N; }
                N = N->dwn; // move to N's next level down
                C = C->dwn; // move to C's next node down
                
            }
            else if(C->rgt && (rank - C->rgt->rank) < block_i) {
                cnt += (C->rank - C->rgt->rank);
                ++C->rank;
                cout << "  Go right: 1" << endl;
                
                // NEW STUFF
                C->rgt->prev = C;
                // NEW STUFF

                C = C->rgt;
            }
            else {
                C->rgt = N;
                ++C->rank;
                cout << "  Insert 'N': 2, then go down*************************" << endl;

                // NEW STUFF
                if(C->level > 0) {
                    C->dwn->prev = C;
                    N->dwn->prev = N;
                }
                // NEW STUFF
                
                if(!C->dwn) { hold_C = C; hold_N = N; }
                N = N->dwn; // move to N's next level down
                C = C->dwn; // move to C's next node down
            }
        }
        else if(C->level > temp) {
            cout << "part 2,\n";
            if(C->rgt && (rank - C->rgt->rank) >= block_i) {
                rank -= C->rgt->rank;
                ++C->rank;
                cout << "  Go down: 1" << endl;

                // NEW STUFF
                C->dwn->prev = C;
                // NEW STUFF

                C = C->dwn;
            }
            else if(C->rgt && (rank - C->rgt->rank + 1) < block_i) {
                cnt += (C->rank - C->rgt->rank);
                ++C->rank;
                cout << "  Go right: 1" << endl;

                // NEW STUFF
                C->rgt->prev = C;
                // NEW STUFF

                C = C->rgt;
            }
            else {
                if(C->rgt)
                    rank -= C->rgt->rank;
                ++C->rank;
                cout << "  Go down: 2" << endl;

                // NEW STUFF
                C->dwn->prev = C;
                // NEW STUFF

                C = C->dwn;
            }
        }
        else if(C->level < temp) {
            cout << "part 3,\n";
            if(C->rgt && (rank - C->rgt->rank + 1) == block_i) {
                cout << "  Go down: 1" << endl;
                N->rgt = C->rgt;
                C->rank -= C->rgt->rank; // Decrement rank by value of right pointers rank
                N->rank += N->rgt->rank; // Increment rank by value of right pointers rank
                C->rgt = NULL;

                if(C->level == 0) {
                    stringstream ss;
                    ss << block_i - 1;
                    C->label = hashing(hold_N->level, hold_N->rank, ss.str(), "0");
                }

                /* call function in increment entire tower of 'N' */
                Node *incr = top;
                while(incr->level > C->level) {
                    incr->rank += N->rgt->rank;
                    incr = incr->dwn;
                }
                incr = NULL;
                delete incr;

                // NEW STUFF
                if(C->level > 0) {
                    C->dwn->prev = C;
                    N->dwn->prev = N;
                }
                // NEW STUFF

                if(!C->dwn) { hold_C = C; hold_N = N; }
                N = N->dwn; // move to N's next level down
                C = C->dwn; // move to C's next node down
            }
            else if(C->rgt) {
                if(C->rgt && (rank - C->rgt->rank + cnt) < block_i) {
                    C->rank -= cnt;
                }
                else if (C->rgt && (rank - C->rgt->rank + 1) == block_i - 1) {
                    C->rank -= 1;
                }

                // NEW STUFF
                C->rgt->prev = C;
                // NEW STUFF

                C = C->rgt;
            }
            else {
                if(C->rgt && (rank - C->rgt->rank + cnt) < block_i) {
                    C->rank -= cnt;
                }
                else if (rank == block_i) {
                    C->rank -= 1;
                }

                // NEW STUFF
                if(C->level > 0) {
                    C->dwn->prev = C;
                    N->dwn->prev = N;
                }
                // NEW STUFF

                if(!C->dwn) { hold_C = C; hold_N = N; }
                N = N->dwn; // move to N's next level down
                C = C->dwn; // move to C's next node down
            }
        }
        if(C) {
            printf("    N-data: %d N-level: %d N-rank: %d, curr_rank: %d\n", N->data, N->level, N->rank, rank);
            printf("    C-data: %d C-level: %d C-rank: %d\n", C->data, C->level, C->rank);
        }
    }
    cout << endl;
    // reset hash values along traversed path
    insertHash(hold_N, hold_C, block_i);

    // printf("\npress ENTER to continue\n");
    // cin.get();
}
// reset hash values along traversed path
void insertHash(Node *N, Node *C, int block_i) {
    cout << "hashing for hold_N" << endl;
    stringstream ss;
    ss << block_i;
    while(hold_N) {
        string fd = "0";
        string fr = "0";
        if(hold_N->level > 0) { 
            if(hold_N->dwn) { fd = hold_N->dwn->label; }
            if(hold_N->rgt) { fr = hold_N->rgt->label; }
            hold_N->label = hashing(hold_N->level, hold_N->rank, fd, fr);
        }
        else if(hold_N->level == 0) {
            if(hold_N->rgt) { fr = hold_N->rgt->label; }
            hold_N->label = hashing(hold_N->level, hold_N->rank, ss.str(), fr);
        }
        printf("data: %d level: %d rank: %d\n", hold_N->data, hold_N->level, hold_N->rank);
        cout << "  label: " << hold_N->label << endl << endl;
        Node *tmp = hold_N;
        hold_N = hold_N->prev;
    }
    

    cout << "hashing for hold_C" << endl << endl;
    int block_value;
    if(hold_C->rgt == hold_N)
        block_value = block_i - 1;
    
    while(hold_C) {
        string fd = "0";
        string fr = "0";
        if(temp == 0 && hold_C->level == 0) {
            if(hold_C->rgt) { fr = hold_C->rgt->label; }
            ss >> block_value;
            cout << "x(v): " << ss.str() << endl;
            hold_C->label = hashing(hold_C->level, hold_C->rank, ss.str(), fr);
            --block_value;
        }
        else if(hold_C->level > 0) { 
            if(hold_C->dwn) { fd = hold_C->dwn->label; }
            if(hold_C->rgt) { fr = hold_C->rgt->label; }
            hold_C->label = hashing(hold_C->level, hold_C->rank, fd, fr);
        }
        printf("data: %d level: %d rank: %d\n", hold_C->data, hold_C->level, hold_C->rank);
        cout << "  label: " << hold_C->label << endl << endl;
        Node *tmp = hold_C;
        hold_C = hold_C->prev;
        tmp = NULL;
    }
}
//******** TRY #4 *****
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
                cout << "rank = " << rank << endl;

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
            if(C->rgt && (rank - C->rgt->rank + 1) == block_i) {
                N->rgt = C->rgt;
                C->rgt = NULL;

                if(C->level > 0) {
                    C->dwn->prev = C;
                    N->dwn->prev = N;
                }

                if(!C->dwn) { hold_C = C; hold_N = N; }
                N = N->dwn; // move to N's next level down
                C = C->dwn; // move to C's next node down
            }
            else if(C->rgt) {
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
    insertHash(hold_N, hold_C, block_i, temp);
}

//************************ HASH ***********************************************************
// reset hash values along traversed path
void insertHash(Node *hold_N, Node *hold_C, int block_i, int temp) {
    stringstream ss;
    while(hold_N) {
        string fd = "0";
        string fr = "0";
        if(hold_N->level > 0) { 
            if(hold_N->dwn) { fd = hold_N->dwn->label; }
            if(hold_N->rgt) { fr = hold_N->rgt->label; }
            hold_N->label = hashing(hold_N->level, hold_N->rank, fd, fr);
            if(hold_N->rgt)
                hold_N->rank  = hold_N->rgt->rank + hold_N->dwn->rank;
            else
                hold_N->rank = hold_N->dwn->rank;
        }
        else if(hold_N->level == 0) {
            ss << hash_number;
            ++hash_number;
            if(hold_N->rgt) { fr = hold_N->rgt->label; }
            hold_N->label = hashing(hold_N->level, hold_N->rank, ss.str(), fr);
            ss.clear();
            if(hold_N->rgt)
                hold_N->rank  = hold_N->rgt->rank + 1;
            else
                hold_N->rank = 1;
        }
        Node *tmp = hold_N;
        hold_N = hold_N->prev;
        tmp = NULL;
    }
    
    while(hold_C) {
        string fd = "0";
        string fr = "0";
        if(hold_C->level > 0) {
            if(hold_C->dwn) { fd = hold_C->dwn->label; }
            if(hold_C->rgt) { fr = hold_C->rgt->label; }
            hold_C->label = hashing(hold_C->level, hold_C->rank, fd, fr);
            if(hold_C->rgt)
                hold_C->rank  = hold_C->rgt->rank + hold_C->dwn->rank;
            else
                hold_C->rank = hold_C->dwn->rank;
        }
        else if(hold_C->level == 0) {
            if(hold_C->rgt) {
                if(hold_C->data != -1)
                    hold_C->rank  = hold_C->rgt->rank + 1;
                else
                    hold_C->rank  = hold_C->rgt->rank;
            }
            else
                hold_C->rank = 1;
        }
        Node *tmp = hold_C;
        hold_C = hold_C->prev;
        tmp = NULL;
    }
}
/********************************************************************************/
/************************* DELETE ALGORITHM *************************************/
//******** TRY #1 *************
void deleteElement(int block_i) {
    Node *curr = root;         // Current Node from root
    Node *dele = NULL;
    Node *hold_curr = NULL;
    Node *hold_dele = NULL;
    Node *top = NULL;
    int rank = root->rank;  // Used to find block_i
    bool block_i_found = false;

    if(rank < block_i) {
        block_i_found = false;
    }
    else {
        while(curr) {
            if(curr->rgt && (start_rank - curr->rgt->rank + 1) == block_i) {
                dele = curr->rgt;
                top = dele; // holds top level of node to be deleted
                int temp = dele->level; // holds height of node to be deleted

                if(dele->rgt) {
                    rank -= curr->rgt->rank;
                    curr->rgt = dele->rgt;
                    dele->rgt = NULL;
                    if(!curr->dwn) { hold_curr = curr; }
                    dele = dele->dwn;
                    curr = curr->dwn;
                }
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
            else if(curr->rgt && (rank - curr->rgt->rank) < block_i ){
                if(curr->rgt) { curr->rgt->prev = curr; }
                curr = curr->rgt;
            }
            else if(curr->rgt && (rank - curr->rgt->rank) >= block_i) {
                rank -= curr->rgt->rank;
                if(curr->dwn) { curr->dwn->prev = curr; }
                curr = curr->dwn;
            }
        }
    }
    // set new hash and rank values in skip list
    setHashandRank(NULL, hold_curr);

    // delete block_i from memory
    do {
        Node *temp = top->dwn;
        top->rgt = NULL;
        top->dwn = NULL;
        top->prev = NULL;
        delete top;
    } while(temp);
}

//******** TRY #2 *****
void deleteElement(int block_i) {
Node *curr = root;          // Current Node from root
Node *dele = NULL;          // traverses the node to be delete
Node *hold_curr = NULL;     // gets passed to setHashandRank() function
Node *top = NULL;           // holds top of node to be deleted
int rank = root->rank;      // used to find block_i
bool block_i_found = false;

printf("***** Delete Block number: %d\n", block_i);

if(rank < block_i) {
    cout << "roots rank is less than block number given" << endl;
    block_i_found = false;
}
else {
    cout << "1st while loop" << endl;
    while(curr) {
        if(curr->rgt && (rank - curr->rgt->rank + 1) == block_i) {
            cout << "   error 1.1" << endl;
            dele = curr->rgt;
            top = dele; // holds top level of node to be deleted
            curr->rgt = NULL;
            int temp = dele->level; // holds height of node to be deleted

            if(dele->rgt) {
                cout << "   error 2.0" << endl;
                rank -= curr->rgt->rank;
                curr->rgt = dele->rgt;
                dele->rgt = NULL;
                if(!curr->dwn) { hold_curr = curr; }
                dele = dele->dwn;
                curr = curr->dwn;
            }
            cout << "2nd while loop" << endl;
            while(curr) {
                if(curr->rgt) {
                    cout << "   error 2.1" << endl;
                    curr->rgt->prev = curr;
                    curr = curr->rgt;
                }
                else if(!curr->rgt && dele->rgt && curr->level == dele->level) {
                    cout << "   error 2.2" << endl;
                    curr->rgt = dele->rgt;
                    dele->rgt = NULL;
                    
                    if(curr->level > 0) { curr->dwn->prev = curr; }
                    if(!curr->dwn) { hold_curr = curr; }

                    dele = dele->dwn;
                    curr = curr->dwn;
                }
                else {
                    cout << "   error 2.3" << endl;
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
            cout << "   error 1.2" << endl;
            if(curr->rgt) { curr->rgt->prev = curr; }
            curr = curr->rgt;
        }
        else if(curr->rgt && (rank - curr->rgt->rank + 1) > block_i) {
            cout << "   error 1.3" << endl;
            rank -= curr->rgt->rank;
            if(curr->dwn) { curr->dwn->prev = curr; }
            curr = curr->dwn;
        }
        else {
            cout << "   error 1.4" << endl;
            if(curr->dwn) { curr->dwn->prev = curr; }
            curr = curr->dwn;
        }
    }
    if(hold_curr)
        cout << "hold_curr" << endl;
    if(curr)
        cout << "curr" << endl;

    cout << "set hash and rank" << endl;
    // set new hash and rank values in skip list
    setHashandRank(NULL, hold_curr);

    cout << "delete top" << endl;
    // delete block_i from memory
    del(top);
}

if(block_i_found)
    printf("Block %d has been Deleted!\n\n", block_i);
else
    printf("Block %d does not exist!\n\n", block_i);
}

//******** TRY #3 *****
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
    cout << "1st while loop" << endl;
    while(curr) {
        if(curr->rgt && (rank - curr->rgt->rank + 1) == block_i) {
            cout << "   error 1.1" << endl;
            dele = curr->rgt;
            curr->rgt = NULL;
            top = dele; // used to delete element at end of function
                
            cout << "2nd while loop" << endl;
            while(curr) {
                if(curr->rgt) {
                    cout << "   error 2.1" << endl;
                    curr->rgt->prev = curr;
                    curr = curr->rgt;
                }
                else if(!curr->rgt && dele->rgt && curr->level == dele->level) {
                    cout << "   error 2.2" << endl;
                    curr->rgt = dele->rgt;
                    dele->rgt = NULL;
                    
                    if(curr->level > 0) { curr->dwn->prev = curr; }
                    if(!curr->dwn) { hold_curr = curr; }

                    dele = dele->dwn;
                    curr = curr->dwn;
                }
                else {
                    cout << "   error 2.3" << endl;
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
            cout << "   error 1.2" << endl;
            if(curr->rgt) { curr->rgt->prev = curr; }
            curr = curr->rgt;
        }
        else if(curr->rgt && (rank - curr->rgt->rank + 1) > block_i) {
            cout << "   error 1.3" << endl;
            rank -= curr->rgt->rank;
            if(curr->dwn) { curr->dwn->prev = curr; }
            curr = curr->dwn;
        }
        else {
            cout << "   error 1.4" << endl;
            if(curr->dwn) { curr->dwn->prev = curr; }
            curr = curr->dwn;
        }
    }
    if(hold_curr)
        cout << "hold_curr" << endl;
    if(curr)
        cout << "curr" << endl;

    cout << "set hash and rank" << endl;
    // set new hash and rank values in skip list
    setHashandRank(NULL, hold_curr);

    cout << "delete top" << endl;
    // delete block_i from memory
    del(top);
}

if(block_i_found)
    printf("Block %d has been Deleted!\n\n", block_i);
else
    printf("Block %d does not exist!\n\n", block_i);
}
/********************************************************************************/
/************************* SEARCH ALGORITHM *************************************/
//******** TRY #1 *****
Node* start = root;
printf("Searching for data: %d\n", data);
printf("  from: %d level: %d rank: %d ", start->data, start->level, start->rank);
while(start->level != 0 || (start->rgt && start->rgt->data <= data)) {
    if(start->rgt && start->rgt->data <= data) { // if right pointer is NOT NULL
        printf("    to: %d level: %d rank: %d\n", start->rgt->data, start->rgt->level, start->rgt->rank);
        start = start->rgt;
    }
    else if(start->dwn) { // go down while level does NOT equal 0
        printf("    to: %d level: %d rank: %d\n", start->data, start->dwn->level, start->dwn->rank);
        start = start->dwn;
    }
}
if(start->data == data && start->level == 0)
    printf("data: %d, level: %d has been found\n\n", start->data, start->level);
else
    printf("data: %d has NOT been found\n\n", data);

//******** TRY #2 ******
Node* start = root;
int curr_rank = start->rank;
bool block_i_found = false;

printf("Searching for block: %d\n", block_i);
printf("  from: %d level: %d curr_rank: %d \n", start->data, start->level, start->rank);
while(start) { 
    if ( start->rgt && (curr_rank - start->rgt->rank) < block_i && start->level > 0) {
        start = start->rgt;     // move to next node on path
        printf("1    to: %d level: %d rank: %d, curr_rank: %d\n", start->data, start->level, start->rank, curr_rank);
    }
    else if(start->rgt && start->level == 0) {
        start = start->rgt;     // move to next node on path
        printf("2    to: %d level: %d rank: %d, curr_rank: %d\n", start->data, start->level, start->rank, curr_rank);
    }
    else if( start->rgt && (curr_rank - start->rgt->rank) >= block_i ){
        curr_rank -= start->rgt->rank;
        start = start->dwn;     // move to next node on path
        printf("3    to: %d level: %d rank: %d, curr_rank: %d\n", start->data, start->level, start->rank, curr_rank);
    }
    else if(!start->rgt && start->level >= 0) {
        if(curr_rank == block_i && start->level == 0) { 
            block_i_found = true; 
            break;
        }
        start = start->dwn;     // move to next node on path
        printf("4    to: %d level: %d rank: %d, curr_rank: %d\n", start->data, start->level, start->rank, curr_rank);
    }
}
if(block_i_found)
    printf("block %d has been found!\n", block_i);
else
    printf("block %d has not been found!\n", block_i);

//******** TRY #3 *************
Node* curr = root;
int start_rank = root->rank;
bool block_i_found = false;

printf("Searching for block: %d\n", block_i);
printf("  from: %d level: %d rank: %d\n", curr->data, curr->level, curr->rank);

/* POSSIBLE PARAMETERS TO USE */
while(start_rank != block_i || curr->level >= 0) {    
    if(curr->rgt && (start_rank - curr->rgt->rank + 1) == block_i) {
        curr_rank = block_i;
        curr = curr->rgt;
        printf("1.0    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
        while(curr->level > 0) {
            curr = curr->dwn;
            printf("1.5    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
        }
        block_i_found = true; 
        break;
    }
    else if(curr->rgt && (curr_rank - curr->rgt->rank) >= block_i ){
        start_rank -= curr->rgt->rank;
        curr = curr->dwn;
        printf("2.0    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
    }
    else if(curr->rgt && (curr_rank - curr->rgt->rank) < block_i ){
        curr = curr->rgt;
        printf("3.0    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
    }
    else {
        curr = curr->dwn;
        printf("4.0    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
    }
}
if(block_i_found)
    printf("block %d has been found!\n", block_i);
else
    printf("block %d has not been found!\n", block_i);

/********************************************************************************/
/************************* AT RANK ALGORITHM *************************************/
//******** TRY #1 *************
list <tuple> Tuple_List;
int l;      // l(v) = level of node 'v'
int q;      // q(v) = rank of successor of node 'v'
// true for right, false for down
bool d;     // d(v) = if previous node is right or below 'v'
string g;   // g(v) = label of successor of node 'v'
Node* start = root;
int rank = start->rank;

// while(start->data != block_i || start->level >= 0) {
while(start) {    
    tuple A; // temp tuple to store in list

    cout << "curr node: " << start->data << ", level: " << start->level << endl;
    l = start->level; // level of node 'v'
    if ( start->rgt && (rank - start->rgt->rank) < block_i && start->level > 0) {
        cout << "Go right" << endl;
        d = true;               // previous node is right of node 'v'
        q = start->dwn->rank;   // rank of successor node
        g = start->dwn->label;  // label of successor node 'v'
        start = start->rgt;     // move to next node on path
    }
    else if(start->rgt && start->level == 0) {
        cout << "Go right" << endl;
        d = true;               // previous node is right of node 'v'
        q = 1;                  // if level = 0, then q = 1
        g = start->label;       // if no successor, use label at current node
        start = start->rgt;     // move to next node on path
    }
    else if( start->rgt && (rank - start->rgt->rank) >= block_i ){
        cout << "Go down" << endl;
        rank -= start->rgt->rank;
        d = false;              // previous node is below node 'v'
        q = start->rgt->rank;   // rank of successor node
        g = start->rgt->label;  // label of successor node 'v'
        start = start->dwn;     // move to next node on path
    }
    else if(!start->rgt && start->level >= 0) {
        cout << "Go down" << endl;
        d = false;              // previous node is below node 'v'
        q = start->rank;        // if no successor, us rank at current node
        g = start->label;       // if no successor, use label at current node
        if(rank == block_i && start->level == 0) { Tau = g; g = "0"; }
        start = start->dwn;     // move to next node on path
    }
    
    A = make_tuple(l, q, d, g);    // make a tuple
    // cout << "  l(v) = level of node 'v': " << get<0>(A) <<  endl;
    // cout << "  q(v) = rank of successor of node 'v': " << get<1>(A) << endl; 
    // if(!get<2>(A)) 
    //     cout << "  d(v) = previous node is below node 'v': 0" << endl;
    // else
    //     cout << "  d(v) = previous node is right of node 'v': 1" << endl;
    // cout << "  g(v) = label of successor of node 'v': " << get<3>(A) << endl;// << endl;

    // if(start)
    //     cout << "next node: " << start->data << ", level: " << start->level << endl << endl;
    Tuple_List.push_back(A);            // add new tuple to front of list
}

/* WHEN DONE CLEAR ALL DATA FROM LIST */
// Tuple_List.clear();

return Tuple_List;

//******** TRY #2 *************
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
            printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
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
                    printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
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
                printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
        }
        else if(curr->rgt && (start_rank - curr->rgt->rank) < block_i ){
            d = true;               // previous node is right of node 'v'
            q = curr->dwn->rank;    // q = rank of successor node
            g = curr->dwn->label;   // g = label of successor node
            curr = curr->rgt;
            if(curr)
                printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
        }
        else {
            d = false;          // previous node is below node 'v'
            q = curr->rank;     // if no successor, then q = current rank
            g = curr->label;    // if no successor, then g = current label
            curr = curr->dwn;
            if(curr)
                printf("    to: %d level: %d rank: %d, curr_rank: %d\n", curr->data, curr->level, curr->rank, start_rank);
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

//******** TRY #3 *************
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

    return Tuple_List;
}
/********************************************************************************/
/************************* PRINT ALGORITHM *************************************/
//******** TRY #1 *************
void print(int arr[], int size) {
    for(int i = 0; i < size; ++i){
        Node* start = root;
        printf("\nRoot to data: %d\n", arr[i]);
        printf("  from: %d level: %d rank: %d ", start->data, start->level, start->rank);
        cout << "label: " << start->label << endl;
        while(start->level != 0 || (start->rgt && start->rgt->data <= arr[i])) {
            if(start->rgt && start->rgt->data <= arr[i]) { // if right pointer is NOT NULL
                printf("    to: %d level: %d rank: %d ", start->rgt->data, start->rgt->level, start->rgt->rank);
                cout << "label: " << start->rgt->label << endl;
                start = start->rgt;
            }
            else if(start->dwn) { // go down while level does NOT equal 0
                printf("    to: %d level: %d rank: %d ", start->data, start->dwn->level, start->dwn->rank);
                cout << "label: " << start->dwn->label << endl;
                start = start->dwn;
            }
        }
    }
}

//******** TRY #2 *************
void print() {
    int size = root->rank;
    for(int i = 1; i <= size; ++i) {
        search(i);
    }
}

/********************************************************************************/
/************************* VERIFY ALGORITHM *************************************/
//******** TRY #1 *************
/* 
    If T is the correct representation of block i and sequence pi of length k is the correct 
    proof for T, then the following properties hold for the values computed in iteration k 
    of the for-loop of Algorithm 2:

    1) Value ρk is equal to the number of nodes at the bottom level of the skip list, that is,
    the number n of blocks of the file
    2) Value ξk is equal to n − i
    3) Value γk is equal to the label of the start node of the skip list

*************************************
    For now :
    We will work with block_i = 7 
    Metadata => Mc = root->label = 856E54A080BF8B9AE10F84DF1D5C3D25D11CFE8A
    Tau for block_7 => T = 7A7E0073686635348381639C8F15F1A54825749B
*/
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
    return result;
}

/********************************************************************************/
/*************************** HASH ALGORITHM *************************************/
//******** TRY #1 *************
        // reset hash values along traversed path
        void insertHash(Node *hold_N, Node *hold_C, int block_i, int temp) {
            cout << "************* hashing for hold_N ****************" << endl;
            
            stringstream ss;
            while(hold_N) {
                string fd = "0";
                string fr = "0";
                if(hold_N->level > 0) { 
                    if(hold_N->dwn) { fd = hold_N->dwn->label; }
                    if(hold_N->rgt) { fr = hold_N->rgt->label; }
                    cout << " level: " << hold_N->level << endl;
                    cout << "  rank: " << hold_N->rank << endl;
                    cout << "f(dwn): " << fd << endl;
                    cout << "f(rgt): " << fr << endl;
                    hold_N->label = hashing(hold_N->level, hold_N->rank, fd, fr);
                    cout << "label: " << hold_N->label << endl << endl;
                }
                else if(hold_N->level == 0) {
                    ss << hash_number;
                    ++hash_number;
                    if(hold_N->rgt) { fr = hold_N->rgt->label; }
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
            }
            
            cout << "************* hashing for hold_C *************" << endl << endl;
            while(hold_C) {
                string fd = "0";
                string fr = "0";
                if(hold_C->level > 0) {
                    if(hold_C->dwn) { fd = hold_C->dwn->label; }
                    if(hold_C->rgt) { fr = hold_C->rgt->label; }
                    cout << " level: " << hold_C->level << endl;
                    cout << "  rank: " << hold_C->rank << endl;
                    cout << "f(dwn): " << fd << endl;
                    cout << "f(rgt): " << fr << endl;
                    hold_C->label = hashing(hold_C->level, hold_C->rank, fd, fr);
                    cout << "label: " << hold_C->label << endl << endl;
                }
                else if(hold_C->level == 0) {
                    ss << hash_number;
                    ++hash_number;
                    if(hold_C->rgt) { fr = hold_C->rgt->label; }
                    cout << " level: " << hold_C->level << endl;
                    cout << "  rank: " << hold_C->rank << endl;
                    cout << "f(dwn): " << fd << endl;
                    cout << "f(rgt): " << fr << endl;
                    hold_C->label = hashing(hold_C->level, hold_C->rank, ss.str(), fr);
                    cout << "label: " << hold_C->label << endl << endl;
                    ss.clear();
                }
                Node *tmp = hold_C;
                hold_C = hold_C->prev;
                tmp->prev = NULL;
                tmp = NULL;
            }
        }
//******** TRY #2 *************
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
            hold_N->label = hashing(hold_N->level, hold_N->rank, fd, fr);
        }
        else if(hold_N->level == 0) {
            ss << hash_number; // This will be changed to the metadata eventually
            ++hash_number;
            if(hold_N->rgt) { fr = hold_N->rgt->label; }
            
            if(hold_N->rgt)
                hold_N->rank = hold_N->rgt->rank + 1;
            else
                hold_N->rank = 1;
            hold_N->label = hashing(hold_N->level, hold_N->rank, ss.str(), fr);
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
            hold_C->label = hashing(hold_C->level, hold_C->rank, fd, fr);
        }
        else if(hold_C->level == 0) {
            if(hold_C->rgt) {
                if(hold_C->data != -1) {
                    hold_C->rank = hold_C->rgt->rank + 1;
                }
                else {
                    hold_C->rank = hold_C->rgt->rank;
                    hold_C->label = hashing(hold_C->level, hold_C->rank, fd, hold_C->rgt->label);
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
/********************************************************************************/
/************************ COMPUTE LABEL ALGORITHM *******************************/
//******** TRY #1 *************
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

//******** TRY #2 *************
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
                    ++lambda1;
                    gamma1 = hashing(lambda1, rho1, get<3>(A), gamma1); // gamma1 gets rehashed
                }
                else { // vj previous node is down
                    printf("  2.1.2\n");
                    ++lambda1;
                    gamma1 = hashing(lambda1, rho1, gamma1, get<3>(A));// gamma1 gets rehashed
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
                    ++rho1;
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
    
    return gamma1;
}

string root_label() {
    return root->label;
}



