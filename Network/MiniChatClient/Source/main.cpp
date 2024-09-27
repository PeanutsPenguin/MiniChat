#include "clientFunc.h"

int main()
{
    client newClient = client();

    std::string name = newClient.CreateAndConnect("27015");

    newClient.createReadEvent();

    handleEvent(newClient, name);

    return 0;
}