#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

unsigned int enterChoice(void);
void deposit(FILE *fPtr);
void withdraw(FILE *fPtr);
void logTransaction(int acc,double amount,char type);
void miniStatement(int acc);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("credit.dat","rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        exit(1);
    }

    while((choice = enterChoice()) != 8)
    {
        switch(choice)
        {
            case 5:
                deposit(cfPtr);
                break;

            case 6:
                withdraw(cfPtr);
                break;

            case 7:
            {
                int acc;
                printf("Enter account number: ");
                scanf("%d",&acc);
                miniStatement(acc);
                break;
            }

            default:
                printf("Feature not implemented in this demo.\n");
        }
    }

    fclose(cfPtr);
}

unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\nBank Management System\n");
    printf("1 - Store text file\n");
    printf("2 - Update account\n");
    printf("3 - Add new account\n");
    printf("4 - Delete account\n");
    printf("5 - Deposit money\n");
    printf("6 - Withdraw money\n");
    printf("7 - View mini statement\n");
    printf("8 - Exit\n");
    printf("? ");

    scanf("%u",&menuChoice);

    return menuChoice;
}

void deposit(FILE *fPtr)
{
    struct clientData client={0,"","",0.0};
    int account;
    double amount;

    printf("Enter account number: ");
    scanf("%d",&account);

    fseek(fPtr,(account-1)*sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);

    if(client.acctNum==0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter deposit amount: ");
    scanf("%lf",&amount);

    client.balance += amount;

    fseek(fPtr,-sizeof(struct clientData),SEEK_CUR);
    fwrite(&client,sizeof(struct clientData),1,fPtr);

    logTransaction(account,amount,'D');

    printf("Deposit successful. New Balance: %.2f\n",client.balance);
}

void withdraw(FILE *fPtr)
{
    struct clientData client={0,"","",0.0};
    int account;
    double amount;

    printf("Enter account number: ");
    scanf("%d",&account);

    fseek(fPtr,(account-1)*sizeof(struct clientData),SEEK_SET);
    fread(&client,sizeof(struct clientData),1,fPtr);

    if(client.acctNum==0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter withdrawal amount: ");
    scanf("%lf",&amount);

    if(amount>client.balance)
    {
        printf("Insufficient balance.\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr,-sizeof(struct clientData),SEEK_CUR);
    fwrite(&client,sizeof(struct clientData),1,fPtr);

    logTransaction(account,amount,'W');

    printf("Withdrawal successful. Balance: %.2f\n",client.balance);
}

void logTransaction(int acc,double amount,char type)
{
    FILE *log=fopen("transactions.txt","a");

    time_t t;
    time(&t);

    fprintf(log,"Account:%d | %c | %.2f | %s",
            acc,type,amount,ctime(&t));

    fclose(log);
}

void miniStatement(int acc)
{
    FILE *log=fopen("transactions.txt","r");
    char line[200];

    if(log==NULL)
    {
        printf("No transactions found.\n");
        return;
    }

    printf("\nMini Statement for Account %d\n",acc);
    printf("---------------------------------\n");

    while(fgets(line,sizeof(line),log))
    {
        if(strstr(line,"Account"))
        {
            if(atoi(&line[8])==acc)
                printf("%s",line);
        }
    }

    fclose(log);
}