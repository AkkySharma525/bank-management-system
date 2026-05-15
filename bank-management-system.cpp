/*
===============================================
       BANK MANAGEMENT SYSTEM
       C++ OOP Project

  Concepts Covered:
  1.  Class and Object
  2.  Encapsulation
  3.  Abstraction
  4.  Single Inheritance
  5.  Multilevel Inheritance
  6.  Multiple Inheritance
  7.  Runtime Polymorphism
  8.  Compile Time Polymorphism
  9.  Static Member and Function
  10. Friend Function
  11. Friend Class
  12. Constructor and Destructor
  13. Operator Overloading
  14. Exception Handling
===============================================
*/

#include<iostream>
#include<string>
using namespace std;


// =====================================================
//  EXCEPTION CLASSES
//  Concept : Exception Handling
//
//  These classes are used to handle errors.
//  When something goes wrong, we throw these objects.
// =====================================================

// Base exception class
class BankException {
public:
    string message;

    BankException(string msg) {
        message = msg;
    }

    string what() {
        return message;
    }
};

// Thrown when account balance is too low
// Concept: Inheritance — inherits from BankException
class LowBalanceException : public BankException {
public:
    LowBalanceException() : BankException("Error: Balance is too low!") {
    }
};

// Thrown when user enters 0 or negative amount
// Concept: Inheritance — inherits from BankException
class InvalidAmountException : public BankException {
public:
    InvalidAmountException() : BankException("Error: Amount must be greater than 0!") {
    }
};


// =====================================================
//  ABSTRACT CLASS : Account
//
//  Concept: Abstraction
//  — Has pure virtual functions
//  — Cannot create object of this class directly
//
//  Concept: Encapsulation
//  — balance and pin are protected
//  — Cannot be accessed directly from outside
//
//  Concept: Static Member and Function
//  — totalAccounts and nextAccountNumber are static
//  — They are shared across all Account objects
//
//  Concept: Operator Overloading
//  — == operator is overloaded to compare two accounts
// =====================================================

class Account {
protected:
    int    accountNumber;  // protected: child classes can use it
    double balance;        // protected: cannot be changed directly from outside
    int    pin;            // protected: keeps PIN secure

public:
    // Static members — one copy shared by all objects of this class
    static int totalAccounts;
    static int nextAccountNumber;

    // Constructor
    Account(double startBalance, int userPin) {
        accountNumber     = nextAccountNumber;
        nextAccountNumber = nextAccountNumber + 1;
        balance           = startBalance;
        pin               = userPin;
        totalAccounts     = totalAccounts + 1;
        cout << "Account created. Account Number: " << accountNumber << endl;
    }

    // Pure virtual functions — Abstraction
    // Every child class must define these functions
    virtual void deposit(double amount)  = 0;
    virtual void withdraw(double amount) = 0;
    virtual void showDetails()           = 0;

    // Getter functions — Encapsulation
    double getBalance() {
        return balance;
    }

    int getAccountNumber() {
        return accountNumber;
    }

    bool checkPin(int enteredPin) {
        if (pin == enteredPin) {
            return true;
        }
        return false;
    }

    // Static function — called without creating any object
    // Example: Account::getTotalAccounts()
    static int getTotalAccounts() {
        return totalAccounts;
    }

    // Operator Overloading — using == to compare two accounts
    bool operator==(Account& other) {
        if (accountNumber == other.accountNumber) {
            return true;
        }
        return false;
    }

    // Destructor
    ~Account() {
        totalAccounts = totalAccounts - 1;
        cout << "Account deleted. Account Number: " << accountNumber << endl;
    }
};

// Static members must be defined outside the class — this is a C++ rule
int Account::totalAccounts    = 0;
int Account::nextAccountNumber = 1001;


// =====================================================
//  SAVINGS ACCOUNT
//
//  Concept: Single Inheritance
//  — Inherits from Account class
//
//  Concept: Runtime Polymorphism
//  — deposit() and withdraw() are overridden here
//  — Correct version runs at runtime based on object type
//
//  Concept: Compile Time Polymorphism
//  — deposit() is written twice with different parameters
//  — This is called Function Overloading
// =====================================================

