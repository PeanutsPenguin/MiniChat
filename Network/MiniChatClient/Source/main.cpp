#include "clientFunc.h"

int main()
{
	std::string name = "";

	printf("Enter a name : ");

	std::cin >> name;

	client newClient = client();

	newClient.CreateAndConnect("10.5.5.105", "27015");

	newClient.createEvent();

	handleEvent(newClient, name);

	return 0;
}