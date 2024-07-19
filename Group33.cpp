#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

//loading bar animation for decoration
#include <chrono>
#include <thread>
using namespace std;


//admin username and password
const string admin_username = "admin";
const string admin_password = "password";

//user count
const int MAX_USERS = 100; // maximum number of registered users
int numUsers = 0; // number of registered users

//used for registration and login
struct User {
    string username;
    string password;
} users[MAX_USERS];

//loading bar
const int duration = 5; // total duration of the loading bar in seconds
const int delay = 100; // delay between progress updates in milliseconds

//maximum 50 movies slot
const int Size = 50;

//movies slots
struct Movie {
    string name, time, location;
};

//tickets booked
struct Ticket {
    string name, time, location;
};



//functions prototypes
//by tharini
void registerUser();
void showLoadingBar(int duration, int delay);;
bool loginUser(string& login_username);
bool adminLogin();
void displayadminmenu();
void displaymenu();
void loadUsers();

//by joseph
void readMovies(string moviesOS[], Movie movies[]);
void sort(Movie movies[]);
bool check_exist(string name, string moviesOS[]);
int null_index(string moviesOS[]);
void set_movieOS(Movie movies[], string moviesOS[]);
int check_index(Movie movies[]);
int starting_index(Movie movies[]);
void movie_details(string moviesOS[], Movie movies[]);
void update(Movie movies[], string moviesOS[]);
int check_tickets_index(Ticket tickets[]);
void bookMovies(string moviesOS[], Movie movies[], string login_username);

//by william
void check_record();
void check_out(string login_username, Ticket tickets[], int index);


int main() {

    //read and set up movie list memory
    Movie movies[Size] = {};
    string moviesOS[Size] = {}; //moviesOS is movies on show
    readMovies(moviesOS, movies);

    char choice;
    int admin_choice;
    string login_username;

    //loadUsersFromFile();
    bool isLoggedIn = false;
    bool validInput = false;

    do {
        system("CLS");
        displaymenu();
        cout << "\n\t\t\t\t\t\tEnter your choice: ";
        cin >> choice;

        switch (choice) {
        //register user
        case '1':
            system("CLS");
            registerUser();
            cin.ignore();
            break;
        //user login
        case '2':
            if (!isLoggedIn) {
                system("CLS");
                isLoggedIn = loginUser(login_username);
                showLoadingBar(duration, delay);
                if (isLoggedIn)
                    bookMovies(moviesOS, movies, login_username); //go to user menu
            }
            else {
                cout << "\n\tYou are already logged in" << endl;
                cout << "\tRedirecting you to the user menu...\n" << endl;

                showLoadingBar(duration, delay);

                cin.ignore();

                system("CLS");
                bookMovies(moviesOS, movies, login_username); //go to user menu
            }
            break;
        //user log out
        case '3':
        {
            char logout;

            if (!isLoggedIn) {
                cout << "\n\tYou're not logged in\n\tPress enter to go back to the main menu" << endl;
                cin.ignore();
                cin.ignore();
                displaymenu();
                break;
            }

            cout << "\tLog out?" << endl;
            cout << "\n\tPress 'Y' to logout" << endl;
            cout << "\n\tPress any other key to go back to the main screen" << endl;
            cout << "\t:";
            cin >> logout;

            if (logout == 'Y' || logout == 'y') {
                isLoggedIn = false;
                cout << "\n\tYou have successfully logged out of the system\n\tPress enter to go back to the main menu" << endl;
                cin.ignore();
                cin.ignore();
                displaymenu();

                showLoadingBar(duration, delay);
            }
            else {
                cout << "\n\tYou're still logged in\n\tPress enter to go back to the main menu" << endl;
                cin.ignore();
                cin.ignore();
            }
            break;
        }
        //admin login
        case '4':
        {
            int max_attempts = 3;
            int attempts = 0;
            bool login_successful = false;

            while (attempts < max_attempts && !login_successful) {
                system("CLS");
                if (adminLogin()) {
                    login_successful = true;
                    displayadminmenu();

                    cout << "\n\t\t\t\t\t\t\tEnter your choice: ";
                    cin >> admin_choice;
                    cout << endl << endl;

                    while (admin_choice >= 1 && admin_choice <= 2) {

                        if (admin_choice == 1) {

                            //start of admin update menu
                            update(movies, moviesOS);

                            //update movies.txt
                            ofstream outFile("movies.txt", ios::out);

                            for (int i = starting_index(movies); i < Size; i++) {
                                outFile << movies[i].name << endl;
                                outFile << movies[i].time << " " << movies[i].location << endl << endl;
                            }

                            outFile.close();

                            //Display updated movie table
                            cout << "The new movie table:\n\n";
                            movie_details(moviesOS, movies);
                            cout << "Changes are successfully updated." << endl;
                        }

                        //check ticket records
                        else if (admin_choice == 2)
                            check_record();

                        cout << "\n\t\t\t\t\t  press any key to go back to admin menu interface...";
                        cin.ignore();
                        system("CLS");
                        displayadminmenu();
                        cout << "\n\t\t\t\t\t\tEnter your choice: ";
                        cin >> admin_choice;
                    }
                }
                else {
                    attempts++;
                    cout << "\n\t\t\t\t\t\tIncorrect login credentials. Attempts remaining: " << max_attempts - attempts << endl;
                    if (attempts < 3) {
                        cout << "\n\t\t\t\t\t\tPress any key to try again..." << endl;
                    }
                    else {
                        cout << "\n\t\t\t\t\t\tYou have exceeded the maximum number of login attempts.\n\t\t\t\t\t\tredirecting to the main page...." << endl;
                        showLoadingBar(duration, delay);

                    }
                    cin.ignore();
                    cin.ignore();
                }
            }
            break;
        }
        //exit
        case '5':
            cout << "\nGoodbye!" << endl;
            return 0;
        //invalid input
        default:
            cout << "\nInvalid choice.\n" << endl;
            system("Pause");
            break;
        }
    } while (choice != 5);

    system("Pause");
    return 0;
}