class SavingsAccount : public Account {    // Single Inheritance
public:
    double interestRate;

    // Constructor
    // : Account(startBalance, userPin) passes values to parent constructor
    SavingsAccount(double startBalance, int userPin) : Account(startBalance, userPin) {
        interestRate = 4.0;
    }

    // Runtime Polymorphism — overriding parent's virtual deposit()
    void deposit(double amount) override {
        if (amount <= 0) {
            throw InvalidAmountException();
        }
        balance = balance + amount;
        cout << "Deposited Rs." << amount << " | Balance: Rs." << balance << endl;
    }

    // Runtime Polymorphism — overriding parent's virtual withdraw()
    void withdraw(double amount) override {
        if (amount <= 0) {
            throw InvalidAmountException();
        }
        if (amount > balance) {
            throw LowBalanceException();
        }
        balance = balance - amount;
        cout << "Withdrawn Rs." << amount << " | Balance: Rs." << balance << endl;
    }

    void showDetails() override {
        cout << "  Type        : Savings Account"    << endl;
        cout << "  Account No  : " << accountNumber  << endl;
        cout << "  Balance     : Rs." << balance     << endl;
        cout << "  Interest    : " << interestRate << "%" << endl;
    }

    void addInterest() {
        double interest = (balance * interestRate) / 100;
        balance = balance + interest;
        cout << "Interest added: Rs." << interest << " | Balance: Rs." << balance << endl;
    }

    // Compile Time Polymorphism — Function Overloading
    // Same function name deposit() but different parameters
    void deposit(double amount, string note) {
        if (amount <= 0) {
            throw InvalidAmountException();
        }
        balance = balance + amount;
        cout << "Deposited Rs." << amount << " | Note: " << note << " | Balance: Rs." << balance << endl;
    }
};


// =====================================================
//  CURRENT ACCOUNT
//
//  Concept: Single Inheritance
//  — Inherits from Account class
//
//  Concept: Runtime Polymorphism
//  — deposit() and withdraw() are overridden here
//
//  Note: Current account allows overdraft.
//  Overdraft means the customer can withdraw
//  more than their balance up to a set limit.
//  Example: Balance = Rs.1000, Overdraft = Rs.5000
//  Customer can withdraw up to Rs.6000 total.
// =====================================================

class CurrentAccount : public Account {    // Single Inheritance
public:
    double overdraftLimit;

    // Constructor
    CurrentAccount(double startBalance, int userPin) : Account(startBalance, userPin) {
        overdraftLimit = 5000;
    }

    // Runtime Polymorphism — overriding parent's virtual deposit()
    void deposit(double amount) override {
        if (amount <= 0) {
            throw InvalidAmountException();
        }
        balance = balance + amount;
        cout << "Deposited Rs." << amount << " | Balance: Rs." << balance << endl;
    }

    // Runtime Polymorphism — overriding parent's virtual withdraw()
    void withdraw(double amount) override {
        if (amount <= 0) {
            throw InvalidAmountException();
        }
        // Customer can withdraw balance + overdraftLimit
        if (amount > balance + overdraftLimit) {
            throw LowBalanceException();
        }
        balance = balance - amount;
        cout << "Withdrawn Rs." << amount << " | Balance: Rs." << balance << endl;
    }

    void showDetails() override {
        cout << "  Type           : Current Account"      << endl;
        cout << "  Account No     : " << accountNumber    << endl;
        cout << "  Balance        : Rs." << balance       << endl;
        cout << "  Overdraft Limit: Rs." << overdraftLimit << endl;
    }
};


// =====================================================
//  ABSTRACT CLASS : Person
//
//  Concept: Abstraction
//  — Has pure virtual function showInfo()
//  — Cannot create object of this class directly
// =====================================================

