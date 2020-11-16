#include<iostream>
#include<stack>
#include<queue>
#include<fstream>
#include<utility>
#include <stdio.h>
#include <string.h>
#include<vector>
#include <stdlib.h> 
//#include <time.h> 

#define max_n 1005
#define pos std::pair<int,int>
#define MAX 2147483646
 
using namespace std;
int space = 0;
int move_dir[4][2] = { {0,1},{1,0},{-1,0},{0,-1} };
int board[max_n][max_n];
int dis[max_n][max_n];
int visited[max_n][max_n];
vector<pos> path;

void count_dis(int(&board)[max_n][max_n], int(&dis)[max_n][max_n], int R_x, int R_y, int m, int n) {
    queue<pos> q;

    pos pos_now;
    pos_now.first = R_x;
    pos_now.second = R_y;

    dis[R_x][R_y] = 0;
    q.push(pos_now);
    while (!q.empty()) {
        pos_now = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            pos next_pos;
            next_pos.first = pos_now.first + move_dir[i][0];
            next_pos.second = pos_now.second + move_dir[i][1];
            if (0 <= next_pos.first && next_pos.first < m &&
                0 <= next_pos.second && next_pos.second < n &&
                dis[next_pos.first][next_pos.second] < 0) {
                if (board[next_pos.first][next_pos.second] == 0) {
                    dis[next_pos.first][next_pos.second] = dis[pos_now.first][pos_now.second] + 1;
                    ++space;
                    q.push(next_pos);
                }
            }
        }

    }
}

pos pick_far(int x, int y, int m, int n, bool is_deep) {
    int a, b;
    int r1 = x, r2 = y;
    //srand(time(NULL));
    
    int rand_dir = rand() % 4;
    for (int i = 0; i < 4; i++) {
        //int dir = (i + rand_dir) % 4;
        int dir = i;
        a = x + move_dir[dir][0];
        b = y + move_dir[dir][1];
        if (is_deep) {
            if (0 <= a && a < m &&
                0 <= b && b < n && dis[a][b]>= 0 &&
                (visited[a][b] < visited[r1][r2] || //need to take care the 
                    (visited[a][b] == visited[r1][r2] && dis[a][b] > dis[r1][r2]))
                )
            {
                r1 = a;
                r2 = b;
            }
        }
        else if (0 <= a && a < m &&
                 0 <= b && b < n && dis[a][b] >= 0 &&
            (dis[a][b] > dis[r1][r2] || //need to take care the 
                (dis[a][b] == dis[r1][r2] && visited[a][b] < visited[r1][r2]))
            )
        {
            r1 = a;
            r2 = b;
        }
    }
    return(pos(r1, r2));
}
pos pick_home(int x, int y, int m, int n) {
    int a, b;
    int r1 = x, r2 = y;
    for (int i = 3; i >=0; --i) {
        a = x + move_dir[i][0];
        b = y + move_dir[i][1];
        if (0 <= a && a < m &&
            0 <= b && b < n && dis[a][b] >= 0 &&
            (dis[a][b] < dis[r1][r2] || //we must able find the dir with less dis
                (dis[a][b] == dis[r1][r2] && visited[a][b] < visited[r1][r2]))
            )
        {
            r1 = a;
            r2 = b;
        }
    }
    return(pos(r1, r2));
}
void print_visited(int m, int n, int found,int pow,int pow_max,int x,int y) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (x == i && y == j) {
                cout << "["<<visited[i][j] <<"]"<< ' ';
            }
            else cout << visited[i][j] << ' ';
        }
        cout << " ||| ";
        for (int j = 0; j < n; j++) 
            cout << dis[i][j] << ' ';

        cout << "\n";
    }
    cout << "found: "<<found << ", space:" << space << "\n";
    cout << "energy: " << pow << ", energy_max:" << pow_max << "\n";
    cout << "----------------------------------------------\n";
}
void print_dis(int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            cout << dis[i][j] << ' ';
        cout << "\n";
    }
    
}
void reset(int x, int y, int m, int n) {
    queue<pos> q;
    q.push(pos(x, y));
    pos pos_now;

    int a, b;
    while (!q.empty()) {

        pos_now = q.front();
        q.pop();
        int is_end = 1;

        int x_now = pos_now.first;
        int y_now = pos_now.second;
        if (dis[x_now][y_now] == -1) continue;

        for (int i = 0; i < 4; ++i) {
            a = pos_now.first + move_dir[i][0];
            b = pos_now.second + move_dir[i][1];

            if (0 <= a && a < m && 0 <= b && b < n) {
                if (dis[a][b] >= dis[pos_now.first][pos_now.second])
                    is_end = 0;
            }
        }
        if (is_end&&visited[x_now][y_now]) {
            dis[x_now][y_now] = -1;
            for (int i = 0; i < 4; ++i) {
                a = pos_now.first + move_dir[i][0];
                b = pos_now.second + move_dir[i][1];
                q.push(pos(a, b));
            }
        }

    }
}

void greed(int x, int y, int m, int n, int energy) {

    pos pos_now(x, y);

    int found = 0;

    pos zero(x, y);
    int energy_now= energy;
    pos pos_next;
    bool is_deep;
    vector<pos> to_set;
    while (found < space) {
        //update some state
        //print_visited(m, n, found, energy_now, energy, pos_now.first, pos_now.second);
        //if (energy_now < 0)cout << "bug!!\n";
        //if (energy_now == 0)cout << "limit!!\n";
        path.push_back(pos_now);
        if (pos_now == zero) {
            energy_now = energy;
            is_deep = 0;
            for (pos i : to_set) {
                if(dis[i.first][i.second]!= -1)
                reset(i.first, i.second, m, n);
            }
            to_set.clear();
        }
        if (!visited[pos_now.first][pos_now.second]) {
            visited[pos_now.first][pos_now.second] = 1;
            ++found;


        }
        else ++visited[pos_now.first][pos_now.second];
        //find next step
        if (dis[pos_now.first][pos_now.second] < energy_now) {

            pos_next = pick_far(pos_now.first, pos_now.second, m, n, is_deep);

            if (pos_now == pos_next) {
                is_deep = 1;
                to_set.push_back(pos_now);
            }
            else --energy_now;
        }
        else {
            pos_next = pick_home(pos_now.first, pos_now.second, m, n);
            --energy_now;
        }

        pos_now = pos_next;
    }
    while (pos_now != zero) {
        pos_now = pick_home(pos_now.first, pos_now.second, m, n);
        path.push_back(pos_now);
    }
    path.push_back(zero);
}


int main() {

    std::fstream input;
    input.open("floor.data", std::ios::in);

    int m = 10, n = 10;
    char temp;
    int pow;
    input >> m >> n >> pow;
    path.reserve(m * n);//speed up
    int x, y;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {

            input >> temp;
            while (!((temp == 'R') || ('0' <= temp && temp <= '9')))input >> temp;
            if (temp == 'R') {
                x = i; y = j;
                board[i][j] = -1;
            }
            else {
                board[i][j] = (int)(temp - '0');

            }
        }
    }

    memset(dis, -1, sizeof(dis[0][0]) * max_n * max_n);
    count_dis(board, dis, x, y, m, n);

    greed(x, y, m, n, pow);

   /* for (pos i : path) {
        cout << "(" << i.first << "," << i.second << ")\n";
    }*/
    cout << path.size();
    //print_dis(m, n);

}
