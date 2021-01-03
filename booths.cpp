#include <stdio.h>
#include <iostream>
using namespace std;


/*
Function Name: oneBitFullAdder
Function Arguments: Two 1-bit inputs, a carry-in bit, and two pointers sum and carryout which are the outputs.
Function Return Type: Void
Function Description: The function takes 1-bit inputs and a carry-in bit and performs addition operation. It results in the sum and carry-out.
*/
void oneBitFullAdder(int a, int b, int carryIn, int *sum, int *carryOut) {
	//Calculates the sum of the bits and carry-in
	*sum = ((a ^ b) ^ carryIn);

	//Calculates carryout
	*carryOut = ((a & b) | (a & carryIn)) | (b & carryIn);
}


/*
Function Name: overflowChecker
Function Arguments: 1-bit operand, second bit operand invert signal and result of the operands
Function Return Type: Bool
Function Description: This function checks if there is any overflow. It checks the bInvert argument to determine if the the sign on the two input bits are same or not. If it is not same, then it returns false as there is no overflow. Else, it checks if the output sign is same as the operands. If not, it returns true because it indicates that there is overflow.
*/
bool overflowChecker(int a, int bInvert, int result) {
	//Checks if the sign of two operands are same or not.
	if(bInvert) {
		//If sign are different, then there is not overflow. So false is returned.
		return false;
	} else if(a != result) {
		//If sign are same, it checks if sign of result is same as operands or not. It its different, then there is not overflow. So true is returned. 
		return true;
	}
}


/*
Function Name: oneBitALU
Function Arguments: Two 1-bit inputs, an invert signal for second input, a carry-in bit, a 2-bit signal for operation to be performed, and two pointers output and carryout which are the outputs.
Function Return Type: Void
Function Description: The function takes the 1-bit input along with carry-in bit and performs the specified operations on the bit. The operation to undergo is specified by the operation argument. There are 3 main operations which are logical AND, logical OR and addition operation. Addition is done by calling oneBitFullAdder function. Subtraction could be done using addition operation by performing 2s compliment on the second operand. Here 2s compliment is done by negating the operand and passing 1 as initial carry-in bit. The sum is stored in the variable sum and carry-out is stored in carryOut variable.
*/
void oneBitALU(int bitOne, int bitTwo, int bInvertSignal, int carryIn, int operation, int *output, int *carryOutALU) {
	int sum, carryOut = 0;

	//Negates the second operand if b invert signal is 1.
    if(bInvertSignal == 1) {
    	bitTwo = !bitTwo;
    }
    
    //Determines the operation and performs the operation
    switch(operation) {
        case 00 :
        	//AND operation
            *output = bitOne & bitTwo; 
            break;
        case 01 :
        	//OR operation
            *output = bitOne | bitTwo; 
            break;
        case 10 :
        	//Full Adder is called to perform addition
            oneBitFullAdder(bitOne, bitTwo, carryIn, &sum, &carryOut); 
            *output = sum;
            *carryOutALU = carryOut;
            break;
        default :
            cout << "Invalid operation" << endl;
   	}
}


/*
Function Name: oneBitSpecialALU
Function Arguments: Two 1-bit inputs, an invert signal for second input, a carry-in bit, a 2-bit signal for operation to be performed, and three pointers output, carryout and overflow which are the outputs.
Function Return Type: Void
Function Description: The function performs same operation as oneBitALU function but has a special feature which determines if the operation yields an overflow or not. The overflow is determined by calling overflowChecker function.
*/
void oneBitSpecialALU(int bitOne, int bitTwo, int bInvertSignal, int carryIn, int operation, int *output, int *carryOutALU, bool *overflow) {
	int sum, carryOut = 0;

	//Negates the second operand if b invert signal is 1.
    if(bInvertSignal == 1) {
    	bitTwo = !bitTwo;
    }
    
    //Determines the operation and performs the operation
    switch(operation) {
        case 00 :
        	//AND operation
            *output = bitOne & bitTwo; 
            break;
        case 01 :
        	//OR operation
            *output = bitOne | bitTwo; 
            break;
        case 10 :
        	//Full Adder is called to perform addition
            oneBitFullAdder(bitOne, bitTwo, carryIn, &sum, &carryOut); 
            *output = sum;
            *carryOutALU = carryOut;

            //overflowChecker function is called to determine overflow
            *overflow = overflowChecker(bitOne, bInvertSignal, sum);
            break;
        default :
            cout << "Invalid operation" << endl;
   	}
}


/*
Function Name: sixteenBitALU
Function Arguments: Two 16-bit operands, b invert signal and operation to be performed
Function Return Type: Integer
Function Description: The function loops through each bit and calls the function oneBitALU to perform the operation. In the end, a special 1-bit ALU function is called which checks the overflow.
*/
int* sixteenBitALU(int a[16], int b[16], int bInvertSignal, int operation) {
	int cIn = 0, cOut = 0, i, result, cInFlag = 0;
	bool overflow;
	int *arr = new int[16];

	//Pass initial cIn as 1, if bInvert signal is 1 and cInFlag indicates first cycle, to add 1 after negating to perform 2s compliment
	if((bInvertSignal == 1) && (cInFlag == 0)) {
		cIn = 1;
	}
	
	//Indicates the first iteration.
	cInFlag = 1;

	//Iterates through each bit and calls onBitALU function to perform operation
	for(i=15;i>=0;i--) {
		oneBitALU(a[i], b[i], bInvertSignal,cIn,operation,&result,&cOut);
		arr[i] = result;
		cIn = cOut;
	}

	//Special 1-bit ALU is called in the last iteration. It determines if there is an overflow
	oneBitSpecialALU(a[0], b[0], bInvertSignal,cIn,operation,&result,&cOut,&overflow);
    arr[0] = result;
    
	return arr;
}


