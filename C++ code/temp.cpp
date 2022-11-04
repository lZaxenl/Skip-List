string setHash() {
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