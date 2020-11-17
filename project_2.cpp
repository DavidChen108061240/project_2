#include <iostream>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <time.h>
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
    int **step;
    Pos start;                                          // start point, also recharge place
    Pos visited_node;
    
};

Pos clean (Ground &gnd, Pos cur_pos) {
    int x, y;
    Pos next_pos;
    int do_nothing = 0;

    next_pos.x = 0;
    next_pos.y = 0;
    next_pos.visited = -1;

    x = cur_pos.x;
    y = cur_pos.y;
    
     if ( x + 1 != gnd.row &&  (gnd.ground[x + 1][y] == '0') ) {    // south, need to be clean
        
        next_pos.x = x + 1;
        
        next_pos.y = y;
        next_pos.visited = 1;
        
    } else if ((gnd.ground[x][y - 1] == '0') && y != 0) {              // west, need to be clean
        next_pos.x = x;
        next_pos.y = y - 1;
        next_pos.visited = 1;
    } else if ((gnd.ground[x - 1][y] == '0') && x != 0) {              // north, need to be clean
        next_pos.x = x - 1;
        next_pos.y = y;
        next_pos.visited = 1;
    } else  if ( y + 1 != gnd.col &&  (gnd.ground[x][y + 1] == '0'))  {           // east, need to be cleaned
        next_pos.x = x;
        next_pos.y = y + 1;
        next_pos.visited = 1;
      
    } else {
        do_nothing = 1;
        
    }

    return next_pos;
} 

Pos clean_back (Ground &gnd, Pos cur_pos, int &decreasing_life) {
    int x, y;
    Pos next_pos;
    int do_nothing = 0;

    next_pos.x = 0;
    next_pos.y = 0;
    next_pos.visited = -1;

    x = cur_pos.x;
    y = cur_pos.y;
     if (gnd.step[x][y - 1] == decreasing_life - 1 ) {                       // west, need to be clean
        next_pos.x = x;
        next_pos.y = y - 1;
        next_pos.visited = 1;
        
    } else if (gnd.step[x + 1][y] == decreasing_life - 1 ) {                    // south, need to be clean
        next_pos.x = x + 1;
        next_pos.y = y;
        next_pos.visited = 1;
        // cout << "line 85" << endl;
    } else if ( gnd.step[x][y + 1] == decreasing_life - 1)  {           // east, need to be cleaned
        next_pos.x = x;
        next_pos.y = y + 1;
        next_pos.visited = 1;    
    } else if ( gnd.step[x - 1][y] == decreasing_life - 1 ) {                       // north, need to be clean
        next_pos.x = x - 1;
        next_pos.y = y;
        next_pos.visited = 1;
    } else {
        do_nothing  = 1;
    }  
    return next_pos;
} 

