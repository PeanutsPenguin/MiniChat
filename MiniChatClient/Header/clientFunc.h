#pragma once 

#include "Network/client.h"

void handleEvent(client newClient, std::string name);

bool keyboardEvent(KEY_EVENT_RECORD input, std::stringstream* msg, client* newClient, std::string name, int* index);

