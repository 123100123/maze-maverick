#include <iostream>
#include <random>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <thread>
#include <filesystem>
#include <fstream>
#include <string>
#include <windows.h>
#include <unistd.h>
#include <thread>

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"

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


bool isPairInVector(const vector<pair<int, int>>& vec, const pair<int, int>& target) {
    for (const auto& element : vec) {
        if (element == target) {
            return true;
        }
    }
    return false;
}

void print_colored_table(const vector<vector<int>>& table,vector<pair<int,int>> &passed_blocks,int color){
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

    //green 0       blue 1

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            if(isPairInVector(passed_blocks,{i,j})){
                if(color == 0){
                    cout << GREEN <<setw(width) << table[i][j] << RESET<< " ";
                }else{
                    cout << BLUE <<setw(width) << table[i][j] << RESET<< " ";
                }
            }else{
                cout <<setw(width) << table[i][j]<< " ";
            }
        }
        cout << endl;
    }

}
void print_end_game_table(const vector<vector<int>>& table,vector<pair<int,int>> &passed_blocks,vector<pair<int,int>> &path,bool won){
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

    if(won){
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < table[i].size(); j++) {
                if(isPairInVector(passed_blocks,{i,j})){
                    cout << GREEN <<setw(width) << table[i][j] << RESET<< " ";
                }else{
                    cout <<setw(width) << table[i][j]<< " ";
                }
            }
            cout << endl;
        }
    }else{
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < table[i].size(); j++) {
                if(isPairInVector(passed_blocks,{i,j})){
                    if(isPairInVector(path,{i,j})){
                        cout << GREEN <<setw(width) << table[i][j] << RESET<< " ";
                    }else{
                        cout << RED <<setw(width) << table[i][j] << RESET<< " ";
                    }
                }else{
                    cout <<setw(width) << table[i][j]<< " ";
                }
            }
            cout << endl;
        }
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


    vector<pair<int,int>> empty_coordinates;
    for(int i = 0; i<= x-1;i++){
        for(int j=0; j <= y-1;j++){
            empty_coordinates.emplace_back(i,j);
        }
    }


    //generate where to go (right or down)
    vector<int> way = generate_the_way(x-1,y-1);

    //generate the path based on the way we got
    int sum = 0;
    int i=0,j=0;
    int generated = non_zero_element(-3,3);
    table[i][j] = generated;
    sum += generated;
    empty_coordinates.erase(empty_coordinates.begin());
    empty_coordinates.erase(empty_coordinates.begin()+empty_coordinates.size()-1);

    for(int m = 0; m < way.size()-1;m++){
        int d = way[m];

        if(d==0){
            j+=1;
        }else{
            i+=1;
        }
        empty_coordinates.erase(remove(empty_coordinates.begin(), empty_coordinates.end(), make_pair(j,i)), empty_coordinates.end());

        generated = non_zero_element(-3,3);
        sum += generated;
        table[i][j] = generated;
    }

    if(sum != 0){
        table[y-1][x-1] = sum ;
    }else{
        table[i][j]++;
        table[y-1][x-1] ++;
    }


    //generate the walls
    int wall_amount = min(rand_range(2,5),int(empty_coordinates.size()));

    random_device rd;
    mt19937 g(rd());
    shuffle(empty_coordinates.begin(), empty_coordinates.end(),g);

    for(int n = 0 ; n < wall_amount; n++){
        table[empty_coordinates[0].second][empty_coordinates[0].first] = 0;
        empty_coordinates.erase(empty_coordinates.begin());
    }



    for(pair<int,int> &z : empty_coordinates){
        table[z.second][z.first] = non_zero_element(-3,3);
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
    } else if(total_steps-steps+1 >= ((ending_x-x) + (ending_y - y))){
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


//generate the coordinates to reach the end
vector<pair<int, int>> generate_adv_path(int x, int y, int steps,const int& total_steps ,const int& width, const int& height, vector<vector<int>> table, const int& ending_x, const int& ending_y) {
    table[x][y] = 0;
    steps += 1;
    if ((x + 1 == ending_x && y == ending_y) || (x == ending_x && y + 1 == ending_y)) {
        if (steps == total_steps) {
            return {{x, y}};
        }
    } else if(steps<=total_steps && (total_steps-steps+1 >= ((ending_x-x) + (ending_y - y)))){
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

vector<vector<int>> generate_advanced_table(const int& x,const int& y,const int& min_wall,const int& max_wall,const int& min_num,const int& max_num,const int& total_steps){
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
        empty_coordinates.erase(remove(empty_coordinates.begin(), empty_coordinates.end(), make_pair(step.second,step.first)), empty_coordinates.end());

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
        pair<int,int> current = empty_coordinates[i];
        table[current.second][current.first] = 0;
        empty_coordinates.erase(empty_coordinates.begin());
    }

    for(pair<int,int> &z : empty_coordinates){
        table[z.second][z.first] = non_zero_element(min_num,max_num);
    }

    return table;
}

void stop_watch(string &time,int &total, vector<vector<int>> &table,vector<pair<int,int>> &passed, bool &ended){
    int sec = 0;
    int min = 0;
    while (!ended){
        sleep(1);
        total ++;
        if(sec <59){
            sec ++;
        }else{
            sec = 0;
            min ++;
        }

        string sec_str = to_string(sec);
        string min_str = to_string(min);

        if(sec_str.size() == 1){
            sec_str = '0' + sec_str;
        }
        if(min_str.size() == 1){
            min_str = '0' + min_str;
        }
        time = min_str + ":" + sec_str;

        system("cls");

        print_colored_table(table,passed,1);
        cout<<"w(up)  s(down)  d(right)  a(left)  z(go back)  q(quit) "<<endl;
        cout<<time<<endl;
    }
}


string date(){
    auto currentTime = chrono::system_clock::now();

    time_t time = chrono::system_clock::to_time_t(currentTime);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&time));

    return buffer;
}