class Person {
public:
    string name;
    int    age;

    // Constructor
    Person(string n, int a) {
        name = n;
        age  = a;
    }

    // Pure virtual function — makes Person an abstract class
    virtual void showInfo() = 0;

    // Virtual destructor
    virtual ~Person() {
    }
};


// =====================================================
//  CUSTOMER CLASS
//
//  Concept: Multilevel Inheritance (Level 2)
//  — Person -> Customer
// =====================================================

class Customer : public Person {    // Multilevel Inheritance — Level 2
public:
    int      customerId;
    Account* account;   // Pointer allows storing SavingsAccount or CurrentAccount

    static int nextCustomerId;

    // Constructor
    // : Person(n, a) passes values to parent Person constructor
    Customer(string n, int a) : Person(n, a) {
        customerId     = nextCustomerId;
        nextCustomerId = nextCustomerId + 1;
        account        = NULL;
    }

    void setAccount(Account* acc) {
        account = acc;
    }

    // Implementing pure virtual from Person — Abstraction
    void showInfo() override {
        cout << "  Customer ID : " << customerId << endl;
        cout << "  Name        : " << name       << endl;
        cout << "  Age         : " << age        << endl;
        if (account != NULL) {
            account->showDetails();
        }
    }

    // Destructor — frees memory when object is deleted
    ~Customer() {
        if (account != NULL) {
            delete account;
        }
    }
};

int Customer::nextCustomerId = 1;


// =====================================================
//  LOAN CLASS
//
//  This class is created to demonstrate
//  Multiple Inheritance in PremiumCustomer.
// =====================================================

class Loan {
public:
    double loanAmount;
    bool   isApproved;

    // Constructor
    Loan() {
        loanAmount = 0;
        isApproved = false;
    }

    void applyForLoan(double amount) {
        loanAmount = amount;
        isApproved = false;
        cout << "Loan request of Rs." << amount << " submitted." << endl;
    }

    void approveLoan() {
        isApproved = true;
        cout << "Loan of Rs." << loanAmount << " approved!" << endl;
    }

    void showLoanInfo() {
        cout << "  Loan Amount : Rs." << loanAmount << endl;
        if (isApproved == true) {
            cout << "  Loan Status : Approved" << endl;
        } else {
            cout << "  Loan Status : Pending"  << endl;
        }
    }
};


// =====================================================
//  PREMIUM CUSTOMER
//
//  Concept: Multiple Inheritance
//  — Inherits from both Customer and Loan
//
//  Concept: Multilevel Inheritance (Level 3)
//  — Person -> Customer -> PremiumCustomer
// =====================================================

class PremiumCustomer : public Customer, public Loan {    // Multiple Inheritance
public:
    int rewardPoints;

    // Constructor — calls both parent constructors
    PremiumCustomer(string n, int a) : Customer(n, a), Loan() {
        rewardPoints = 0;
        cout << "Premium Customer created: " << n << endl;
    }

    void addRewardPoints(int pts) {
        rewardPoints = rewardPoints + pts;
        cout << "Points added: " << pts << " | Total: " << rewardPoints << endl;
    }

    // Override showInfo — shows customer info + loan + reward points
    void showInfo() override {
        Customer::showInfo();
        cout << "  Reward Points : " << rewardPoints << endl;
        showLoanInfo();
    }
};


// =====================================================
//  BANK CLASS
//
//  Concept: Friend Function and Friend Class
//  — Private data of Bank can only be accessed
//    by its friend function and friend class
// =====================================================

class Auditor;    // Forward declaration — tells compiler Auditor class exists

class Bank {
private:
    string bankName;      // private — not accessible from outside
    double totalBalance;  // private — not accessible from outside

    // Declaring friend function and friend class
    // They will be able to access the private members above
    friend void showPrivateBankData(Bank b);
    friend class Auditor;

public:
    // Constructor
    Bank(string name) {
        bankName     = name;
        totalBalance = 0;
        cout << "Bank created: " << bankName << endl;
    }

