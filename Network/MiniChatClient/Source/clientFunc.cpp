#include "clientFunc.h"


void handleEvent(client newClient, std::string name)
{
    int index = 0;
    bool isClosing = false;

    //Get the handle input mode
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        printf("HANDLER ERROR");


    DWORD readevents;
    INPUT_RECORD records[128];
    std::string message = name + " >";

    HANDLE events[2];

    events[0] = hStdin;
    events[1] = newClient.getEvent();

    for (;;)
    {
        DWORD object = WaitForMultipleObjects(2, events, false, INFINITE);

        switch (object)
        {
        case WAIT_OBJECT_0:
            break;
        case WAIT_OBJECT_0 + 1:
            newClient.receivemessage();
            break;
        default:
            continue;
        }


        if (!GetNumberOfConsoleInputEvents(hStdin, &readevents))
            printf("READ CONSOLE INPUT");

        if (readevents < 1)  continue;

        if (!ReadConsoleInput(hStdin, records, 128, &readevents))
            printf("READ CONSOLE INPUT");

        for (int idx = 0; idx < (int)readevents; ++idx)
        {
            switch (records[idx].EventType)
            {
            case FOCUS_EVENT:
                break;
            case KEY_EVENT:
                if (records[idx].Event.KeyEvent.bKeyDown)
                    isClosing = keyboardEvent(records[idx].Event.KeyEvent, &message, &newClient, name, &index);
                break;
            case MENU_EVENT:
                break;
            case MOUSE_EVENT:
                break;
            case WINDOW_BUFFER_SIZE_EVENT:
                break;
            }
        }

        if (isClosing)
            return;
    }
}

bool keyboardEvent(KEY_EVENT_RECORD input, std::stringstream* msg, client* newClient, std::string name, int* index)
{
    if (input.wVirtualKeyCode == VK_RIGHT)
    {
        if(*index < msg->str().length())
        {
            std::cout << "\033[1C";
            *index += 1;
        }
    }
    else if (input.wVirtualKeyCode == VK_LEFT)
    {
        *index -= 1;
        std::cout << "\b";
    }
    else if (input.wVirtualKeyCode == VK_BACK)
    {
        *index -= 1;
        std::cout << '\b' << ' ' << "\b";

        msg->seekp(-1, msg->cur);

        *msg << " ";
        
        msg->seekp(-1, msg->cur);
    }
    else if (input.wVirtualKeyCode == VK_RETURN)
    {
        if (msg->str() == "/quit")
            return true;
        
        *index = 0;

        newClient->sendMessaage(msg->str().c_str(), (int)msg->str().length());

        std::cout << '\n';

        msg->str("");
    }
    else 
    {
        *index += 1;
        char key = (char)input.uChar.UnicodeChar;

        if(key != NULL && key != '\0')
        {
            *msg << key;
            std::cout << key;
        }
    }

    return false;
}