void handle_endgame(string &mapName,string &username,string &time,bool &won,int &total_time){
    cout << total_time << endl;
    string result;
    if(won){
        result = "won";
    }else{
        result = "lost";
    }

    int previous = 0;
    for (const auto & entry : filesystem::directory_iterator("./history")) {
        previous ++;
    }

    for(int x = 2 ; x <= previous +1; x++){
        string last_name = "./history/" + to_string(x-1) + ".txt";
        string new_name = "./history/" + to_string(x) + ".txt";
        rename(last_name.c_str(),new_name.c_str());
    }
    if(filesystem::exists("./history/11.txt")){
        filesystem::remove("./history/0.txt");
    }

    string date_str = date();
    //cout<<username << " " << mapName << " " << time << " " << date_str << result;

    ofstream file("./history/"+to_string(1) +".txt");
    file << username << " " << mapName << " " << time << " " << date() << " " << result;
    file.close();

    //stats
    //total games
    //wins
    //last win
    //total play time

    if(filesystem::exists("./users/"+ username+".txt")){
        ifstream file1("./users/"+ username+".txt");

        string total_games_str,wins_str,last_win,play_time_str;

        file1 >> total_games_str >> wins_str >> last_win >> play_time_str;

        int total_games = stoi(total_games_str) +1;
        int wins = stoi(wins_str);
        last_win = date_str;
        int play_time = stoi(play_time_str) + total_time;

        if(won) {
            wins++;
            last_win = date_str;
        }

        file1.close();

        ofstream file2("./users/"+ username+".txt");
        file2 << total_games << endl << wins << endl << date_str << endl << play_time;

        file2.close();

    }else{
        ofstream file2("./users/"+ username+".txt");

        string wins;
        if(won){
            wins = "1";
        }else{
            wins = "0";
            date_str = "no-wins";
        }

        file2 << "1" << endl << wins << endl << date_str << endl << total_time;

        file2.close();
    }


}