void Ground_Path(Ground &gnd, ostream& file_out) {
    Pos cur_pos, next_pos;                                // current position, next_position
    stack<Pos> path_stack;                                // path
    stack<Pos> path_back_stack;
    queue<Pos> whole_path_queue;                          // Whole path
    stack<Pos> tmp_stack;
    int decreasing_life = gnd.life;                       // renew life
    cur_pos = gnd.start;                                  // cur_pos start from start pt
    int SUM_STEP = 0;
    int NO_battery = 0;
    int RENEW_STEP = 0;
    int from_here = 0;
    int do_nothing = 0;
    int search_undone = 0;
    
    do {
        // cout << "line 123" << endl;
        
        if(NO_battery) {
            // check near number 
            while(!path_stack.empty()) {
                path_stack.pop();
                // cout << "pathback 129" << endl;
            }
            path_stack.push(gnd.start);
            
            // cout << "line 134" << endl;
            do {
                // cout << "in the do loop" << endl;
                next_pos = clean_back(gnd, cur_pos, decreasing_life);                           // must find it
                
                if ( cur_pos.x + 1 != gnd.row   && gnd.ground[cur_pos.x + 1][cur_pos.y] == '0') {                    // south, need to be clean
                    from_here = 1;
                } else if (cur_pos.y + 1 != gnd.col  && gnd.ground[cur_pos.x][cur_pos.y + 1] == '0' )  {           // east, need to be cleaned
                    from_here = 1;    
                } else if (gnd.ground[cur_pos.x - 1][cur_pos.y] == '0' && cur_pos.x != 0) {                       // north, need to be clean
                    from_here = 1;    
                } else if (gnd.ground[cur_pos.x][cur_pos.y - 1] == '0' && cur_pos.y != 0) {                       // west, need to be clean
                    from_here = 1;
                } else {
                    do_nothing  = 1;
                }  
                
                if (next_pos.visited != -1) {
                    if(from_here) {
                        tmp_stack.push(cur_pos);
                        // cout << "in tmp _from_here " << cur_pos.x <<  " " << cur_pos.y << endl;
                    }
                    whole_path_queue.push(cur_pos);
                    if (gnd.ground[cur_pos.x][cur_pos.y] == '0') gnd.ground[cur_pos.x][cur_pos.y] = 'A';         // have already gone
                    if (do_nothing) gnd.ground[cur_pos.x][cur_pos.y] = 'E';
                    
                    
                    cur_pos = next_pos;
                    // cout << "cur_pos.x and cur_pos.y = " << cur_pos.x << " " << cur_pos.y << endl;
                    decreasing_life--;
                    SUM_STEP++;
                    if (cur_pos.x == gnd.start.x && cur_pos.y == gnd.start.y) {
                        decreasing_life = gnd.life;
                        NO_battery = 0;
                        whole_path_queue.push(cur_pos);
                    }
                                                         // sum step
                } 
                
            } while (decreasing_life != gnd.life)  ;                                                         // back to the recharge pt
            
            
            // cout << "line 168" << endl;
            while (!tmp_stack.empty()) {                                   // pop the best road, back to R pt to recharge
                cur_pos = tmp_stack.top();
                // cout << "tmp_stack top the cur_pos" << cur_pos.x << " " << cur_pos.y << endl;
                whole_path_queue.push(cur_pos);
                path_stack.push(cur_pos);
                tmp_stack.pop();
                SUM_STEP++;
                decreasing_life--;
                // cout << "when in tmp_stack" << cur_pos.x << " " << cur_pos.y << endl;
            }
                                                            // two while loop end, cur_pos is where the last time need to charge location
        }
        
        from_here = 0;
        
        next_pos = clean(gnd, cur_pos);  
                                                       // normal clean
        if (next_pos.visited != -1) {
            path_stack.push(cur_pos);
            whole_path_queue.push(cur_pos);
            gnd.ground[cur_pos.x][cur_pos.y] = 'A';         // have already gone
            cur_pos = next_pos;

            decreasing_life--;
            RENEW_STEP++;                                   // move forward, step+1, life-1
            SUM_STEP++;                                     // sum step
            
            if(decreasing_life == gnd.step[cur_pos.x][cur_pos.y])  {
                NO_battery = 1;                             /////// go back R
            }        
            
        } else {                                            // four way can not go 
            gnd.ground[cur_pos.x][cur_pos.y] = 'A';         // this is the end of this way
            whole_path_queue.push(cur_pos);
            cur_pos = path_stack.top();
                                                           // pop element, move back, push into queue
            path_stack.pop();
            decreasing_life--;

            RENEW_STEP++;                                   // move back, step+1, life-1
            SUM_STEP++;
            
            if(decreasing_life == gnd.step[cur_pos.x][cur_pos.y] ) {
                NO_battery = 1;      ///////  go back R
            }
            
        } 
    } while (!path_stack.empty());
    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (int i = 0; i < gnd.row; i++) {
        for (int j = 0; j < gnd.col; j++ ) {
            if (gnd.ground[i][j] == '0') {
                // cout << "unfound i j = " << i << " " << j << endl;
                cur_pos.x = i;
                cur_pos.y = j;
                search_undone = 1;
                decreasing_life = gnd.step[i][j];
            }
            if (search_undone == 1 ) {
                do {
                    // cout << "line 233" << endl;
                    
                    // cout << "decreasing_life = " << decreasing_life << endl;
                    next_pos = clean_back(gnd, cur_pos, decreasing_life);

                    if ( cur_pos.x + 1 != gnd.row   && gnd.ground[cur_pos.x + 1][cur_pos.y] == '0') {                    // south, need to be clean
                        from_here = 1;
                    } else if (cur_pos.y + 1 != gnd.col  && gnd.ground[cur_pos.x][cur_pos.y + 1] == '0' )  {           // east, need to be cleaned
                        from_here = 1;    
                    } else if (cur_pos.x != 0 && gnd.ground[cur_pos.x - 1][cur_pos.y] == '0') {                       // north, need to be clean
                        from_here = 1;    
                    } else if (cur_pos.y != 0 && gnd.ground[cur_pos.x][cur_pos.y - 1] == '0') {                       // west, need to be clean
                        from_here = 1;
                    } else {
                        do_nothing  = 1;
                    }  
                            
                    if (next_pos.visited != -1) {
                        if(from_here) {
                            tmp_stack.push(cur_pos);
                            // cout << "in tmp _from_here " << cur_pos.x <<  " " << cur_pos.y << endl;
                        }
                        whole_path_queue.push(cur_pos);
                        if (gnd.ground[cur_pos.x][cur_pos.y] == '0') gnd.ground[cur_pos.x][cur_pos.y] = 'A';         // have already gone
                        if (do_nothing) gnd.ground[cur_pos.x][cur_pos.y] = 'E';
                                
                        cur_pos = next_pos;
                        // cout << "cur_pos.x and cur_pos.y = " << cur_pos.x << " " << cur_pos.y << endl;
                        decreasing_life--;
                        SUM_STEP++;
                        if (cur_pos.x == gnd.start.x && cur_pos.y == gnd.start.y) {
                            decreasing_life = gnd.life;
                            search_undone = 0;
                        }
                                                                    // sum step
                    } 

                } while(search_undone == 1);

                // cout << "line 272" << endl;
                while (!tmp_stack.empty()) {                                   // pop the best road, back to R pt to recharge
                cur_pos = tmp_stack.top();
                // cout << "tmp_stack top the cur_pos" << cur_pos.x << " " << cur_pos.y << endl;
                whole_path_queue.push(cur_pos);
                path_stack.push(cur_pos);
                tmp_stack.pop();
                SUM_STEP++;
                decreasing_life--;
                // cout << "when in tmp_stack" << cur_pos.x << " " << cur_pos.y << endl;
            }

                do {      
                    if(NO_battery) {
                        // check near number 
                        while(!path_stack.empty()) {
                            path_stack.pop();
                            // cout << "pathback 129" << endl;
                        }
                        path_stack.push(gnd.start);
                        
                        // cout << "line 134" << endl;
                        do {
                            // cout << "in the do loop" << endl;
                            next_pos = clean_back(gnd, cur_pos, decreasing_life);                           // must find it
                            
                            if ( cur_pos.x + 1 != gnd.row   && gnd.ground[cur_pos.x + 1][cur_pos.y] == '0') {                    // south, need to be clean
                                from_here = 1;
                            } else if (cur_pos.y + 1 != gnd.col  && gnd.ground[cur_pos.x][cur_pos.y + 1] == '0' )  {           // east, need to be cleaned
                                from_here = 1;    
                            } else if (gnd.ground[cur_pos.x - 1][cur_pos.y] == '0' && cur_pos.x != 0) {                       // north, need to be clean
                                from_here = 1;    
                            } else if (gnd.ground[cur_pos.x][cur_pos.y - 1] == '0' && cur_pos.y != 0) {                       // west, need to be clean
                                from_here = 1;
                            } else {
                                do_nothing  = 1;
                            }  
                            
                            if (next_pos.visited != -1) {
                                if(from_here) {
                                    tmp_stack.push(cur_pos);
                                    // cout << "in tmp _from_here " << cur_pos.x <<  " " << cur_pos.y << endl;
                                }
                                whole_path_queue.push(cur_pos);
                                if (gnd.ground[cur_pos.x][cur_pos.y] == '0') gnd.ground[cur_pos.x][cur_pos.y] = 'A';         // have already gone
                                if (do_nothing) gnd.ground[cur_pos.x][cur_pos.y] = 'E';
                                
                                cur_pos = next_pos;
                                // cout << "cur_pos.x and cur_pos.y = " << cur_pos.x << " " << cur_pos.y << endl;
                                decreasing_life--;
                                SUM_STEP++;
                                if (cur_pos.x == gnd.start.x && cur_pos.y == gnd.start.y) {
                                    decreasing_life = gnd.life;
                                    NO_battery = 0;
                                }
                                                                    // sum step
                            } 
                            
                        } while (decreasing_life != gnd.life)  ;                                                         // back to the recharge pt
                        
                       
                        while (!tmp_stack.empty()) {                                   // pop the best road, back to R pt to recharge
                            cur_pos = tmp_stack.top();
                            // cout << "tmp_stack top the cur_pos" << cur_pos.x << " " << cur_pos.y << endl;
                            whole_path_queue.push(cur_pos);
                            path_stack.push(cur_pos);
                            tmp_stack.pop();
                            SUM_STEP++;
                            decreasing_life--;
                            // cout << "when in tmp_stack" << cur_pos.x << " " << cur_pos.y << endl;
                        }
                                                                        // two while loop end, cur_pos is where the last time need to charge location
                    }
                    
                    from_here = 0;
                    
                    next_pos = clean(gnd, cur_pos);                     // normal clean

                    if (next_pos.visited != -1) {
                        path_stack.push(cur_pos);
                        whole_path_queue.push(cur_pos);
                        gnd.ground[cur_pos.x][cur_pos.y] = 'A';         // have already gone
                        cur_pos = next_pos;

                        decreasing_life--;
                        RENEW_STEP++;                                   // move forward, step+1, life-1
                        SUM_STEP++;                                     // sum step
                        
                        if(decreasing_life == gnd.step[cur_pos.x][cur_pos.y])  {
                            NO_battery = 1;                             /////// go back R
                        }         
                    } else {                                            // four way can not go 
                        gnd.ground[cur_pos.x][cur_pos.y] = 'A';         // this is the end of this way
                        whole_path_queue.push(cur_pos);
                        cur_pos = path_stack.top();
                                                                    // pop element, move back, push into queue
                        path_stack.pop();
                        decreasing_life--;

                        RENEW_STEP++;                                   // move back, step+1, life-1
                        SUM_STEP++;
                        
                        if(decreasing_life == gnd.step[cur_pos.x][cur_pos.y] ) {
                            NO_battery = 1;      ///////  go back R
                        }
                    } 
                } while (!path_stack.empty());
            }
        } 
    }
    
    
    cout << "whole step is " << SUM_STEP << endl;
    
    while (!whole_path_queue.empty()) {
        cur_pos = whole_path_queue.front();
        whole_path_queue.pop();
        file_out << setw(3) << cur_pos.x << setw(3) << cur_pos.y << endl;
    }
    


     
}

