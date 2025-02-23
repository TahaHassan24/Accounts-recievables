#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
const char gl_filename[] = "GeneralLedgerEntries.dat";

typedef struct{
	float salesDebit,salesCredit;
    float salesReturnDebit,salesReturnCredit;         
    float bankDebit,bankCredit;
} gl_acc;

typedef struct{
	int t_id;
	char date[20];
	char details[30];
	float debit;
	float credit;
	float balance;
} transaction;

typedef struct{
	int accountid;
	char name[30];
	transaction transactions[50];
} account;

void addTransaction(int acc_id) {
    FILE *file = fopen("accounts.dat", "rb+");
    if (file == NULL) {
        printf("Error! Could not open file. Please make an account if you have not made one.");
        return;
    }

    transaction r;
    int tr_id,i;
    account a,b;
    fseek(file,acc_id * sizeof(account),SEEK_SET);
    fread(&b,sizeof(account),1,file);
    for (tr_id=0; tr_id < 50;tr_id++) {
        if(b.transactions[tr_id].t_id==0){
            break;
        }
    }
    if(tr_id==50){
        printf("Error! All transaction slots are full.\n");
        fclose(file);
        return;
    }
    for(i=tr_id; i<49; i++){
        if(b.transactions[i+1].t_id!=0){
            b.transactions[i]=b.transactions[i+1];
        }
    }

    memset(&b.transactions[49],0,sizeof(transaction));
    fseek(file,acc_id * sizeof(account),SEEK_SET);
    fread(&a, sizeof(account),1,file);
    printf("Getting the current date.....\n");
    time_t t=time(NULL);
    struct tm *tm_info=localtime(&t);
    if(tm_info==NULL || t==-1){
        printf("Failed to obtain the current date\n");
        printf("Enter the date (DD-MM-YYYY): ");
        do {
            scanf(" %[^\n]s", a.transactions[tr_id].date);
        } 
		while (strlen(a.transactions[tr_id].date) != 10);
    } 
	else{
        strftime(a.transactions[tr_id].date, sizeof(a.transactions[tr_id].date), "%d-%m-%Y", tm_info);
    }

    int choice;
    printf("Choose one of the following entries:\n1.Sales\n2.Sales return\n3.Bank\n");
    scanf(" %d", &choice);
    switch(choice){
        case 1:
            a.transactions[tr_id].credit = 0;
            strcpy(a.transactions[tr_id].details, "Sales");
            printf("Enter value of sales: ");
            scanf(" %f",&a.transactions[tr_id].debit);
            if(tr_id == 0){
                a.transactions[tr_id].balance=a.transactions[tr_id].debit;
            } 
			else{
                a.transactions[tr_id].balance=a.transactions[tr_id-1].balance+a.transactions[tr_id].debit;
            }
           
            break;

        case 2:
            a.transactions[tr_id].debit=0;
            strcpy(a.transactions[tr_id].details,"Sales return");
            printf("Enter value of sales return: ");
            scanf(" %f", &a.transactions[tr_id].credit);
            if(tr_id==0){
                a.transactions[tr_id].balance=a.transactions[tr_id].credit*-1;
            } 
			else{
                a.transactions[tr_id].balance=a.transactions[tr_id-1].balance-a.transactions[tr_id].credit;
            }
           
            break;

        case 3:
            a.transactions[tr_id].debit=0;
            strcpy(a.transactions[tr_id].details,"Bank");
            printf("Enter value of payment received: ");
            scanf(" %f",&a.transactions[tr_id].credit);
            if(tr_id==0){
                a.transactions[tr_id].balance=a.transactions[tr_id].credit*-1;
            } 
			else {
                a.transactions[tr_id].balance=a.transactions[tr_id-1].balance-a.transactions[tr_id].credit;
            }
            break;
    }
    a.transactions[tr_id].t_id = tr_id + 1;

    fseek(file,acc_id * sizeof(account),SEEK_SET);
    fwrite(&a,sizeof(account),1,file);

    printf("Transaction ID is %d\n", a.transactions[tr_id].t_id);
    printf("Transaction Details: %s, %f, %f\n", a.transactions[tr_id].details, a.transactions[tr_id].debit, a.transactions[tr_id].credit);

    fclose(file);
}


