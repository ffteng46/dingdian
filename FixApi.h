/*
 * 顶点中间服务平台外围接入头文件 (C++语言标准接口文件)
 * FixApi.h   2007年4月 创建
 * 与FixApi.lib FixApi.dll一并提供。
 * 适合LOSAP外围接入。
 */

#if !defined(AFX_FIXAPI_H__VER2001__INCLUDED_)
#define AFX_FIXAPI_H__VER2001__INCLUDED_

#ifdef USE_FIXAPI_CLASS
#include <FixApi/FixConn.h>
#endif
#ifdef WIN32
#ifndef FIX_API_EXPORT
#define FIX_API_EXPORT  __declspec(dllexport)
#endif
#else
#define FIX_API_EXPORT extern "C"
#define WINAPI
#endif

//连接句柄
#define HANDLE_CONN     long

//会话句柄
#define HANDLE_SESSION  long

//服务句柄
#define HANDLE_SVC      long

#define BOOL bool
/*  函数名称: Fix_Initialize
 第三方接口库初始化。只有调用一次就可以，不能多次调用。
 返回数据: 
 返回True表示初始化成功；False表示失败。
 */
FIX_API_EXPORT BOOL Fix_Initialize();
FIX_API_EXPORT BOOL Fix_SetAppInfo(const char *pszAppName, const char *pszVer);
/*  函数名称: Fix_Uninitialize
 第三方接口库卸载。只有调用一次就可以，不能多次调用。
 返回数据: 
 返回True表示初始化成功；False表示失败。
 */FIX_API_EXPORT BOOL Fix_Uninitialize();

/*
 函数名称: Fix_SetDefaultInfo
 设置每个业务请求包缺省的头信息
 参数说明: 
 pszUser     -- [in] 系统要求的柜员代码(八位的字符串)。
 pszWTFS     -- [in] 委托方式。
 pszFBDM     -- [in] 发生营业部的代码信息(四位的字符串)
 pszDestFBDM -- [in] 目标营业部的代码信息(四位的字符串)

 返回数据: 
 系统返回类型为HANDLE_CONN的连接句柄。如果连接失败则返回0; 成功不为0;
 */FIX_API_EXPORT BOOL Fix_SetDefaultInfo(const char *pszUser, const char *pszWTFS,
		const char *pszFBDM, const char *pszDestFBDM);

/*
 *	函数名称:Fix_SetDefaultQYBM
 设置缺省的区域代码
 */FIX_API_EXPORT BOOL Fix_SetDefaultQYBM(const char *pszQYBM);


 /*
   函数名称: Fix_ConnectEx
             连接到顶点中间件服务器。
   参数说明: (建议以下参数可配置)
      pszAddr:服务器地址字符串,格式 [ip]@[port]/tcp 例如:127.0.0.1@9901/tcp
      pszUser:通信用户名 (由客户提供,如没有设置通信用户，为空)
      pszPwd: 通信密码 (由客户提供,如没有设置通信用户，为空)
      pszFileCert: 客户端证书文件(由客户提供)
      pszCertPwd: 客户端证书密码
      pszProcotol:
 	             "SSLv3":采用SSLv3协议;
 				 "TLSv1":采用TLSv1协议;
 				 为空或其他: pszFileCert不为空,证书标准安全认证方式
 				             pszFileCert为空, 标准认证方式
 	 pszFileCA:信任证书文件(一般情况下为空,因为信任证书已经在客户端证书中，除非客户有特别提供)
 	 bVerify:验证服务器证书; true表示验证; false:不验证服务器证书; 建议设置为 true
 	 nTimeOUT:认证超时时间(单位秒)
 */
 FIX_API_EXPORT HANDLE_CONN Fix_ConnectEx( const char *pszAddr, const char *pszUser, const char *pszPwd, const char *pszFileCert, const char *pszCertPwd, const char *pszFileCA, const char *pszProcotol, bool bVerify, long nTimeOut );

     /*
      函数名称: Fix_Connect(建议使用Fix_ConnectEx函数)
      连接到顶点中间件服务器。
      参数说明:
      pszAddr  -- [in] 为要连接的服务器地址; 格式为: "ip地址@端口/tcp"
      pszUser  -- [in]  通信用户名称; 由客户提供。
      pszPwd   -- [in] 通信用户的密码; 由客户提供。
      nTimeOut -- [in] 连接超时的时间。单位为秒。

      返回数据:
      系统返回类型为HANDLE_CONN的连接句柄。如果连接失败则返回0; 成功不为0;
      此连接默认启用断线自动重连机制，如果要关闭请在Fix_Connect成功后调用Fix_SetAutoReconnect函数
      */
      FIX_API_EXPORT HANDLE_CONN Fix_Connect(const char *pszAddr, const char *pszUser,
     		const char *pszPwd, int nTimeOut);