void set_route(Ground &gnd, Pos now) {
    queue<Pos> whole_queue;
    
    int time = 0;
    gnd.step[now.x][now.y] = 0;
    whole_queue.push(now);
    
    while (1) {
        whole_queue.pop();
        if (now.x - 1 >= 0 && gnd.step[now.x - 1][now.y] == -1 && gnd.ground[now.x - 1][now.y] != '1') {
            
                now.x--;
                whole_queue.push(now);
                now.x++;
                gnd.step[now.x - 1][now.y] = gnd.step[now.x][now.y] + 1;
        }
        if (now.x + 1 < gnd.row && gnd.step[now.x + 1][now.y] == -1 && gnd.ground[now.x + 1][now.y] != '1') {
            
                now.x++;
                whole_queue.push(now);
                now.x--;
                gnd.step[now.x + 1][now.y] = gnd.step[now.x][now.y] + 1;
        }
        if (now.y - 1 >= 0 && gnd.step[now.x][now.y - 1] == -1 && gnd.ground[now.x][now.y - 1] != '1') {
            
                now.y--;
                whole_queue.push(now);
                now.y++;
                gnd.step[now.x][now.y - 1] = gnd.step[now.x][now.y] + 1;
        }
        if (now.y + 1 < gnd.col && gnd.step[now.x][now.y + 1] == -1 && gnd.ground[now.x][now.y + 1] != '1') {
            
                now.y++;
                whole_queue.push(now);
                now.y--;
                gnd.step[now.x][now.y + 1] = gnd.step[now.x][now.y] + 1;
        }
        if(whole_queue.empty()) {
            break;
        }
        now = whole_queue.front();
        /*
        if(now.x + 1 < gnd.row && gnd.step[now.x + 1][now.y] != -1) {
            gnd.step[now.x][now.y] = gnd.step[now.x + 1][now.y] + 1;
        } else if (now.y + 1 < gnd.col && gnd.step[now.x][now.y + 1] != -1) {
            gnd.step[now.x][now.y] = gnd.step[now.x][now.y + 1] + 1;
        } else if (now.x - 1 >= 0 && gnd.step[now.x - 1][now.y] != -1) {
            gnd.step[now.x][now.y] = gnd.step[now.x - 1][now.y] + 1;
        } else  
            gnd.step[now.x][now.y] = gnd.step[now.x][now.y - 1] + 1;
        */
    }
}