void play(vector<vector<int>> &table,int& len,string &mapName){
    int width = table[0].size();
    int height = table.size();
    vector<pair<int, int>> path_found = path(0,0,0,0,len,width,height,table,height-1,width-1);
    path_found.emplace_back(height-1,width-1);


    while(true){
        bool ended = false;
        string time;
        int total;

        vector<pair<int, int>> passed = {
                {0, 0}
        };
        int x=0,y=0;


        char key;

        int move_index = 0;
        thread t1(stop_watch,ref(time),ref(total),ref(table),ref(passed),ref(ended));
        while (true){
            cout<<"w(up)  s(down)  d(right)  a(left)  z(go back)  q(quit) "<<endl;
            cout<<time<<endl;

            if(x == height-1 && y == width-1){
                break;
            }
            cin>>key;
            if(key=='a' || key== 'w' || key=='d' || key== 's'){
                bool changed = false;
                if(key == 'a' && is_valid(x,y-1,width,height,table) && !isPairInVector(passed,{x,y-1})){
                    y--;
                    changed =true;
                }else if(key == 'w' && is_valid(x-1,y,width,height,table) && !isPairInVector(passed,{x-1,y})){
                    x--;
                    changed =true;
                }else if(key == 'd' && is_valid(x,y+1,width,height,table) && !isPairInVector(passed,{x,y+1})){
                    y++;
                    changed =true;
                }else if(key == 's' && is_valid(x+1,y,width,height,table) && !isPairInVector(passed,{x+1,y})){
                    x++;
                    changed =true;
                }
                if(changed){
                    passed.emplace_back(x,y);
                }
            }
            else if(key == 'z' && passed.size()!=1){
                passed.pop_back();
                x = passed[passed.size()-1].first;
                y = passed[passed.size()-1].second;
            }
            else if(key == 'q'){
                t1.join();
                return;
            }

            system("cls");
            print_colored_table(table,passed,1);
        }
        ended = true;
        t1.join();
        bool won = true;
        if(passed.size() == path_found.size()){
            for(int i =0; i <= passed.size()-1;i++){
                if(passed[i] != path_found[i]){
                    won = false;
                    break;
                }
            }
        }else{
            won = false;
        }


        if(won){
            system("cls");
            cout<<"WON"<<endl;
        }else{
            system("cls");
            cout<<"LOST"<<endl;
        }

        print_end_game_table(table,passed,path_found,won);

        string username;
        cout<<"input username: ";
        cin >> username;

        handle_endgame(mapName,username,time,won, total);


        bool try_again = false;
        while(true){
            cout<<"wanna try again?(y/n): ";
            char response;
            cin >> response;
            if(response == 'y'){
                try_again = true;
                break;
            }else if(response == 'n'){
                try_again = false;
                break;
            }else{
                cout<<"wrong input"<<endl;
            }
        }
        if(!try_again){
            break;
        }
    }


}


string map_name(){
    vector<string> names;
    int x = 0;
    for (const auto & entry : filesystem::directory_iterator("./maps")) {
        x++;
        cout << x <<". " << entry.path().filename().string() << endl;
        names.push_back(entry.path().filename().string());
    }

    while (true){
        cout<<"choose a number(0 to go back): ";
        int num;
        cin >> num;
        system("cls");
        if(1 <= num && num <= x+1){
            return names[num-1];
        }else{
            cout<< "wrong input";
        }
    }
}

void import_maze(vector<vector<int>> &table, int len){
    int x = table[0].size();
    int y = table.size();

    if(!filesystem::is_directory("./maps")){
        filesystem::create_directory("./maps");
    }

    string name;
    while(true){
        cout<<"maze's name: "<<endl;
        cin>>name;
        if(filesystem::is_directory("./maps/"+name)){
            cout<<"name already exists "<<endl;
        }else{
            filesystem::create_directory("./maps/"+name);
            break;
        }
    }



    ofstream map("./maps/"+name+"/map.txt");
    map << x << " " << y << endl;
    for(vector<int> &i : table){
        for(int &j : i){
            map << j <<" ";
        }
    }
    map << endl <<len;

    map.close();
}

vector<vector<int>> input_maze(){
    system("cls");
    cout << "input width: ";
    int x;
    cin >> x;

    system("cls");
    cout << "input height: ";
    int y;
    cin >> y;

    system("cls");
    cout<<"input table: "<<endl;
    //input the table
    vector<vector<int>> table(y,vector<int>(x,0));
    for(int i = 0; i < y ;i++){
        for(int j = 0; j < x; j++){
            int inp;
            cin>>inp;
            table[i][j] = inp;
        }
    }
    system("cls");
    return table;
}

vector<vector<int>> read_maze(int &len,string &address){
    ifstream map(address);

    int x,y;
    map >> x >> y;

    vector<vector<int>> table(y, vector<int>(x));

    for(int i = 0; i< y; i ++){
        for(int j = 0; j < x; j++){
            map >> table[i][j];
        }
    }
    map >> len;
    map.close();
    return table;
}


