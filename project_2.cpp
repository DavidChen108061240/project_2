#include <iostream>
#include <stdlib.h>
#include <stack>
#include <fstream>
#include <iomanip>
#include <cstring>
using namespace std;
#define N 1000

class Pos {
public:    
    int x, y;
};
class Ground {
public:
    int row, col, life;                                 // row col power
    char ground[N][N];                                  // whole ground
    Pos start;                                          // start point, also recharge place
    
};

Pos clean (Ground gnd, Pos cur_pos) {
    int x, y;
    Pos next_pos;
    int donothing = 0;
    next_pos.x = -1;
    next_pos.y = -1;
    x = cur_pos.x;
    y = cur_pos.y;
    
    if(gnd.ground[x][y + 1] == '0' && y != gnd.col - 1)  {           // east, need to be cleaned
        next_pos.x = x;
        next_pos.y = y + 1;
    } else if (gnd.ground[x + 1][y] == '0' && x != gnd.row - 1) {    // south, need to be clean
        next_pos.x = x + 1;
        next_pos.y = y;
    } else if (gnd.ground[x][y - 1] == '0' && y != 0) {              // west, need to be clean
        next_pos.x = x;
        next_pos.y = y - 1;
    } else if (gnd.ground[x - 1][y] == '0' && x != 0) {              // north, need to be clean
        next_pos.x = x - 1;
        next_pos.y = y;
    } else {
        donothing = 1;
    }
    return next_pos;
} 

void Ground_Path(Ground gnd) {
    Pos cur_pos, next_pos;                                // current position, next_position
    stack<Pos> path_stack;                                // path
    int decreasing_life = gnd.life;                       // renew life
    cur_pos = gnd.start;
    do {
        next_pos = clean(gnd, cur_pos);
        decreasing_life--;
        if (cur_pos.x == gnd.start.x && cur_pos.y == gnd.start.y) {
            path_stack.push(cur_pos);
            gnd.ground[cur_pos.x][cur_pos.y] = 'R';
            decreasing_life = gnd.life; 
        } else if (next_pos.x != -1) {
            path_stack.push(cur_pos);
            gnd.ground[cur_pos.x][cur_pos.y] = 'A';         // have already gone 1 time
            cur_pos = next_pos;
        } else {                                            // four way can not go 
            gnd.ground[cur_pos.x][cur_pos.y] = 'E';         // this is the end of this way
            cur_pos = path_stack.top();
            path_stack.pop();
            decreasing_life++;
        } 
    } while (!path_stack.empty());

    if (path_stack.empty()) {
        cout << "dead road "<< endl;
    } else {
        stack<Pos> tmp_stack;
        int order = 0;
        while (!path_stack.empty()) {
            cur_pos = path_stack.top();
            path_stack.pop();
            tmp_stack.push(cur_pos);
        }
        while (!tmp_stack.empty()) {
            order++;
            cur_pos = tmp_stack.top();
            tmp_stack.pop();
            gnd.ground[cur_pos.x][cur_pos.y] = order;
        }
        cout << endl;

        for (int i = 0; i < gnd.row; i++) {
            for (int j = 0; j < gnd.col; j++) {
                if(gnd.ground[i][j] > 0) {
                    cout << setw(2) << gnd.ground[i][j] << " ";
                } else {
                    cout << setw(2) << " ! ";
                }
            }
            cout << endl;
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
    
    cout << G.row << " " << G.col << " " << G.life << endl;

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
    file_in.close();
    // cout << G.start.x << " " << G.start.y << endl;


    return 0;
}