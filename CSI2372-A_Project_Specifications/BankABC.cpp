//************************************************************************
//
//   Project
//   Fall 2019
//
//   Mohamed TALEB
//
// This program reads information about clients and transactions in the following 2 files:
//          - clients.txt
//          - transact.txt
//
// It also allows certain operations on the read data:
//          - Display client bank accounts
//          - Updating client accounts
//
//************************************************************************
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>

#include "BankABC.h"
#pragma warning(suppress : 4996)

using namespace std;

double DepositAccount::rate = 6.50;
double totalAccounts = 0.0;

//******************************************************************
// Basic functions of the BankAccount class
//******************************************************************
inline BankAccount::BankAccount()
{
    accountId = type = updateDate = 0;
    clientName = new char[1];
    balance = 0.0;
}

inline BankAccount::BankAccount(long id, int newType,
                                    char * name, long newDate, double newBalance):
                                    accountId(id), type(newType),
                                    updateDate(newDate), balance(newBalance)
{
     clientName = new char[strlen(name) + 1];
#pragma warning(suppress : 4996)		//was an express error on VS
     strcpy(clientName, name);
}

inline BankAccount::~BankAccount()
{
     if (clientName)
     {
          delete [] clientName;
     }
}

inline void BankAccount::setAccountId(long account)
{
     accountId = account;
}

inline void BankAccount::setType(int newType)
{
     type = newType;
}

inline void BankAccount::setClientName(char * name)
{
     if (clientName)
     {
          delete [] clientName;
     }
     clientName = new char[strlen(name) + 1];
#pragma warning(suppress : 4996)  //was an express error for strcpy
	 strcpy(clientName, name);
}

inline void BankAccount::setUpdateDate(long newDate)
{
     updateDate = newDate;
}

inline void BankAccount::setBalance(double newBalance)
{
     balance = newBalance;
}

void BankAccount::print()
{
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << accountId << "\t\t" << type << "\t" << updateDate << "\t\t" << balance;
}

//******************************************************************
// Basic functions of the DepositAccount class
//******************************************************************
inline DepositAccount::DepositAccount(long id, int newType, char * name,
                                    long newDate, double balanceArg, int nbyear) :
                                    BankAccount(id, newType, name,
                                    newDate, balanceArg), nbyears(nbyear)
{}

inline void DepositAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

