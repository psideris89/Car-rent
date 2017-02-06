#include <stdio.h>
#include <stdlib.h>

/* Constants for computing charges */
#define KM_CHARGE          0.15
#define BASE_CHARGE_A      25
#define BASE_CHARGE_B      35.0
#define KM_LIMIT_CHARGE_B  100
#define BASE_CHARGE_C      120.0
#define KM_LIMIT_CHARGE_C1 900
#define ADD_CHARGE_C1      60.0
#define KM_LIMIT_CHARGE_C2 1500
#define ADD_CHARGE_C2     130.0

/* Constants for producing the charging tableau */
#define MIN_DAYS     1
#define MAX_DAYS     20
#define STEP_DAYS    1
#define MIN_KMS      20
#define MAX_KMS      200
#define STEP_KMS     20

/* Functions used */
float amountDueA(int nDays,float kmsDriven);
float amountDueB(int nDays,float kmsDriven);
float amountDueC(int nDays,float kmsDriven);
void printTableau();

int main()
{
    char code,validCode;
    int nDays, ostart, oend;
    float nkms, amount;

    printf("\nWelcome to the Car Rental Charging System!\n");

	do {
        /* Input data */
        printf("\nPlease enter the following data:\n");

		printf("Category code  ('Q' to quit): ");
        scanf("%c",&code);
        validCode = (code == 'A') || (code=='a') || (code == 'B') || (code=='b') || (code == 'C') || (code=='c');
        if (validCode) {
            do { /* Input number of days and validity check */
                printf("Number of days: ");
                scanf("%d",&nDays);
                if (nDays<=0) {
                    printf("Only positive number of days accepted. Please try again.\n");
                }
            } while (nDays<=0);
            do { /* Input odometer reading at the start of the renting period and validity check */
                printf("Odometer reading at the start of the renting period: ");
                scanf("%d",&ostart);
                if (ostart<=0) {
                    printf("Only positive odometer readings accepted. Please try again.\n");
                }
            } while (ostart<=0);
            do { /* Input odometer reading at the end of the renting period and validity check */
                printf("Odometer reading at the end of the renting period: ");
                scanf("%d",&oend);
                if (oend<=0) {
                    printf("Only positive odometer readings accepted. Please try again.\n");
                }
                if (oend<ostart) {
                    printf("Odometer reading at the end of the renting period should not be less than odometer reading at start. Please try again.\n");
                }
            } while ( (oend<=0) && (oend<ostart) );

            /* Compute kilometers during the renting period */
            nkms = (oend - ostart ) /10.0;
        }

        /* Depending on the category code, compute the amount due */
        switch (code) {
            case 'A': case 'a':
                amount = amountDueA(nDays,nkms);
                break;
            case 'B': case 'b':
                amount = amountDueB(nDays,nkms);
                break;
            case 'C': case 'c':
                amount = amountDueC(nDays,nkms);
                break;
            case 'Q': case 'q':
                printTableau();
                break;
            default:
                printf("Wrong category code. Try again.\n");
                break;
        }
        if (validCode) {
            /* Print customer summary */
            printf("\nCustomer summary:");
            printf("\n\tCategory code: %c",code);
            printf("\n\tRental period (days): %d",nDays);
            printf("\n\tOdometer reading at start: %d",ostart);
            printf("\n\tOdometer reading at end: %d",oend);
            printf("\n\tNumber of kilometers driven: %3.1f",nkms);
            printf("\n\nAmount due: %4.2f\n",amount);
        }
        getchar(); /* Just used to avoid problems with scanf */
    } while ( (code != 'Q') && (code != 'q') );
    printf("\n\nProgram terminated. Bye!\n");
    return 0;
}

/* Compute charges for category A */
float amountDueA(int nDays,float kmsDriven)
{
    float amount;

	amount = nDays*BASE_CHARGE_A + kmsDriven*KM_CHARGE;
	return amount;
}

/* Compute charges for category B */
float amountDueB(int nDays,float kmsDriven)
{
    float amount,kmPerDay,additional;

	kmPerDay = kmsDriven/nDays;
	if (kmPerDay <= KM_LIMIT_CHARGE_B) {
		additional = 0;
	} else {
		additional = (kmPerDay - KM_LIMIT_CHARGE_B) * KM_CHARGE * nDays;
    }
	amount = BASE_CHARGE_B * nDays + additional;
	return amount;
}

/* Compute charges for category C */
float amountDueC(int nDays,float kmsDriven)
{
    int nWeeks;
    float kmPerWeek, additional, amount;

	nWeeks = nDays / 7;
	if ( (nDays % 7) > 0) {
		nWeeks += 1;
    }
	kmPerWeek = kmsDriven/nWeeks;
	if (kmPerWeek <= KM_LIMIT_CHARGE_C1) {
		additional = 0;
	} else if (kmPerWeek <= KM_LIMIT_CHARGE_C2) {
		additional = ADD_CHARGE_C1 * nWeeks;
    } else {
		additional = ADD_CHARGE_C2 * nWeeks + nWeeks * (kmPerWeek - KM_LIMIT_CHARGE_C2) * KM_CHARGE;
    }
	amount = BASE_CHARGE_C * nWeeks + additional;
	return amount;
}

/* Print charging tableau */
void printTableau()
{
    int nDays;
    int kmsDriven;
    float amountA,amountB,amountC;
    char code;

    printf("\nCustomer optimal charging tableaux");
    printf("\n----------------------------------");
    printf("\n\n  ");
    for (kmsDriven=MIN_KMS;kmsDriven<=MAX_KMS;kmsDriven+=STEP_KMS) {
        printf("%4d",kmsDriven);
    }
    printf("\n");
    for (nDays=MIN_DAYS;nDays<=MAX_DAYS;nDays+=STEP_DAYS) {
        printf("%2d",nDays);
        for (kmsDriven=MIN_KMS;kmsDriven<=MAX_KMS;kmsDriven+=STEP_KMS) {
            /* find the optimal charging and present its code */
            amountA = amountDueA(nDays,kmsDriven*nDays);
            amountB = amountDueB(nDays,kmsDriven*nDays);
            amountC = amountDueC(nDays,kmsDriven*nDays);
            if (amountA<amountB && amountA<amountC) {
                code = 'A';
            } else if (amountB<amountC) {
                code = 'B';
            } else {
                code = 'C';
            }
            printf("%4c",code);
        }
        printf("\n");
    }
}

