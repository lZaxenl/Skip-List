#include "header.h"
#include "quicksort.h"
#include "sha1.h"

void update(skipList skip_list, string &client_Mc);
void insertBlock(skipList skip_list, string &client_Mc);
void deleteBlock(skipList skip_list, string client_Mc);
void search(skipList skip_list);
void print(skipList skip_list);
void pause();

int main()
{
    srand((unsigned)time(0));// seed random number generator
    skipList skip_list(4, 0.670);// create a new skip list
    
    // int   arr[] = {5, 7, 11, 1, 15, 19, 23, 27, 31, 29, 4};//, 2, 6, 10, 14, 16, 20, 24, 28, 32, 36 };
    // int     s[] = {3, 2, 4,  0,  0,  2,  1,  3,  1,  4, 0};// DELETE WHEN DONE // temp ranks for array
    // int block[] = {2, 3, 4,  1,  5,  6,  7,  8,  10, 9, 2};
    // string client_Mc = "D243476D5052FA50C0C699C2282567908B43D47C"; // Mc = f(s), which corresponds with the top leftmost node

    // int   arr[] = {5, 7, 11, 1, 15, 19, 23, 27, 31, 29};//, 2, 6, 10, 14, 16, 20, 24, 28, 32, 36 };
    // int     s[] = {3, 2, 4,  0,  0,  2,  1,  3,  1,  4};// DELETE WHEN DONE // temp ranks for array
    // int block[] = {2, 3, 4,  1,  5,  6,  7,  8,  10, 9};
    

    int   arr[] = {1, 5, 7, 11, 15, 19, 23, 27, 29, 31};
    int     s[] = {0, 3, 2,  4,  0,  2,  1,  3,  4,  1};
    int block[] = {1, 2, 3,  4,  5,  6,  7,  8,  9, 10};

    int size = sizeof(arr) / sizeof(arr[0]);
    for(int i = 0; i < size; ++i) {
        skip_list.insertElement(arr[i], block[i], s[i]);
    }
    cout << "\nhello world\n" << endl; // TEST TO MAKE SURE I MADE IT THIS FAR

    // Mc = f(s), which corresponds with the top leftmost node
    string client_Mc = skip_list.getRL();
// ****************** MENU *********************
    bool cont = true;

    while(cont) {
        enum menu {upd = '1', ser = '2', prt = '3'};
        char choice;
        printf("+----------------+\n");
        printf("|   Main MENU    |\n");
        printf("+----------------+\n");
        printf("|  1) Update     |\n");
        printf("|  2) Search     |\n");
        printf("|  3) Print      |\n");
        printf("|  0) Quit       |\n");
        printf("+----------------+\n");
        printf("Select your option: ");
        cin >> choice;

        switch(choice) {
            case upd:
                update(skip_list, client_Mc);
                break;
            case ser:
                search(skip_list);
                break;
            case prt:
                print(skip_list);
                break;
            default:
                if(choice == '0') { cont = false; }
                break;
        }        
    }
    skip_list.delete_skipList();

    
    return 0;
}
/***********************************************************************************/
/***********************************************************************************/
// ************** Update Menu ******************
// controls: insertion, deletion, and Modification
void update(skipList skip_list, string &client_Mc){
    bool cont = true;
    while(cont) {
        enum menu {ins = '1', mod = '2', dele = '3'};
        char choice;
        printf("+----------------+\n");
        printf("|  Update Menu   |\n");
        printf("+----------------+\n");
        printf("|  1) Insert     |\n");
        printf("|  2) Modify     |\n");
        printf("|  3) Delete     |\n");
        printf("|  0) Main Menu  |\n");
        printf("+----------------+\n");
        printf("Select your option: ");
        cin >> choice;
        string z = client_Mc;
        switch(choice) {
            case ins:
                insertBlock(skip_list, client_Mc);
                break;
            case mod:
                printf("Modify does not work at this time\n");
                break;
            case dele:
                deleteBlock(skip_list, client_Mc);
                break;
            default:
                if(choice == '0') { cont = false; }
                break;
        }
    }
}
// ***************************************************
// **************** Insert function ******************
// ***************************************************
/*
    1) Ask for location to insert new data block
    2) Ask if they want a specific height
        a) if yes then get the desired height
        b) if no then generate a random height
*/
void insertBlock(skipList skip_list, string &client_Mc) {
    int block_i;

    // retrieve the location the client wants to insert the new data block
    cout << "\n***** Insert a data block *****" << endl;
    cout << "Input location for data block: ";
    cin >> block_i;
    int lvl;// = 4;//skip_list.randomLevel();
    cout << "Pick the level of new node: ";
    cin >> lvl;

    pause();
// 1) call atRank() for the data block before insertion location: atRank(block_i - 1)
    cout << "\n********** Part 1: client verifies data received from server **********" << endl;
    list <tuple> Tuple_List = skip_list.atRank(block_i-1); // call server to run atRank function
    string Tau_before = skip_list.getT();  // get tau before insertion, for block_i - 1
    
    string rootLabel = skip_list.getRL(); // DELETE WHEN NO LONGER NEEDED
    // verify data received from server side on clients side // DELETE WHEN NO LONGER NEEDED
    
    cout << "               Verify block " << block_i-1 << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "********************************************" << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "Tau_before: " << Tau_before << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "root label: " << rootLabel << endl;// DELETE WHEN NO LONGER NEEDED

    bool verify_data = skip_list.verify(block_i-1, client_Mc, Tau_before, Tuple_List);
    if(verify_data) 
        cout << "*** block " << block_i-1 << " is verfied ***" << endl << endl;
    else
        cout << "*** block " << block_i-1 << " is not verfied ***" << endl << endl;
    pause();
// 2) once the client verifies the given proof, then they compute what the next start label should be
     cout << "\n********** Part 2: client computes what new start label should be **********" << endl;
    string newClient_Mc = skip_list.computeLabel(Tau_before, Tuple_List, lvl);
    cout << "\tComputed new client Mc: " << newClient_Mc << endl;
    pause();
// 3) After data is verified, then have the server insert the new data block
    cout << "\n********** Part 3: server inserts new data block **********" << endl;
    skip_list.insertElement(999, block_i, lvl);  // insert block_i into temporary skip list
    pause();
// 4) verify the insertion
    cout << "\n********** Part 4: verify new data block after insertion **********" << endl;
    Tuple_List.clear();
    Tuple_List = skip_list.atRank(block_i); // call server to run atRank function
    string Tau_after = skip_list.getT();  // get tau after insertion, for block_i
    
    rootLabel = skip_list.getRL(); // DELETE WHEN NO LONGER NEEDED
    // verify data received from server side on clients side // DELETE WHEN NO LONGER NEEDED
    cout << "               Verify block " << block_i << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "********************************************" << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "Tau_after: " << Tau_after << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "root label: " << rootLabel << endl;// DELETE WHEN NO LONGER NEEDED
    cout << endl;
    
    bool decision = skip_list.verify(block_i, newClient_Mc, Tau_after, Tuple_List);
    if(decision)
        cout << "*** Insertion of block " << block_i << " was successful ***" << endl;
    else
        cout << "*** Insertion of block " << block_i << " was not successful ***" << endl;
    
    cout << "Old client Mc: " << client_Mc << endl;
    client_Mc = newClient_Mc;
    cout << "New client Mc: " << client_Mc << endl;

    Tuple_List.clear();
    pause();
}
// ***************************************************
// ************** Deletion function ******************
// ***************************************************
void deleteBlock(skipList skip_list, string client_Mc) {
    int block_i;
    cout << "\n***** Delete a data block *****" << endl;
    cout << "Input block number you wish to delete: ";
    cin >> block_i;

// 1) call atRank() twice
    /****************************************************************************************/
    // block before deletion
    list <tuple> Tuple_List_Before = skip_list.atRank(block_i-1); // call server to run atRank function
    string Tau_before = skip_list.getT();  // get tau for block before deletion
    string rootLabel = skip_list.getRL(); // DELETE WHEN NO LONGER NEEDED
    // verify data received from server side on clients side // DELETE WHEN NO LONGER NEEDED
    cout << "               Verify block " << block_i-1 << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "********************************************" << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "       Tau: " << Tau_before << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "root label: " << rootLabel << endl;// DELETE WHEN NO LONGER NEEDED

    bool verify_data = skip_list.verify(block_i-1, client_Mc, Tau_before, Tuple_List_Before);
    if(verify_data) 
        cout << "block " << block_i-1 << " is verfied" << endl << endl;
    else
        cout << "block " << block_i-1 << " is not verfied" << endl << endl;
    /****************************************************************************************/
    // block to be deleted
    list <tuple> Tuple_List_After = skip_list.atRank(block_i); // call server to run atRank function
    string Tau_delete = skip_list.getT();  // get tau of block to be delete
    
    rootLabel = skip_list.getRL(); // DELETE WHEN NO LONGER NEEDED
    // verify data received from server side on clients side // DELETE WHEN NO LONGER NEEDED
    cout << "               Verify block " << block_i << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "********************************************" << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "       Tau: " << Tau_delete << endl;// DELETE WHEN NO LONGER NEEDED
    cout << "root label: " << rootLabel << endl;// DELETE WHEN NO LONGER NEEDED

    verify_data = skip_list.verify(block_i, client_Mc, Tau_before, Tuple_List_After);
    if(verify_data) 
        cout << "block " << block_i << " is verfied" << endl << endl;
    else
        cout << "block " << block_i << " is not verfied" << endl << endl;
    /****************************************************************************************/

// 2) 

    skip_list.deleteElement(block_i);
    skip_list.print();
    pause();
}
// ***************************************************
// ************** Search function ******************
// ***************************************************
void search(skipList skip_list) {
    int block_i;

    cout << "\n***** Search for a data block *****" << endl;
    cout << "Input data block to search for: ";
    cin >> block_i;

    skip_list.search(block_i);
    pause();
}
// ***************************************************
// ************** Print skip list ******************
// ***************************************************
void print(skipList skip_list) {
    cout << "\n***** Print Skip List *****" << endl;
    skip_list.print();
    pause();
}
// ***************************************************
// ************** Pause function ******************
// ***************************************************
// creats a pause point before a menu is displayed again
void pause() {
    printf("\npress ENTER to continue\n");
    cin.get();
    // cin.get();
}