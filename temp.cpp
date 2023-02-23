
struct tuple {
    int l;      // l(v) = level of node 'v'
    int q;      // q(v) = rank of successor of node 'v'
    bool d;     // d(v) = if previous node is to the right (true) or below (false) 'v'
    string g;   // g(v) = label of successor of node 'v'

    tuple *next;
    tuple *prev;
}

tuple::tuple() {
    l = 0;
    q = 0;
    d = false;
    g = "";

    next = NULL;
    prev = NULL;
}

void atRank(int block_i) {
    Node *curr = root;         // Current Node from root
    int temp = N->level;    // Holds the highest level for new node
    int rank = root->rank;  // Used to find block_i
    // tuple *tupleList = new tuple();
    // tupleList->l = root->level;
    // tupleList->q = root->rank;
    // tupleList->g = root->label;

    while(curr) {
        tuple *A = new tuple();
/* when curr level is equal to block_i level */
        if(curr->level == temp) {
            if(curr->rgt && ((rank - curr->rgt->rank + 1) >= block_i || (rank - curr->rgt->rank) == block_i)) {
                rank -= curr->rgt->rank;

                Node* temp = curr->rgt; // Swap right pointers
                curr->rgt = N;
                N->rgt = temp;

                if(curr->level > 0) {
                    curr->dwn->prev = curr;
                    N->dwn->prev = N;
                }
                
                if(!curr->dwn) { hold_C = curr; hold_N = N; }
                N = N->dwn; // move to N's next level down
                curr = curr->dwn; // move to C's next node down
                
            }
            else if(curr->rgt && (rank - curr->rgt->rank) < block_i) {
                curr->rgt->prev = curr;
                curr = curr->rgt;
            }
            else {
                curr->rgt = N;

                if(curr->level > 0) {
                    curr->dwn->prev = curr;
                    N->dwn->prev = N;
                }
                
                if(!curr->dwn) { hold_C = curr; hold_N = N; }
                N = N->dwn; // move to N's next level down
                curr = curr->dwn; // move to C's next node down
            }
        }
/* when curr level is higher then block_i level */
        else if(curr->level > temp) {
            string dir = "";
            if(curr->rgt && (rank - curr->rgt->rank) >= block_i) {
                rank -= curr->rgt->rank;
                dir = "go down";
            }
            else if(curr->rgt && (rank - curr->rgt->rank + 1) < block_i) {
                dir = "go right";
            }
            else {
                if(curr->rgt)
                    rank -= curr->rgt->rank;
                dir = "go down";
            }
            
            // set parameters for tuple list
            A->l = curr->level;
            if(dir == "go down") {
                A->d = true;
                A->q = curr->rgt->rank;
                A->g = curr->rgt->label;
                curr = curr->dwn;
            }
            else if(dir == "go right") {
                A->d = false;
                A->q = curr->dwn->rank;
                A->g = curr->dwn->label;
                curr = curr->rgt;
            }

            // insert node into tuple list
            tuple *temp = A;
            A = new tuple();
            temp->prev = A;
            A->next = temp;
        }
/* when curr level is lower then block_i level */
        else if(curr->level < temp) {
            if(curr->rgt && (rank - curr->rgt->rank + 1) >= block_i) {
                rank -= curr->rgt->rank;
                N->rgt = curr->rgt;
                C->rank -= curr->rgt->rank;
                curr->rgt = NULL;

                if(curr->level > 0) {
                    curr->dwn->prev = curr;
                    N->dwn->prev = N;
                }

                if(!curr->dwn) { hold_C = curr; hold_N = N; }
                N = N->dwn; // move to N's next level down
                curr = curr->dwn; // move to C's next node down
            }
            else if(curr->rgt && (rank - curr->rgt->rank + 1) < block_i) {
                curr->rgt->prev = curr;
                curr = curr->rgt;
            }
            else {
                if(curr->level > 0) {
                    curr->dwn->prev = curr;
                    N->dwn->prev = N;
                }

                if(!curr->dwn) { hold_C = curr; hold_N = N; }
                N = N->dwn; // move to N's next level down
                curr = curr->dwn; // move to C's next node down
            }
        }
    }
}


