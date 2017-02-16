#include <iostream>
#include "fixinit.h"
using namespace std;

int main(int argc, char *argv[])
{
    FixProcess *fp = new FixProcess();
    if(fp->initFixInterface()){
        fp->createAbossSystemConnect();
    }
    cout << "Hello World!" << endl;
    return 0;
}
