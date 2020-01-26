// R. Jesse Chaney

// What do you include to get the standard i/o functions?

// What do you include to get the new type account_t into your code?
#include <stdio.h>
#include "lab1.h"

int main(int argc, char *argv[])
{

  /*
    The rest of your code goes in here.
   */
account_t act;
account_t* act_p;
act_p = &act;

act.account_number = 12345;
act_p->balance = 678.9f;
printf("account number is %d \n",act.account_number);
printf("account balance is %.1f \n", act_p->balance);
printf("macro is %s", YOUR_NAME);
  return(0);
}
