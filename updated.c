#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 100

struct BankAccount {
    char name[50];
    int accNumber;
    float balance;

    float savingsGoal;
    float savedAmount;

    char transactions[MAX][100];
    int transCount;

    int lastWithdrawDay;
    int streak;
};

// Get current day
int getCurrentDay() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_yday;
}

// Deposit
void deposit(struct BankAccount *acc, float amount) {
    acc->balance += amount;

    sprintf(acc->transactions[acc->transCount++], "Deposited: %.2f", amount);

    int today = getCurrentDay();
    if (acc->lastWithdrawDay != today) {
        acc->streak++;
    }

    printf("\nDeposit successful!\n");
    printf("Streak: %d days without spending. Great job!\n", acc->streak);
}

// Withdraw
void withdraw(struct BankAccount *acc, float amount) {
    if (amount > acc->balance) {
        printf("\nInsufficient balance!\n");
        return;
    }

    // Correct logic: balance + amount
    printf("\nIf you hadn't spent Rs. %.2f, your account balance would be Rs. %.2f\n",
           amount, acc->balance + amount);

    acc->balance -= amount;

    sprintf(acc->transactions[acc->transCount++], "Withdrawn: %.2f", amount);

    acc->lastWithdrawDay = getCurrentDay();
    acc->streak = 0;

    printf("Withdrawal successful!\n");
}

// Set goal
void setGoal(struct BankAccount *acc, float goal) {
    acc->savingsGoal = goal;
    printf("\nSavings goal set to Rs. %.2f\n", goal);
}

// Add to savings
void addToSavings(struct BankAccount *acc, float amount) {
    if (amount > acc->balance) {
        printf("\nNot enough balance!\n");
        return;
    }

    acc->balance -= amount;
    acc->savedAmount += amount;

    sprintf(acc->transactions[acc->transCount++], "Saved: %.2f", amount);

    printf("\nAdded to savings!\n");
    printf("Progress: Rs. %.2f / Rs. %.2f\n", acc->savedAmount, acc->savingsGoal);

    if (acc->savedAmount >= acc->savingsGoal && acc->savingsGoal > 0) {
        printf("Congratulations! Goal achieved!\n");
    }
}

// Statement
void showStatement(struct BankAccount *acc) {
    printf("\nTransaction History:\n");

    if (acc->transCount == 0) {
        printf("No transactions yet.\n");
        return;
    }

    for (int i = 0; i < acc->transCount; i++) {
        printf("- %s\n", acc->transactions[i]);
    }
}

// Details
void showDetails(struct BankAccount *acc) {
    printf("\nAccount Holder: %s\n", acc->name);
    printf("Account Number: %d\n", acc->accNumber);
    printf("Balance: Rs. %.2f\n", acc->balance);
    printf("Savings: Rs. %.2f / Rs. %.2f\n", acc->savedAmount, acc->savingsGoal);
}

int main() {
    struct BankAccount user;

    printf("===== Welcome to Smart Banking System =====\n");

    // Account creation
    printf("Enter your name: ");
    scanf("%s", user.name);

    printf("Create account number: ");
    scanf("%d", &user.accNumber);

    user.balance = 0;
    user.savingsGoal = 0;
    user.savedAmount = 0;
    user.transCount = 0;
    user.lastWithdrawDay = -1;
    user.streak = 0;

    // Verification
    int enteredAcc;
    printf("\nEnter account number to login: ");
    scanf("%d", &enteredAcc);

    if (enteredAcc != user.accNumber) {
        printf("Invalid account number!\n");
        return 0;
    }

    printf("\nWelcome, %s! Account verified successfully.\n", user.name);

    int choice;
    float amount;

    do {
        printf("\n========= MENU =========\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Set Savings Goal\n");
        printf("4. Add to Savings\n");
        printf("5. View Statement\n");
        printf("6. View Account Details\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter amount: ");
                scanf("%f", &amount);
                deposit(&user, amount);
                break;

            case 2:
                printf("Enter amount: ");
                scanf("%f", &amount);
                withdraw(&user, amount);
                break;

            case 3:
                printf("Enter goal amount: ");
                scanf("%f", &amount);
                setGoal(&user, amount);
                break;

            case 4:
                printf("Enter amount to save: ");
                scanf("%f", &amount);
                addToSavings(&user, amount);
                break;

            case 5:
                showStatement(&user);
                break;

            case 6:
                showDetails(&user);
                break;

            case 0:
                printf("\nThank you for using Smart Banking System!\n");
                break;

            default:
                printf("\nInvalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}