void DepositAccount::print()
{
    Bonus();
    BankAccount::print();
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Functions of the class DepositAccount. It allows to calculate
// the annual bonus on deposit accounts.
// Inputs: Nothing
// Outputs: Nothing
//******************************************************************
void DepositAccount::Bonus()
{
  setBalance(getBalance() + (getBalance()* getRate() * (getNbYears())/36000.00));
}



//******************************************************************
// Basic functions of the LoanAccount class
//******************************************************************
inline LoanAccount::LoanAccount(long id, int newType, char * name,
                                    long newDate, double newBalance, int nbyear,
                                    double newRate) : BankAccount(id, newType,
                                    name, newDate, newBalance), nbyears(nbyear), rate(newRate)
{ }

inline void LoanAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

inline void LoanAccount::setRate(double newRate)
{
     rate = newRate;
}

void LoanAccount::print()
{
	
	BankAccount::print();
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Basic functions of the Transaction class
//******************************************************************
inline Transaction::Transaction(long idTr, int typeTr, long dateTr,
                                    int codeTr = 01, double amountTr):
                                    accountId(idTr), type(typeTr),
                                    date(dateTr), code(codeTr),
                                    amount(amountTr)
{ }

inline void Transaction::setAccountId(long compteTr)
{
     accountId = compteTr;
}

inline void Transaction::setType(int typeTr)
{
     type = typeTr;
}

inline void Transaction::setDate(long dateTr)
{
     date = dateTr;
}

inline void Transaction::setCode(int codeTr)
{
     code = codeTr;
}

inline void Transaction::setAmount(double amountTr)
{
     amount = amountTr;
}




//****************************************************************************
// Purpose: Sort a list of bank accounts in ascending order of ids and types.
//
// WARNING: This sort leaves the last account (of id 0) of the list which has
// its position to assure the tests of end of list later !!!
//
// Inputs: listAccount(BankAccount *), a list of bank accounts.
// Outputs: listAccount(BankAccount *), sorted list of bank accounts.
//****************************************************************************
void sortAccounts(BankAccount ** list)
{
    BankAccount * temp = nullptr;
    for(int i=0;i<K_SizeMax;i++){				//loop for sorting the account ID 
        for(int j=0;j<K_SizeMax-1;j++){
            if(list[j+1]->getAccountId()==0){
                break;
            }else if(list[j]->getAccountId() > list[j+1]->getAccountId()){
                temp = list[j];
                list[j] = list[j+1];
                list[j+1] = temp;
            }
        }
    }
    string name1;
    string name2;
    for(int i=0;i<K_SizeMax;i++){				//loop for sorting the account type
        for(int j=0;j<K_SizeMax-1;j++){
            if(list[j+1]->getAccountId()==0){
                break;
            }else if(list[j]->getType() > list[j+1]->getType()){
                name1=list[j]->getClientName();
                name2=list[j+1]->getClientName();
                if(name1 == name2){
                    temp = list[j];
                    list[j] = list[j+1];
                    list[j+1] = temp;
                }
            }
        }
    }
}

//******************************************************************
// Purpose: This function reads the file 'clients.txt' and builds
// an array containing the different bank accounts of customers.
//
// Inputs: Nothing
// Output: listAccount(type: BankAccount *), the list of bank
//         accounts read from the file 'clients.txt'.
//******************************************************************
BankAccount ** readAccounts()
{
    ifstream inputFile("clients.txt");	// Opening the input file
    if (!inputFile)            		// If the file is not found...
    {
        cout << "File not found !!!" << endl;
        exit(0);
    }

    BankAccount ** listAccounts = new BankAccount*[K_SizeMax];
    if (!listAccounts) {
        cout << "Full memory. !!!" << endl;
        return listAccounts;
    }

    BankAccount ** pAccount = listAccounts;

    long accountRead, dateRead;
    int TypeRead, nbyearRead, counter = 0;
    double balanceRead, RateRead;
    char nameRead[60];

    inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
    inputFile.getline(nameRead, 60);

    while (inputFile && (counter < K_SizeMax - 1)){
        // YOU HAVE TO DO SOMETHING FROM HERE !!!
		switch (TypeRead) {					//create account accordong to type
			case 1:pAccount[0] = new BankAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead); break;
			case 2:pAccount[0] = new BankAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead); break;
			case 3:pAccount[0] = new DepositAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead); break;
			case 4:
				balanceRead = balanceRead + (balanceRead * RateRead * (nbyearRead / 36000.00));		//see instruction for how to calculate balance on loan upon creation
				pAccount[0] = new LoanAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead, RateRead);
				break;
			default:
				cout << "error during readind account " << accountRead;
				break;
		}

 
        // UNTIL THIS POINT.

          inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
          inputFile.getline(nameRead, 60);
          pAccount++;
          counter++;
    }
     *pAccount = new BankAccount();
     return listAccounts;
}





//*****************************************************************************************
// Purpose: This function validates whether the transaction code
//          corresponds to the correct account:
//              - 01 ==> account (01: Check, 02: Savings, 03: Deposit and 04: Loan)
//              - 02 ==> account (01: Check, 02: Savings)
//              - 03 ==> account (01: Check).
//
// Inputs: trans (Type: Transaction) an instance of the Transaction class.
// Outputs: true (Type bool) if the transaction matches one of the accounts listed above.
// false (Type bool) otherwise.
//*****************************************************************************************
Bool BankAccount::validateTransaction(const Transaction trans) const
{
    if ( ( (trans.getCode() == 02) && ( isDepositAccount() || isLoanAccount()) ) ||
         ( (trans.getCode() == 03) && ( isDepositAccount() || isLoanAccount() || isSavingsAccount() ) ) )
      {
       return FALSE;
      }
    else
      {
       return TRUE;
      }

}





//******************************************************************************
// Purpose: This function is used to execute the transaction already performed
// (update the balance of an account).
//
// Inputs: trans (Transaction Type), instance of Transaction class
// Outputs: Nothing
//*******************************************************************************
void BankAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
       {
         if (trans.getCode() == 01)    // Deposit
           {
             setBalance(getBalance() + trans.getAmount());
           }
         else
           { if (trans.getCode() == 02)    // Withdrawal
                {
                  if (getBalance() >= trans.getAmount())
                     { setBalance(getBalance() - (trans.getAmount() + 0.50)); }
                  else {cout << " insufficient balance!! " << endl; }
                }
             else 			// Check
                {
                  if (getBalance() >= trans.getAmount())
                     {
                       setBalance(getBalance() - trans.getAmount());
                     }
                  else {cout << " insufficient balance!! " << endl; }
                }
           }

       }

}

//***********************************************************************
// Purpose: This function is used to execute a read transaction
// (updating the balance of the loan account).
//
// Inputs: trans (Transaction Type), instance of the Transaction class
// Outputs: Nothing
//***********************************************************************
void LoanAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
     {
        if (trans.getCode() == 01)    // Deposit
        {
          setBalance(getBalance() - trans.getAmount());

        }
     }
}