//function definitions
//add previously added users to the struct
void loadUsers() {
    ifstream myfile("users.txt");
    if (myfile.is_open()) {
        string username, password;
        while (getline(myfile, username, ' ') && getline(myfile, password)) {
            users[numUsers].username = username;
            users[numUsers].password = password;
            numUsers++;
        }
        myfile.close();
    }
    else {
        cout << "Error: unable to open file" << endl;
    }
}


//user registration
void registerUser() {
    //register user banner
    cout << "\t              ( \\    " << endl;
    cout << "\t   __         _)_\\_    " << endl;
    cout << "\t  ' \\;---.-._S_____2_                               _        __    " << endl;
    cout << "\t    /   / /_/       (______           _____ ___   ____ _ (_)_____ / /_ ___   _____" << endl;
    cout << "\t __(  _;-'    =    =|____._'.__          / ___// _ \\ / __ `// // ___// __// _ \\ / ___/" << endl;
    cout << "\t/   _/     _  @\\ _(@(      '--'.     / /   /  __// /_/ // /(__  )/ /_ /  __// /" << endl;
    cout << "\t(_ /      /\\  _   =( ) ___     \\    /_/    \\___/ \\__, //_//____/ \\__/ \\___//_/" << endl;
    cout << "\t  /      /\\ \\_ '.___'-.___~.    '\\           /____/   ____   ____  _      __" << endl;
    cout << "\t /      /\\ \\__'--') '-.__c` \\    |                   / __ \\ / __ \\| | /| / /" << endl;
    cout << "\t|     .'  )___'--'/  /`)     \\   /                   / / / // /_/ /| |/ |/ /" << endl;
    cout << "\t|    |'-|    _|--'\\_(_/       '.'                  /_/ /_/ \\____/ |__/|__/" << endl;
    cout << "\t|    |   \\_  -\\    " << endl;
    cout << "\t\\   |     \\ /`)    " << endl;
    cout << "\t  '._/       (_/     " << endl;
    cout << "\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
    cout << "\t--------------------------------------------------[hello!]-------------------------------------------------------- " << endl;
    cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;
    cin.ignore();

    if (numUsers >= MAX_USERS) {
        cout << "Error: maximum number of users reached" << endl;
        return;
    }

    string username, password;
    cout << "\n\n\t\t\t\t\tPlease enter a username\t\t: ";
    getline(cin, username);

    cout << "\t\t\t\t\tPlease enter a password\t\t: ";
    getline(cin, password);

    users[numUsers].username = username;
    users[numUsers].password = password;
    numUsers++;

    // Save user to users.txt file
    ofstream myfile;
    myfile.open("users.txt", ios_base::app);
    myfile << username << " " << password << endl;
    myfile.close();

    cout << "\n\n\t\t\t\t\t\t  Registration successful!" << endl;
    cout << "\n\t\t\t\t\t  [press any key to go back to main menu]" << endl;
}


