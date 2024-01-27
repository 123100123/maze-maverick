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
#include <cctype>

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"

//Printing the Blank White Table
void print_table(const vector<vector<int>>& table) {
    int rows = table.size();

    // Calculating the Space Sizes Between Each Number
    int width = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            int len = to_string(table[i][j]).length();
            if (len > width) {
                width = len;
            }
        }
    }

    //Printing the Table Using the Spaces Found
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            cout << right << setw(width) << table[i][j] << " ";
        }
        cout << endl;
    }
}

//Printing blank string tables
void print_string_table(const vector<vector<string>>& table) {
    int rows = table.size();

    //find how much the space be
    int width = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            int len = table[i][j].length();
            if (len > width) {
                width = len;
            }
        }
    }

    //print the table based on the space we need
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            if(j > 0){
                cout << left << setw(width) << table[i][j] << " ";
            }else{
                cout << left << setw(4) << table[i][j];
            }

        }
        cout << endl;
    }
}


// Checking if a Certain Iterated Element is in the Vector
bool isPairInVector(const vector<pair<int, int>>& vec, const pair<int, int>& target) {
    for (const auto& element : vec) {
        if (element == target) {
            return true;
        }
    }
    return false;
}

// One Function To Print The Solved Maze And The Maze While Playing
void print_colored_table(const vector<vector<int>>& table,vector<pair<int,int>> &passed_blocks,int color){
    int rows = table.size();

    // Calculating the Space Sizes Between Each Number
    int width = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            int len = to_string(table[i][j]).length();
            if (len > width) {
                width = len;
            }
        }
    }

    // Green --> 0   Blue --> 1

    // Green For The Solved Maze And Blue While The Player Is Playing
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            if(isPairInVector(passed_blocks,{i,j})){
                if(color == 0){
                    cout << right  << GREEN <<setw(width) << table[i][j] << RESET<< " ";
                }else{
                    cout << right  << BLUE <<setw(width) << table[i][j] << RESET<< " ";
                }
            }else{
                cout << right  <<setw(width) << table[i][j]<< " ";
            }
        }
        cout << endl;
    }

}

// Function For Printing The Table After The Game Has Been Played
void print_end_game_table(const vector<vector<int>>& table,vector<pair<int,int>> &passed_blocks,vector<pair<int,int>> &path,bool won){
    int rows = table.size();

    // Calculating the Space Sizes Between Each Number
    int width = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < table[i].size(); j++) {
            int len = to_string(table[i][j]).length();
            if (len > width) {
                width = len;
            }
        }
    }

    // Printing the Correct Path Green 
    if(won){
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < table[i].size(); j++) {
                if(isPairInVector(passed_blocks,{i,j})){
                    cout << right  << GREEN <<setw(width) << table[i][j] << RESET<< " ";
                }else{
                    cout << right  <<setw(width) << table[i][j]<< " ";
                }
            }
            cout << endl;
        }
    }
    //Printing the Correct Steps Green And The Wrong Ones Red
    else{
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < table[i].size(); j++) {
                if(isPairInVector(passed_blocks,{i,j})){
                    if(isPairInVector(path,{i,j})){
                        cout << right  << GREEN <<setw(width) << table[i][j] << RESET<< " ";
                    }else{
                        cout << right  << RED <<setw(width) << table[i][j] << RESET<< " ";
                    }
                }else{
                    cout << right  <<setw(width) << table[i][j]<< " ";
                }
            }
            cout << endl;
        }
    }

}

