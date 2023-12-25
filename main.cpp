#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
using namespace std;


void print_table(const vector<vector<int>>& table) {
    int rows = table.size();


    int width = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            int len = to_string(table[i][j]).length();
            if (len > width) {
                width = len;
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            cout << setw(width) << table[i][j] << " ";
        }
        cout << endl;
    }
}
int rand_range(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);

    return dis(gen);
}
int non_zero_element(int min, int max){
    int seed = rand_range(0,1);
    if(seed == 0){
        return rand_range(min,-1);
    }else{
        return rand_range(1,max);
    }
}
vector<int> generate_the_way(int rights, int downs) {
    vector<int> resultVector;

    for (int i = 0; i < downs; ++i) {
        resultVector.push_back(1);
    }

    for (int i = 0; i < rights; ++i) {
        resultVector.push_back(0);
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(resultVector.begin(), resultVector.end(), g);
    return resultVector;
}
vector<vector<int>> generate_basic_table(int x, int y){
    //generate the table with all nines
    vector<vector<int>> table(y, vector<int>(x, 9));

    //generate where to go (right or down)
    vector<int> path = generate_the_way(x-1,y-1);

    //generate the path based on the way we got
    int sum = 0;
    int i=0,j=0;
    int generated = non_zero_element(-3,3);
    table[i][j] = generated;
    sum += generated;

    for(int d : path){
        if(d==0){
            j+=1;
        }else{
            i+=1;
        }
        generated = non_zero_element(-3,3);
        sum += generated;
        table[i][j] = generated;
    }
    table[y-1][x-1] = sum - table[y-1][x-1];

    print_table(table);
    cout<<endl;

    //generate the walls
    int wall_amount = rand_range(2,5);
    unordered_map<int,int> walls;
    for(int g = 1 ; g <=wall_amount; g++){
        while(true){
            int w = rand_range(0,x-1);
            int h = rand_range(0,y-1);

            if(table[h][w]==9){
                if(walls.find(h) == walls.end()){
                    walls[h] = w;
                    table[h][w] = 0;
                    break;
                }else{
                    if (walls[h]!=w){
                        walls[h] = w;
                        table[h][w] = 0;
                        break;
                    }
                }
            }
        }
    }

    //make the rest of the table random
    for(int m = 0; m <= y-1;m++){
        for(int n = 0; n<= x-1;n++){
            if(table[m][n] == 9){
                table[m][n] = non_zero_element(-3,3);
            }
        }
    }

    return table;

}

bool is_valid(const int& x, const int& y, const int& width, const int& height, const vector<vector<int>>& table) {
    return x >= 0 && x < height &&
           y >= 0 && y < width &&
           (table[x][y] != 0 || table[x][y] == 0 && table[0][y] != 0);
}

vector<pair<int, int>> path(int x, int y, int steps, int sum, const int& width, const int& height, vector<vector<int>> table, const int& ending_x, const int& ending_y) {
    if ((x + 1 == ending_x && y == ending_y) || (x == ending_x && y + 1 == ending_y)) {
        if ((steps == width+height-2) && (sum == table[x][y])) {
            return {{x, y}};
        }
    } else {
        sum += table[x][y];
        table[x][y] = 0;
        steps += 1;

        if (is_valid(x + 1, y, width, height, table)) {
            vector<pair<int, int>> down_path = path(x + 1, y, steps, sum, width, height, table, ending_x, ending_y);
            if (!down_path.empty()) {
                down_path.insert(down_path.begin(), {x, y});
                return down_path;
            }
        }

        if (is_valid(x, y + 1, width, height, table)) {
            vector<pair<int, int>> right_path = path(x, y + 1, steps, sum, width, height, table, ending_x, ending_y);
            if (!right_path.empty()) {
                right_path.insert(right_path.begin(), {x, y});
                return right_path;
            }
        }

        if (is_valid(x - 1, y, width, height, table)) {
            vector<pair<int, int>> up_path = path(x - 1, y, steps, sum, width, height, table, ending_x, ending_y);
            if (!up_path.empty()) {
                up_path.insert(up_path.begin(), {x, y});
                return up_path;
            }
        }

        if (is_valid(x, y - 1, width, height, table)) {
            vector<pair<int, int>> left_path = path(x, y - 1, steps, sum, width, height, table, ending_x, ending_y);
            if (!left_path.empty()) {
                left_path.insert(left_path.begin(), {x, y});
                return left_path;
            }
        }
    }

    return {};  // Empty vector if no valid path is found
}



int main() {
    //input and print the table
    cout<<"basic table|     width: ";
    int x;
    cin>>x;
    cout<<"basic table|     height: ";
    int y;
    cin>>y;
    cout<<endl;
    vector<vector<int>> table = generate_basic_table(x,y);
    print_table(table);
    cout<<endl;
    vector<pair<int, int>> result = path(0, 0, 0, 0, x, y, table, x - 1, y - 1);

    if (!result.empty()) {
        cout << "Path: ";
        for (const auto& step : result) {
            cout << "(" << step.first << ", " << step.second << ") ";
        }
        cout << endl;
    } else {
        cout << "No valid path found." << endl;
    }

    return 0;
}

