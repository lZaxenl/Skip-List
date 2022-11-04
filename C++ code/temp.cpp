string setHash() {
    struct pointer {
        Node *curr;
        Node *next;
    };
    hash(root);


}
void hash(Node *curr) {
    if(curr->rgt) {
        Node *R = curr->rgt;
        del(R);
    }  
    if(N->dwn) {
        Node *D = curr->dwn;
        del(D);
    }
    curr->rgt = NULL;
    curr->dwn = NULL;
}


/* USE THE SEARCH FUNCTION TO CREATE A RECURSIVE FUNCTION */
/* THEN USE IT TO SET THE HASH VALUES FOR ALL THE NODES IN THE PATH */
/* THIS WILL HAPPEN AFTER EVERY NEW NODE INSERTION */

void insertElement(int value, int block_i, /* DELETE WHEN DONE */ int lvl) {
    Node *N = createNode(value, /* DELETE WHEN DONE */ lvl); // New node being inserted
    Node *C = root;         // Current Node from root
    Node *top = N;          // holds top of tower 'N'
    int temp = N->level;    // Holds the highest level for new node
    int rank = root->rank;  // Used to find block_i
    int cnt = 0;            // used to adjust ranks after insertion

    pointer *path; // saving the path to set hash values after insertion
    
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
                N = N->dwn; // move to N's next level down
                C = C->dwn; // move to C's next node down
            }
            else if(C->rgt && (rank - C->rgt->rank) < block_i) {
                cnt += (C->rank - C->rgt->rank);
                ++C->rank;
                C = C->rgt;
            }
            else {
                C->rgt = N;
                ++C->rank;
                N = N->dwn; // move to N's next level down
                C = C->dwn; // move to next node down
            }
        }
        else if(C->level > temp) {
            if(C->rgt && (rank - C->rgt->rank) >= block_i) {
                rank -= C->rgt->rank;
                ++C->rank;
                C = C->dwn;
            }
            else if(C->rgt && (rank - C->rgt->rank) < block_i && (rank - C->rgt->rank) > 0) {
                cnt += (C->rank - C->rgt->rank);
                ++C->rank;
                C = C->rgt;
            }
            else {
                ++C->rank;
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

                N = N->dwn; // move to N's next level down
                C = C->dwn; // move to C's next node down
            }
            else if(C->rgt) {
                if(C->rgt && (rank - C->rgt->rank + cnt) < block_i) {
                    C->rank -= cnt;
                }
                C = C->rgt;
            }
            else {
                if(C->rgt && (rank - C->rgt->rank + cnt) < block_i) {
                    C->rank -= cnt;
                }
                C = C->dwn;
                N = N->dwn; // move to N's next level down
            }
        }
    }
    if(N)
        printf("true\n");
    // printf("\npress ENTER to continue\n");
    // cin.get();
}



