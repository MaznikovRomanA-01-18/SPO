#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <map>

#define MAX_BUFFER_SIZE 64

using namespace std;

int main()
{
    // 1
    string pipe_name;
    cout << "Enter pipe name: ";
    cin >> pipe_name;
    string pipe = "\\\\.\\pipe\\" + pipe_name;
    HANDLE client_pipe = CreateNamedPipe(pipe.c_str(),
                                    PIPE_ACCESS_DUPLEX,
                                    PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
                                    PIPE_UNLIMITED_INSTANCES,
                                    MAX_BUFFER_SIZE,MAX_BUFFER_SIZE,0,
                                    NULL);
    if (client_pipe == INVALID_HANDLE_VALUE)
    {
        cout << "Error:"<< GetLastError()<<'\n';
    }

    // 2,3
    string command (MAX_BUFFER_SIZE, '\0');
    string keyword, key, value, response{};
    DWORD lpNumberOfBytesRead;
    map < string, string > data {};

    while (true)
    {
        cout << "Waiting for a client... ";
        if (!ConnectNamedPipe(client_pipe, NULL))
        {
           cout << "Error: "<< GetLastError()<<'\n';
           CloseHandle(client_pipe);
        }
        else
        {
           cout << "connected"<<'\n';
        }

        while(true)
        {

            cout << "\nWaiting for command... ";
            auto fr = ReadFile(client_pipe, &command[0], command.size(), &lpNumberOfBytesRead, NULL);
            if (!fr)
            {
                auto err = GetLastError();
                cout<<"Error:"<<err;
            }
            else
            {
                cout << "received,\n"<<'\n';;
            }
            command.resize(command.find('\0'));
            cout << command;
            istringstream parser {command};
            parser >> ws >> keyword;

            if (keyword == "set")
                {
                    parser >> key >> value;
                    data[key] = value;
                    response = "acknowledged";
                }
            else if (keyword == "get")
                {
                    parser >> key;
                    if (data.find(key) != data.end())
                        response = "found " + data[key];
                    else
                        response = "missing";
                }
            else if (keyword == "list")
                {
                    for (auto i = data.begin(); i != data.end(); ++ i)
                        response += i->first + " ";
                }
            else if (keyword == "delete")
                {
                    parser >> key;
                    auto del = data.find(key);
                    if (del != data.end())
                    {
                        data.erase(del);
                        response = "deleted";
                    }
                    else
                        response = "missing";
                }
            else if (keyword == "quit")
            {
                DisconnectNamedPipe(client_pipe);
                command.replace(0, command.size(), command.size(), '\0');
                command.resize(MAX_BUFFER_SIZE, '\0');
                break;
            }
            else
            {
                 cerr << "Incorrect command! (command: \"" << command << "\")\n";
                response = "incorrect command";
            }


            auto frr = WriteFile(client_pipe, response.c_str(), response.size(), &lpNumberOfBytesRead, NULL);
            if (!frr)
            {
                auto err = GetLastError();
                cout<<"Error:"<<err;
            }


             command.replace(0, command.size(), command.size(), '\0');
             command.resize(MAX_BUFFER_SIZE, '\0');
             response.clear();
             keyword.clear();
             key.clear();
             value.clear();
        }


        // 5
        char answer;
        bool exit = false;
        std::cout << "\nDo you want to destroy pipe \"" << pipe_name << "\" (y/n)?: ";
        while (cin >> answer)
        {
            if (answer == 'y')
            {
                (CloseHandle(client_pipe), "occurred while closing pipe", pipe_name);
                exit = true;
                break;
            }
            else if (answer == 'n')
                break;
            else
            {
                cout << "(y/n): ";
                continue;
            }
        }
        if (exit)
            break;
    }
}