void addAccount(){
    FILE *file=fopen("accounts.dat","ab");
    if(file==NULL){
        printf("Error! Could not open file.\n");
        return;
    }
    
    account a,b;
    int i;
    printf("Enter account name: ");
    scanf(" %[^\n]s",&a.name);

    for(i=0;i<50;i++){
    	strcpy(a.transactions[i].date," ");
    	strcpy(a.transactions[i].details," ");
        a.transactions[i].t_id=0; 
        a.transactions[i].debit=0;
        a.transactions[i].credit=0; 
        a.transactions[i].balance=0; 
    }
    fseek(file,0,SEEK_END);
    long fileSize=ftell(file);
    int a_id=fileSize/sizeof(account); 

    a.accountid=a_id;
    printf("Account id is %d\n",a.accountid);
    fwrite(&a, sizeof(account),1,file);
    
    fclose(file);

    char choice;
    printf("Do you want to add transactions (y/n): ");
    scanf(" %c",&choice);
    if(choice=='y' ||choice =='Y'){
        addTransaction(a_id);
    }
}


void ViewAccount(int id){
    FILE *file = fopen("accounts.dat", "rb"); 
    if (file == NULL) {
        printf("Error! Could not open file.\n");
        return;
    }
    account a;
    int i,found=0,j,d;
    while(fread(&a, sizeof(account),1, file)){
        if(id==a.accountid){
            found=1;
            break;
        }
    }

    if(found==0){
        printf("Error! Account ID not found.\n");
        fclose(file);
        return;
    }

    printf("---------------------------------------------- %s (ID: %d) ---------------------------------------------\n", a.name, a.accountid);
    printf("Date\t\tDetails\t\t\tDebit ($)\t\tCredit ($)\t\tBalance ($)\n");
	
    for (i=0; a.transactions[i].t_id!=0; i++){
    	printf("%s",a.transactions[i].date);
    	printf("\t%s",a.transactions[i].details);
    	for(j=0;j<24-strlen(a.transactions[i].details);j++){
    		printf(" ");
		}
		printf("%.2f",a.transactions[i].debit);
		if(a.transactions[i].debit>100){
			printf("\t\t");
		}
		else{
			printf("\t\t\t");
		}
		printf("%.2f",a.transactions[i].credit);
		if(a.transactions[i].credit>1000){
			printf("\t\t");
		}
		else{
			printf("\t\t\t");
		}
		printf("%.2f\n",a.transactions[i].balance);
		
       
    }
    
    fclose(file);
}




void ViewAll(){
    FILE *file = fopen("accounts.dat", "rb"); 
    if (file == NULL) {
        printf("Error! Could not open file.\n");
        return;
    }
    
    account a;
    int i;
    printf("Account name (Account ID):\n");
    while (fread(&a, sizeof(account), 1, file)) {
        printf("%s (%d)\n", a.name, a.accountid);
    }

    fclose(file);

    printf("Enter Account ID for the account you want to view: ");
    scanf("%d",&i);
    ViewAccount(i);
}

gl_acc GetGLAcc() {
    gl_acc totals = {0};
    
    FILE *file=fopen("accounts.dat", "rb");
    if(file==NULL){
        printf("Error! Could not open accounts.dat file.\n");
        return totals;
    }

    account a;
    int i;

    while(fread(&a, sizeof(account), 1, file)){
        for(i=0; i < 50; i++){
            if(a.transactions[i].t_id==0){
                continue;
            }
            if(strcmp(a.transactions[i].details, "Sales")== 0){
                totals.salesDebit+=a.transactions[i].credit;
                totals.salesCredit+=a.transactions[i].debit;
            } 
			else if(strcmp(a.transactions[i].details, "Sales return")==0){
                totals.salesReturnDebit+=a.transactions[i].credit;
                totals.salesReturnCredit+=a.transactions[i].debit;
            } 
			else if(strcmp(a.transactions[i].details, "Bank")==0){
                totals.bankDebit+=a.transactions[i].credit;
                totals.bankCredit+=a.transactions[i].debit;
            }
        }
    }

    fclose(file);
    return totals;
}

