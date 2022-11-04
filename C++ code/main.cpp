#include "header.h"
#include "quicksort.h"
#include "sha1.h"


int main()
{
    // seed random number generator
    srand((unsigned)time(0));
    
    // create a new skip list
    skipList skip_list(4, 0.670);
    string client_Mc = "b24";
    // int   arr[] = {5, 7, 11, 1, 15, 19, 23, 27, 31, 29};//, 2, 6, 10, 14, 16, 20, 24, 28, 32, 36 };
    // int     s[] = {3, 2, 4,  0,  0,  2,  1,  3,  1,  4};// DELETE WHEN DONE // temp ranks for array
    // int block[] = {2, 3, 4,  1,  5,  6,  7,  8,  10, 9};

    // int   arr[] = {1, 5, 7, 11, 15, 19, 26, 27, 29, 31, 25, 24, 23};
    // int     s[] = {0, 3, 2,  4,  0,  2,  1,  3,  4,  1,  1,  1,  1};
    // int block[] = {1, 2, 3,  4,  5,  6,  7,  8,  9, 10,  7,  7,  7};

    int   arr[] = {1, 5, 7, 11, 15, 19, 23, 27, 29, 31};
    int     s[] = {0, 3, 2,  4,  0,  2,  1,  3,  4,  1};
    int block[] = {1, 2, 3,  4,  5,  6,  7,  8,  9, 10};

    int size = sizeof(arr) / sizeof(arr[0]);
    
    for(int i = 0; i < size; ++i) {
        skip_list.insertElement(arr[i], block[i], s[i]);
    }
    cout << "\nhello world\n" << endl; // TEST TO MAKE SURE I MADE IT THIS FAR

//************** ALGORITHM 1: AT RANK *******************
    bool verify_data = verify_block_i_integrity(skip_list, client_Mc, 4);
    // if(verify_data) 
    //     cout << "accept" << endl; 
    // else
    //     cout << "reject" << endl;

//************** Sha1 HASH FUNCTION *******************
// SHA1 sha1;
// cout << sha1("00") << endl;  
// cout << getHash(0, 0) << endl;

//************** SORT AND PRINT SKIP LIST *******************
    // quickSort(arr, 0, sizeof(arr)/sizeof(arr[0]) - 1);
    // skip_list.print();

//*********** SEARCH FOR A VALUE IN SKIP LIST ***************
    // for(int k = 1; k < 11; k++) {
    //     skip_list.search(k);
    //     printf("\n");
    // }
    // skip_list.search(3);
    // skip_list.search(4);
    // skip_list.search(7);
    // skip_list.search(15);

    skip_list.delete_skipList();
    return 0;
}