/*新增系统全局的备份地址*/
 FIX_API_EXPORT BOOL Fix_AddBackupSvrAddr(const char *pszAddr);
/*
 函数名称: Fix_Close
 与顶点中间件连接关闭。
 参数说明: 
 conn -- [in] 类型为HANDLE_CONN的连接句柄。该句柄由Fix_Connect生成的。
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */FIX_API_EXPORT BOOL Fix_Close(HANDLE_CONN conn);

/*
 函数名称: Fix_AllocateSession
 根据一个连接句柄分配一个会话句柄，用于数据交互。
 由用户自己在应用层通过调用Fix_AllocateSession分配出来的对象必须由用户自己调用Fix_ReleaseSession来释放。
 反之则不必要。原则上遵循“谁分配谁释放”。这样就可以确保内存不泄露。
 参数说明: 
 conn -- [in] 类型为HANDLE_CONN的连接句柄。该句柄由Fix_Connect生成的。
 返回数据: 
 返回值类型为HANDLE_SESSION的会话对象；如果对象值为空表示对象分配失败。否则表示成功。
 */
 FIX_API_EXPORT HANDLE_SESSION Fix_AllocateSession(HANDLE_CONN conn);

/*
 函数名称: Fix_ReleaseSession
 释放会话句柄。
 参数说明: 
 sess -- [in] 类型为HANDLE_SESSION的会话对象。
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */
 FIX_API_EXPORT BOOL Fix_ReleaseSession(HANDLE_SESSION sess);

/*
 函数名称: Fix_SetTimeOut
 设置会话交互的超时时间。
 参数说明: 
 conn    -- [in] 类型为HANDLE_CONN的连接句柄。该句柄由Fix_Connect生成的。
 timeout -- [in] 业务应答超时时间；单位为秒，系统默认为30秒。
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */
 FIX_API_EXPORT BOOL Fix_SetTimeOut(HANDLE_SESSION sess, int timeout);

/*
 函数名称: Fix_SetWTFS
 设置会话的委托方式.(必须在Fix_CreateHead函数之前调用)
 参数说明: 
 sess -- [in] 类型为HANDLE_SESSION的会话句柄。
 val  -- [in] 字符串类型。用于表示客户的接入方式；
 比如电话委托，磁卡委托，互联网委托等。
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */
 FIX_API_EXPORT BOOL Fix_SetWTFS(HANDLE_SESSION sess, const char *val);

/*
 函数名称: Fix_SetFBDM
 设置会话的发生营业部代码.(必须在Fix_CreateHead函数之前调用)
 参数说明: 
 sess -- [in] 类型为HANDLE_SESSION的会话句柄。
 val  -- [in] 字符串类型。用于表示客户业务发生的营业代码,必须是四位的营业部代码;
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */
 FIX_API_EXPORT BOOL Fix_SetFBDM(HANDLE_SESSION sess, const char *val);

/*
 函数名称: Fix_SetFBDM
 设置会话的发生营业部代码.(必须在Fix_CreateHead函数之前调用)
 参数说明: 
 sess -- [in] 类型为HANDLE_SESSION的会话句柄。
 val  -- [in] 字符串类型。用于表示客户业务到达的目标的营业代码,必须是四位的营业部代码;
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */
 FIX_API_EXPORT BOOL Fix_SetDestFBDM(HANDLE_SESSION sess, const char *val);

/*
 函数名称: Fix_SetNode
 设置会话的业务发生的节点信息.(必须在Fix_CreateHead函数之前调用)
 参数说明: 
 sess -- [in] 类型为HANDLE_SESSION的会话句柄。
 val  -- [in] 字符串类型。一般是客户网卡信息或IP地址。
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */
 FIX_API_EXPORT BOOL Fix_SetNode(HANDLE_SESSION sess, const char *val);

/*
 函数名称: Fix_SetGYDM
 设置会话的业务发生的柜员代码信息.(必须在Fix_CreateHead函数之前调用)
 参数说明: sess -- [in] 类型为HANDLE_SESSION的会话句柄。
 val  -- [in] 字符串类型。对一些柜台特殊业务，需要用到柜员信息进行认证。
 返回数据: 返回值为True表示成功; FALSE表示失败。
 */
 FIX_API_EXPORT BOOL Fix_SetGYDM(HANDLE_SESSION sess, const char *val);

