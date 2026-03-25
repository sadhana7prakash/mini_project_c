#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;

    double goalAmount;
    double savedAmount;
};

unsigned int enterChoice(void);
void deposit(FILE *fPtr);
void withdraw(FILE *fPtr);
void addToSavings(FILE *fPtr);
void setGoal(FILE *fPtr);
void viewGoal(FILE *fPtr);
void ghostSimulation(int acc);
void streakTracker(int acc);
void logTransaction(int acc,double amount,char type);
void miniStatement(int acc);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    cfPtr = fopen("credit.dat","rb+");

if (cfPtr == NULL)
{
    cfPtr = fopen("credit.dat","wb+");   
    
    if (cfPtr == NULL)
    {
        printf("File could not be created.\n");
        exit(1);
    }
}

    while((choice = enterChoice()) != 12)
    {
        switch(choice)
        {
            case 5: deposit(cfPtr); break;
            case 6: withdraw(cfPtr); break;

            case 7:
            {
                int acc;
                printf("Enter account: ");
                scanf("%d",&acc);
                miniStatement(acc);
                break;
            }

            case 8: setGoal(cfPtr); break;
            case 9: viewGoal(cfPtr); break;
            case 10: addToSavings(cfPtr); break;

            case 11:
            {
                int acc;
                printf("Enter account: ");
                scanf("%d",&acc);
                ghostSimulation(acc);
                streakTracker(acc);
                break;
            }
        }
    }

    fclose(cfPtr);
    return 0;
}

unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n1-4 Basic Features\n");
    printf("5 Deposit\n6 Withdraw\n7 Statement\n");
    printf("8 Set Goal\n9 View Goal\n10 Add to Savings\n");
    printf("11 Smart Insights\n12 Exit\n? ");

    scanf("%u",&choice);
    return choice;
}

void deposit(FILE *fPtr)
{
    struct clientData c = {0};
    int acc; double amt;

    printf("Account: "); scanf("%d",&acc);

    fseek(fPtr,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fPtr);

    if(c.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Amount: "); scanf("%lf",&amt);

    c.balance += amt;

    printf("💡 Tip: Consider saving some money!\n");

    fseek(fPtr, -(long)sizeof(c), SEEK_CUR);
    fwrite(&c,sizeof(c),1,fPtr);

    logTransaction(acc,amt,'D');
}

void withdraw(FILE *fPtr)
{
    struct clientData c = {0};
    int acc; double amt;

    printf("Account: "); scanf("%d",&acc);

    fseek(fPtr,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fPtr);

    if(c.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Amount: "); scanf("%lf",&amt);

    if(amt > c.balance)
    {
        printf("Insufficient balance\n");
        return;
    }

    c.balance -= amt;

    printf("⚠ You spent money. Stay mindful!\n");

    fseek(fPtr, -(long)sizeof(c), SEEK_CUR);
    fwrite(&c,sizeof(c),1,fPtr);

    logTransaction(acc,amt,'W');
}

void addToSavings(FILE *fPtr)
{
    struct clientData c = {0};
    int acc; double amt;

    printf("Account: "); scanf("%d",&acc);

    fseek(fPtr,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fPtr);

    if(c.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Amount to save: "); scanf("%lf",&amt);

    if(amt > c.balance)
    {
        printf("Not enough balance\n");
        return;
    }

    c.balance -= amt;
    c.savedAmount += amt;

    fseek(fPtr, -(long)sizeof(c), SEEK_CUR);
    fwrite(&c,sizeof(c),1,fPtr);

    printf("Saved successfully!\n");
}

void setGoal(FILE *fPtr)
{
    struct clientData c = {0};
    int acc;

    printf("Account: "); 
    scanf("%d",&acc);

    fseek(fPtr,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fPtr);

    if(c.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Goal: "); scanf("%lf",&c.goalAmount);

    fseek(fPtr, -(long)sizeof(c), SEEK_CUR);
    fwrite(&c,sizeof(c),1,fPtr);
}

void viewGoal(FILE *fPtr)
{
    struct clientData c = {0};
    int acc;

    printf("Account: "); scanf("%d",&acc);

    fseek(fPtr,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fPtr);

    if(c.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Goal: %.2f Saved: %.2f Remaining: %.2f\n",
           c.goalAmount,c.savedAmount,
           c.goalAmount-c.savedAmount);
}

void ghostSimulation(int acc)
{
    FILE *log = fopen("transactions.txt","r");
    if(log == NULL)
    {
        printf("No transaction history found\n");
        return;
    }

    char line[200];
    double total = 0;

    while(fgets(line,200,log))
    {
        int fileAcc;
        char type;
        double amt;

        if(sscanf(line,"Account:%d | %c | %lf",&fileAcc,&type,&amt) == 3)
        {
            if(fileAcc == acc && type == 'W')
                total += amt;
        }
    }

    printf("👻 If you saved your spending: %.2f\n",total*1.1);

    fclose(log);
}

void streakTracker(int acc)
{
    FILE *log = fopen("transactions.txt","r");
    if(log == NULL)
    {
        printf("No transaction history found\n");
        return;
    }

    char line[200];
    int streak = 0;

    while(fgets(line,200,log))
    {
        int fileAcc;
        char type;

        if(sscanf(line,"Account:%d | %c",&fileAcc,&type) == 2)
        {
            if(fileAcc == acc)
            {
                if(type == 'W')
                    streak = 0;
                else
                    streak++;
            }
        }
    }

    printf("🔥 No-spend streak: %d transactions\n",streak);

    fclose(log);
}

void logTransaction(int acc,double amt,char type)
{
    FILE *log = fopen("transactions.txt","a");
    if(log == NULL)
    {
        printf("Error writing transaction\n");
        return;
    }

    time_t t;
    time(&t);

    fprintf(log,"Account:%d | %c | %.2f | %s",
            acc,type,amt,ctime(&t));

    fclose(log);
}

void miniStatement(int acc)
{
    FILE *log = fopen("transactions.txt","r");
    if(log == NULL)
    {
        printf("No transaction history found\n");
        return;
    }

    char line[200];

    while(fgets(line,200,log))
    {
        int fileAcc;

        if(sscanf(line,"Account:%d",&fileAcc) == 1)
        {
            if(fileAcc == acc)
                printf("%s",line);
        }
    }

    fclose(log);
}