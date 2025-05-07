#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

/*
static int roll_random_number()
{
	int output;

	srand(time(NULL));
	output = rand() % 10 + 1;

	cout << output << endl;

	return 0;
}
*/

int main()
{
	int output_number = 0;
	srand(time(NULL));

	cout << rand() * 100 << endl;
		

	return 0;
}