    void addMoney(double amount) {
        totalBalance = totalBalance + amount;
    }

    void showBankInfo() {
        cout << "  Bank Name      : " << bankName << endl;
        // Calling static function using class name — no object needed
        cout << "  Total Accounts : " << Account::getTotalAccounts() << endl;
    }
};


// =====================================================
//  FRIEND FUNCTION
//
//  Concept: Friend Function
//  — A normal function declared as friend of Bank
//  — Can access private members of Bank
// =====================================================

void showPrivateBankData(Bank b) {
    // Accessing private members — allowed because this is a friend function
    cout << "  Bank Name     : " << b.bankName     << endl;
    cout << "  Total Balance : Rs." << b.totalBalance << endl;
}


// =====================================================
//  AUDITOR CLASS
//
//  Concept: Friend Class
//  — Auditor is a friend class of Bank
//  — Can access private members of Bank
// =====================================================

class Auditor {
public:
    void doAudit(Bank b) {
        // Accessing private members — allowed because Auditor is a friend class
        cout << "  Bank Name      : " << b.bankName       << endl;
        cout << "  Total Balance  : Rs." << b.totalBalance << endl;
        cout << "  Total Accounts : " << Account::getTotalAccounts() << endl;
    }
};


// =====================================================
//                   MAIN FUNCTION
// =====================================================

int main() {

    cout << "================================" << endl;
    cout << "    Bank Management System      " << endl;
    cout << "================================" << endl;


    // --- Create Bank ---
    Bank myBank("ABC Bank");
    cout << endl;


    // --- Create Savings Account Customer ---
    cout << "--- Creating Savings Account ---" << endl;
    Customer* c1           = new Customer("Rahul", 25);
    SavingsAccount* savAcc = new SavingsAccount(5000, 1111);
    c1->setAccount(savAcc);
    myBank.addMoney(5000);
    cout << endl;


    // --- Create Current Account Customer ---
    cout << "--- Creating Current Account ---" << endl;
    Customer* c2           = new Customer("Priya", 30);
    CurrentAccount* curAcc = new CurrentAccount(10000, 2222);
    c2->setAccount(curAcc);
    myBank.addMoney(10000);
    cout << endl;


    // --- Create Premium Customer ---
    cout << "--- Creating Premium Customer ---" << endl;
    PremiumCustomer* prem  = new PremiumCustomer("Vikram", 35);
    SavingsAccount* premAcc = new SavingsAccount(25000, 3333);
    prem->setAccount(premAcc);
    myBank.addMoney(25000);
    cout << endl;


    // --- Show Customer Details ---
    cout << "--- Rahul Details ---" << endl;
    c1->showInfo();
    cout << endl;

    cout << "--- Priya Details ---" << endl;
    c2->showInfo();
    cout << endl;


    // --- Exception Handling Demo ---
    cout << "--- Transactions with Exception Handling ---" << endl;
    try {
        savAcc->deposit(2000);            // Normal deposit
        savAcc->deposit(500, "Salary");   // Overloaded deposit — Compile Time Polymorphism
        savAcc->withdraw(1000);           // Normal withdraw
        savAcc->withdraw(99999);          // Will throw LowBalanceException
    }
    catch (LowBalanceException e) {
        cout << e.what() << endl;
    }
    catch (InvalidAmountException e) {
        cout << e.what() << endl;
    }
    catch (BankException e) {
        cout << e.what() << endl;
    }
    cout << endl;


    // --- Runtime Polymorphism Demo ---
    cout << "--- Runtime Polymorphism Demo ---" << endl;
    Account* ptr;              // Base class pointer

    ptr = savAcc;              // ptr now points to SavingsAccount object
    ptr->deposit(200);         // SavingsAccount deposit() runs at runtime

    ptr = curAcc;              // ptr now points to CurrentAccount object
    ptr->deposit(200);         // CurrentAccount deposit() runs at runtime

    cout << "Same pointer called deposit() — different class ran each time." << endl;
    cout << "This is Runtime Polymorphism." << endl;
    cout << endl;


    // --- Interest ---
    cout << "--- Adding Interest ---" << endl;
    savAcc->addInterest();
    cout << endl;


    // --- Operator Overloading Demo ---
    cout << "--- Operator Overloading Demo ---" << endl;
    if (*savAcc == *savAcc) {
        cout << "Same account compared: Result = SAME (== operator worked)" << endl;
    }
    if (!(*savAcc == *curAcc)) {
        cout << "Different accounts compared: Result = DIFFERENT (== operator worked)" << endl;
    }
    cout << endl;


    // --- Static Function Demo ---
    cout << "--- Static Function Demo ---" << endl;
    cout << "Calling getTotalAccounts() without any object:" << endl;
    cout << "Total Accounts = " << Account::getTotalAccounts() << endl;
    cout << endl;


    // --- Premium Customer Features ---
    cout << "--- Premium Customer: Loan and Rewards ---" << endl;
    prem->applyForLoan(50000);
    prem->approveLoan();
    prem->addRewardPoints(200);
    cout << endl;

    cout << "--- Vikram Full Details ---" << endl;
    prem->showInfo();
    cout << endl;


    // --- Bank Info ---
    cout << "--- Bank Info ---" << endl;
    myBank.showBankInfo();
    cout << endl;


    // --- Friend Function Demo ---
    cout << "--- Friend Function Demo ---" << endl;
    cout << "Accessing private Bank data using friend function:" << endl;
    showPrivateBankData(myBank);
    cout << endl;


    // --- Friend Class Demo ---
    cout << "--- Friend Class Demo ---" << endl;
    cout << "Accessing private Bank data using Auditor (friend class):" << endl;
    Auditor auditor;
    auditor.doAudit(myBank);
    cout << endl;


    cout << "================================" << endl;
    cout << "         Program End            " << endl;
    cout << "================================" << endl;


    // Free allocated memory
    delete c1;
    delete c2;
    delete prem;

    return 0;
}


