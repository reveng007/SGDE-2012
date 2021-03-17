#include<stdio.h>
#include<string.h>

int main(int argc, char **argv)
{
	int result;

	result = strcmp(argv[1], "password");

	if (result == 0)
	{
		printf("Correct password\n");
	}
	else
	{
		printf("Please try again \n\n");
	}

	return 0;
}