//user login verifiication
bool loginUser(string& login_username) {
    loadUsers();
    //login user banner
    cout << "\t  _,                           _     " << endl;
    cout << "\t.'  `.                 ,___.>'',' - .. - .   " << endl;
    cout << "\t`-.   ;           .--'-        . - ._@;      " << endl;
    cout << "\t   ;  !_.--..._ .'      /     .[_@'`'.       " << endl;
    cout << "\t  ;            /       : .'  ; :_.._  `.         __           __ __" << endl;
    cout << "\t   :           ;        ;[   _T-;  `.'-. `-.    / /_   ___   / // /____     __  __ _____ ___   _____" << endl;
    cout << "\t   \\        .-:      ; `.`-=_,88p.   _.}.-'    / __ \\ / _ \\ / // // __ \\   / / / // ___// _ \\ / ___/" << endl;
    cout << "\t    `-.__.-'   \\    /L._ Y', P$T888;  ""        / / / //  __// // // /_/ /  / /_/ /(__  )/  __// /    " << endl;
    cout << "\t             .-'_.-'  / ;$$$$$$]8P;          /_/ /_/ \\___//_//_/ \\____/   \\__,_//____/ \\___//_/     " << endl;
    cout << "\t             \\ /     / / 'Y$$P' ^,'         " << endl;
    cout << "\t              ;\\_    `.\\_._                " << endl;
    cout << "\t              ]__\\     \\___;                  " << endl;
    cout << "\n\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
    cout << "\t-------------------------------------------------[login now]------------------------------------------------------ " << endl;
    cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;

    string login_password;
    cin.ignore();
    cout << "\n\n\t\t\t\t\t\tplease enter your username: ";
    getline(cin, login_username);
    cout << endl;
    cout << "\t\t\t\t\t\tplease enter your password: ";
    getline(cin, login_password);

    for (int i = 0; i < numUsers; i++) {
        if (users[i].username == login_username && users[i].password == login_password) {
            cout << "\n\n\t  login successful! Welcome " << users[i].username << endl;
            cout << "\t  press any key to continue..." << endl;
            cin.ignore();
            system("CLS");
            return true;
        }
    }
    cout << "\n\n\tError: invalid username or password" << endl;
    cout << "\tyou'll be redirected back to the main menu" << endl;

    cin.ignore();

    return false;
}


//loading bar decoration
void showLoadingBar(int duration, int delay) {
    int progress = 0;
    int barWidth = 40;

    cout << "\n\t\t\t\t\t\tLoading: [";
    for (int i = 0; i < barWidth; ++i) {
        cout << " ";
    }
    cout << "] 0%";

    for (int i = 0; i < barWidth; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // Delay for smooth progress animation

        cout << "\b\b\b"; // Move cursor back
        cout << "||"; // Update progress
        ++progress;

        cout.flush();
    }

    cout << "\b\b\b"; // Move cursor back
    cout << "] 100% Done!" << std::endl;
}


//admin login verification
bool adminLogin() {
    //admin authorization page banner
    cout << "\n\t ______     _____     __    __     __     __   __        __         ______     ______     __     __   __" << endl;
    cout << "\t/\\  __ \\   /\\  __-.  /\\ \"-./  \\   /\\ \\   /\\ \"-.\\ \\      /\\ \\       /\\  __ \\   /\\  ___\\   /\\ \\   /\\ \"-.\\ \\ " << endl;
    cout << "\t\\ \\  __ \\  \\ \\ \\/\\ \\ \\ \\ \\-./\\ \\  \\ \\ \\  \\ \\ \\-.  \\     \\ \\ \\____  \\ \\ \\/\\ \\  \\ \\ \\__ \\  \\ \\ \\  \\ \\ \\-.  \\" << endl;
    cout << "\t \\ \\_\\ \\_\\  \\ \\____-  \\ \\_\\ \\ \\_\\  \\ \\_\\  \\ \\_\\\\\"\\_\\     \\ \\_____\\  \\ \\_____\\  \\ \\_____\\  \\ \\_\\  \\ \\_\\\\\"\\_\\ " << endl;
    cout << "\t  \\/_/\\/_/   \\/____/   \\/_/  \\/_/   \\/_/   \\/_/ \\/_/      \\/_____/   \\/_____/   \\/_____/   \\/_/   \\/_/ \\/_/" << endl;

    cout << "\n\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
    cout << "\t--------------------------------------------[authorization page]-------------------------------------------------- " << endl;
    cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;


    string inputUsername;
    string inputPassword;

    cout << "\n\n\t\t\t\t\t\tPlease enter your username: ";
    cin >> inputUsername;
    //cout << endl;
    cout << "\n\t\t\t\t\t\tPlease enter your password: ";
    cin >> inputPassword;
    cout << endl;

    if (inputUsername == admin_username && inputPassword == admin_password) {
        cout << "\n\t\t\t\t\t\tWelcome, admin!\n\t\t\t\t\t\tredirecting to admin menu" << endl;
        showLoadingBar(duration, delay);

        cin.ignore();
        system("CLS");

        return true;
    }
    else {
        return false;
    }
}


