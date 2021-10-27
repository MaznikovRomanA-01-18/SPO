#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include <string>
#define MAX_BUFFER_SIZE 64
using namespace std;

int main()
{
    //punkt 1
    string name;
    cout << "Enter pipe name: ";
    cin >> name;
    auto path = "\\\\.\\pipe\\" + name;
    HANDLE client_pipe = CreateFile(path.c_str(),GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);

    if (client_pipe == INVALID_HANDLE_VALUE)
    {
        cout << "Error:"<< GetLastError()<<endl;
    }

    //punkt 2
    string command (64,'\0');
    DWORD lpNumberOfBytesWritten;
    DWORD lpNumberOfBytesRead;

    //punkt 5
    while (true)
    {
        cout << "> ";
        getline(cin >> ws, command);
        WriteFile(client_pipe, command.c_str(), command.size(), &lpNumberOfBytesWritten, NULL);
        //punkt 3
        if (command == "quit")
        {
          CloseHandle(client_pipe);
          break;
        }
        else
        {
            string lpBuffer (MAX_BUFFER_SIZE, '\0');
            ReadFile(client_pipe, &lpBuffer[0], lpBuffer.size(), &lpNumberOfBytesRead, NULL);
            cout << lpBuffer <<'\n';
        }
        command.replace(0,command.size(),command.size(),'\0');
    }
}
