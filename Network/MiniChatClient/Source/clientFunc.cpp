#include "clientFunc.h"


void handleEvent(client newClient, std::string name)
{
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
                {
                    if (records[idx].Event.KeyEvent.uChar.UnicodeChar == '\r')
                    {
                        printf("\n");
                        message += '\0';
                        newClient.sendMessaage(message.c_str(), (int)message.length());
                        message = name + " >";
                    }
                    else
                    {
                        message += (char)records[idx].Event.KeyEvent.uChar.UnicodeChar;
                        printf("%c", records[idx].Event.KeyEvent.uChar.UnicodeChar);
                    }
                }
                break;
            case MENU_EVENT:
                break;
            case MOUSE_EVENT:
                break;
            case WINDOW_BUFFER_SIZE_EVENT:
                break;
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

    return false;
}