//display admin menu
void displayadminmenu() {
    cout << "\n\t                               ,^      /l            " << endl;
    cout << "\t                             ;  l   ;' l            " << endl;
    cout << "\t                      \\^\\.   j' ,| ,'   l            " << endl;
    cout << "\t                      |0 '';;],, [; _, ,j            " << endl;
    cout << "\t                    {_, ''.    ''L     ;;            " << endl;
    cout << "\t                    ,:' '               ':            " << endl;
    cout << "\t                 _,:I   -,                ;,        " << endl;
    cout << "\t                _,u    QJ,                 ;,                    __                                 __" << endl;
    cout << "\t               --}    ---.,,__             ';,  _      __ ___   / /_____ ____   ____ ___   ___     / /_ ____" << endl;
    cout << "\t                ||    T_=--..,_             ,; | | /| / // _ \\./ // ___// __ \\ / __ `__ \\ / _ \\   / __// __ \\" << endl;
    cout << "\t            ,:''-';,    ',                    ;| |/ |/ //  __// // /__ / /_/ // / / / / //  __/  / /_ / /_/ /    " << endl;
    cout << "\t          ,;'     ^';,                         |__/|__/ \\___//_/ \\___/ \\____//_/ /_/ /_/ \\___/   \\__/ \\____/ " << endl;
    cout << "\t        ,C   u-      ';,          ___   / /__  __   _____ (_)____   ___   ____ ___   ____ _ _____     " << endl;
    cout << "\t       ,:   ||;j       ;         / _ \\ / // / / /  / ___// // __ \\ / _ \\ / __ `__ \\ / __ `// ___/" << endl;
    cout << "\t       .j     u-　      ;.       /  __// // /_/ /  / /__ / // / / //  __// / / / / // /_/ /(__  ) " << endl;
    cout << "\t       '~    --        ;:       \\___//_/ \\__, /   \\___//_//_/ /_/ \\___//_/ /_/ /_/ \\__,_//____/ " << endl;
    cout << "\t       .;               j               /____/  ,;      " << endl;
    cout << "\t       i                {                       ;    " << endl;

    cout << "\n\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
    cout << "\t----------------------------------------------------[admin menu]-------------------------------------------------- " << endl;
    cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;

    cout << "\t\t\t\t\t\t\t1\tPerform Update" << endl;
    cout << "\t\t\t\t\t\t\t2\tCheck ticket records" << endl;
    cout << "\t\t\t\t\t\t\tPress any other number to exit " << endl;


}


//display main menu
void displaymenu() {

    cout << "\n\t                               ,^      /l            " << endl;
    cout << "\t                             ;  l   ;' l            " << endl;
    cout << "\t                      \\^\\.   j' ,| ,'   l            " << endl;
    cout << "\t                      |0 '';;],, [; _, ,j            " << endl;
    cout << "\t                    {_, ''.    ''L     ;;            " << endl;
    cout << "\t                    ,:' '               ':            " << endl;
    cout << "\t                 _,:I   -,                ;,        " << endl;
    cout << "\t                _,u    QJ,                 ;,                    __                                 __" << endl;
    cout << "\t               --}    ---.,,__             ';,  _      __ ___   / /_____ ____   ____ ___   ___     / /_ ____" << endl;
    cout << "\t                ||    T_=--..,_             ,; | | /| / // _ \\./ // ___// __ \\ / __ `__ \\ / _ \\   / __// __ \\" << endl;
    cout << "\t            ,:''-';,    ',                    ;| |/ |/ //  __// // /__ / /_/ // / / / / //  __/  / /_ / /_/ /    " << endl;
    cout << "\t          ,;'     ^';,                         |__/|__/ \\___//_/ \\___/ \\____//_/ /_/ /_/ \\___/   \\__/ \\____/ " << endl;
    cout << "\t        ,C   u-      ';,          ___   / /__  __   _____ (_)____   ___   ____ ___   ____ _ _____     " << endl;
    cout << "\t       ,:   ||;j       ;         / _ \\ / // / / /  / ___// // __ \\ / _ \\ / __ `__ \\ / __ `// ___/" << endl;
    cout << "\t       .j     u-　      ;.       /  __// // /_/ /  / /__ / // / / //  __// / / / / // /_/ /(__  ) " << endl;
    cout << "\t       '~    --        ;:       \\___//_/ \\__, /   \\___//_//_/ /_/ \\___//_/ /_/ /_/ \\__,_//____/ " << endl;
    cout << "\t       .;               j               /____/  ,;      " << endl;
    cout << "\t       i                {                       ;    " << endl;

    cout << "\n\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
    cout << "\t----------------------------------------------------[main menu]--------------------------------------------------- " << endl;
    cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;

    cout << "\n\t\t\t\t\t1\tdont have an account? REGISTER NOW" << endl;
    cout << "\t\t\t\t\t2\texisting user?        Login" << endl;
    cout << "\t\t\t\t\t3\tlog out" << endl;
    cout << "\t\t\t\t\t4\tadmin login" << endl;
    cout << "\t\t\t\t\t5\texit" << endl;


}


//find the starting index for the null structs in array
int check_tickets_index(Ticket tickets[]) {
    for (int i = 0; i < Size; i++) {
        if (tickets[i].name == "" && tickets[i].time == "" && tickets[i].location == "")
            return i;
    }

    return 50;
}


//check for existence of same movie in moviesOS array
bool check_exist(string name, string moviesOS[]) {
    for (int i = 0; i < Size; i++) {
        if (name == moviesOS[i])
            return true;
    }

    return false;
}


//check for first position without null string
int null_index(string moviesOS[]) {
    int pos = 0;
    for (int i = 0; i < Size; i++) {
        if (moviesOS[i] != "")
            pos++;
        else
            break;
    }

    return pos;
}


//find out movies on show without repeating movies
void set_movieOS(Movie movies[], string moviesOS[]) {
    for (int i = 0; i < Size; i++) {
        string name = movies[i].name;

        if (!check_exist(name, moviesOS)) {
            int index = null_index(moviesOS);
            moviesOS[index] = name;
        }
    }
}