/*
Function Name: displayBits
Function Arguments: Cycle Counter, multiplicand register, accumulator register, multiplier register and intial multiplier bit register
Function Return Type: Void
Function Description: The function displays the bits in all of the registers in organised way.
*/
void displayBits(int cycleCounter[], int regMD[], int regAC[], int regMQ[], int regMQO) {
	int i;

	//Display cycle counter
	for(i=0;i<4;i++) {
		cout<<cycleCounter[i];
	}
	cout<<"           ";

	//Display multiplicand register
	for(i=0;i<16;i++) {
		cout<<regMD[i];
	}
	cout<<"   ";

	//Display accumulator register
	for(i=0;i<16;i++) {
		cout<<regAC[i];
	}
	cout<<"   ";

	//Display multiplier register
	for(i=0;i<16;i++) {
		cout<<regMQ[i];
	}
	cout<<"   ";

	//Display MQ-1 bit
	cout<<regMQO;
	cout<<endl;
}


/*
Function Name: boothsMultiplier
Function Arguments: 16-bit operands multiplicand and multiplier
Function Return Type: Int
Function Description: This function performs Booths 2s compliment multiplication. It initializes the cycle counter, cycle counter bits, accumulator and the MQ-1 bit. Then it calls the specific operation to be performed on the input bits. The operation to be performed is identified by checking the last bit of multiplier and MQ-1 bit. The operation done is the step 1 of the iteration. In the second step, it right shifts all the bits and the first bit of AC is the new bit added to the rightmost. Cycle Counter is updated on each iteration and display fcuntion is called.
*/
int* boothsMultiplier(int mtpcd[], int mtplr[]) {
	int cycleCounter[4] = {1,1,1,1};
	int cycleCounterSize = 16;
	int regAC[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int regMQ[16];
	int regMD[16];
	int i, x, regMQO = 0;
	int *result;
	int op[32];

	int adder[4] = {0,0,0,1};
	int j = 0, cOut, res;
	int arr[4];

	int *final_output = new int[32];

	//Put multiplier into register MQ and multiplicand into register MD
	for(i=0;i<16;i++) {
		regMQ[i] = mtplr[i];
		regMD[i] = mtpcd[i];		
	}

	//Display the title for each state
	cout<<"Cycle counter  "<<"MD                 "<<"AC                 "<<"MQ                 "<<"MQ-1 "<<endl;
	
	//Display the initial contents for each state in the counter and registers
	displayBits(cycleCounter, regMD, regAC, regMQ, regMQO);

	//Iterate through cycle counter
	for(x=cycleCounterSize-1; x>=0; x--) {
		
		//Check the last bit of multiplier and MQ-1 bit, and then perform the required operation
		if((regMQ[15] == 1) && (regMQO == 0)) {
			//Subtration of operation. bInvertSignal is passed as 1 and addition operation 10 is passed.
			result = sixteenBitALU(regAC, regMD, 1, 10);
			
			for(i=0;i<16;i++) {
				regAC[i] = result[i];
			}
		} else if((regMQ[15] == 0) && (regMQO == 1)) {
			//Addition of operation. bInvertSignal is passed as 0 and addition operation 10 is passed.
			result = sixteenBitALU(regAC, regMD, 0, 10);

			for(i=0;i<16;i++) {
				regAC[i] = result[i];
			}
		}

		//Display the contents of the counter and registers after performing operation
		displayBits(cycleCounter, regMD, regAC, regMQ, regMQO);


		//Right shift operation is performed
		//MQ-1 is set as last bit of MQ
		regMQO = regMQ[15];
		for(i=14;i>=0;i--)
		{
		    regMQ[i+1] = regMQ[i];
		}
		regMQ[0] = regAC[15];

		//First but of AC is set as new rightmost bit
		regAC [0] = regAC[0];
		for(i=14;i>=0;i--)
		{
		    regAC[i+1] = regAC[i];
		}

		//Display the contents of the counter and registers after performing shift operation
		displayBits(cycleCounter, regMD, regAC, regMQ, regMQO);
		cout<<endl;
	 	
	 	//Cycle counter bit is updated
		j = 1;
	 	for(i=3;i>=0;i--) {
	 		oneBitALU(cycleCounter[i], adder[i], 1, j, 10,&res,&cOut);
	 		arr[i] = res;
	 		j = cOut;
	 	}
	 	for(i=3;i>=0;i--) {
	 		cycleCounter[i] = arr[i];
	 	}
 	}

 	//Final product is assigned to a new variable and returned
 	for(i=0;i<16;i++) {
 		final_output[i] = regAC[i];
 	}

 	for(j=0;j<16;j++) {
 		final_output[i++] = regMQ[j];
 	}

 	return final_output;
}


/*
Function Name: main
Function Arguments: None
Function Return Type: Int
Function Description: This is the main driver. It takes input from the user and calls the boothsMultiplier to perform the Booths 2s compliment multiplication function. Then it displays the final result from the mutiplication.
*/
int main()
{	
	int numBits = 16;
    int mtpcd[numBits], mtplr[numBits], i;
    int* product;
    
    //Get user inputs for multiplicand and multiplier
    cout << "Enter the 16-bit multiplicand\n";
    for(i=0;i<numBits;i++) {
		cin >> mtpcd[i];		
	}
    
    cout << "Enter the 16-bit multiplier\n";
    for(i=0;i<numBits;i++) {
		cin >> mtplr[i];		
	}
    
    //Call booths multiplier function to perform Booth’s 2’s complement number multiplier
    product = boothsMultiplier(mtpcd, mtplr);

    //Displays the final product after multiplication
    cout<<"Final product is:"<<endl;
    for(i=0;i<32;i++) {
    	cout<<product[i];
    }
    
    return 0;
}