/*
 函数名称: Fix_CreateHead
 设置会话的业务功能号.具体的功能号的定义请参照【第三方接入业务接口文档】。
 参数说明: 
 sess  -- [in]  类型为HANDLE_SESSION的会话句柄。
 nFunc -- [in]  整型。即为系统提供的业务功能号。
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */
 FIX_API_EXPORT BOOL Fix_CreateHead(HANDLE_SESSION sess, int nFunc);
#ifdef WIN32
FIX_API_EXPORT BOOL Fix_CreateHead( HANDLE_SESSION sess, const char *pszFunc );
#endif
FIX_API_EXPORT BOOL Fix_CreateReq(HANDLE_SESSION sess, int nFunc);
/*
 函数名称: Fix_SetItem
 设置该会话要发送给中间件的业务的请求域数据 或服务应答域数据。
 注:( 服务应答域数据只是针对一些中间件业务需要第三处理机处理的情况下使用。
 而且是要求通过注册服务实现; 一般的情况下不使用该方式接入。)
 参数说明: 
 sess -- [in]  类型为HANDLE_SESSION的会话句柄。
 id   -- [in]  请求域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
 val  -- [in]  字符串类型;对应于id的业务数据。
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */
FIX_API_EXPORT BOOL Fix_SetItem(HANDLE_SESSION sess, int id, const char *val);
FIX_API_EXPORT BOOL Fix_SetString(HANDLE_SESSION sess, int id, const char *val);
FIX_API_EXPORT BOOL Fix_SetLong(HANDLE_SESSION sess, int id, long val);
FIX_API_EXPORT BOOL Fix_SetInt(HANDLE_SESSION sess, int id, int val);
FIX_API_EXPORT BOOL Fix_SetDouble(HANDLE_SESSION sess, int id, double val);

/*
 函数名称: Fix_SetItem
 设置该会话要发送给中间件的业务的请求域数据 或服务应答域数据。  
 参数说明: 
 sess -- [in] 类型为HANDLE_SESSION的会话句柄。
 id   -- [in] 请求域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
 val  -- [in] 整型;对应于id的业务数据。
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */
#ifdef WIN32
FIX_API_EXPORT BOOL Fix_SetItem( HANDLE_SESSION sess, int id, int val );
#endif

/*
 函数名称: Fix_SetItem
 设置该会话要发送给中间件的业务的请求域数据 或服务应答域数据。  
 参数说明: 
 sess -- [in] 类型为HANDLE_SESSION的会话句柄。
 id   -- [in] 请求域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
 val  -- [in] 浮点类型;对应于id的业务数据。
 返回数据: 
 返回值为True表示成功; FALSE表示失败。
 */
#ifdef WIN32
FIX_API_EXPORT BOOL Fix_SetItem( HANDLE_SESSION sess, int id, double val );
#endif

/*
 函数名称: Fix_Run
 提交业务数据，并等待全部的业务结果返回。  
 参数说明: 
 sess -- [in]  类型为HANDLE_SESSION的会话句柄。

 返回数据: 
 返回值为True表示服务业务处理成功; FALSE表示失败,这个失败是表示业务通信上的失败；
 失败后，可以通过Fix_GetCode取出错误码(必定是一个负数)。通过Fix_GetErrMsg取出错误的信息。
 */
FIX_API_EXPORT BOOL Fix_Run(HANDLE_SESSION sess);

/*
 函数名称: Fix_Cancel
 直接放弃等待业务的应答数据。(在同步方式下使用)
 参数说明: 
 sess -- [in]  类型为HANDLE_SESSION的会话句柄。

 返回数据: 
 返回值为TRUE表示成功; FALSE表示失败。
 */
FIX_API_EXPORT BOOL Fix_Cancel(HANDLE_SESSION sess);

/*
 函数名称: Fix_AsyncRun
 异步提交业务数据，不等待待全部的业务结果返回。
 和函数Fix_IsReplyed配合使用.
 参数说明: 
 sess -- [in]  类型为HANDLE_SESSION的会话句柄。

 返回数据: 
 返回值为True表示服务业务处理成功; FALSE表示失败,这个失败是表示业务通信上的失败；
 失败后，可以通过Fix_GetCode取出错误码(必定是一个负数)。通过Fix_GetErrMsg取出错误的信息。
 */
FIX_API_EXPORT BOOL Fix_AsyncRun(HANDLE_SESSION sess);