//find the starting index for the null structs in array
int check_index(Movie movies[]) {
    for (int i = 0; i < Size; i++) {
        if (movies[i].name == "" && movies[i].time == "" && movies[i].location == "")
            return i;
    }

    return 50;
}


//find starting index that contains movie slot in movies array
int starting_index(Movie movies[]) {
    for (int i = 0; i < Size; i++) {
        if (movies[i].name == "" && movies[i].time == "" && movies[i].location == "");
        else
            return i;
    }
    return 50;
}


//display movie details(name, time, hall)
void movie_details(string moviesOS[], Movie movies[]) {
    set_movieOS(movies, moviesOS);

    //admin banner
    cout << "\n\t\t\t\t                                   __                     _              " << endl;
    cout << "\t\t\t\t   ____   ____  _      __   _____ / /_   ____  _      __ (_)____   ____ _ " << endl;
    cout << "\t\t\t\t  / __ \\ / __ \\| | /| / /  / ___// __ \\ / __ \\| | /| / // // __ \\ / __ `/" << endl;
    cout << "\t\t\t\t / / / // /_/ /| |/ |/ /  (__  )/ / / // /_/ /| |/ |/ // // / / // /_/ / " << endl;
    cout << "\t\t\t\t/_/ /_/ \\____/ |__/|__/  /____//_/ /_/ \\____/ |__/|__//_//_/ /_/ \\__, /  " << endl;
    cout << "\t\t\t\t                                                                /____/   " << endl;
    cout << "\n\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
    cout << "\t--------------------------------------------[movies and showtimes]------------------------------------------------ " << endl;
    cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;
    cout << endl;
    cout << "\t\t\t\t\t" << left << setw(30) << "Movies" << setw(17) << "| Showtime" << setw(15) << "| Halls" << endl;

    for (int i = 0; i < null_index(moviesOS); i++) {
        cout << "\t\t\t\t\t---------------------------------------------------------" << endl;
        for (int j = 0; j < Size; j++) {
            if (movies[j].name == moviesOS[i])
                cout << "\t\t\t\t\t" << setw(30) << movies[j].name << "| " << setw(15) << movies[j].time << "| " << setw(15) << movies[j].location << endl;
        }
    }
    cout << "\t\t\t\t\t---------------------------------------------------------" << endl;
}


