#pragma once
//#pragma warning(disable:4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif

#define DEFAULT_HTTP_HEADER "User-Agent->Mozilla&Upgrade-Insecure-Requests->1"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

// .NET Header Files:
/*#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>
#include <vcclr.h>*/

// Windows Header Files:
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <wincontypes.h>
#include <regex>
#include <iterator>
#include <cstring>  
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <thread>
#include <sys/types.h>
#include <stdlib.h>
#include <istream>
#include <ostream>
#include <iostream>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <chrono>
#include <errno.h>
#include <exception>
#include <stdexcept>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <streambuf>
//#include <unistd.h>
//#include <vcclr.h>

// Third party Lib:
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/HTMLparser.h>
#include <curl/curl.h>
#include <boost/type_traits/type_with_alignment.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/random.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/regex.hpp>
#include <boost/throw_exception.hpp>
#include "nlohmann/json.hpp"
#include <nlohmann/detail/input/input_adapters.hpp>
#include <nlohmann/json_fwd.hpp>

// Class Header Files:
#include "SocketUtility.h"
#include "EncoderUtility.h"
#include "HttpRequestCurl.h"
#include "HostUtility.h"
#include "HtmlUtility.h"
#include "DropboxApi.h"
#include "ProcessThread.h"
#include "ThreadManager.h"
#include "crawl.h"
#include "sql.h"
#include "nosql.h"

/*using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Collections;*/
using namespace boost;
using namespace boost::beast;
using namespace boost::uuids::detail;
using namespace boost::algorithm;
using namespace nlohmann;
using namespace std;
