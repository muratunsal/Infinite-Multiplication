#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define SIZE_OF_LINE 256

typedef struct Node {
    int data;
    struct Node* next;
}Node;

Node* newNode(int data)//creates a list
{
    Node* new_node = (Node *)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}
void push(Node** head_ref, int new_data)//pushes nodes to list
{
    Node* new_node = newNode(new_data);

    new_node->next = (*head_ref);

    (*head_ref) = new_node;
}
void printList(Node *node) //prints the lists
{
    while(node != NULL)
    {
        printf("%d",node->data);
        node = node->next;
    }
    printf("\n");
}
void reverseList(struct Node** head_ref)//reverses the list
{
    Node* next;
    Node* prev = NULL;
    Node* current = *head_ref;


    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head_ref = prev;

}
int getListSize(Node* head) //returns the number of the elements in the list
{
    int count = 0;
    Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}
Node* charToList(char *a,int base){ //Converts char to lists
    Node* list = NULL;

    int i;
    int x  ;

    for(i = strlen(a); i > 1 ;i--){
        x = a[i-2] - '0';
        if(x >= base){
        	printf("Error : Any digit of the number can not be greater than or equal to base !! "); //if any digit of the number is greater than or equal to
        																							//base it prints an error and exits.
        	exit(0);
		}
        push(&list, x);
    }
    return list;
}
Node* zerosList(int length) //returns a list that has (length) elements all of them equal to 0
{
    Node* head = NULL;
    while (length--)
        push(&head, 0);
    return head;
}
Node* removeLeadingZeros(Node* list){ //Removes leading zeros 
    while (list->data == 0) {
        list = list->next;
    }
    return list;
}
Node* multiplyLinkedLists(Node* first, Node* second, int base) //multiplies the lists
{
    int i,j;

    reverseList(&first);			//first reverse both lists.
    reverseList(&second);

    Node* result = zerosList(getListSize(first) + getListSize(second)+1); //fill the result linked list. Maximum digit count of the result is size of first list + size of second + 1

    Node *ptr1;
    Node *ptr2 = second;					//Creating pointers
    Node *ptrRes1 = result,*ptrRes2;


    for( i = 0; i < getListSize(second) ; i ++){  // outer loop runs (size of the second list) times
        ptrRes2 = ptrRes1; //For shifting 1 digit when second numbers digit multiplied by every digit of first.

        ptr1 = first; //Pointing the last digit of the first for every digit of second num.

        int carrier = 0; 

        for( j = 0; j < getListSize(first) ; j ++){ // inner for loop runs (size of the first list) times

            int product = (ptr1->data) * (ptr2->data) + carrier; 

            ptrRes2->data += (product % base); // Assigning the multiplication value to result list.


            carrier = (product / base) + (ptrRes2->data / base); // Setting the carrier . If it is greater than 9 it transfers carrier/10 to the next node.

            ptrRes2->data = (ptrRes2->data % base);

            ptr1 = ptr1->next;
            ptrRes2 = ptrRes2->next;
        }

        if (carrier > 0) {
            ptrRes2->data += carrier;
        }

        ptrRes1 = ptrRes1->next;	
        ptr2 = ptr2->next;
    }

    reverseList(&first);
    reverseList(&second);  //reversing the lists back
    reverseList(&result);

    return removeLeadingZeros(result);  // return after removing leading zeros.
}

Node* toDecimal(Node* list, int base){  //converts num to decimal if it's base value is different than 10.
    int i, j;
    reverseList(&list);
    Node* ptr = list;
    double numOfDigits = ceil(getListSize(list)*log10(base));  //Num of digits is the number of the digits of the decimal form of the number.
    Node* decimalList = zerosList((int)numOfDigits);
    Node* decimalPtr = decimalList;
    int digits[(int)numOfDigits];						 // An array that keeps each digit of the decimal value.

    for(i = 0 ; i < numOfDigits ; i++){
        digits[i] = 0 ;
    }

    for( i = 0; i < getListSize(list) ; i++){
        int product = ptr->data * pow(base, i);
        int length = 0;
        if(product != 0){
            length = 1 + log10(product);
        }

        for(j = 0 ; j < length ; j++){
            digits[j] += product % 10;
            product /= 10;
        }
        ptr = ptr -> next;
    }
    for(j = 0; j < numOfDigits ; j++) {
        if (j != numOfDigits - 1) {										//Same carrier operations for the decimal
            if (digits[j] > 9) {
                digits[j + 1] += digits[j] / 10;
                digits[j] = digits[j] % 10;
            }
        }
    }
    for(j = 0; j < numOfDigits ; j++) {

        decimalPtr-> data = digits[j];
        decimalPtr = decimalPtr -> next;


    }
    reverseList(&decimalList); 
    decimalList = removeLeadingZeros(decimalList);

//returns after reversing and removing the leading zeros.

    return decimalList;

}
int main(int argc,char* argv[])
{
    if(argc==1){
         printf("Error message!");      
    }

    if(argc>=2)
    {
    FILE* file = fopen(argv[1], "r");
    Node* first = NULL;
    Node* second = NULL;


    char firstChar[SIZE_OF_LINE];
    char secondChar[SIZE_OF_LINE];      //char arrays to read num from input file.
    char baseChar[3];


    if(file == NULL){
        printf("Unable to open the file...");
    }else{
        char line[SIZE_OF_LINE];


        int i ;
        for(  i = 0; i < 3 ; i++){
            fgets(line,sizeof(line),file);

            if(i == 0){
                strcpy(firstChar,line);
            }
            if(i == 1){
                strcpy(secondChar,line);
            }
            if(i == 2){
                strcpy(baseChar,line);
            }
        }
    }
    int base = atoi(baseChar);
    if(base < 2 || base > 10){
        printf("Base must be in this range !!! (2 - 10)");   // If base is not in the range prints an error.
    } else {
        first = charToList(firstChar,base);
        second = charToList(secondChar,base);
        printf("{Multiplier in base %d}    : ", base);
        printList(first);
        printf("{Multiplicand in base %d}  : ", base);
        printList(second);
        Node *result = multiplyLinkedLists(first, second, base);
        printf("{Result in base %d}        : ", base);
        printList(result);
    }														
    if (base != 10) {					//Prints the decimal values of the given numbers.
        first = toDecimal(first, base);
        second = toDecimal(second, base);
        Node *result = multiplyLinkedLists(first, second, 10);
        printf("{Multiplier in decimal}   : ");
        printList(first);
        printf("{Multiplicand in decimal} : ");
        printList(second);
        printf("{Result in decimal}       : ");
        printList(result);
    }
}


}