// Random Number Generator 
int rand_range(int min, int max) {
    // Seed
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

// Random Number Generator Disregarding 0
int non_zero_element(int min, int max) {
    int seed = 0;
    while (seed == 0) {
        seed = rand_range(min, max);
    }

    return seed;
}

// Path Finding For Basic Maze
vector<int> generate_the_way(int rights, int downs) {
    // In The Basic Maze You Should Go (x-1) Steps Down And (y-1) Steps Right
    // We Generated A Vector Of 1's And 0's And Shuffled Them To Make The Path 
    vector<int> resultVector;

    for (int i = 0; i < downs; ++i) {
        resultVector.push_back(1);
    }
    for (int i = 0; i < rights; ++i) {
        resultVector.push_back(0);
    }

    // Shuffle 
    random_device rd;
    mt19937 g(rd());
    shuffle(resultVector.begin(), resultVector.end(), g);
    return resultVector;
}

//Generating The Basic Table
vector<vector<int>> generate_basic_table(int x, int y){
    // First We Generate A Table with All The Cell Values Being 9 
    vector<vector<int>> table(y, vector<int>(x, 9));

    vector<pair<int,int>> empty_coordinates;
    for(int i = 0; i<= x-1;i++){
        for(int j=0; j <= y-1;j++){
            empty_coordinates.emplace_back(i,j);
        }
    }

    // Generating The Random Path
    vector<int> way = generate_the_way(x-1,y-1);

    // Filling The Cells In The Path With Numbers And Calculating Sum
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

    //Sum Of All The Cell Values Not Being 0
    if(sum != 0){
        table[y-1][x-1] = sum ;
    }else{
        table[i][j]++;
        table[y-1][x-1] ++;
    }

    // Adding The Walls To The Cells Not In The Path
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

// Checking If The Steps Taken Are In The Boundaries Of The Table
bool is_valid(const int& x, const int& y, const int& width, const int& height, const vector<vector<int>>& table) {
    return x >= 0 && x < height &&
           y >= 0 && y < width &&
           table[x][y] != 0;
}

// Recursive DFS Algorithm For Finding Advanced Path  
vector<pair<int, int>> path(int x, int y, int steps, int sum,const int& total_steps ,const int& width, const int& height, vector<vector<int>> table, const int& ending_x, const int& ending_y) {
    // Going From The Start Position , Checking -left Right Up Down- For A Path
    sum += table[x][y];
    table[x][y] = 0;
    steps += 1;

    // Stops If End Has Been Reached
    if ((x + 1 == ending_x && y == ending_y) || (x == ending_x && y + 1 == ending_y)) {
        if ((steps == total_steps) && (sum == table[ending_x][ending_y])) {
            return {{x, y}};
        }
    } else if(total_steps-steps+1 >= ((ending_x-x) + (ending_y - y))){
        // Down
        if (is_valid(x + 1, y, width, height, table)) {
            vector<pair<int, int>> down_path = path(x + 1, y, steps, sum,total_steps, width, height, table, ending_x, ending_y);
            if (!down_path.empty()) {
                down_path.insert(down_path.begin(), {x, y});
                return down_path;
            }
        }
        // Right
        if (is_valid(x, y + 1, width, height, table)) {
            vector<pair<int, int>> right_path = path(x, y + 1, steps, sum,total_steps, width, height, table, ending_x, ending_y);
            if (!right_path.empty()) {
                right_path.insert(right_path.begin(), {x, y});
                return right_path;
            }
        }
        // Up
        if (is_valid(x - 1, y, width, height, table)) {
            vector<pair<int, int>> up_path = path(x - 1, y, steps, sum,total_steps, width, height, table, ending_x, ending_y);
            if (!up_path.empty()) {
                up_path.insert(up_path.begin(), {x, y});
                return up_path;
            }
        }
        // Left
        if (is_valid(x, y - 1, width, height, table)) {
            vector<pair<int, int>> left_path = path(x, y - 1, steps, sum, total_steps,width, height, table, ending_x, ending_y);
            if (!left_path.empty()) {
                left_path.insert(left_path.begin(), {x, y});
                return left_path;
            }
        }
    }
    // Returning An Empty Vector If A Path Not Found
    return {};
}


// Generating The Path For The Advanced Part Of The Maze Using DFS Algorithm
vector<pair<int, int>> generate_adv_path(int x, int y, int steps,const int& total_steps ,const int& width, const int& height, vector<vector<int>> table, const int& ending_x, const int& ending_y) {
    table[x][y] = 0;
    steps += 1;
    // Stoping When End has Been Reached 
    if ((x + 1 == ending_x && y == ending_y) || (x == ending_x && y + 1 == ending_y)) {
        if (steps == total_steps) {
            return {{x, y}};
        }
    } else if(steps<=total_steps && (total_steps-steps+1 >= ((ending_x-x) + (ending_y - y)))) {
        // randomly choose which way to check(up, down, left, or right)

        vector<int> random_nums = {1, 2, 3, 4};
        random_device rd;
        mt19937 g(rd());
        shuffle(random_nums.begin(), random_nums.end(), g);

        for (int &num: random_nums) {
            if (num == 1) {
                if (is_valid(x + 1, y, width, height, table)) {
                    vector<pair<int, int>> down_path = generate_adv_path(x + 1, y, steps, total_steps, width, height,
                                                                         table, ending_x, ending_y);
                    if (!down_path.empty()) {
                        down_path.insert(down_path.begin(), {x, y});
                        return down_path;
                    }
                }
            } else if (num == 2) {
                if (is_valid(x - 1, y, width, height, table)) {
                    vector<pair<int, int>> up_path = generate_adv_path(x - 1, y, steps, total_steps, width, height,
                                                                       table, ending_x, ending_y);
                    if (!up_path.empty()) {
                        up_path.insert(up_path.begin(), {x, y});
                        return up_path;
                    }
                }
            } else if (num == 3) {
                if (is_valid(x, y + 1, width, height, table)) {
                    vector<pair<int, int>> right_path = generate_adv_path(x, y + 1, steps, total_steps, width, height,
                                                                          table, ending_x, ending_y);
                    if (!right_path.empty()) {
                        right_path.insert(right_path.begin(), {x, y});
                        return right_path;
                    }
                }
            } else if (num == 4) {
                if (is_valid(x, y - 1, width, height, table)) {
                    vector<pair<int, int>> left_path = generate_adv_path(x, y - 1, steps, total_steps, width, height,
                                                                         table, ending_x, ending_y);
                    if (!left_path.empty()) {
                        left_path.insert(left_path.begin(), {x, y});
                        return left_path;
                    }
                }
            }
        }
    }

    // Returing Empty Vector If A Path Not Found
    return {};
}

// Generating The Advanced Table And It's Path
vector<vector<int>> generate_advanced_table(const int& x,const int& y,const int& min_wall,const int& max_wall,const int& min_num,const int& max_num,const int& total_steps){
    
    int empty_num = max_num;  //what to fill the first iteration with
    if(empty_num==0){
        empty_num++;
    }

    // First Keeping Track Of All Coordinates
    vector<pair<int,int>> empty_coordinates;

    for(int i = 0; i<= x-1;i++){
        for(int j=0; j <= y-1;j++){
            empty_coordinates.emplace_back(i,j);
        }
    }

    // First Filling All The Cells With The 'empty_num' Value Then Generating The Path
    vector<vector<int>> table(y, vector<int>(x, empty_num));
    vector<pair<int,int>> way = generate_adv_path(0,0,0,total_steps,x,y,table,y-1,x-1);

    // Fill The Path With Numbers Between Max And Min Value Given For Each Cell
    // Keep Track Of The Sum Value
    int sum=0;
    for(const pair<int,int>& step : way){
        // Remove The Coordinates Of The Path From The Vector 'empty_coordinates'
        empty_coordinates.erase(remove(empty_coordinates.begin(), empty_coordinates.end(), make_pair(step.second,step.first)), empty_coordinates.end());

        // As Said In The Beggining
        int random_num = non_zero_element(min_num,max_num);
        sum += random_num;
        table[step.first][step.second] = random_num;
    }

    // The Sum Shouldn't Equal 0
    if(sum==0){
        sum = 1;
        table[way[way.size()-1].first][way[way.size()-1].second] -= 1;
    }
    empty_coordinates.pop_back();
    table[y-1][x-1] = sum;

    // Shuffle The Rest Of The Cell's Coordinates And Put Walls In Random Positions
    int wall_amount = min(rand_range(min_wall,max_wall),x*y-total_steps);
    random_device rd;
    mt19937 g(rd());
    shuffle(empty_coordinates.begin(), empty_coordinates.end(),g);

    // Fill The Cells With Walls Or 0's
    for(int i = 0 ; i<wall_amount;i++){
        pair<int,int> current = empty_coordinates[i];
        table[current.second][current.first] = 0;
        empty_coordinates.erase(empty_coordinates.begin());
    }

    // Fill The Rest Of The Non-Path And Non-Wall Cells With Random Numbers
    for(pair<int,int> &z : empty_coordinates){
        table[z.second][z.first] = non_zero_element(min_num,max_num);
    }

    return table;
}

void stop_watch(string &time, vector<vector<int>> &table,vector<pair<int,int>> &passed, bool &ended){
    int sec = 0;
    int min = 0;
    while (!ended){
        sleep(1);
        if(sec <59){
            sec ++;
        }else{
            sec = 0;
            min ++;
        }

        string sec_str = to_string(sec);
        string min_str = to_string(min);

        if(sec_str.size() == 1){
            sec_str = to_string(0) + sec_str;
        }
        if(min_str.size() == 1){
            min_str = to_string(0) + min_str;
        }
        time = min_str + ":" + sec_str;

        system("cls");

        print_colored_table(table,passed,1);
        cout<<" w(Up)  s(Down)  d(Right)  a(Left)  z(Go Back)  q(Quit) "<<endl;
        cout<<time<<endl;
    }
}

// Storing The Current Date
string date(){
    auto currentTime = chrono::system_clock::now();

    time_t time = chrono::system_clock::to_time_t(currentTime);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&time));

    return buffer;
}

