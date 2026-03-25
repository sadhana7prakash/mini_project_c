#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

struct clientData
{
    int acctNum;
    char name[30];
    double balance;
    double goal;
    double saved;
};

int getAccount()
{
    int acc;
    printf("Enter account number (1-100): ");
    scanf("%d",&acc);

    if(acc < 1 || acc > MAX)
    {
        printf("Invalid account number!\n");
        return -1;
    }
    return acc;
}

void logTransaction(int acc,double amt,char type)
{
    FILE *log = fopen("log.txt","a");

    if(log == NULL)
    {
        printf("Error opening log file!\n");
        return;
    }

    time_t t;
    time(&t);

    fprintf(log,"Acc:%d | %c | %.2f | %s",
            acc,type,amt,ctime(&t));

    fclose(log);
}

void createAccount(FILE *fp)
{
    struct clientData c = {0};

    printf("Enter account number (1-100): ");
    scanf("%d",&c.acctNum);

    printf("Enter name: ");
    scanf(" %[^\n]",c.name);

    printf("Initial deposit: ");
    scanf("%lf",&c.balance);

    fseek(fp,(c.acctNum-1)*sizeof(c),SEEK_SET);
    fwrite(&c,sizeof(c),1,fp);

    printf("✅ Account created successfully!\n");
}

void deposit(FILE *fp)
{
    struct clientData c;
    int acc = getAccount();
    if(acc == -1) return;

    fseek(fp,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fp);

    if(c.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    double amt;
    printf("Enter amount: ");
    scanf("%lf",&amt);

    c.balance += amt;

    fseek(fp, -(long)sizeof(c), SEEK_CUR);
    fwrite(&c,sizeof(c),1,fp);

    logTransaction(acc,amt,'D');

    printf("✅ Deposited! Balance: %.2f\n",c.balance);
}

void withdraw(FILE *fp)
{
    struct clientData c;
    int acc = getAccount();
    if(acc == -1) return;

    fseek(fp,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fp);

    if(c.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    double amt;
    printf("Enter amount: ");
    scanf("%lf",&amt);

    if(amt > c.balance)
    {
        printf("❌ Insufficient balance!\n");
        return;
    }

    c.balance -= amt;

    fseek(fp, -(long)sizeof(c), SEEK_CUR);
    fwrite(&c,sizeof(c),1,fp);

    logTransaction(acc,amt,'W');

    printf("✅ Withdrawn! Balance: %.2f\n",c.balance);
}

void checkBalance(FILE *fp)
{
    struct clientData c;
    int acc = getAccount();
    if(acc == -1) return;

    fseek(fp,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fp);

    if(c.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("\nName: %s\nBalance: %.2f\n",c.name,c.balance);
}

void setGoal(FILE *fp)
{
    struct clientData c;
    int acc = getAccount();
    if(acc == -1) return;

    fseek(fp,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fp);

    printf("Enter goal amount: ");
    scanf("%lf",&c.goal);

    fseek(fp, -(long)sizeof(c), SEEK_CUR);
    fwrite(&c,sizeof(c),1,fp);

    printf("🎯 Goal set!\n");
}

void viewGoal(FILE *fp)
{
    struct clientData c;
    int acc = getAccount();
    if(acc == -1) return;

    fseek(fp,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fp);

    printf("Goal: %.2f | Saved: %.2f | Remaining: %.2f\n",
           c.goal,c.saved,c.goal-c.saved);
}

void saveMoney(FILE *fp)
{
    struct clientData c;
    int acc = getAccount();
    if(acc == -1) return;

    fseek(fp,(acc-1)*sizeof(c),SEEK_SET);
    fread(&c,sizeof(c),1,fp);

    double amt;
    printf("Enter saving amount: ");
    scanf("%lf",&amt);

    if(amt > c.balance)
    {
        printf("❌ Not enough balance!\n");
        return;
    }

    c.balance -= amt;
    c.saved += amt;

    fseek(fp, -(long)sizeof(c), SEEK_CUR);
    fwrite(&c,sizeof(c),1,fp);

    printf("💰 Saved successfully!\n");
}

void statement(int acc)
{
    FILE *log = fopen("log.txt","r");

    if(log == NULL)
    {
        printf("No transaction history found!\n");
        return;
    }

    char line[200];
    int found = 0;

    printf("\n📄 Mini Statement:\n");

    while(fgets(line,200,log))
    {
        int a;
        char type;
        double amt;

        if(sscanf(line,"Acc:%d | %c | %lf",&a,&type,&amt) == 3)
        {
            if(a == acc)
            {
                printf("%s",line);
                found = 1;
            }
        }
    }

    if(!found)
        printf("No transactions found.\n");

    fclose(log);
}

void insights(int acc)
{
    FILE *log = fopen("log.txt","r");

    if(log == NULL)
    {
        printf("No data available!\n");
        return;
    }

    char line[200];
    double spent = 0;
    int streak = 0;

    while(fgets(line,200,log))
    {
        int a;
        char type;
        double amt;

        if(sscanf(line,"Acc:%d | %c | %lf",&a,&type,&amt) == 3)
        {
            if(a == acc)
            {
                if(type == 'W')
                {
                    spent += amt;
                    streak = 0;
                }
                else
                {
                    streak++;
                }
            }
        }
    }

    printf("\n💡 If you saved spending: %.2f\n",spent*1.1);
    printf("🔥 No-spend streak: %d\n",streak);

    fclose(log);
}

int main()
{
    FILE *fp = fopen("bank.dat","rb+");

    if(fp == NULL)
    {
        fp = fopen("bank.dat","wb+");

        struct clientData blank = {0};

        for(int i=0;i<MAX;i++)
        {
            fwrite(&blank,sizeof(blank),1,fp);
        }
    }

    int choice;

    while(1)
    {
        printf("\n====== SMART BANK ======\n");
        printf("1 Create Account\n");
        printf("2 Deposit\n");
        printf("3 Withdraw\n");
        printf("4 Check Balance\n");
        printf("5 Set Goal\n");
        printf("6 View Goal\n");
        printf("7 Save Money\n");
        printf("8 Statement\n");
        printf("9 Smart Insights\n");
        printf("10 Exit\n");
        printf("Enter choice: ");

        scanf("%d",&choice);

        if(choice == 10) break;

        switch(choice)
        {
            case 1: createAccount(fp); break;
            case 2: deposit(fp); break;
            case 3: withdraw(fp); break;
            case 4: checkBalance(fp); break;
            case 5: setGoal(fp); break;
            case 6: viewGoal(fp); break;
            case 7: saveMoney(fp); break;

            case 8:
            {
                int acc = getAccount();
                if(acc != -1) statement(acc);
                break;
            }

            case 9:
            {
                int acc = getAccount();
                if(acc != -1) insights(acc);
                break;
            }

            default:
                printf("Invalid choice!\n");
        }
    }

    fclose(fp);
    return 0;
}