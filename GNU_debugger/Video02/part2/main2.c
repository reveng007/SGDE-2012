#include<stdio.h>

int IamGlobalVariable;

int AddNum(int n1, int n2)
{
	int sum = 0;

	sum = n1 + n2;

	return sum;
}

int SubsNum(int n1, int n2)
{
	int sub = 0;

	sub = n1 - n2;

	return sub;
}


int main (int argc, char **argv)
{
	int input_1 = atoi(argv[1]);
	int input_2 = atoi(argv[2]);

	printf("\n\n Simple Add/Substract\n\n");

	printf("Sum of %d + %d = %d\n\n", input_1, input_2, AddNum(input_1, input_2));

	printf("Difference of %d - %d = %d\n\n", input_1, input_2, SubsNum(input_1, input_2));	
	getchar();

	return 0;
}