/*
 函数名称: Fix_IsReplyed
 异步提交业务数据后，检查数据是否全部返回。
 参数说明: 
 sess -- [in]  类型为HANDLE_SESSION的会话句柄。
 nMSec [in/option] 指定等待的时间，默认不等待。单位为毫秒。

 返回数据: 
 返回值为True表示应答数据全部返回; FALSE表示需要继续等待。
 */
FIX_API_EXPORT BOOL Fix_IsReplyed(HANDLE_SESSION sess, int nMSec = -1);

/*
 函数名称: Fix_GetCode
 读取业务提交失败的错误代码。  
 参数说明: 
 sess -- [in] 类型为HANDLE_SESSION的会话句柄。
 返回数据: 
 返回负整数。
 */
FIX_API_EXPORT int Fix_GetCode(HANDLE_SESSION sess);

/*
 函数名称: Fix_GetErrMsg
 读取业务提交失败的错误信息。  
 参数说明: 
 sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
 out    -- [in/out] 用于输出错误信息的字符串.
 outlen -- [in] 参数out缓冲区的大小.
 返回数据: 
 返回错误信息字符串。
 */
FIX_API_EXPORT char *Fix_GetErrMsg(HANDLE_SESSION sess, char *out, int outlen);

/*
 函数名称: Fix_GetCount
 读取中间件返回的业务应答数据的行数。  
 参数说明: 
 sess -- [in] 类型为HANDLE_SESSION的会话句柄。
 返回数据: 
 返回行数量。
 */
FIX_API_EXPORT int Fix_GetCount(HANDLE_SESSION sess);

/*
 函数名称: Fix_GetItem
 1、从中间件的返回结果集的指定行中读取业务应答数据。
 2、或者是读取中间件请求的业务请求。
 参数说明: 
 sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
 id     -- [in] 指定域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
 out    -- [in/out] 用于输出业务信息的字符串.
 outlen -- [in] 指定out缓冲区的大小.
 row    -- [in][option] 指定返回数据结果集中的行数(行数索引值以0开始).如果不设置这个值，
 表示读取第一行的数据。
 返回数据: 
 返回指定行的的字符串数据。
 */
FIX_API_EXPORT char *Fix_GetItem(HANDLE_SESSION sess, int id, char *out,
		int outlen, int row = -1);

/*
 函数名称: Fix_GetLong
 1、从中间件的返回结果集的指定行中读取业务应答数据。
 2、或者是读取中间件请求的业务请求。
 参数说明: 
 sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
 id     -- [in] 指定域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
 row    -- [in][option] 指定返回数据结果集中的行数(行数索引值以0开始).如果不设置这个值，
 表示读取第一行的数据。
 返回数据: 
 返回指定行的整型数据。
 */
FIX_API_EXPORT long Fix_GetLong(HANDLE_SESSION sess, int id, int row = -1);

/*
 函数名称: Fix_GetDouble
 1、从中间件的返回结果集的指定行中读取业务应答数据。
 2、或者是读取中间件请求的业务请求。
 参数说明: 
 sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
 id     -- [in] 指定域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
 row    -- [in][option] 指定返回数据结果集中的行数(行数索引值以0开始).如果不设置这个值，
 表示读取第一行的数据。
 返回数据: 
 返回指定行的浮点数据。
 */FIX_API_EXPORT double Fix_GetDouble(HANDLE_SESSION sess, int id,
		int row = -1);

/*
 函数名称: Fix_HaveItem
 1、检查中间件的返回结果集的指定行中是否含有指定域。
 2、或者检查中间件请求的业务数据是否含有指定域。
 参数说明: 
 sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
 id     -- [in] 指定域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
 row    -- [in][option] 指定返回数据结果集中的行数(行数索引值以0开始).如果不设置这个值，
 表示读取第一行的数据。
 返回数据: 
 返回True表示,含有指定的域数据；否则没有。
 */FIX_API_EXPORT BOOL Fix_HaveItem(HANDLE_SESSION sess, int id, int row = -1);


//简单加密函数(用户密码加密)
FIX_API_EXPORT char *Fix_Encode(char *pData);

/*
 函数名称: Fix_GetToken
 取出对于的令牌字符串(可选、与后台系统配置有关)
 */

FIX_API_EXPORT char *Fix_GetToken(HANDLE_SESSION sess, char *out, int outlen);
/*
 函数名称: Fix_SetToken
 设置会话的令牌(可选、与后台系统配置有关)
*/
FIX_API_EXPORT BOOL Fix_SetToken(HANDLE_SESSION sess, const char *val);