//admin menu: change movie slot(showtime/hall), add movie slot, delete movie slot
void update(Movie movies[], string moviesOS[]) {
    int choice;
    char choice2;

    do {
        system("CLS");
        int search_index[50], index, no = 0;
        string name, changed_time, changed_location, add_name, add_time, add_hall;
        bool valid_name = false;

        do {
            system("CLS");
            cout << "\n\t\t\t\t              __            _                                        " << endl;
            cout << "\t\t\t\t  ____ _ ____/ /____ ___   (_)____     ____ ___   ___   ____   __  __" << endl;
            cout << "\t\t\t\t / __ `// __  // __ `__ \\ / // __ \\   / __ `__ \\ / _ \\ / __ \\ / / / /" << endl;
            cout << "\t\t\t\t/ /_/ // /_/ // / / / / // // / / /  / / / / / //  __// / / // /_/ / " << endl;
            cout << "\t\t\t\t\\__,_/ \\__,_//_/ /_/ /_//_//_/ /_/  /_/ /_/ /_/ \\___//_/ /_/ \\__,_/  " << endl;
            cout << endl;
            cout << endl;
            cout << "\n\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
            cout << "\t-----------------------------------------------[admin update menu]------------------------------------------------ " << endl;
            cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;

            cout << "\n\t\t\t\t\t1\tChange movie slot : Showtime / Hall" << endl;
            cout << "\t\t\t\t\t2\tAdd movie slot" << endl;
            cout << "\t\t\t\t\t3\tDelete movie slot" << endl;
            cout << "\t\t\t\t\t4\tExit" << endl;


            cout << "\n\tEnter your option: ";
            cin >> choice;
            cin.ignore();
            cout << endl;

            if (choice > 4 || choice < 1) {
                cout << "\n\tInvalid input. Please press enter to continue..." << endl;
                cin.ignore();
            }

        } while (choice > 4 || choice < 1);

        system("CLS");


        //display movie list as reference when not exit
        if (choice != 4)
            movie_details(moviesOS, movies);

        //check for valid movie name, display specific movie details if valid
        while (!valid_name && choice != 2 && choice != 4) {
            cout << "\n\tEnter the name of movie to perform update/changes: ";
            getline(cin, name);
            cout << endl << endl;
            cout << "\t\t\t\t\t" << left << setw(30) << name << "|   Showtime" << setw(15) << "|    Halls" << endl;
            cout << "\t\t\t\t\t---------------------------------------------------------" << endl;

            //Display specific movie details for admin as reference
            for (int i = 0; i < Size; i++) {
                if (movies[i].name == name) {
                    valid_name = true;
                    no++;
                    search_index[no] = i; //collect index of specific movie
                    cout << "\t\t\t\t\t(" << no << ")" << right << setw(29) << "| " << setw(10) << movies[i].time << "|   " << movies[i].location << endl;
                }
            }

            if (!valid_name)
                cout << "\t\t\t\t\tMovie not found in database.\n\t\t\t\t\tPlease be careful with the spellings and capital letters.\n\t\t\t\t\t---------------------------------------------------------" << endl;
        }

        //change showtime or hall
        if (choice == 1) {

            cout << "\n\t\t\t\t\tPress 'A' for changing showtime.\n\t\t\t\t\tPress 'B' for changing hall.\n\tEnter: ";
            cin >> choice2;
            cout << endl;

            //change showtime
            if (choice2 == 'A') {
                cout << "\tWhich slot's showtime do you want to change (Enter number): ";
                cin >> no;
                index = search_index[no];
                cout << "\tChanging showtime from " << movies[index].time << " to: ";
                cin.ignore();
                getline(cin, changed_time);
                movies[index].time = changed_time;
                sort(movies);

                showLoadingBar(duration, delay);
                cout << "\n\t\t\t\t\t\t[ successfully added the changes ]";
                cout << "\n\tPress enter to go back to admin update menu..." << endl;
                cin.ignore();
            }

            //change hall
            else if (choice2 == 'B') {
                cout << "\tWhich slot's hall do you want to change (Enter number): ";
                cin >> no;
                index = search_index[no];
                cout << "\tChanging hall from " << movies[index].location << " to: ";
                cin.ignore();
                getline(cin, changed_location);
                movies[index].location = changed_location;

                showLoadingBar(duration, delay);
                cout << "\n\t\t\t\t\t\t[ successfully added the changes ]";
                cout << "\n\tPress enter to go back to admin update menu..." << endl;
                cin.ignore();
            }
            else
                cout << "\n\tInvalid choice! Exiting back to Admin Interface..." << endl;
        }

        //add new movie slot
        else if (choice == 2) {
            cout << "\tMovie Name: ";
            getline(cin, add_name);
            cout << "\tShowtime: ";
            cin >> add_time;
            cin.ignore();
            cout << "\tHall: ";
            getline(cin, add_hall);

            index = check_index(movies);
            if (index == Size)
                cout << "\n\tError: The movie storage is full. Movie not added.\n\tExiting back to Admin Interface..." << endl;
            else {
                movies[index].name = add_name;
                movies[index].time = add_time;
                movies[index].location = add_hall;
                sort(movies);

                showLoadingBar(duration, delay);
                cout << "\n\t\t\t\t\t\t[ successfully added the changes ]";
                cout << "\n\tPress enter to go back to admin update menu..." << endl;
                cin.ignore();
            }
        }

        //delete movie slot
        else if (choice == 3) {
            cout << "\n\tWhich slot to delete (Enter number): ";
            cin >> no;
            index = search_index[no];
            for (int i = index; i < Size - 1; i++)
                movies[i] = movies[i + 1];
            movies[Size - 1] = movies[0];

            showLoadingBar(duration, delay);
            cout << "\n\t\t\t\t\t\t[ successfully added the changes ]";
            cout << "\n\tPress enter to go back to admin update menu..." << endl;
            cin.ignore();
        }

    } while (choice != 4);
}


