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

void generate_basic_table(int x, int y){
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

    print_table(table);
}
int main() {
    cout<<"basic table|     width: ";
    int x;
    cin>>x;
    cout<<"basic table|     height: ";
    int y;
    cin>>y;
    generate_basic_table(x, y);
    cout<<endl;

    return 0;
}