void menu_generate_basic_maze(){
    system("cls");
    cout<<"input width and height(exp: x y): ";
    int x;
    int y;
    cin>>x;
    cin>>y;

    system("cls");
    vector<vector<int>> table = generate_basic_table(x,y);
    import_maze(table,x+y-2);

    system("cls");
    print_table(table);
    cout << endl << "0 to exit";
    cin >> x;
}

void menu_generate_advanced_maze(){
    int x,y;
    cout<<"input width and height(exp:x y): ";
    cin>>x>>y;

    system("cls");
    int min_wall,max_wall;
    cout<<"min and max amount of walls(exp:min max): ";
    cin>>min_wall>>max_wall;

    system("cls");
    int min_num,max_num;
    cout<<"numbers min and max(exp:min max): ";
    cin>>min_num>>max_num;

    system("cls");
    int path_len;
    cout<<"path length: ";
    cin>>path_len;


    system("cls");
    vector<vector<int>> table = generate_advanced_table(x,y,min_wall,max_wall,min_num,max_num,path_len);


    import_maze(table,path_len);
    system("cls");


    print_table(table);
    cout << endl << "0 to exit: ";
    cin >> x;
}

void menu_solve_advanced_maze(){
    system("cls");
    vector<vector<int>> table = input_maze();

    int len;
    cout<< "input the length of the path: ";
    cin>>len;

    int y = table.size();
    int x = table[0].size();

    vector<pair<int,int>> result = path(0,0,0,0,len,x,y,table,y-1,x-1);

    if(!result.empty()){
        result.emplace_back(y-1,x-1);
        print_colored_table(table,result,0);
    }else{
        cout<<"no path found";
    }
    cin >> x;

}

void menu_solve_basic_maze(){
    //input the table
    vector<vector<int>> table = input_maze();

    int x = table[0].size();
    int y = table.size();


    vector<pair<int, int>> result = path(0, 0, 0, 0,x+y-2, x, y, table, y - 1, x - 1);


    if(!result.empty()){
        result.emplace_back(y-1,x-1);
        print_colored_table(table,result,0);
    }else{
        cout<<"no path found";
    }

    cin >> x;
}

void menu_play_previous(){
    int len;
    string name = map_name();
    string dir = "./maps/" + name + "/map.txt";
    vector<vector<int>> table = read_maze(len,dir);

    play(table,len,name);
}

void menu_play_new(){
    vector<vector<int>> table = input_maze();

    cout<<"input path length: ";
    int len;
    cin>>len;

    string name = "No-Name";
    play(table,len,name);
}

void menu_solve_existing_maze(){
    string name = map_name();
    string dir = "./maps/" + name + "/map.txt";

    int len;
    vector<vector<int>> table = read_maze(len,dir);

    int x = table[0].size();
    int y = table.size();

    vector<pair<int, int>> result = path(0, 0, 0, 0,x+y-2, x, y, table, y - 1, x - 1);


    if(!result.empty()){
        result.emplace_back(y-1,x-1);
        print_colored_table(table,result,0);
    }else{
        cout<<"no path found";
    }

    cin >>x;
}

void menu_play_through_history(string &dir){
    int len;
    vector<vector<int>> table = read_maze(len,dir);

    string name = "No-Name";
    play(table,len,name);
}
void menu_history(){
    if(!filesystem::is_directory("./maps")){
        cout<<"no previous games";
    }else{
        while(true){
            system("cls");
            cout<<"1. show the history\n"
                  "2. clear history\n"
                  "choose an option(0 to go back): ";
            int choice;
            cin >> choice;
            if(choice == 1){
                system("cls");
                string name = map_name();
                cout << name <<endl;
                string dir = "./maps/" + name +"/map.txt";
                cout<<dir<<endl;
                menu_play_through_history(dir);
            }else if(choice == 2){
                while(true){
                    system("cls");
                    cout<<"are you sure?(y/n)";
                    char ch;
                    cin>> ch;
                    if(ch == 'y'){
                        filesystem::remove("./maps");
                    }else if(ch == 'n'){
                        break;
                    }
                }
            }else if(choice == 0){
                break;
                system("cls");
            }
        }
    }
}

