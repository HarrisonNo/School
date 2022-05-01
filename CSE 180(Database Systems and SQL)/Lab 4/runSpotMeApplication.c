/**
 * runSupermarketApplication skeleton, to be modified by students
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "libpq-fe.h"

/* These constants would normally be in a header file */
/* Maximum length of string used to submit a connection */
#define MAXCONNECTIONSTRINGSIZE 501
/* Maximum length of string used to submit a SQL statement */
#define MAXSQLSTATEMENTSTRINGSIZE 2001
/* Maximum length of string version of integer; you don't have to use a value this big */
#define  MAXNUMBERSTRINGSIZE        20

void printCustomerPaymentInfoTests(PGconn *conn,int theCustID);//Header
void updateIsValidTests(PGconn *conn, char *theCardType, int theCardNum);//Header
void changeSomeAmountPaidTests(PGconn *conn, int theCustID, int maxReduction);//Header

//Executes an sql command and immediately error checks it, returning -1
//TAKEN FROM http://www.kbs.twi.tudelft.nl/Documentation/Database/Postgres/libpq-example.html
#define AssertCommand(res,conn,command){res = PQexec(conn, command);if(PQresultStatus(res) != PGRES_COMMAND_OK){PQclear(res);return -100;}}
#define AssertTuple(res,conn,command){res = PQexec(conn, command);if(PQresultStatus(res) != PGRES_TUPLES_OK){PQclear(res);return -100;}}

/* Exit with success after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void good_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_SUCCESS);
}

/* Exit with failure after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void bad_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_FAILURE);
}

/* The three C functions that for Lab4 should appear below.
 * Write those functions, as described in Lab4 Section 4 (and Section 5,
 * which describes the Stored Function used by the third C function).
 *
 * Write the tests of those function in main, as described in Section 6
 * of Lab4.
 */

/* Function: printCustomerPaymentInfo:
 * -------------------------------------
 * Parameters:  connection, and thecustID, which should be the custID of a customer.
 * Prints custID, number of expired payments and number of unexpired payments.
 * Return 0 if normal execution, -1 if no such customer.
 * bad_exit if SQL statement execution fails.
 */


int printCustomerPaymentInfo(PGconn *conn, int theCustID)
{
    PGresult *res;
    char Name[100];//Max name size of 100
    char Statement[MAXSQLSTATEMENTSTRINGSIZE]="SELECT name FROM Customers WHERE custID='";
    char Value[MAXNUMBERSTRINGSIZE];
    int NumberExpired,NumberNotExpired;
    //Start creating the necessary command in a string
    sprintf(Value,"%d",theCustID);
    strcat(Statement,Value);
    strcat(Statement,"'");
    AssertTuple(res,conn,&Statement[0]);
    if(PQntuples(res)==0){
        PQclear(res);
        return -1;//No such customer exists
    }
    strcpy(&Name[0],PQgetvalue(res,0,0));//Snag the name of the customer from the 0th column of what should be the only tuple returned
    PQclear(res);
    //I have no idea if strcmp works on string dates so I just make two sql calls
    //Get all expired
    strcpy(Statement,"SELECT PM.custID FROM Payments P, PaymentMethods PM WHERE PM.custID='");
    strcat(Statement,Value);
    strcat(Statement,"' AND PM.cardType=P.cardType AND PM.cardNum=P.cardNum AND P.paymentDate>PM.expirationDate");
    AssertTuple(res,conn,Statement);
    NumberExpired=PQntuples(res);
    PQclear(res);
    //Get all non expired
    strcpy(Statement,"SELECT PM.custID FROM Payments P, PaymentMethods PM WHERE PM.custID='");
    strcat(Statement,Value);
    strcat(Statement,"' AND PM.cardType=P.cardType AND PM.cardNum=P.cardNum AND P.paymentDate<=PM.expirationDate");

    AssertTuple(res,conn,Statement);
    NumberNotExpired=PQntuples(res);
    PQclear(res);
    //Print result
    printf("Customer %i %s expired: %i unexpired: %i\n",theCustID,&Name[0],NumberExpired,NumberNotExpired);
    return 0;//Succeeded
}

/* Function: updateIsValid:
 * ----------------------------
 * Parameters:  connection, and theCardType and theCardNum which should correspond to a payment method.
 * Updates the isValid status of expired payment method if isValid for that
 * payment method isn't already FALSE.
 * Returns 1 if isValid was updated for the payment method, 0 if it wasn't updated, and -1 if the payment method doesn't exist.
 */

int updateIsValid(PGconn *conn, char *theCardType, int theCardNum)
{
    PGresult *res;
    char Statement[MAXSQLSTATEMENTSTRINGSIZE]="SELECT isValid FROM PaymentMethods WHERE cardType='";
    char CardNum[MAXNUMBERSTRINGSIZE];
    sprintf(CardNum,"%d",theCardNum);
    strcat(Statement,theCardType);
    strcat(Statement,"' AND cardNum='");
    strcat(Statement,CardNum);
    strcat(Statement,"'");
    AssertTuple(res,conn,Statement);
    if(PQntuples(res)==0){
        PQclear(res);
        return -1;//No such payment method exists
    }
    PQclear(res);
    //Reset Statement to update
    strcpy(Statement,"UPDATE paymentMethods SET isValid=FALSE WHERE cardType='");
    strcat(Statement,theCardType);
    strcat(Statement,"' AND cardNum='");
    strcat(Statement,CardNum);
    strcat(Statement,"' AND expirationDate<CURRENT_DATE AND (isValid=TRUE OR isValid IS NULL)");
    AssertCommand(res,conn,Statement);
    if(atoi(PQcmdTuples(res))==0){
        PQclear(res);
        return 0;//No need to update
    }
    else{
        PQclear(res);
        return 1;//Updated the tuple
    }
}