void ViewTrialBalance(){
	transaction t;
	int i;
    double totalCredit=0,totalDebit=0,accountDebit=0,accountCredit=0;;
    account a;
    printf("-----------------------Trial Balance-----------------------\n");
    printf("Account Name\t\t\tDebit\t\tCredit\n\n");
    FILE *file=fopen("accounts.dat","rb");
    if(file==NULL){
        printf("Error! Could not open file.\n");
        return;
    }
    while(fread(&a,sizeof(account),1,file)){
        accountDebit=0;
		accountCredit=0;
        for(i=0;i<50;i++){
        	if(a.transactions[i].t_id!=0){
        		if(a.transactions[i].balance>0){
        			accountDebit=a.transactions[i].balance;
				}
				else{
					accountCredit=a.transactions[i].balance;
				}
			}
		}
		if(a.transactions[i].t_id==0){
			break;
		}
		printf("%s",a.name);
        for(i=0;i<29-strlen(a.name);i++){
        	printf(" ");
		}
		printf("\t%.2f\t\t%.2f\n",accountDebit,accountCredit);
        totalDebit+=accountDebit;
        totalCredit+=accountCredit;
    }
    fclose(file);
	gl_acc x = GetGLAcc();
    
    printf("%s","Sales");
    for(i=0;i<24;i++){
    	printf(" ");
	}
	printf("\t%.2f\t\t%.2f\n",x.salesDebit,x.salesCredit);
	printf("%s","Sales Return");
    for(i=0;i<17;i++){
    	printf(" ");
	}
	printf("\t%.2f\t\t%.2f\n",x.salesReturnDebit,x.salesReturnCredit);
	printf("%s","Bank");
    for(i=0;i<25;i++){
    	printf(" ");
	}
	printf("\t%.2f\t\t%.2f\n", x.bankDebit, x.bankCredit);
    totalDebit+=x.salesDebit+x.salesReturnDebit+x.bankDebit;
    totalCredit+=x.salesCredit+x.salesReturnCredit+x.bankCredit;
    printf("\nTotal:\t\t\t\t%.2f\t%.2f\n\n\n",totalDebit,totalCredit); 
}

int main(void){
	int *debitptr,*creditptr,*balanceptr;
	int i,j, checkaccid,checktid,choice;
	printf("---------------Accounts Management system---------------\n");
	
	do{
		
		do{

			printf("\n\nChoose what you want to do: \n1.Add an account\n2.Add a transaction \n3.View an account \n4.View trial balance \n5.Exit \n\nYour choice: ");
			scanf(" %d",&i);
			switch(i){
				case 1:
					addAccount();
					break;
				case 2:
					
					printf("Enter account id: ");
					scanf(" %d",&checkaccid);
					addTransaction(checkaccid);
					break;
				case 3:
					do{
						printf("1.View all account names and ID\n2.Enter Account ID\nChoice: ");
						scanf(" %d",&choice);
						switch(choice){
							case 1:
								ViewAll();
								break;
							case 2:
								printf("Account ID: ");
								scanf(" %d",&checkaccid);
								ViewAccount(checkaccid);
								break;
						}
					}
					while(choice<1||choice>2);
					break;
				case 4:
					ViewTrialBalance();
					break;
				case 5:
					printf("Exiting...");
					break;
				default:
					printf("Invalid input! Please try again.\n\n");
					break;
			}
		}
		while(i<1||i>5);
	}
	while(i!=5);

	
	return 0;
}
