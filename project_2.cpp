#include <iostream>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <fstream>
#include <iomanip>
#include <cstring>
using namespace std;


   
class Pos {
public:    
    int x, y;
    int visited;
};
class Ground {
public:
    int row, col, life;                                 // row col power
    char **ground;                                      // whole ground

    Pos start;                                          // start point, also recharge place
    Pos visited_node;
    
};

Pos clean (Ground &gnd, Pos cur_pos) {
    int x, y;
    Pos next_pos;

    next_pos.x = 0;
    next_pos.y = 0;
    next_pos.visited = -1;
    x = cur_pos.x;
    y = cur_pos.y;
    
    if(gnd.ground[x][y + 1] == '0' &&  y != gnd.col - 1)  {           // east, need to be cleaned
        next_pos.x = x;
        next_pos.y = y + 1;
        next_pos.visited = 1;
    } else if (gnd.ground[x + 1][y] == '0' && x != gnd.row - 1) {    // south, need to be clean
        next_pos.x = x + 1;
        next_pos.y = y;
        next_pos.visited = 1;
    } else if (gnd.ground[x][y - 1] == '0' && y != 0) {              // west, need to be clean
        next_pos.x = x;
        next_pos.y = y - 1;
        next_pos.visited = 1;
    } else if (gnd.ground[x - 1][y] == '0' && x != 0) {              // north, need to be clean
        next_pos.x = x - 1;
        next_pos.y = y;
        next_pos.visited = 1;
    } 
    return next_pos;
} 

void Ground_Path(Ground &gnd) {
    Pos cur_pos, next_pos;                                // current position, next_position
    stack<Pos> path_stack;                                // path
    stack<Pos> tmp_stack;
    queue<Pos> whole_path_queue;                          // Whole path
    int decreasing_life = gnd.life;                       // renew life
    cur_pos = gnd.start;
    int SUM_STEP = 0;
    int NO_battery = 0;
    int RENEW_STEP = 0;
    int many_stack = 0;

    do {
        /*if(!NO_battery) {
            while (!path_stack.empty()) {               // pop the best road, back to R pt to recharge
                cur_pos = path_stack.top();
                   
                whole_path_queue.push(cur_pos);
                tmp_stack.push(cur_pos);
                path_stack.pop();
                SUM_STEP++;
            }
            many_stack = 0;
            decreasing_life = gnd.life;
            NO_battery = 0;
            
            while (!tmp_stack.empty()) {
                cur_pos = tmp_stack.top();

                whole_path_queue.push(cur_pos);
                path_stack.push(cur_pos);
                many_stack++;
                decreasing_life--;
                tmp_stack.pop();
                SUM_STEP++;
            }
                                                            // two while loop end, cur_pos is where the last time need to charge location
        }*/
            next_pos = clean(gnd, cur_pos);
                
        if (next_pos.visited != -1) {
            path_stack.push(cur_pos);
            whole_path_queue.push(cur_pos);
            gnd.ground[cur_pos.x][cur_pos.y] = 'A';         // have already gone
                
            cur_pos = next_pos;
            decreasing_life--;
            RENEW_STEP++;                                   // move forward, step+1, life-1
            SUM_STEP++;                                     // sum step
            many_stack++;
            /*if((decreasing_life - many_stack) <= 1) {
                NO_battery = 1;                             /////// go back R
            }   */     
        } else {                                            // four way can not go 
            gnd.ground[cur_pos.x][cur_pos.y] = 'E';         // this is the end of this way
            cur_pos = path_stack.top();
            whole_path_queue.push(cur_pos);                       // pop element, move back, push into queue
            path_stack.pop();
            decreasing_life--;

            RENEW_STEP++;                                   // move back, step+1, life-1
            SUM_STEP++;
            many_stack--;
            /*if((decreasing_life - many_stack) <= 1) {
                NO_battery = 1;      ///////  go back R
            }*/
        } 
    } while (!path_stack.empty());
        
    cout << "whole step is " << SUM_STEP << endl;

    if (path_stack.empty()) {
        cout << "dead road "<< endl;
    } else {
        while (!whole_path_queue.empty()) {
            cur_pos = whole_path_queue.front();
            whole_path_queue.pop();
            cout << setw(4) << cur_pos.x << cur_pos.y << endl;
        }
    }      
}

int main (int argc, char **argv) {
    Ground G;
    int row, col;
    ifstream file_in;                                   // istream
    ofstream file_out;                                  // ostream

    file_in.open(argv[1], ios::in);                     // open argv[1]
    file_out.open("108061240_proj1.final", ios::out | ios::trunc);  
                                                        // open a file and clean data and rewrite it, if this file doesn't exist, new a file
    if (!file_in.is_open())                             // check if input file open sucessfully
    {
        cout << "Open file fails!\n";
        return 0;
    } 
    if(!file_out.is_open()) {                           // check if output file open sucessfully
        cout << "open out_file fails!\n";
        return -1;
    }
    file_in >> G.row >> G.col >> G.life;

    G.ground = new char* [G.row];
    for (int i = 0; i < G.row; ++i) {
            G.ground[i] = new char [G.col];                                      // bool has value 1 or 0
    }
    for (int i = 0; i < G.row; ++i) {                                         // set all value 0
        for (int j = 0; j < G.col; ++j) {
            file_in >> G.ground[i][j] ;
            cout << G.ground[i][j] << " ";
            if (G.ground[i][j] == 'R') {
                G.start.x = i;
                G.start.y = j;
            }
        }   
        cout << endl;         
    }
    Ground_Path(G);
/*
    for ( int i = 0; i < G.row ; i++) {
        for (int j = 0; (j < G.col) ; j++ ) { 
            file_in >> G.ground[i][j];
                  
            cout << G.ground[i][j] << " ";
            if (G.ground[i][j] == 'R') {
                G.start.x = i;
                G.start.y = j;
            }
        }
        cout << endl;
    }
*/
    file_in.close();
    cout << G.start.x << " " << G.start.y << endl;


    return 0;
}