/* Function: changeSomeAmountPaid:
 * -------------------------------
 * Parameters:  connection, theCustID which should be the custID of a customer
 * whose amountPaid values in Payments might be reduced, and maxReduction,
 * the maximum total reduction of amountPaid values.
 * Executes by invoking a Stored Function, changeSomeAmountPaidFunction, which
 * returns a negative if there is an error, and otherwise returns the total
 * reduction performed by the Stored Function.
 */

int changeSomeAmountPaid(PGconn *conn, int theCustID, int maxReduction)
{
    PGresult *res;
    int result;
    char Statement[MAXSQLSTATEMENTSTRINGSIZE]="SELECT changeSomeAmountPaidFunction(";
    char custIDchar[MAXNUMBERSTRINGSIZE];
    char maxReductionchar[MAXSQLSTATEMENTSTRINGSIZE];
    sprintf(custIDchar,"%d",theCustID);
    sprintf(maxReductionchar,"%d",maxReduction);
    strcat(Statement,custIDchar);
    strcat(Statement,",");
    strcat(Statement,maxReductionchar);
    strcat(Statement,");");
    AssertTuple(res,conn,Statement);
    result=atoi(PQgetvalue(res,0,0));
    PQclear(res);
    return result;
}

int main(int argc, char **argv)
{
    PGconn *conn;
    int theResult;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./runSpotMeApplication <username> <password>\n");
        exit(EXIT_FAILURE);
    }

    char *userID = argv[1];
    char *pwd = argv[2];

    char conninfo[MAXCONNECTIONSTRINGSIZE] = "host=cse180-db.lt.ucsc.edu user=";
    strcat(conninfo, userID);
    strcat(conninfo, " password=");
    strcat(conninfo, pwd);

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see if the database connection was successfully made. */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s\n",
                PQerrorMessage(conn));
        bad_exit(conn);
    }
    
    /* Perform the calls to printCustomerPaymentInfo listed in Section 6 of Lab4,
     * and print their results as described.
     */
    printCustomerPaymentInfoTests(conn,104);
    printCustomerPaymentInfoTests(conn,102);
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to updateIsValid listed in Section 6 of Lab4,
     * and print their results as described.
     */
    updateIsValidTests(conn,"M",5380746);
    updateIsValidTests(conn,"V",6011024);
    updateIsValidTests(conn,"M",5380346);
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to changeSomeAmountPaid listed in Section 6 of Lab4,
     * and print their results as described.
     */
    //DOES NOT GIVE RIGHT RESULTS
    //For some reason the embedded function always stops early
    changeSomeAmountPaidTests(conn,104,14);
    changeSomeAmountPaidTests(conn,100,16);
    changeSomeAmountPaidTests(conn,20,4);
    changeSomeAmountPaidTests(conn,104,14);
    //Exit
    good_exit(conn);
    return 0;
}

//These "Test" functions act as a printf wrapper for each function
void printCustomerPaymentInfoTests(PGconn *conn,int theCustID){
    int result=printCustomerPaymentInfo(conn,theCustID);
    if(result==-1){
        printf("No customer exists whose CustID is %i\n",theCustID);
    }
    else if(result!=0){//Unknown result
        printf("ERROR: Something went wrong when trying to find CustID %i, exiting\n",theCustID);
        bad_exit(conn);
    }
    return;
}
void updateIsValidTests(PGconn *conn, char *theCardType, int theCardNum){
    int result=updateIsValid(conn,theCardType,theCardNum);
    if(result==-1){
        printf("No such CardType %s and CardNum %i\n",theCardType,theCardNum);
    }
    else if(result==0){
        printf("Validity not changed for CardType %s and CardNum %i\n",theCardType,theCardNum);
    }
    else if(result==1){
        printf("Validity changed for CardType %s and CardNum %i\n",theCardType,theCardNum);
    }
    else{//Unknown result
        printf("ERROR: Something went wrong when trying to update CardType %s and CardNum %i, exiting\n",theCardType,theCardNum);
        bad_exit(conn);
    }
    return;
}
void changeSomeAmountPaidTests(PGconn *conn, int theCustID, int maxReduction){
    int result=changeSomeAmountPaid(conn,theCustID,maxReduction);
    if(result>=0){
        printf("Total reduction for CustID %i and maxReduction %i: is %i\n",theCustID,maxReduction,result);
    }
    else{
        switch(result){
            case -1:
                printf("WARNING: maxReduction %i is not positive\n",maxReduction);
                break;
            case -2:
                printf("WARNING: CustID %i does not exist\n",theCustID);
                break;
            default:
                printf("ERROR: Something went wrong when trying to change CustID %i's amountPaid by %i, exiting\n",theCustID,maxReduction);
                bad_exit(conn);
                break;
        }
    }
    return;
}