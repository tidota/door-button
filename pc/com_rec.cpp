// com_rec.cpp
//
// This program sends commands to the device through the USB connection and receive the data.
// The received data is displayed on the terminal (which can be piped)

// updates:
// 140402-1153: created the file and defined the procedure and formats
// 140413-0923: completed the communication with the device (it was needed to change to non-canonical mode)
// 140912-1338: new format of input (p0000y0000) applied so that decimal part of degree can be entered


#include <iostream>
#include <fstream>
#include <string>

#include <ctime>

#include <sstream>
#include <iomanip>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>

using namespace std;

int port;

void finish(int sig)
{
    // close the connection
    close(port);
    cout << "port closed" << endl;
    exit(0);
}

void recv(int port, char *dat)
{
    int i = 0;
    char buff = '\n';
    char result;

    do
    {
        result = read(port, &buff,1);

        if(buff == '\n')
        {
            dat[i] = '\0';
            break;
        }
        else if(buff != '\r')
        {
            dat[i] = buff;
            i++;
        }
    }
    while(result > 0);
}

// === format of output ===
// data: <pitch> <yaw> <data in mm>  *each item is deliminated by tabs

int main(void)
{
    string path_port;
    path_port = "/dev/ttyUSB0";
    cout << "port: " << path_port << endl;

    cout << "opening the port" << endl;
    port = open(path_port.c_str(), O_RDONLY | O_NOCTTY);

    if(port >= 0)
    {
        cout << "port opened" << endl;
    }else
    {
        cout << "file open error: check the setting" << endl;
        return 1;
    }

    signal(SIGINT, finish);

    // recording date // resrouce: http://www.cplusplus.com/reference/ctime/strftime/
    char time_recorded[32];            // time of recording
    time_t rawtime;
    struct tm *timeinfo;

    // first interaction with the device
    char res[32]; // response for recv function

    cout << "start" << endl;

    // for all specified angles
    while(1)
    {
        // wait for the data
        recv(port, res);

        if (strcmp("beep", res) == 0)
        {
            // beep
            cout << "beep" << endl;
            system("notify-send -u critical \"!!! ATTENTION !!!\" \"Someone pressed the door button\"");
            system("cvlc --play-and-exit --no-repeat alarm.ogg");
        }
        else if (strcmp("email", res) == 0)
        {
            // get the time
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(time_recorded, 32, "%H:%M:%S",timeinfo);

            // output the received data
            cout << "sending email (" << time_recorded << ")" << endl;
            system("echo \"Someone pressed the door button.\" | mail -s \"Door Button Notification\" tidota@hawaii.edu");

        }
        else
        {
            cout << "[info] " << res << endl;
        }
    }

    return 0;
}