void menu_play_history(){
    if(!filesystem::exists("./history/1.txt")){
        cout<<"no recent games";
    }

    int x = 0;
    for (const auto & entry : filesystem::directory_iterator("./history/")) {
        x++;

        ifstream file("./history/" + to_string(x) + ".txt");

        string username,mapName,date,result,time;

        file >> username >>mapName >> time >> date >> result;

        cout<< x <<". "<<username << " " << mapName << " " << time << " " << result << " " << date << endl;
    }
    cout<<"0 to go back: ";

    cin >> x;
    //system("cls");
}

void read_user(string &dir,string &username){
    system("cls");
    //stats
    //total games
    //wins
    //last win
    //total play time
    string total_games,wins,last_win,total_play_time;
    ifstream file(dir);

    file >> total_games >> wins >> last_win >> total_play_time;

    cout<< "name: " << username << endl;
    cout<< "games played: " << total_games <<endl;
    cout<< "games won: " << wins <<endl;
    cout<< "last win: " << last_win << endl;
    cout<< "play time: " << total_play_time << endl;

    int x;
    cin >> x;
}
void menu_users(){
    vector<string> names;


    while(true){
        int x = 0;
        system("cls");
        for (const auto & entry : filesystem::directory_iterator("./users")) {
            x++;
            string name =entry.path().filename().string();
            name = name.substr(0,name.size()-4);
            cout << x <<". " << name << endl;
            names.push_back(name);

        }
        cout<<"choose a user(0 to quit): ";
        int option;
        cin >> option;
        if(1 <= option && option <=x){
            option --;
            string dir = "./users/" + names[option] +".txt";
            read_user(dir,names[option]);
        }else if(option == 0){
            break;
        }
    }
}

void menu_welcome(){
    while (true){
        cout << "main menu";
        system("cls");
        cout<<"1. create a maze \n"
              "2. solve a maze \n"
              "3. playground \n"
              "4. history\n"
              "5. users \n"
              "6. exit\n"
              "choose an option: ";
        int choice;
        cin>>choice;

        system("cls");

        switch (choice) {
            case 1:
                while(true){
                    system("cls");
                    cout<<"1. basic maze\n"
                          "2. advanced maze\n"
                          "choose an option(0 to go back): ";
                    cin>>choice;
                    system("cls");
                    if(choice == 1){
                        menu_generate_basic_maze();
                    }else if(choice == 2){
                        menu_generate_advanced_maze();
                    }else if(choice == 0){
                        system("cls");
                        break;
                    }
                }
                break;
            case 2:
                while(true){
                    system("cls");
                    cout<<"1. solve a previous maze\n"
                          "2. solve a new maze\n"
                          "choose an option(0 to go back):";
                    cin >> choice;
                    system("cls");
                    if(choice == 1){
                        menu_solve_existing_maze();
                    }else if(choice == 2) {
                        while(true){
                            system("cls");
                            cout << "1. basic maze\n"
                                    "2. advanced maze\n"
                                    "choose an option(0 to go back): ";
                            cin >> choice;
                            if (choice == 1) {
                                menu_solve_basic_maze();
                            }
                            else if (choice == 2) {
                                menu_solve_advanced_maze();
                            }else if(choice == 0){
                                system("cls");
                                break;
                            }
                        }
                    }else if(choice == 0){
                        system("cls");
                        break;
                    }
                }
                break;
            case 3:
                while(true){
                    system("cls");
                    cout<<"1. play a previous maze\n"
                          "2. play a new maze\n"
                          "choose an option(0 to go back): ";
                    cin >> choice;

                    system("cls");
                    if(choice == 1){
                        menu_play_previous();
                    }else if(choice == 2){
                        menu_play_new();
                    }else if(choice == 0){
                        break;
                    }
                }
                break;
            case 4:
                //system("cls");
                //menu_history();
                system("cls");
                menu_play_history();
                break;

            case 5:
                menu_users();
                break;
            case 6:
                return;
            default:
                break;
        }
    }
}

int main() {
    if(!filesystem::is_directory("./history")) {
        filesystem::create_directory("./history");
    }
    if(!filesystem::is_directory("./users")) {
        filesystem::create_directory("./users");
    }
    menu_welcome();
    return 0;
}