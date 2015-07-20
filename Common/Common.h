//#define LOGGER_DISABLE

#define DIR_SEP "/"
#define DIR_SEP_CHR '/'

#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define unlink _unlink
#define snprintf _snprintf
#define vscprintf _vscprintf

#define fseeko _fseeki64
#define ftello _ftelli64
#define atoll _atoi64
#define stat64 _stat64
#define fstat64 _fstat64
#define fileno _fileno

#include "CommonTypes.h"
#include "NonCopyable.h"

#include "Logger.h"
#include "Utils.h"
#include "Mutex.h"
#include "StringUtil.h"
#include "Timer.h"
#include "WinVer.h"

