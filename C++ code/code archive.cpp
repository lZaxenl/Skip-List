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
        // cout << "part 1,\t";
        if(C->rgt && ((rank - C->rgt->rank + 1) == block_i || (rank - C->rgt->rank) == block_i)) {
            // cout << "  Insert 'N': 1, then go down*************************" << endl;
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
            // cout << "  Go right: 1" << endl;
            cnt += (C->rank - C->rgt->rank);
            ++C->rank;
            C = C->rgt;
        }
        else {
            // cout << "  Insert 'N': 2, then go down************************" << endl;
            C->rgt = N;
            ++C->rank;
            N = N->dwn; // move to N's next level down
            C = C->dwn; // move to next node down
        }
    }
    else if(C->level > temp) {// part 2
        // cout << "part 2,\t";
        if(C->rgt && (rank - C->rgt->rank) >= block_i) {
            // cout << "  Go down: 1" << endl;
            rank -= C->rgt->rank;
            ++C->rank;
            C = C->dwn;
        }
        else if(C->rgt && (rank - C->rgt->rank) < block_i && (rank - C->rgt->rank) > 0) {
            // cout << "  Go right: 1" << endl;
            cnt += (C->rank - C->rgt->rank);
            ++C->rank;
            C = C->rgt;
        }
        else {
            // cout << "  Go down: 2" << endl;
            ++C->rank;
            C = C->dwn;
        }
    }
    else if(C->level < temp) {// part 3
        // cout << "part 3,\t";
        if(C->rgt && (rank - C->rgt->rank + 1) == block_i) {
            // cout << "  Go down: 1" << endl;
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

            N = N->dwn; // move to N's next level down
            C = C->dwn; // move to next node down
        }
        else if(C->rgt) {
            // cout << "  Go right: 1" << endl;
            if(C->rgt && (rank - C->rgt->rank + cnt) < block_i) {
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
            // cout << "  Go down: 2" << endl;
            if(C->rgt && (rank - C->rgt->rank + cnt) < block_i) {
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