// Checking If The Value Entered Is A Number
bool is_number(string &entery){
    for (int i = 0; i < entery.size(); i++)
    {
        if(entery[i] == ' ' || (!(isdigit(entery[i])) && !(i == 0 && entery[0] == '-'))){
            return false;
        }
    }
    return true;
}

// Substituting Cin And Checking For Number 
int get_number(string question){
    int y;
    while (true){
        cout<<question;
        string x;
        getline(cin >> ws,x);
        if(is_number(x)){
            y = stoi(x);
            break;
        }else{
            system("cls");
        }
    }
    return y;
}

//Entering Smth To Continue
void input_to_exit(){
    while(true){
        cout<<"Input To Exit : ";
        string x;
        cin>>x;
        break;
    }
}

// Storing The Users Playing Information After He Played (MapName,UserName,Date,WinStatus,TotalTime)
void handle_endgame(string &mapName,string &username,string &time,bool &won,int &total_time){
    string result;
    if(won){
        result = "Won";
    }else{
        result = "Lost";
    }

    int previous = 0;
    for (auto & entry : filesystem::directory_iterator("./")) {
        previous ++;
    }

    for(int x = previous ; x >=1; x--){
        string last_name = "./history/" + to_string(x) + ".txt";
        string new_name = "./history/" + to_string(x+1) + ".txt";

        rename(last_name.c_str(),new_name.c_str());
    }

    if(filesystem::exists("./history/11.txt")){
        filesystem::remove("./history/11.txt");
    }

    string date_str = date();

    ofstream file("./history/"+to_string(1) +".txt");
    file << username << endl << mapName << endl << time << endl << date() << endl << result;
    file.close();

    // Stats
    // Total Games
    // Wins
    // Last Win
    // Total Play Time

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

// A User Playing The Game With A Table
void play(vector<vector<int>> &table,int& len,string &mapName){
    int width = table[0].size();
    int height = table.size();
    vector<pair<int, int>> path_found = path(0,0,0,0,len,width,height,table,height-1,width-1);
    if(path_found.empty()){
        system("cls");
        cout << " there is no path in this maze! " << endl;
        input_to_exit();
        return;
    }
    path_found.emplace_back(height-1,width-1);

    auto start_time = chrono::high_resolution_clock::now();
    while(true){
        bool ended = false;
        string time;

        int sum = 0;

        vector<pair<int, int>> passed = {
            {0, 0}
        };
        int x=0,y=0;

        char key;

        thread t1(stop_watch, ref(time), ref(table), ref(passed), ref(ended));
        while (true){
            cout<<" w(Up)  s(Down)  d(Right)  a(Left)  z(Go Back)  q(Quit) "<<endl;
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
                ended = true;
                t1.join();
                return;
            }

            system("cls");
            print_colored_table(table,passed,1);
        }
        ended = true;
        t1.join();

        auto end_time = chrono::high_resolution_clock::now();
        int duration = chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

        for(pair<int,int> &block : passed){
            sum += table[block.first][block.second];
        }
        sum -= table[height-1][width-1];
        bool won = (passed.size()-1 == len) && (sum == table[height-1][width-1]);



        system("cls");

        if(won){
            cout<<"WON"<<endl;
        }else{

            cout<<"LOST"<<endl;
        }

        print_end_game_table(table,passed,path_found,won);

        string username;
        cout<<"input username: ";
        getline(cin >> ws, username);

        handle_endgame(mapName, username, time, won, duration);

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

// Getting And Adding The Maps Names And Adding Them To The 'maps' File
string map_name(){
    system("cls");
    vector<string> names;
    int x = 0;
    for (const auto & entry : filesystem::directory_iterator("./maps")) {
        x++;
        cout << x <<". " << entry.path().filename().string() << endl;
        names.push_back(entry.path().filename().string());
    }
    if(x == 0){
        system("cls");
        return "!@#";
    }
    while (true){
        int num = get_number("choose a number(0 to go back): ");
        if(1 <= num && num <= x+1){
            return names[num-1];
        }else if(num == 0){
            return "!@#$";
        }
    }
}

string global_name;
void export_maze(vector<vector<int>> &table, int len){
    int x = table[0].size();
    int y = table.size();

    if(!filesystem::is_directory("./maps")){
        filesystem::create_directory("./maps");
    }

    string name;
    while(true){
        cout<<"maze's name: "<<endl;
        getline(cin >> ws, name);
        if(filesystem::is_directory("./maps/"+name)){
            cout<<"name already exists "<<endl;
        }else{
            filesystem::create_directory("./maps/"+name);
            break;
        }
    }

    global_name = name;

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

// Function That Allows The User To Import A Chosen Map
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

// Reading An Existing Map From The 'maps' File
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

// Menu And The UI Shown To The User For Generating Basic Maze
void menu_generate_basic_maze(){
    system("cls");
    int width;
    while(true){
        width = get_number("input width: ");
        if(width >= 3){
            break;
        }else{
            cout<<"wrong input" <<endl;
        }
    }

    system("cls");

    int height;
    while(true){
        height = get_number("input height: ");
        if(height >= 3){
            break;
        }else{
            cout << "wrong input";
        }
    }

    system("cls");

    vector<vector<int>> table = generate_basic_table(width,height);
    export_maze(table, width + height - 2);

    system("cls");
    print_table(table);

    input_to_exit();
}

// Menu And The UI Shown To The User For Generating Advanced Maze
void menu_generate_advanced_maze(){
    system("cls");
    int width;
    while(true){
        width = get_number("input width: ");
        if(width >=3 ){
            break;
        }else{
            system("cls");
        }
    }


    system("cls");
    int height;

    while(true){
        height = get_number("input height: ");
        if(height >=3 ){
            break;
        }else{
            system("cls");
        }
    }

    system("cls");
    int path_len;
    while (true){
        path_len = get_number("input path length:");

        int max = width*height-1;
        int val = path_len - (width + height - 2);

        if(val >= 0 && val%2 == 0 && path_len <= max){
            break;
        }else{
            cout<< "wrong value" << endl;
        }
    }

    int min_wall,max_wall;
    while(true){
        jump:
        system("cls");

        min_wall = get_number("input minimum amount of walls: ");

        if(min_wall < 0){
            goto jump;
        }

        jump2:
        system("cls");

        max_wall = get_number("input maximum amount of walls: ");

        if(max_wall < 0 && max_wall > (width*height-1 -path_len) ){
            goto jump2;
        }

        if(max_wall >= min_wall){
            break;
        }else{
            cout << "wrong values" <<endl;
        }
    }

    system("cls");
    int min_num,max_num;
    while(true){

        min_num = get_number("input minimum value: ");

        system("cls");

        max_num = get_number("input maximum value: ");

        if(max_num >= min_num){
            break;
        }else{
            cout << "wrong values"<< endl;
        }
    }
    system("cls");
    vector<vector<int>> table = generate_advanced_table(width,height,min_wall,max_wall,min_num,max_num,path_len);


    export_maze(table, path_len);
    system("cls");

    print_table(table);
    while(true){
        cout<<"input to exit: ";
        string x;
        cin>>x;
        break;
    }
}

// Menu And The UI Shown To The User For Solving The Advanced Maze
void menu_solve_advanced_maze(){
    system("cls");
    // Getting The Table
    vector<vector<int>> table = input_maze();

    int len = get_number("input the length of the path: ");

    system("cls");

    export_maze(table, len);
    system("cls");

    int y = table.size();
    int x = table[0].size();

    vector<pair<int,int>> result = path(0,0,0,0,len,x,y,table,y-1,x-1);

    if(!result.empty()){
        result.emplace_back(y-1,x-1);
        print_colored_table(table,result,0);
    }else{
        cout<<"no path found" << endl;
    }

    input_to_exit();
}

// Menu And The UI Shown To The User For Solving The Basic Maze
void menu_solve_basic_maze(){
    // Getting The Table
    vector<vector<int>> table = input_maze();

    int x = table[0].size();
    int y = table.size();

    system("cls");

    export_maze(table, x + y - 2);
    system("cls");

    vector<pair<int, int>> result = path(0, 0, 0, 0,x+y-2, x, y, table, y - 1, x - 1);

    if(!result.empty()){
        result.emplace_back(y-1,x-1);
        print_colored_table(table,result,0);
    }else{
        cout<<"no path found";
    }

    input_to_exit();
}

// Menu And The UI Shown To The User For Selecting A Map From The Previous Stored Ones
void menu_play_previous(){
    int len;
    string name = map_name();
    // Quit If 0
    if(name == "!@#$"){
        return;
    }
    // If No Maps Were Found
    if(name == "!@#"){
        cout << "no maps found!" <<endl;
        input_to_exit();
        return;
    }
    string dir = "./maps/" + name + "/map.txt";
    vector<vector<int>> table = read_maze(len,dir);

    play(table,len,name);
}

// Playing A New Table Given By User
void menu_play_new(){
    vector<vector<int>> table = input_maze();
    int width = table[0].size();
    int height = table.size();
    int path_len;
    while (true){
        path_len = get_number("input path length:");

        int max = width*height-1;
        int val = path_len - (width + height - 2);

        if(val >= 0 && val%2 == 0 && path_len <= max){
            break;
        }else{
            cout<< "wrong value" << endl;
        }
    }


    system("cls");
    export_maze(table, path_len);

    system("cls");

    play(table,path_len,global_name);
}

// Solving A Maze With Has Existed
void menu_solve_existing_maze(){
    system("cls");
    string name = map_name();
    // Quit If 0
    if(name == "!@#$"){
        return;
    }
    // If No Maps Were Found
    if(name == "!@#"){
        cout << "no maps found!" <<endl;
        input_to_exit();
        return;
    }
    string dir = "./maps/" + name + "/map.txt";

    int len;
    vector<vector<int>> table = read_maze(len,dir);

    int x = table[0].size();
    int y = table.size();

    vector<pair<int, int>> result = path(0, 0, 0, 0,len, x, y, table, y - 1, x - 1);


    system("cls");
    if(!result.empty()){
        result.emplace_back(y-1,x-1);
        print_colored_table(table,result,0);
    }else{
        cout<<"no path found" << endl;
    }

    input_to_exit();
}

// Menu For Reading The Previous Maps
void menu_play_through_history(string &dir){
    int len;
    vector<vector<int>> table = read_maze(len,dir);

    string name = "No-Name";
    play(table,len,name);
}

// Menu For Reading The History Of The Games Played 
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
struct Player{
    string name;
    int wins;
    int playtime;
};

// Getting Users Info
void get_info(string dir,int &wins,int &play_time){
    //stats
    //total games
    //wins
    //last win
    //total play time
    string total_games,last_win;
    ifstream file(dir);
    file >> total_games >> wins >> last_win >> play_time;

    file.close();
}

// Comparing The Stats Of Two Players
bool compare_sort(const Player &a,const Player &b){
    if(a.wins != b.wins){
        return a.wins > b.wins;
    }else{
        return a.playtime < b.playtime;
    }
}

string translate_time(const string total_time){
    int sec = stoi(total_time) % 60;
    int min = stoi(total_time) /60;

    string min_str = to_string(min),sec_str(to_string(sec));
    if(min_str.size() == 1){
        min_str = "0" + min_str;
    }
    if(sec_str.size() == 1){
        sec_str = "0" + sec_str;
    }

    return min_str + ":" + sec_str;
}


void menu_leaderboard(){
    vector<Player> players;
    for (const auto & entry : filesystem::directory_iterator("./users/")) {
        string name =entry.path().filename().string();
        name = name.substr(0,name.size()-4);
        int wins,playtime;
        get_info("./users/"+name+".txt",wins,playtime);

        Player player;
        player.name = name;
        player.wins = wins;
        player.playtime = playtime;

        players.push_back(player);
    }

    sort(players.begin(), players.end(), compare_sort);

    vector<vector<string>> table;
    table.push_back({" ","name","wins","playtime"});
    int x =0;
    for(const Player &player: players){
        if(x <= 2){
            x++;

            vector<string> row = {to_string(x) + ". ",player.name,to_string(player.wins), translate_time(to_string(player.playtime))};
            table.push_back(row);
        }else{
            break;
        }
    }

    print_string_table(table);
    input_to_exit();
}

// Menu For Showing The History Of The Users Info
void menu_play_history(){
    vector<vector<string>> table;

    table.push_back({" ","name","maze played","playtime","state","date"});

    int x = 0;
    for (const auto & entry : filesystem::directory_iterator("./history/")) {
        x++;

        ifstream file("./history/" + to_string(x) + ".txt");

        string username,mapName,date,result,time;
        getline(file >>ws,username);
        getline(file >>ws,mapName);
        getline(file >>ws,time);
        getline(file >>ws,date);
        getline(file >> ws,result);

        vector<string> row = {to_string(x)+". ",username,mapName,time,result,date};
        table.push_back(row);
        //cout << x <<". "<<username << "|--|" << mapName << "|--|" << time << "|--|" << result << "|--|" << date << endl;

    }

    print_string_table(table);

    input_to_exit();
}

// Showing Users Information Such as TotalGames,Stats,Wins,LastWin And TotalPlayTime
void read_user(string &dir,string &username){
    system("cls");
    // Stats
    // Total Games
    // Wins
    // Last Win
    // Total Play Time
    string total_games,wins,last_win,total_play_time;
    ifstream file(dir);

    file >> total_games >> wins >> last_win >> total_play_time;

    cout<< "name: " << username << endl;
    cout<< "games played: " << total_games <<endl;
    cout<< "games won: " << wins <<endl;
    cout<< "last win: " << last_win << endl;

    cout<< "playtime: " << translate_time(total_play_time) << endl;

    input_to_exit();
}

// Menu Shown To Get The Information Of The Users Who Have Played 
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
        if(x == 0){
            cout<<"no users found! \n";
            input_to_exit();
            return;
        }

        int option = get_number("choose a user(0 to quit): ");
        
        if(1 <= option && option <=x){
            option --;
            string dir = "./users/" + names[option] +".txt";
            read_user(dir,names[option]);
        }else if(option == 0){
            break;
        }
    }
}

void create_base_folders(){
    if(!filesystem::is_directory("./history")) {
        filesystem::create_directory("./history");
    }
    if(!filesystem::is_directory("./users")) {
        filesystem::create_directory("./users");
    }
    if(!filesystem::is_directory("./maps")) {
        filesystem::create_directory("./maps");
    }
}
/*
bool hasFiles(const string& folderPath) {
    for (const auto& entry : filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() || entry.is_directory()) {
            return true;
        }
    }
    return false;
}


void menu_reset(){
    while(true){
        system("cls");
        int choice = get_number("1. clear maps\n"
                                "2. clear history\n"
                                "3. reset users\n"
                                "choose an option(0 to go back): ");
        system("cls");
        switch (choice) {
            case 1:
                if(hasFiles("./maps")){
                    filesystem::remove("./maps");
                }else{
                    cout<< "no maps found" << endl;
                }
                input_to_exit();
                break;
            case 2:
                if(filesystem::exists("./history/1.txt")) {
                    filesystem::remove("./history");
                    cout << "history cleared"<<endl;
                }else{
                    cout << "no recent games found"<<endl;
                }
                input_to_exit();
                break;
            case 3:
                if(hasFiles("./users/")){
                    filesystem::remove("./users");
                }else{
                    cout<< "no users found" << endl;
                }
                input_to_exit();
                break;
            case 0:
                return;
            default:
                continue;
        }
        create_base_folders();
    }
}
*/

void menu_welcome(){
    while (true){
        cout << "main menu";
        system("cls");
        string question =
                "Maze Maverick\n"
                "1. create a maze \n"
                "2. solve a maze \n"
                "3. playground \n"
                "4. history\n"
                "5. users \n"
                "6. leader board \n"
                "7. exit \n"
                "choose an option: ";
        int choice = get_number(question);

        system("cls");

        switch (choice) {
            case 1:
                while(true){
                    system("cls");
                    question =
                            "1. creat a basic maze\n"
                            "2. create an advanced maze\n"
                            "choose an option(0 to go back): ";
                    choice = get_number(question);
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
                    question =
                            "1. Solve A Previous Maze \n"
                            "2. Solve A New Maze \n"
                            "Choose An Option (0 To Go Back): ";

                    choice = get_number(question);
                    system("cls");
                    if(choice == 1){
                        menu_solve_existing_maze();
                    }else if(choice == 2) {
                        while(true){
                            system("cls");
                            question =
                                    "1. solve a basic maze\n"
                                    "2. solve an advanced maze\n"
                                    "choose an option(0 to go back): ";
                            choice = get_number(question);
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
                    question =
                            "1. Play A Previous Maze \n"
                            "2. Play A New Maze \n"
                            "Choose An Option (0 To Go Back): ";
                    choice = get_number(question);

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
                system("cls");
                menu_play_history();
                break;

            case 5:
                menu_users();
                break;
            case 6:
                menu_leaderboard();
                break;
            case 7:
                return;
            default:
                break;
        }
    }
}

int main() {
    create_base_folders();
    menu_welcome();
    return 0;
}