void print(ostream& file_out, Ground &gnd) {          // ostream&
    
    char *arr;
    arr = new char[2 * gnd.col + 1];                        // to input '1', '0', ' '
    
    for (int i = 0; i < gnd.row; ++i) {                     // the top 4 row need no assign, they are not the playground
        for (int j = 0; j < gnd.col; ++j) {
            						// if space[i][j] = 1, i.e has element 1
                arr[2*j] = gnd.ground[i][j];                         // when put 1, add another element ' ' into it
                arr[2*j + 1] = ' ';
            
        }
        arr[2 * gnd.col] = '\n';
        file_out.write(arr, 2 * gnd.col + 1);               // write(memory_block, size);
    }													// the way find in the internet
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
            // cout << G.ground[i][j] << " ";
            if (G.ground[i][j] == 'R') {
                G.start.x = i;
                G.start.y = j;
            }
        }   
        // cout << endl;         
    }
    cout << endl;
    
    G.step = new int* [G.row];
    for (int i = 0; i < G.row; ++i) {
            G.step[i] = new int [G.col];                                      // bool has value 1 or 0
    }
    
    for (int i = 0; i < G.row; ++i) {                                         // set all value 0
        for (int j = 0; j < G.col; ++j) {
            G.step[i][j] = -1;
        }          
    }
    

    set_route(G, G.start);
    
    cout << G.start.x << " start pt" << G.start.y << endl; 
    Ground_Path(G, file_out);
    /*
    for (int i = 0; i < G.row; ++i) {                                         // set all value 0
        for (int j = 0; j < G.col; ++j) {
        
            cout << G.ground[i][j] << " ";
            if (G.ground[i][j] == 'R') {
                G.start.x = i;
                G.start.y = j;
            }
        }   
        cout << endl;         
    }    
    */

    /*
    for (int i = 0; i < G.row; ++i) {                                         // output all step from R pt 
        for (int j = 0; j < G.col; ++j) {
            cout << setw(3) << G.step[i][j]; 
        }   
        cout << endl;         
    }
    */

    cout << "line 337" << endl;
    cout << G.start.x << " " << G.start.y << endl;

    

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
    print(file_out, G);
    file_in.close();
    cout << (double)clock() / CLOCKS_PER_SEC << "S";
    return 0;
}



// set_route(gnd, now);