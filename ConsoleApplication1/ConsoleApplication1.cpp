/*
Using several functions to clearify:
1. Whether a number is a prime number.
2. If it is not a prime number, get the list of the composite number devisor list of it.
3. List the output.

*/

#include <iostream>

using namespace std;

int console_input = 114514;
int count_prime_number = 1919810;
string composite_number_devisor_list = "";

static bool if_prime_number( int input)									// Being used to judge if the input is a prime number. 
{
	bool final_result = true;
	int devision = 2; 

	for (devision = 2; devision < input; devision++)
	{
		if (input % devision == 0)
		{
			final_result = false; 
			break;
		}
	}

	return final_result;
}

static string get_composite_number_devisor_list(int input_number)    // Form a string line that represents the list of composite number devisor of an input number. 
{
	if (if_prime_number(input_number) == true)
	{
		
	}
}
/*
int main()
{
	cin >> console_input;


	return 0;
}

*/