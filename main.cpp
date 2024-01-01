#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <algorithm>

using namespace std;

//printing tables
void print_table(const vector<vector<int>>& table) {
    int rows = table.size();

    //find how much the space be
    int width = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            int len = to_string(table[i][j]).length();
            if (len > width) {
                width = len;
            }
        }
    }

    //print the table based on the space we need
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            cout << setw(width) << table[i][j] << " ";
        }
        cout << endl;
    }
}


//random number generation
int rand_range(int min, int max) {
    //seed
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> dis(min, max);

    return dis(gen);
}
//random number generation but overlooking 0
int non_zero_element(int min, int max) {
    int seed = 0;
    while (seed == 0) {
        seed = rand_range(min, max);
    }

    return seed;
}


vector<int> generate_the_way(int rights, int downs) {
    //in basic tables there should always be (x-1) rights and (y-1) downs to reach the end
    vector<int> resultVector;

    for (int i = 0; i < downs; ++i) {
        resultVector.push_back(1);
    }
    for (int i = 0; i < rights; ++i) {
        resultVector.push_back(0);
    }

    //shuffle
    random_device rd;
    mt19937 g(rd());
    shuffle(resultVector.begin(), resultVector.end(), g);
    return resultVector;
}

vector<vector<int>> generate_basic_table(int x, int y){
    //generate the table with all nines
    vector<vector<int>> table(y, vector<int>(x, 9));

    //generate where to go (right or down)
    vector<int> way = generate_the_way(x-1,y-1);

    //generate the path based on the way we got
    int sum = 0;
    int i=0,j=0;
    int generated = non_zero_element(-3,3);
    table[i][j] = generated;
    sum += generated;

    for(int d : way){
        //for each 0 we go right, for each 1 we go down
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
    int wall_amount = min(rand_range(2,5),x*y-(x+y-2));
    unordered_map<int,int> walls;
    for(int g = 1 ; g <=wall_amount; g++){
        while(true){
            //keeps randomly picking spots until it finds an empty block(9) to replace it with 0
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

    //make the rest of the blocks random
    for(int m = 0; m <= y-1;m++){
        for(int n = 0; n<= x-1;n++){
            if(table[m][n] == 9){
                table[m][n] = non_zero_element(-3,3);
            }
        }
    }

    return table;

}

//pathfinding
bool is_valid(const int& x, const int& y, const int& width, const int& height, const vector<vector<int>>& table) {
    return x >= 0 && x < height &&
           y >= 0 && y < width &&
           table[x][y] != 0;
}
vector<pair<int, int>> path(int x, int y, int steps, int sum,const int& total_steps ,const int& width, const int& height, vector<vector<int>> table, const int& ending_x, const int& ending_y) {
    //starts from (0,0) and keeps checking left,right, up and down for valid blocks
    sum += table[x][y];
    table[x][y] = 0;
    steps += 1;

    //stops if the next block(right or left) was the ending block
    if ((x + 1 == ending_x && y == ending_y) || (x == ending_x && y + 1 == ending_y)) {
        if ((steps == total_steps) && (sum == table[ending_x][ending_y])) {
            return {{x, y}};
        }
    } else if(steps<=total_steps){
        //down path
        if (is_valid(x + 1, y, width, height, table)) {
            vector<pair<int, int>> down_path = path(x + 1, y, steps, sum,total_steps, width, height, table, ending_x, ending_y);
            if (!down_path.empty()) {
                down_path.insert(down_path.begin(), {x, y});
                return down_path;
            }
        }
        //right path
        if (is_valid(x, y + 1, width, height, table)) {
            vector<pair<int, int>> right_path = path(x, y + 1, steps, sum,total_steps, width, height, table, ending_x, ending_y);
            if (!right_path.empty()) {
                right_path.insert(right_path.begin(), {x, y});
                return right_path;
            }
        }
        //up path
        if (is_valid(x - 1, y, width, height, table)) {
            vector<pair<int, int>> up_path = path(x - 1, y, steps, sum,total_steps, width, height, table, ending_x, ending_y);
            if (!up_path.empty()) {
                up_path.insert(up_path.begin(), {x, y});
                return up_path;
            }
        }
        //left_path
        if (is_valid(x, y - 1, width, height, table)) {
            vector<pair<int, int>> left_path = path(x, y - 1, steps, sum, total_steps,width, height, table, ending_x, ending_y);
            if (!left_path.empty()) {
                left_path.insert(left_path.begin(), {x, y});
                return left_path;
            }
        }
    }

    //return empty so that the way wouldn't continue to be checked
    return {};
}

//find the path in the basic table
void basic_table_path_finding(vector<vector<int>>& table){
    int y = table.size();
    int x = table[0].size();
    vector<pair<int, int>> result = path(0, 0, 0, 0,x+y-2, x, y, table, y - 1, x - 1);

    if (!result.empty()) {
        result.push_back(make_pair(y-1,x-1));
        cout << "Path: ";
        for (const auto& step : result) {
            cout << "(" << step.first << ", " << step.second << ") ";
        }
        cout << endl;
    } else {
        cout << "No valid path found." << endl;
    }
}

//basic-table-generation sub-menu
void SM_basic_table(){

    cout<<"1. generate a basic maze\n"
          "2. solve a basic maze\n"
          "choose an option: ";
    int choice;
    cin>>choice;

    if(choice == 2){
        cout<<endl;
        cout << "input width and height(exp: x y): ";
        int x;
        int y;
        cin >> x;
        cin >> y;

        //input the table
        vector<vector<int>> table(y,vector<int>(x,0));
        for(int i=0;i<y;i++){
            for(int j=0;j<x;j++){
                int inp;
                cin>>inp;
                table[i][j] = inp;
            }
        }
        basic_table_path_finding(table);
        cout<<endl;

    }else{
        //input the width and height and then print the table
        cout<<endl<<"input width and height(exp: x y): ";
        int x;
        int y;
        cin>>x;
        cin>>y;

        cout<<endl;
        vector<vector<int>> table = generate_basic_table(x,y);
        print_table(table);

        cout<<endl;
        basic_table_path_finding(table);
    }
}

//generate the coordinates to reach the end
vector<pair<int, int>> generate_adv_path(int x, int y, int steps,const int& total_steps ,const int& width, const int& height, vector<vector<int>> table, const int& ending_x, const int& ending_y) {
    table[x][y] = 0;
    steps += 1;
    if ((x + 1 == ending_x && y == ending_y) || (x == ending_x && y + 1 == ending_y)) {
        if (steps == total_steps) {
            return {{x, y}};
        }
    } else if(steps<=total_steps){
        // randomly choose which way to check(up, down, left, or right)

        int random_num = rand_range(1,4);
        //check orders are listed below and the algorithm is like the pathfinding

        if(random_num == 1){                                         //down,up,right,left
            if (is_valid(x + 1, y, width, height, table)) {
                vector<pair<int, int>> down_path = generate_adv_path(x + 1, y, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!down_path.empty()) {
                    down_path.insert(down_path.begin(), {x, y});
                    return down_path;
                }
            }

            if (is_valid(x - 1, y, width, height, table)) {
                vector<pair<int, int>> up_path = generate_adv_path(x - 1, y, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!up_path.empty()) {
                    up_path.insert(up_path.begin(), {x, y});
                    return up_path;
                }
            }

            if (is_valid(x, y + 1, width, height, table)) {
                vector<pair<int, int>> right_path = generate_adv_path(x, y + 1, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!right_path.empty()) {
                    right_path.insert(right_path.begin(), {x, y});
                    return right_path;
                }
            }

            if (is_valid(x, y - 1, width, height, table)) {
                vector<pair<int, int>> left_path = generate_adv_path(x, y - 1, steps, total_steps,width, height, table, ending_x, ending_y);
                if (!left_path.empty()) {
                    left_path.insert(left_path.begin(), {x, y});
                    return left_path;
                }
            }

        }
        else if(random_num == 2){                                  //left,up,right,down
            if (is_valid(x, y - 1, width, height, table)) {
                vector<pair<int, int>> left_path = generate_adv_path(x, y - 1, steps, total_steps,width, height, table, ending_x, ending_y);
                if (!left_path.empty()) {
                    left_path.insert(left_path.begin(), {x, y});
                    return left_path;
                }
            }

            if (is_valid(x + 1, y, width, height, table)) {
                vector<pair<int, int>> down_path = generate_adv_path(x + 1, y, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!down_path.empty()) {
                    down_path.insert(down_path.begin(), {x, y});
                    return down_path;
                }
            }

            if (is_valid(x - 1, y, width, height, table)) {
                vector<pair<int, int>> up_path = generate_adv_path(x - 1, y, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!up_path.empty()) {
                    up_path.insert(up_path.begin(), {x, y});
                    return up_path;
                }
            }

            if (is_valid(x, y + 1, width, height, table)) {
                vector<pair<int, int>> right_path = generate_adv_path(x, y + 1, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!right_path.empty()) {
                    right_path.insert(right_path.begin(), {x, y});
                    return right_path;
                }
            }

        }else if(random_num == 3){                                   //right,left,down,up
            if (is_valid(x, y + 1, width, height, table)) {
                vector<pair<int, int>> right_path = generate_adv_path(x, y + 1, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!right_path.empty()) {
                    right_path.insert(right_path.begin(), {x, y});
                    return right_path;
                }
            }

            if (is_valid(x, y - 1, width, height, table)) {
                vector<pair<int, int>> left_path = generate_adv_path(x, y - 1, steps, total_steps,width, height, table, ending_x, ending_y);
                if (!left_path.empty()) {
                    left_path.insert(left_path.begin(), {x, y});
                    return left_path;
                }
            }

            if (is_valid(x + 1, y, width, height, table)) {
                vector<pair<int, int>> down_path = generate_adv_path(x + 1, y, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!down_path.empty()) {
                    down_path.insert(down_path.begin(), {x, y});
                    return down_path;
                }
            }

            if (is_valid(x - 1, y, width, height, table)) {
                vector<pair<int, int>> up_path = generate_adv_path(x - 1, y, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!up_path.empty()) {
                    up_path.insert(up_path.begin(), {x, y});
                    return up_path;
                }
            }

        }else{                                                       //right,left,down,up
            if (is_valid(x, y + 1, width, height, table)) {
                vector<pair<int, int>> right_path = generate_adv_path(x, y + 1, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!right_path.empty()) {
                    right_path.insert(right_path.begin(), {x, y});
                    return right_path;
                }
            }

            if (is_valid(x, y - 1, width, height, table)) {
                vector<pair<int, int>> left_path = generate_adv_path(x, y - 1, steps, total_steps,width, height, table, ending_x, ending_y);
                if (!left_path.empty()) {
                    left_path.insert(left_path.begin(), {x, y});
                    return left_path;
                }
            }

            if (is_valid(x + 1, y, width, height, table)) {
                vector<pair<int, int>> down_path = generate_adv_path(x + 1, y, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!down_path.empty()) {
                    down_path.insert(down_path.begin(), {x, y});
                    return down_path;
                }
            }

            if (is_valid(x - 1, y, width, height, table)) {
                vector<pair<int, int>> up_path = generate_adv_path(x - 1, y, steps,total_steps, width, height, table, ending_x, ending_y);
                if (!up_path.empty()) {
                    up_path.insert(up_path.begin(), {x, y});
                    return up_path;
                }
            }
        }
    }

    //returns empty not to follow the wrong path
    return {};
}

void advanced_table_pathfinding(const vector<vector<int>>& table,const int total_steps){
    //path finding on the generated table
    int y = table.size();
    int x = table[0].size();
    vector<pair<int,int>> result = path(0,0,0,0,total_steps,x,y,table,y-1,x-1);
    if (!result.empty()) {
        result.emplace_back(y-1,x-1);
        cout << "Path: ";
        for (const auto& step : result) {
            cout << "(" << step.first << ", " << step.second << ") ";
        }
        cout << endl;
    } else {
        cout << "No valid path found." << endl;
    }
}

void generate_advanced_table(const int& x,const int& y,const int& min_wall,const int& max_wall,const int& min_num,const int& max_num,const int& total_steps){
    int empty_num = max_num +1;  //what to fill the first iteration with
    if(empty_num==0){
        empty_num++;
    }
    vector<pair<int,int>> empty_coordinates;  //keep track of empty blocks
    for(int i = 0; i<= x-1;i++){
        for(int j=0; j <= y-1;j++){
            empty_coordinates.emplace_back(i,j);
        }
    }


    vector<vector<int>> table(y, vector<int>(x, empty_num));    //table layout
    vector<pair<int,int>> way = generate_adv_path(0,0,0,total_steps,x,y,table,y-1,x-1);

    int sum =0;
    for(const pair<int,int>& step : way){
        //remove the filled blocks from the empty set
        empty_coordinates.erase(remove(empty_coordinates.begin(), empty_coordinates.end(), step), empty_coordinates.end());

        int random_num = non_zero_element(min_num,max_num);
        //cout<<step.first<<" "<<step.second<<" /"<<random_num<<endl;
        sum += random_num;
        table[step.first][step.second] = random_num;
    }

    //the last block shouldn't be 0
    if(sum==0){
        sum = 1;
        table[way[way.size()-1].first][way[way.size()-1].second] -= 1;
    }
    empty_coordinates.pop_back();
    table[y-1][x-1] = sum;

    int wall_amount = min(rand_range(min_wall,max_wall),x*y-total_steps);

    //shuffle the empty coords
    random_device rd;
    mt19937 g(rd());
    shuffle(empty_coordinates.begin(), empty_coordinates.end(),g);

    //fill the shuffled list of empty blocks with 0
    for(int i = 0 ; i<wall_amount;i++){
        pair<int,int> current = empty_coordinates[0];
        table[current.first][current.second] = 0;
        empty_coordinates.erase(empty_coordinates.begin());
    }

    //fill the rest of the blocks with random numbers
    for(const pair<int,int> i : empty_coordinates){
        table[i.first][i.second] = non_zero_element(min_num,max_num);
    }
    cout<<endl;
    print_table(table);

    advanced_table_pathfinding(table,total_steps);
}


void SM_advanced_table(){
    int choice;
    cout<<"1. generate an advanced maze\n"
          "2. solve an advanced maze\n"
          "choose an option: ";
    cin>>choice;
    if(choice == 1){
        //vector<vector<int>> table;

        int x,y;
        cout<<"input width and height(exp:x y): ";
        cin>>x>>y;

        int min_wall,max_wall;
        cout<<"\nmin and max amount of walls(exp:min max): ";
        cin>>min_wall>>max_wall;

        int min_num,max_num;
        cout<<"\nnumbers min and max(exp:min max): ";
        cin>>min_num>>max_num;

        int path_len;
        cout<<"\npath length: ";
        cin>>path_len;

        generate_advanced_table(x,y,min_wall,max_wall,min_num,max_num,path_len);
    }else{
        cout<<endl;
        cout << "input width and height(exp: x y): ";
        int x;
        int y;
        cin >> x;
        cin >> y;

        cout<< "\ninput the length of the path: ";
        int len;
        cin>>len;
        //input the table
        vector<vector<int>> table(y,vector<int>(x,0));
        for(int i=0;i<y;i++){
            for(int j=0;j<x;j++){
                int inp;
                cin>>inp;
                table[i][j] = inp;
            }
        }
        advanced_table_pathfinding(table,len);
        //print_table(table);
        cout<<endl;

    }
}

int main() {
    //generate_advanced_table(5,5,1,3,-1,10,22);

    cout<<"1. basic maze \n"
          "2. advanced maze \n"
          "choose an option: ";
    int choice;
    cin>>choice;
    cout<<endl;
    switch (choice) {
        case 1:
            SM_basic_table();
            break;

        case 2:
            SM_advanced_table();
        default:
            break;
    }

    return 0;
}