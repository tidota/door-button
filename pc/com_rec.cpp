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
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>

using namespace std;

// === format of data ===
// data: <pitch><tab><yaw><tab><4 digits of number>
// OK: "OK"
// error: "E-"<message>
// *OK is to be used to respond for the initialization command "I"
void recv(int port, char *dat){
    int i = 0;
    char buff = 0x03;
    char result;
    do{
        result = read(port, &buff,1);
        if(buff == 0x03){
            dat[i] = 0x00;
            return;
        }else if(buff != '\n' && buff != '\r'){
            dat[i] = buff;
            i++;
        }
    }while(result > 0);
}

// === format of output ===
// data: <pitch> <yaw> <data in mm>  *each item is deliminated by tabs

int main(void)
{
    // set the path to the port
    string path_port;
    path_port = "/dev/ttyUSB0";
    cout << "the path: " << path_port << endl;
    // open the connection
    cout << "opening the port" << endl;
    int port;
    port = open(path_port.c_str(), O_RDWR | O_NOCTTY);

    if(port >= 0)
    {
        cout << "port opened" << endl;
    }else
    {
        cout << "file open error: check the setting" << endl;
        return 1;
    }

/*
    // change to non canonical mode
    // http://www.faqs.org/docs/Linux-HOWTO/Serial-Programming-HOWTO.html)
    // http://linux.die.net/man/3/tcflush
        struct termios oldtio,newtio;
        tcgetattr(port,&oldtio); // save current port settings //
    newtio = oldtio;
        newtio.c_lflag = 0;      // non canonical mode //
        //newtio.c_cc[VMIN] = 0;   // blocking read until 5 chars received //
        tcflush(port, TCIFLUSH);
        tcsetattr(port,TCSANOW,&newtio);
*/


    // recording date // resrouce: http://www.cplusplus.com/reference/ctime/strftime/
    char time_recorded[32];            // time of recording
    time_t rawtime;
    struct tm *timeinfo;

    // first interaction with the device
    char res[32]; // response for recv function

    // for all specified angles
    while(1)
    {
        // wait for the data
        recv(port,res);

        // get the time
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(time_recorded, 32, "%H:%M:%S",timeinfo);

        // output the received data
        cout << "range: " << time_recorded << "\t" << res << endl;
    }

    // close the connection
    close(port);
    cout << "port closed" << endl;
    
    return 0;
}
