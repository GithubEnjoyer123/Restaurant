/* Program name: main.cpp
*  Author: Justin Phillips
*  Date last updated: 12/9/2023
* Purpose: Manages a restaurant's tables, reservations, and orders.
*/

//Headers
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

//Enum and arrays to keep track of menu items and prices
enum menuItem{VEGBURG, HAMBURG, CHICKSLIDE, HAMSLIDE, VEGSLIDE, HADDOCK, BLT, FAUXBLT, CHICKBURR, FALAFEL, CHEESEPIZ, PEPPERONIPIZ, FAUXMEAT, FISHCHIPS};
string currentMenu[] = {"Veggie Burger","Hamburger","Fried Chicken Sliders","Hamburger Sliders","Veggie Sliders","Haddock Sandwich","BLT","Faux BLT","Chicken Burrito","Falafel Wrap","Cheese Pizza","Pepperoni Pizza","Faux meat and chips","Fish and chips"};
double menuPrice[] = {35,45,38,38,38,38,42,42,42,42,59,59,77,77};

//structure for keeping track of our reservations before checking in
struct Reservation{ 
    string name;
    string time;
    int size;
    int table;
    bool pending = false;
};

//structure to keep track of our table values after checking in
struct Table{
    bool occupied = false, ordered = false;
    int tableNum;
    int size;
    int numPeople;
};

//structure to keep track of our order values after ordering, limited to 10 items per order
struct Order{
    string name;
    Table table; 
    Reservation res;
    string menu[10];
    double price[10], num, subTotal;
    bool complete = true, isEmpty = true, ordered = false, pending = false;
};

//function to print out the reservation information to user for validation
void printReservation(Reservation input){ 
    cout << "Please confirm the reservation:";
    cout << "\nReservation Name: " << input.name;
    cout << "\nReservation Time: " << input.time;
    cout << "\nNumber in Party: " << input.size; 
    cout << "\nIs this information correct [Y]es, [N]o (make changes), [C]ancel? ";
}

//function that prints our current menu for user selection
void printMenu(int size){
    for(int i = 0; i < size; i++){
        cout << i + 1 << ": " << currentMenu[menuItem(i)] << endl;
    }
}

//function to prompt exit option in menu if all orders are complete and after program has full completed at least 1 order
void exitProgram(int option, Order order[], bool kill){

    for(int i = 0; i < 50; i++){
        if(kill == false || order[i].pending == true){
            return;
        }
    }
    if(option == 6 && kill == true){
        exit(0);
    }

    cout << "6. Close Restaurant ";
}