//*************************************************************************
// Purpose: This function allows to read the file 'transact.txt'
//          and to update the accounts concerned by the transactions read.
//
// Inputs: listAccount (type BankAccount *), the list of bank accounts.
// Output: Nothing.
//*************************************************************************
void updateAccounts(BankAccount ** listAccounts) {
     ifstream inputFile("transact.txt");	// Opening the input file

	 if (!inputFile)            		// If the file is not found...
	 {
		 cout << "Transaction File not found !!!" << endl;
		 return;
	 }

	 long accountRead, dateRead;
	 int TypeRead, transCodeRead, counter = 0;
	 double amountRead;
	 bool validationTrans;

	 inputFile >> accountRead >> TypeRead >> dateRead >> transCodeRead >> amountRead;

	 int i;
	 while (inputFile && (counter < K_SizeMax - 1)) {
		 for (i = 0; i < K_SizeMax; i++)
		 {
			 if (listAccounts[i]->getAccountId() == 0) {			//stop because end of list so the account ID or type dont exist
				 cout << " the transaction on " << accountRead << " of type " << TypeRead << " on date " << dateRead << " transaction code " << transCodeRead
					 << " amount " << amountRead << " cannot be executed. account ID or type dont exist" << endl;
				 break;
			 }
			 
			 if (listAccounts[i]->getAccountId() == accountRead &&		//find the account and type
				 listAccounts[i]->getType() == TypeRead) {
				 Transaction trans(accountRead, TypeRead, dateRead, transCodeRead, amountRead);
				 if (listAccounts[i]->validateTransaction(trans)) {
					 listAccounts[i]->executeTransaction(trans);
				 }
				 else {
					 cout << " the transaction on " << accountRead << " of type " << TypeRead << " on date " << dateRead << " transaction code " << transCodeRead 
						 << " amount " << amountRead <<" cannot be executed. ILLEGAL TRANSACTION ON ACCOUNT TYPE"<< endl;
				 }
				 break;  //other account dont need to be checked
			 }
		 }
		 inputFile >> accountRead >> TypeRead >> dateRead >> transCodeRead >> amountRead;		//preparing next entry
		 counter++;
	 }
}

//******************************************************************************
// Purpose: This function displays the list of bank accounts for all customers.
//
// Inputs: listAccount (type: BankAccount *), the list of bank accounts.
// Outputs: Nothing
//******************************************************************************
void displayAccounts(BankAccount ** listAccounts)
{
    cout << endl << endl << endl;
    cout << "                       THE REPORT OF THE BANK ACCOUNTS OF CLIENTS" << endl;
    cout << "                       ------------------------------------------" << endl << endl;

    int i = 0;
    double total = 0;
    string currentClient;
    
    for(i = 0;i<K_SizeMax;i++){
        if (listAccounts[i]->getAccountId()==0){		//end of the list 
            break;
        }
        if (currentClient == listAccounts[i]->getClientName()){

        }else{								//client has changed so balance has to be produce
            if(currentClient!=""){
                cout<<"Total Balance = "<<total<<endl;
                cout<<"-----------------------------------"<<endl;
                cout<<endl;
                total = 0;
            }
            cout<<"Client Name: "<<listAccounts[i]->getClientName()<<endl;
            currentClient = listAccounts[i]->getClientName();
            cout.setf(ios::fixed);
            cout.precision(2);
            cout<<"Bank Account"<< "\t" << "Type" << "\t" << "Update Date" << "\t" << "Balance"<<"\t\t" << "NB Years" << "\t" << "Rate" << endl;
            cout<<"------------"<< "\t" << "----" << "\t" << "-----------" << "\t" << "-------"<<"\t\t" << "--------" << "\t" << "----" << endl;
        }
        if(listAccounts[i]->isDepositAccount()){
            dynamic_cast<DepositAccount*>(listAccounts[i])->print();
        }else if (listAccounts[i]->isLoanAccount()){
            dynamic_cast<LoanAccount*>(listAccounts[i])->print();
        }else{
            listAccounts[i]->print();
        }
        total += listAccounts[i]->getBalance();
        cout<<endl;
    }
        cout<<"Total Balance = "<<total<<endl;
        cout<<"-----------------------------------"<<endl;
        cout<<endl;
        total = 0;

}




int main()
{
    BankAccount ** list = readAccounts();
    sortAccounts(list);
    displayAccounts(list);
    updateAccounts(list);
    cout << endl << endl;
    cout << "               ************************************************" << endl;
    cout << "               * REAFFICHAGE DES DONNEES APRES LA MISE A JOUR *" << endl;
    cout << "               ************************************************" << endl;
    displayAccounts(list);
    cout << endl;

	system("PAUSE");
	return 0;
}
