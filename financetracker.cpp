#include<iostream>
#include<fstream>
#include<ctime>//for getting the current date to store expenses every day 
#include <sstream>
#include<vector>

using namespace std;


const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";







class FinanceTracker{
   
private:

 float totalIncome = 0;

float budgetGoal = 0;
bool incomeSet = false;
bool goalSet = false;
string currentMonth;
vector<pair<string,float>>dailyExpenses; //for storing current date and expense amount for that day

 string getCurrentDate() {
        time_t t = time(nullptr);
        tm *now = localtime(&t);
        stringstream ss;
        ss << (now->tm_year + 1900) << "_";
        ss << (now->tm_mon + 1 < 10 ? "0" : "") << (now->tm_mon + 1);
        return ss.str();  // e.g., "2025_06"
    }
//file for current date
     string getFileName() {
        time_t t = time(nullptr);
        tm *now = localtime(&t);
        stringstream ss;
        ss << (now->tm_year + 1900) << "_";
        ss << (now->tm_mon + 1 < 10 ? "0" : "") << now->tm_mon + 1;
        return "FinanceRecords/" + ss.str() + ".txt";
    }

 void loadData() { 
      string filename = getFileName();
        ifstream file(filename);
        if (!file) return;

        file >> incomeSet >> goalSet >> totalIncome >> budgetGoal;
        string line;
        while (getline(file, line)) {
            if (line == "Expenses:") break;
        }
        while (getline(file, line)) {
            if (line.length() < 12) continue;
            string date = line.substr(0, 10);
            float amount = stof(line.substr(11));
            dailyExpenses.push_back({date, amount});
        }
        file.close();
    }
void savedata() {
 string filename = getFileName();
        ofstream file(filename);
        if (!file) return;

        file << "Income:" << totalIncome << "\n";
        file << "BudgetGoal:" << budgetGoal << "\n";
        file << "Expenses:\n";
        for (const auto &entry : dailyExpenses) {
            file << entry.first << " " << entry.second << "\n";
        }
        file.close();
       
}
public:

//constructor
    FinanceTracker(){
 
        currentMonth = getFileName();
        
        loadData();
  
 }

void add_income() {
    if (incomeSet) {
            cout << "Income already set for this month.\n";
            return;
        }
        cout << "Enter monthly income: Rs ";
        cin >> totalIncome;
        incomeSet = true;
        cout << "Monthly income recorded.\n";
}
void set_goals() {

     if (goalSet) {
            cout << "Budget goal already set for this month.\n";
            return;
        }
        cout << "Enter budget goal for the month: Rs ";
        cin >> budgetGoal;
        goalSet = true;
        cout << "Budget goal set.\n";
}

void add_expense() {
     float amount;
        string today = getCurrentDate();
        cout << "Enter expense amount for " << today << ": ";
        cin >> amount;
        dailyExpenses.push_back({today, amount});
        cout << "Expense recorded.\n";
}

void view_details() {
     float totalExpense=0;
     for (auto& e : dailyExpenses) totalExpense += e.second;

        cout << "\n--- Monthly Summary (" << currentMonth << ") ---\n";
        cout << "Total Income     : Rs " << totalIncome << endl;
        cout << "Total Expense    : Rs " << totalExpense << endl;
        cout << "Remaining Balance: Rs " << totalIncome - totalExpense << endl;

}



void view_progress() {
    if (!goalSet) {
            cout << "Please set your budget goal first.\n";
            return;
        }
        float totalExpense = 0;
        for (auto& e : dailyExpenses) totalExpense += e.second;
        float saved = totalIncome - totalExpense;

        cout << "\n--- Savings Progress ---\n";
        cout << "You have saved: Rs " << saved << " out of Rs " << budgetGoal << endl;
        float percent = (saved / budgetGoal) * 100;
        cout << "Progress: " << percent << "%\n";

}
 void save_exit() {
        savedata();
        cout << "Data saved successfully. Goodbye!\n";
    }



 



};

//Using enum 
enum MenueOption{
     addIncome=1,
     addExpense,
     viewDetails,
     setGoals,
     viewProgress,
     saveExit,
};


int main(){

    //constructor called by default
    FinanceTracker tracker;

int choice=0;
while(choice!='n'||choice!='N')   {
     cout <<BOLD<< "\n==== Personal"<<BLUE<<" Finance Tracker"<<RESET<<BOLD<<" ====\n"<<RESET<<endl;
        cout << YELLOW<<"1. "<<RESET<<GREEN<<"Add Income\n"<<RESET;
        cout << YELLOW<<"2. "<<RESET<<GREEN<<"Add Expense\n"<<RESET;
        cout << YELLOW<<"3. "<<RESET<<GREEN<<"View Summary\n"<<RESET;
        cout << YELLOW<<"4. "<<RESET<<GREEN<<"Set Budget/Savings Goal\n"<<RESET;
        cout << YELLOW<<"5. "<<RESET<<GREEN<<"View Progress\n"<<RESET;
        cout << YELLOW<<"6. "<<RESET<<GREEN<<"Save and Exit\n"<<RESET;
        cout <<RED<<BOLD<< "Enter your choice: "<<RESET;
        cin >> choice;
try{
    switch(choice){
        case addIncome:
        tracker.add_income();
        break;

        case addExpense:
        tracker.add_expense();
         break;

        case viewDetails:
        tracker.view_details();
         break;

        case setGoals:
        tracker.set_goals();
         break;

        case viewProgress:
        tracker.view_progress();
      
         break;

        case saveExit:
        tracker.save_exit();
         break;

        default:
        throw invalid_argument("Invalid menu choice.");

    }
}

//This block handles any exceptions that are thrown inside the try block.
catch(exception &e){
    cout<<"ERROR:"<<e.what()<<endl;
}
}
return 0;
}