int main(){

    //Our variables. r is for reservation, size is for total order size, orderNum is our order iteration, choice is user unput
    //num is secondary input value along with choice, tablesize keeps track of the size of our tables.
    int r = 0, size = 50, ordNum = 0, option, choice, num = 0, tableSize[] = {2,2,2,2,2,2,2,2,10,10,6,6,6,6,4,4,4,4,4,4};
    Reservation reservation[50];    //array of reservations
    Table table[20];                //array of tables
    Order order[50];                //array of orders               
    char confirm;                   //confirmation variable
    bool exit = false;              //exit variable       

   
   //initializes our table values
    for(int i = 0; i < 20; i++){
        table[i].size = tableSize[i];
        table[i].numPeople = 0;
        table[i].tableNum = i;
    }

    //main menu loop
    while(true){
        //prints our main menu
        cout << "Welcome to Messijoes\n1. Make Reservation\n2. Check-in Reservation\n3. Enter Order\n4. Complete Order\n5. Calculate and Pay Bill\n";

        //calls our exit function to prompt exit option if conditions met
        exitProgram(option, order, exit);

        cin >> option;

        //conditional for our first option of menu
        if(option == 1){
            //First block here grabs our reservation name/party size and validates it
            cout << "Enter a name for the reservation: ";
            cin.ignore();
            getline(cin, reservation[r].name);
            cout << "Enter the number of people in the party: ";
            cin >> reservation[r].size; 
            while(true){
                if(cin.fail()){
                    cout << "You entered something that is not a number. Please try again. ";
                    cin.clear();
                    cin.ignore();
                }else if(reservation[r].size <= 0 || reservation[r].size > 10){
                    cout << "The number of people must be between 1 and 10.";
                    cout << " Enter the number of people in the party: ";
                    cin.clear();
                    cin.ignore();
                    cin >> reservation[r].size; 
                }else{
                    cin.clear();
                    cin.ignore();
                    reservation[r].size = abs(reservation[r].size);
                    break;
                }
            }
            //Gets our time for reservation and validates it. Then asks user for confirmation of reservation
            cout << "Enter the time for the reservation in HH: :MM AM/PM: ";
            getline(cin, reservation[r].time);
            while(true){  
                printReservation(reservation[r]);
                cin >> confirm;
                //if yes confirmation, we mark reservation number as complete and iterate r then break, else continue loop
                if(confirm == 'Y' || confirm == 'y'){
                    order[r].pending = true;
                    order[r].complete = false;
                    r++;
                    break;
                }else if(confirm == 'N' || confirm == 'n'){
                    cout << "What do you want to change?\n1. Name\n2. Number of People\n3. Time\n4. Cancel\n";
                    cin >> confirm;
                    //Our various confirmation conditions
                    if(confirm == '1'){
                        cout << "Enter a name for the reservation: ";
                        cin.ignore();
                        getline(cin, reservation[r].name);
                    }else if(confirm == '2'){
                        while(true){
                            cout << "enter the number of people in the party: ";
                            cin >> reservation[r].size;
                            reservation[r].size = abs(reservation[r].size);
                            if(cin.fail()){
                                cout << "You entered something that is not a number. Please try again. ";
                                cin.clear();
                                cin.ignore(256, 'n');
                            }else if(reservation[r].size < 1 || reservation[r].size > 10){
                                cout << "The number of people must be between 1 and 10.";
                                cout << "Enter the number of people in the party: ";
                            }else{
                                break;
                            }
                        }
                    }else if(confirm == '3'){
                        cout << "Enter the time for the reservation in HH: :MM AM/PM: ";
                        cin.ignore();
                        getline(cin, reservation[r].time);
                    }else if(confirm == '4'){
                        break;
                    }
                }else if(confirm == 'c' || confirm == 'C'){
                    break;
                }else{
                    cout << "Invalid input\n";
                } 
            }
        //Option 2 checks in reservation from previous option   
        }else if(option == 2){
            //bools to track if reservation number and size are available
            bool resAvailibility = false, resSize = false;
                while(true){
                    cout << "Choose the reservation to check in\n";
                    //loops through reservations and prints them if they exist
                    for(int i = 0; i < r; i++){
                        if(reservation[i].size != 0 && reservation[i].time != "" && reservation[i].name != ""){
                            cout << i + 1 << ": " << reservation[i].name << " - " << reservation[i].time << ", " << reservation[i].size << " people\n";
                            resAvailibility = true;
                        } 
                    }
                    //if no reservations printed earlier, we break and print error message
                    if(resAvailibility == false){
                        cout << "There are no reservations to check in. please make a reservation first.\n";
                        break;
                    }
                    cin >> choice;
                    choice = choice - 1;
                    //we redo the loops for mininput
                    if (cin.fail()){
                        cout << "you entered something that is not a number. Please try again. That is not a valid choice.";
                        cin.clear();
                        cin.ignore(256, '\n');
                        continue;
                    }else if(reservation[choice].size == 0 || reservation[choice].time == "" || reservation[choice].name == ""){
                        cout << "that is not a valid choice.\n";
                        continue;
                    }
                       
                    cout << "Please assign a table:\n";
                    //print list of available tables that aren't taken, and are the correct size
                    while(true){
                        for(int i = 0; i < 20; i++){
                            if(table[i].occupied == false && table[i].size >= reservation[choice].size){
                                cout << table[i].tableNum + 1 << ": " << table[i].size << " people\n";
                                resSize = true;
                            } 
                        }
                        //if nothing was printed previously, we break and print error message
                        if(resSize == false){
                            cout << "There are no open tables that will fit the party. Please complete some orders and have the customers pay their bills to free up tables.";
                            break;   
                        }
                        cin >> num;
                        num = num - 1;
                        //input validation, if input is valid we assign reservation to the table selected.
                        if(cin.fail()){
                            cin.clear();
                            cin.ignore();
                            cout << "you entered something that is not a number. please try again. that is not a valid choice.\n";
                        }else if(table[num].occupied == true || table[num].size < reservation[choice].size || num > 19 || num < 0){
                            cout << "that is not a valid choice.\n";
                        }else{
                            order[choice].pending = false;
                            table[num].occupied = true;
                            table[num].numPeople = reservation[choice].size;
                            swap(reservation[choice], reservation[size - 1]);
                            size--;
                            break;
                        }
                    }break;

            }
            //our option 3 block is for ordering
        }else if(option == 3){
            cout << "Please choose the table for the order:\n";
            //bool if an order is ready
            bool orderReady = false;
            while(true){
                //for loop checks that a table is occupied, has people, and hasn't placed an order yet
                for(int i = 0; i < 20; i++){
                    if(table[i].occupied == true && table[i].numPeople != 0 && table[i].ordered == false){
                        cout << table[i].tableNum + 1 << ": " << "Table " << table[i].tableNum + 1 << endl;
                        orderReady = true;
                    }   
                }
                //if no order is ready, we break and print error message
                if(orderReady == false){
                    cout << "There are no tables without orders.\n";
                    break;
                }
                //num is associated with our table number for this block 
                cin >> num;
                num = num - 1;
                //input validation
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(256, '\n');
                    cout << "You entered something that is not a number. Please try again. That is not a valid choice.\n";
                    continue;
                }else if(table[num].occupied == false || num < 0 || num > 19){
                    cout << "that is not a valid choice.\n";
                    continue;
                }else{
                    choice = num;
                }  
             
                //This is where we print our menu and prompt for user selection.
                for(int i = 0; i < table[num].numPeople; i++){
                    int entree = 0; 
                    
                    do{
                        cout << "Please choose entree number " << i + 1 << endl;

                        printMenu(14);
                        cin >> entree;
                        entree = entree - 1;
                        if(cin.fail()){
                            cout << "you entered something that is not a number. please try again. that is not a valid choice ";
                            cin.clear();
                            cin.ignore();
                        }else if(entree > 13 || entree < 0){
                            cout << "That is not a valid choice ";
                        }
                    }while(entree > 13 || entree < 0);
                    //We assign order prices to the order number we are on.
                    order[ordNum].menu[i] = currentMenu[menuItem(entree)];
                    order[ordNum].price[i] = menuPrice[entree];
                    order[ordNum].subTotal += order[ordNum].price[i];
                }
                
                //We assign all our table values to the order number we are on for it. Since we haven't iterated through our pending order yet, we have to use ordNum - 1
                table[num].ordered = true;
                order[ordNum-1].pending = true;
                order[ordNum].ordered = true;
                order[ordNum].isEmpty = false;
                order[ordNum].num = ordNum;
                order[ordNum].table.tableNum = table[num].tableNum;
                order[ordNum].table.size = table[num].size;
                order[ordNum].table.numPeople = table[num].numPeople;
                order[ordNum].table.occupied = table[num].occupied;
                ordNum = ordNum + 1;
                
                break;
            }

        //Our option 4 block is for completing orders
        }else if(option == 4){
            //bool to check if an order is ready. our value variable is the user input for this block.
            int value = 0;
            bool item = false;
            while(true){
                cout << "Choose the order to complete:\n";
                //Prints out all the orders that is confirmed ordered and has not be completed yet.
                for(int i = 0; i < 20; i++){
                    if(order[i].ordered == true && order[i].complete == false){
                        cout << order[i].num + 1 << ": " << "Table " << order[i].table.tableNum + 1 << endl;
                        item = true;
                    }
                }
                //Breaks loop if nothing was printed previously  
                if(item == false){
                    cout << "There are no orders to complete.\n";
                    break;
                }
                cin >> value;
                value = value - 1;
                //input validation
                if(cin.fail()){
                    cout << "you entered something that is not a number. please try again. that is not a valid choice. ";
                    cin.clear();
                    cin.ignore(256, '\n');
                    continue;
                }else if(order[value].ordered== false){
                    cout << "that is not a valid choice. ";
                    continue;
                    //This is the conditional where we break out of the loop and set the order complete to false
                }else if(order[value].complete == false){
                    order[value].complete = true;
                    break;
                }else if (num < 0 || num > 19){
                    cout << "that is not a valid choice. ";
                    continue;
                }
                break;
            }
        //Our option 5 block is for calculating and paying bills
        }else if(option == 5){
            //We have our pay for user input, tableNum for the table number to reset our values, and paymentReady to check if payment is available
            int pay = 0, tableNum = 0;      
            bool paymentReady = false;
            while(true){
                cout << "Choose the order to calculate the bill:\n";
                //Prints out all the orders that is confirmed ordered, isn't empty in values and have a subtotal > 0
                for(int i = 0; i < 50; i++){
                    if(order[i].complete == true && order[i].isEmpty == false && order[i].subTotal > 0){
                        cout << order[i].num + 1 << ": " << "Table " << order[i].table.tableNum + 1 << endl;
                        paymentReady = true;
                    }
                }
                //breaks if nothing was printed previously
                if(paymentReady == false){
                    cout << "There are no orders ready for payment\n";
                    break;
                }
                cin >> pay;
                pay = pay - 1; 
                //Since we have confirmed that the order is ready for payment, we can clear the table values since they're not longer seated
                for(int i = 0; i < 50; i++){
                    if(order[pay].complete == true){
                        tableNum = order[pay].table.tableNum;
                        table[tableNum].occupied = false;
                        table[tableNum].numPeople = 0;
                        table[tableNum].ordered = false;
                    }
                    //If order was not found, we break and print error message
                }if(order[pay].complete == false){
                    cout << "This table either does not exist, is not occupied or has not ordered yet.\n";
                }
                //Input validation
                if(cin.fail()){
                    cin.clear();
                    cin.ignore(256, '\n');
                    continue;
                    //If the inputed order has values, we it the completion of it to true
                }else if(order[pay].isEmpty == false){
                    order[pay].complete = true;
                }else{
                    continue;
                }
                
                 //Prints all our information about the order and calculates the bill   
                cout << "Here is the bill for table " << order[pay].table.tableNum + 1 << endl;
                cout << fixed << setprecision(2);
                for(int i = 0; i < order[pay].table.numPeople; i++){
                    cout << order[pay].menu[i] << " - $" << order[pay].price[i] << endl;
                }

                cout << "Subtotal: $" << order[pay].subTotal << endl;
                cout << "Tip: $" << (order[pay].subTotal * 0.20) << endl;
                cout << "Tax: $" << (order[pay].subTotal * 0.10) << endl;
                cout << "Total: $" << (order[pay].subTotal * 0.20) + (order[pay].subTotal * 0.10) + order[pay].subTotal << endl;

                //We prompt the user to pay
                while(true){
                    cout << "pay bill? ";
                    cin >> confirm;
                    if(confirm == 'Y' || confirm == 'y'){
                        for(int i = 0; i < 50; i++){
                            if(order[pay].complete == true){
                                int tableNum = order[pay].table.tableNum;
                                table[tableNum].occupied = false;
                            }
                        }
                        //After payment confirmation, reset the values on the order
                        exit = true;
                        order[pay-1].pending = false;
                        order[pay].ordered = true;
                        order[pay].isEmpty = false;
                        order[pay].subTotal = 0;
                        cout << defaultfloat;
                        break;
                    }else if(confirm == 'N' || confirm == 'n'){
                        break;
                    }else{
                        cout << "enter y or n. ";
                    }
                }break;         
            
            }
            //Our exit block is for exiting the program if requirements are met. The rest is input validation.
        }else if(option == 6){
            exitProgram(option, order, exit);
            cout << "that is an invalid choice. please try again.\n";
        }else if(cin.fail()){
            cout << "you entered something that is not a number. please try again. that is an invalid choice. please try again.\n";
            cin.clear();
            cin.ignore(256, '\n');
        }else if(option < 0 || option > 6){
            cout << "that is an invalid choice. please try again.\n";
        }
    }
}