#include "fixinit.h"
#include "FixApi.h"
#include <iostream>
//#include "TestApiDlg.h"
//#include "fiddef.h"
//#include "blsdef_3rd.h"
#include <string>

using namespace  std;
BOOL FixProcess::initFixInterface(){
    if(Fix_Initialize()){
        cout<<"init fix interface success!!"<<endl;;
        return true;
    }else{
        cout<<"init fix interface error!"<<endl;
        return false;
    }
}

void FixProcess::createAbossSystemConnect(){
    if(systemConnect){
        Fix_Close(systemConnect);
        systemConnect = NULL;
    }
    string sysAddr = "192.168.1.1@9000/tcp";
    systemConnect = Fix_Connect(sysAddr.c_str(),"","",1000);
    if(!systemConnect){
        cout<<"can not connect to "<<sysAddr<<endl;
        return ;
    }
}