/*
    面向主交易系统的客户数据订阅函数
    pFunc为回调函数指针,函数定义为: bool CallBackFunc( HANDLE_CONN conn, HANDLE_SESS sess, long nSubs, void *pData )
    其中conn为连接句柄, nSvc表示服务号，nSubs为Fix_MDBSubscibeByCustomer还回的值，用于指明某个订阅的数据。
	pszKHH, pszPwd,分别为客户号与客户密码[密码为明文]。
    pData就是订阅时的指针(可以为NULL值)
	返回值大于0,订阅成功，否则失败.返回值可用于退订函数(数据退订函数)的输入参数
*/
long Fix_SubscibeByCustomer(HANDLE_CONN conn, long nSvc, void *pFunc, void *pData,const char *pszKHH, const char *pszPwd );

//数据退订函数
//nHandle: 订阅句柄；为Fix_MDBSubscibeByCustomer的返回值
FIX_API_EXPORT BOOL Fix_UnSubscibeByHandle( HANDLE_CONN conn, long  nSubs );

//获取错误信息，比如在Fix_Connect或者Fix_ConnectEx失败的时调用
FIX_API_EXPORT char *Fix_GetLastErrMsg( char *out, int outlen );
/*
 * 设置会话应答回调函数
 * hSess = NULL,表示设置全部业务会话应答的回调函数；否则为指定会话的回调函数
 * 回调函数原型：bool Func(HANDLE hConn, HANDLE hSess, int nRecv );
 * 当前情况下，nRecv均为1，在2.5.7版本以前，nRecv为long类型，现在改为int类型，保持与windows版本一致
 */
FIX_API_EXPORT BOOL Fix_RegReplyCallFunc( HANDLE_SESSION hSess, void *pFunc );

//设置会话附加信息
FIX_API_EXPORT BOOL Fix_SetSessData( HANDLE_SESSION hSess, void *pData );

//读取会话附加信息
FIX_API_EXPORT void *Fix_GetSessData( HANDLE_SESSION hSess );

//outlen是输入输出变量
FIX_API_EXPORT BOOL Fix_GetItemBuf( HANDLE_SESSION sess,  char *out, int &outlen, long row );

//设置日志路径，默认在当前路径的log目录下
FIX_API_EXPORT BOOL Fix_SetLogPath( const char *pszPath );

//设置日志级别 默认为0，表示记录连接日志 1，表示连接日志+通信报文日志
FIX_API_EXPORT BOOL Fix_SetLogLevel( int nLevel );

/*
   函数名称: Fix_GetColumnCount
             获取指定行数据的列数量
   参数说明:
             sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
			 nRow   -- [in] 行序号,从零开始
   返回数据:
             返回列数量。
*/

FIX_API_EXPORT int Fix_GetColumnCount( HANDLE_SESSION sess, int nRow );

/*
   函数名称: Fix_GetValWithIdByIndex
             获取指定行、指定列下的字段id号 以及相应的数据
   参数说明:
             sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
			 nRow   -- [in] 行序号,从零开始(配合Fix_GetCount)
			 nIndex -- [in] 列序号,从零开始(Fix_GetColumnCount)

			 nFid   -- [out] 域id
			 out	-- [out] 域对应的数据(内存上层分配)
			 nSize	-- [in/out] out的大小，函数会输出相应的真实数据大小
   返回数据:
             true:成功
			 false:失败(比如序号越界或者out内存不足)
*/
FIX_API_EXPORT BOOL Fix_GetValWithIdByIndex( HANDLE_SESSION sess, int nRow, int nIndex, long &nFid, char *out, int &nSize );

//开启日志功能(默认开启日志功能)
FIX_API_EXPORT BOOL Fix_WriteLog(BOOL bWrite);
FIX_API_EXPORT BOOL Fix_Write(HANDLE_SESSION sess);
// 启用/禁用HANDLE_CONN断线自动重连机制，默认启用，如果第三方采用自己管理HANDLE_CONN连接, 必须把自动重连关闭，避免不必要的资源浪费
FIX_API_EXPORT BOOL Fix_SetAutoReconnect(HANDLE_CONN conn, BOOL bAutoReconnect);

// 获取连接HANDLE_CONN断线自动重连状态
FIX_API_EXPORT BOOL Fix_GetAutoReconnect(HANDLE_CONN conn);
/*
   函数名称: Fix_IsConnect
                  判断连接状态 返回值 true:正常; false:中断
*/
FIX_API_EXPORT BOOL Fix_IsConnect( HANDLE_CONN conn );

#endif