/*
===============================================
                CONCEPT WISE
===============================================

1. CLASS & OBJECT
   Account, Customer, Bank are classes.
   savAcc, curAcc, myBank are objects.

2. ENCAPSULATION
   balance and pin are protected in Account.
   They cannot be changed directly from outside.
   Only accessible through public functions.

3. ABSTRACTION
   Account and Person have pure virtual functions.
   So we cannot create their objects directly.
   Only child class objects can be created.

4. SINGLE INHERITANCE
   SavingsAccount and CurrentAccount
   both inherit from Account class.

5. MULTILEVEL INHERITANCE
   Person -> Customer -> PremiumCustomer
   Three levels of inheritance.

6. MULTIPLE INHERITANCE
   PremiumCustomer inherits from
   both Customer and Loan at the same time.

7. RUNTIME POLYMORPHISM
   Account* ptr points to SavingsAccount or CurrentAccount.
   deposit() called on ptr runs different code at runtime.

8. COMPILE TIME POLYMORPHISM
   deposit(amount) and deposit(amount, note)
   Same name, different parameters — Function Overloading.

9. STATIC MEMBER & FUNCTION
   totalAccounts is static — shared by all objects.
   Account::getTotalAccounts() — called without any object.

10. FRIEND FUNCTION
    showPrivateBankData() is a normal function
    but declared as friend of Bank so it can
    access Bank's private members.

11. FRIEND CLASS
    Auditor is declared as friend class of Bank
    so it can access Bank's private members.

12. CONSTRUCTOR & DESTRUCTOR
    Every class has a constructor to set initial values.
    Destructor frees memory and prints a message.

13. OPERATOR OVERLOADING
    == operator is overloaded in Account class
    to compare two account objects directly.

14. EXCEPTION HANDLING
    Custom exceptions inherit from BankException.
    try-catch is used during transactions to
    handle errors gracefully.

===============================================
*/