//user menu: book tickets, edit booked tickets, perform payment
void bookMovies(string moviesOS[], Movie movies[], string login_username) {

    Ticket tickets[Size] = {};
    int option, index, ticket_index = 0;
    string name, dummy;

    do {
        system("CLS");

        //user menu banner
        cout << "\n\t                               ,^      /l            " << endl;
        cout << "\t                             ;  l   ;' l            " << endl;
        cout << "\t                      \\^\\.   j' ,| ,'   l            " << endl;
        cout << "\t                      |0 '';;],, [; _, ,j            " << endl;
        cout << "\t                    {_, ''.    ''L     ;;            " << endl;
        cout << "\t                    ,:' '               ':            " << endl;
        cout << "\t                 _,:I   -,                ;,        " << endl;
        cout << "\t                _,u    QJ,                 ;,                    __                                 __" << endl;
        cout << "\t               --}    ---.,,__             ';,  _      __ ___   / /_____ ____   ____ ___   ___     / /_ ____" << endl;
        cout << "\t                ||    T_=--..,_             ,; | | /| / // _ \\./ // ___// __ \\ / __ `__ \\ / _ \\   / __// __ \\" << endl;
        cout << "\t            ,:''-';,    ',                    ;| |/ |/ //  __// // /__ / /_/ // / / / / //  __/  / /_ / /_/ /    " << endl;
        cout << "\t          ,;'     ^';,                         |__/|__/ \\___//_/ \\___/ \\____//_/ /_/ /_/ \\___/   \\__/ \\____/ " << endl;
        cout << "\t        ,C   u-      ';,          ___   / /__  __   _____ (_)____   ___   ____ ___   ____ _ _____     " << endl;
        cout << "\t       ,:   ||;j       ;         / _ \\ / // / / /  / ___// // __ \\ / _ \\ / __ `__ \\ / __ `// ___/" << endl;
        cout << "\t       .j     u-　      ;.       /  __// // /_/ /  / /__ / // / / //  __// / / / / // /_/ /(__  ) " << endl;
        cout << "\t       '~    --        ;:       \\___//_/ \\__, /   \\___//_//_/ /_/ \\___//_/ /_/ /_/ \\__,_//____/ " << endl;
        cout << "\t       .;               j               /____/  ,;      " << endl;
        cout << "\t       i                {                       ;    " << endl;

        cout << "\n\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
        cout << "\t-----------------------------------------------[user menu]-------------------------------------------------------- " << endl;
        cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;

        cout << "\t\t\t\t\t\t1) Book tickets\n\t\t\t\t\t\t2) Edit booked ticket\n";
        cout << "\t\t\t\t\t\t3) Perform payment\n\t\t\t\t\t\t4) Exit" << endl;
        cout << "\t\t\t\t\t\tEnter your option: ";
        cin >> option;

        while (option < 1 || option > 4) {
            cout << "\n\tInvalid input. Please enter again." << endl;
            cout << "\tEnter your option: ";
            cin >> option;
        }

        system("CLS");
        // book tickets
        if (option == 1) {

            cout << endl;
            cin.ignore();
            int search_index[50], no = 0;
            bool valid_name = false;
            movie_details(moviesOS, movies);

            //check for valid movie name, display specific movie details if valid
            while (!valid_name) {
                cout << "\n\tEnter the name of movie to book tickets: ";
                getline(cin, name);
                cout << endl << endl;
                cout << "\t\t\t\t\t" << left << setw(30) << name << "|   Showtime" << setw(15) << "|    Halls" << endl;
                cout << "\t\t\t\t\t---------------------------------------------------------" << endl;

                //Display specific movie details for admin as reference
                for (int i = 0; i < Size; i++) {
                    if (movies[i].name == name) {
                        valid_name = true;
                        no++;
                        search_index[no] = i; //collect index of specific movie
                        cout << "\t\t\t\t\t(" << no << ")" << right << setw(29) << "| " << setw(10) << movies[i].time << "|   " << movies[i].location << endl;
                    }
                }

                if (!valid_name)
                    cout << "\t\t\t\t\tMovie not found in database.\n\t\t\t\t\tPlease be careful with the spellings and capital letters.\n\t\t\t\t\t---------------------------------------------------------" << endl;
            }

            cout << "\tEnter the slot number to book your ticket: ";
            cin >> no;
            index = search_index[no];
            tickets[ticket_index].name = movies[index].name;
            tickets[ticket_index].time = movies[index].time;
            tickets[ticket_index].location = movies[index].location;
            ticket_index++;
        }
        //delete booked tickets
        else if (option == 2) {

            cout << "\n\t\t\t\t       __       __       __           __   _        __          __ " << endl;
            cout << "\t\t\t\t  ____/ /___   / /___   / /_ ___     / /_ (_)_____ / /__ ___   / /_" << endl;
            cout << "\t\t\t\t / __  // _ \\ / // _ \\ / __// _ \\   / __// // ___// //_// _ \\ / __/" << endl;
            cout << "\t\t\t\t/ /_/ //  __// //  __// /_ /  __/  / /_ / // /__ / ,<  /  __// /_  " << endl;
            cout << "\t\t\t\t\\__,_/ \\___//_/ \\___/ \\__/ \\___/   \\__//_/ \\___//_/|_| \\___/ \\__/  " << endl;
            cout << endl;

            cout << "\n\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
            cout << "\t-------------------------------------------------[delete ticket]------------------------------------------------- " << endl;
            cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;

            int num;
            index = check_tickets_index(tickets);

            if (index == 0)
                cout << "\tYou didn't book any ticket yet." << endl;
            else {
                cout << "\tThis is the list of your booked tickets." << endl;

                for (int i = 0; i < index; i++) {
                    cout << "\t\t\t\t\t" << i + 1 << ") " << tickets[i].name << "\t" << tickets[i].time << "\t" << tickets[i].location << endl;
                }

                cout << endl;
                cout << "\tEnter no. of ticket that you want to delete.\n\tEnter 0 if you don't want to delete anything." << endl;
                cout << "\tNo: ";
                cin >> num;
                if (num != 0) {
                    for (int i = num - 1; i < index; i++)
                        tickets[i] = tickets[i + 1];

                    tickets[index - 1].name = "";
                    tickets[index - 1].time = "";
                    tickets[index - 1].location = "";

                    ticket_index = index - 1;
                }
            }
        }
        //perform payment
        else if (option == 3) {

            cout << "\n\t\t\t\t\t         __                 __                  __ " << endl;
            cout << "\t\t\t\t\t  _____ / /_   ___   _____ / /__ ____   __  __ / /_" << endl;
            cout << "\t\t\t\t\t / ___// __ \\ / _ \\ / ___// //_// __ \\ / / / // __/" << endl;
            cout << "\t\t\t\t\t/ /__ / / / //  __// /__ / ,<  / /_/ // /_/ // /_  " << endl;
            cout << "\t\t\t\t\t\\___//_/ /_/ \\___/ \\___//_/|_| \\____/ \\__,_/ \\__/  " << endl;
            cout << endl;

            cout << "\n\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
            cout << "\t-------------------------------------------------[payment gateway]------------------------------------------------- " << endl;
            cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;

            index = check_tickets_index(tickets);

            if (index == 0)
                cout << "\n\t\t\t\t\tYou didn't book any ticket yet so you don't have to pay." << endl;
            else {
                cout << "\n\t\t\t\t\tThis is the list of your booked tickets.\n" << endl;

                for (int i = 0; i < index; i++) {
                    cout << "\t\t\t\t\t" << i + 1 << ") " << tickets[i].name << "\t" << tickets[i].time << "\t" << tickets[i].location << endl;
                }

                cin.ignore();
                cout << "\n\t-------------------------" << endl;
                cout << "\t Total payment is RM" << index * 8 << endl;
                cout << "\t-------------------------" << endl;
                cout << "\n\n\tEnter your credit card number: ";
                getline(cin, dummy);

                cout << "\n\t[ Validating your card. This may take a few seconds ... ]" << endl;
                showLoadingBar(duration, delay);
                cout << "\tPayment completed." << endl;

                //update ticket records
                check_out(login_username, tickets, index);
               
                option = 4;
            }
        }
        //exit
        else
            cout << "\tYour booking is not saved!" << endl;

        cin.ignore();
        cout << "\n\tPress enter to continue..." << endl;
        cin.ignore();

    } while (option != 4);
}


