//Defines the entry point for the console application.
#include <iostream>
#include <vector>
#include <bitset>
#include <string>
using namespace std;

/*
This function reads a line of int values into a vector function and returns that
vector.
*/
vector<int> readlineofints(int count) {
    // Input values
    vector<int> linevalues(count);
    for (int j = 0; j < count; j++) {
        int val;
        cin >> val;
        linevalues[j] = val;
    }
    return linevalues; // Return line values as a vector
}

/*This function clears the answer array from the start_row down to the end
*/
void clear_answer(int n,int(&answer)[15], int start_row) {
    std::fill(answer + start_row, answer + 15, 0); //Reset array to 0
}


bool recursive_check(int n, int row, int(&answer)[15], const int(&orig)[15], int static_columns, int forward_diags, int backward_diags) {
        if (row >= n) { return true; } //If we've passed n, return true
        int possibles_mask = ~(static_columns | forward_diags | backward_diags); //Create mask as compliment of 'or' between int values
        if (orig[row] == 0) { //If this row has no x's and is 0
            for (int i = 0; i < n; i++) { //Iterate through the row
                if (((possibles_mask >> (n - i - 1)) & 1) == 1) { //If the ith bit is 1, indicating this position is currently possible
                    int hole_punch = 1 << (n - i - 1); //Set hole puncher
                    int new_static_columns = static_columns | hole_punch; //Punch hole
                    int new_forward_diags = forward_diags | hole_punch; //Punch hole
                    int new_backward_diags = backward_diags | hole_punch; //Punch hole
                    if (recursive_check(n, row + 1,answer, orig, new_static_columns, (new_forward_diags >> 1), (new_backward_diags << 1))) { //Recurse
                        answer[row] = answer[row] | hole_punch; //Punch hole in answer if this worked
                        return true; //Return true if the recursion worked
                    }
                }
            }
        }
        else if (orig[row] != 0) { //If there is a pre-set value in this row
            //Do a pre-check of the entire row to see if an already existing X conflicts, making this all impossible
            if (row > 0) { //If this isn't the first row (where the mask of pre-existing X's includes this row's X's already and will thus conflict
                for (int i = 0; i < n; i++) { //Iterate through the row
                    if (((orig[row] >> (n - i - 1)) & 1) == 1) { //If the ith bit is 1, indicating this position has an X
                        if (((possibles_mask >> (n - i - 1)) & 1) == 0) { //If the position of this x conflicts, clear and return false
                            clear_answer(n, answer, row); //Clear the answer row from here down to be safe
                            return false;
                        }
                    }
                }
            }
            //If no pre-existing X conflicts, try the pre-existing X as a solution
            for (int i = 0; i < n; i++) { //Iterate through the row
                if (((orig[row] >> (n - i - 1)) & 1) == 1) { //If the ith bit is 1, indicating this position has an X
                        //Test to see if this X can work
                        int hole_punch = 1 << (n - i - 1); //Set hole puncher
                        int new_static_columns = static_columns | hole_punch; //Punch hole
                        int new_forward_diags = forward_diags | hole_punch; //Punch hole
                        int new_backward_diags = backward_diags | hole_punch; //Punch hole
                        if (recursive_check(n, row + 1, answer, orig, new_static_columns, (new_forward_diags >> 1), (new_backward_diags << 1))) { //Recurse
                            answer[row] = answer[row] | hole_punch; //Punch hole in answer if this worked
                            return true; //Return true if the recursion worked
                        }
                }
            }
        }
        clear_answer(n, answer, row); //Clear the answer row from here down to be safe
        return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    // get test case count
    int t;
    std::cin >> t;

    // loop over all the test cases
    for (int i = 1; i <= t; i++) {
        // Read in params
        vector<int> params = readlineofints(1);
        int n = params[0];

        int rows[15] = {0};
        int answer[15] = { 0 };
        bool possible = true;
        // Iterate over printers
        for (int j = 0; j < n; j++) {
            string x;
            cin >> x;
            bool row_used = false;
            for (int k = 0; k < n; k++) {                 
                if (x.substr(k,1) == "x") { //If this point is an x
                    if (row_used == true) {//If this is the second x in this row, 
                        possible = false; //mark this one as impossible
                        break; //break out of this loop
                    }  
                    int hole_puncher = 1 << (n - k-1); //1 left shifted to current position
                    rows[j] = rows[j] | hole_puncher; //Punch hole at point of x
                    row_used = true; //Note that the row has been used
                }
            }
        }
        
        if (possible) { //If we don't already know this one is impossible
            possible = recursive_check(n,0, answer, rows, rows[0],rows[0], rows[0]); //Start the recursive check
        }

        //Print result, incrementing to account for indexing offset at input
        if (possible) {
            std::cout << "Case #" << i << ": " << std::endl;
            for (int row = 0; row < n; row++) {
                for (int column = 0; column < n; column++) {
                    if (((answer[row] >> (n - column - 1)) & 1) == 1) { //If the ith bit is 1
                        std::cout << "x"; //Write x
                    }
                    else { //Otherwise
                        std::cout << "."; //Write .
                    }
                }
                std::cout << std::endl; //Write carriage return
            }
        }
        else {
            std::cout << "Case #" << i << ": impossible" << std::endl;
        }
    }
    return 0;
}