//admin: check ticket records
void check_record() {

    system("CLS");
    string userID, username, ticket, dummy;
    double profit, total_profit = 0;
    char discard;

    cout << "\n\t\t\t\t   __   _        __          __                                        __     " << endl;
    cout << "\t\t\t\t  / /_ (_)_____ / /__ ___   / /_   _____ ___   _____ ____   _____ ____/ /_____" << endl;
    cout << "\t\t\t\t / __// // ___// //_// _ \\ / __/  / ___// _ \\ / ___// __ \\ / ___// __  // ___/" << endl;
    cout << "\t\t\t\t/ /_ / // /__ / ,<  /  __// /_   / /   /  __// /__ / /_/ // /   / /_/ /(__  ) " << endl;
    cout << "\t\t\t\t\\__//_/ \\___//_/|_| \\___/ \\__/  /_/    \\___/ \\___/ \\____//_/    \\__,_//____/  " << endl;
    cout << endl;
    cout << "\n\n\t------------------------------------------------------------------------------------------------------------------ " << endl;
    cout << "\t-------------------------------------------------[ticket records]------------------------------------------------- " << endl;
    cout << "\t------------------------------------------------------------------------------------------------------------------ " << endl;

    ifstream inRecord("ticket records.txt");

    //read in ticket records.txt
    if (inRecord.is_open()) {
        while (!inRecord.eof()) {
            inRecord >> userID;
            getline(inRecord, username);
            getline(inRecord, ticket);
            inRecord >> profit;
            getline(inRecord, dummy);
            inRecord.get(discard);

            cout << "\tUserID: " << username << endl;
            cout << "\t" << ticket << endl;
            cout << "\tPrice: " << profit << endl << endl;
            total_profit += profit;

        }
        inRecord.close();

        cout << "\n\tCurrent total income: " << total_profit << endl;
    }
    else
        cout << "\tError: Unable to open ticket records.txt!";

    cin.ignore();
}


//sort movies in ascending order according to time
void sort(Movie movies[]) {
    for (int i = 0; i < Size - 1; i++) {
        for (int j = 0; j < Size - 1; j++) {
            if (movies[j].time > movies[j + 1].time) {
                Movie temp = movies[j];
                movies[j] = movies[j + 1];
                movies[j + 1] = temp;
            }
        }
    }
}


//read in movies.txt
void readMovies(string moviesOS[], Movie movies[]) {
    ifstream inMovies("movies.txt");

    if (inMovies.is_open()) {
        int i = 0;

        while (!inMovies.eof()) {
            getline(inMovies, movies[i].name);
            inMovies >> movies[i].time;
            inMovies.ignore();
            getline(inMovies, movies[i].location);
            inMovies.ignore();
            i++;
        }
        inMovies.close();
    }
    else
        cout << "\tUnable to open movies.txt!";

    //sort the movies array ascendingly according to time
    sort(movies);
}


//update ticket record.txt by each purchase
void check_out(string login_username, Ticket tickets[], int index) {
    ofstream outRecord("ticket records.txt", ios::app);

    for (int i = 0; i < index; i++) {
        outRecord << "User: " << login_username << endl;
        outRecord << tickets[i].name << " " << tickets[i].time << " " << tickets[i].location << endl;
        outRecord << "8" << endl;
    }
    outRecord.close();
}