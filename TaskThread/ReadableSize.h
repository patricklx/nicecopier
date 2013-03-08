#ifndef READABLESIZE_H
#define READABLESIZE_H

#include <windows.h>

class Util{

    public:
        static QString toReadableSpeed(double size)
        {
            double floatSize = size;
            QString filesize;
            if (floatSize<1000)
            {
                filesize.sprintf("%.2fB/s", floatSize);
                return( filesize );
            }
            floatSize = floatSize / 1000.0;
            if (floatSize<1000)
            {
                filesize.sprintf("%.2fKb/s", floatSize);
                return( filesize );
            }
            floatSize = floatSize / 1000.0;
            if (floatSize<1000)
            {
                filesize.sprintf("%.2fMb/s", floatSize);
                return( filesize );
            }
            floatSize = floatSize / 1000.0;

            if (floatSize<1000)
            {
                filesize.sprintf("%.2fGb/s", floatSize);
                return( filesize );
            }
            floatSize = floatSize / 1000.0;

            if (floatSize<1000)
            {
                filesize.sprintf("%.2fTb/s", floatSize);
                return( filesize );
            }

            return "invalid speed";
        }

        static QString toReadableSize(double size)
        {
            double floatSize = size;
            QString filesize;
            if (floatSize<1000)
            {
                filesize.sprintf("%.3fB", floatSize);
                return( filesize );
            }
            floatSize = floatSize / 1000.0;
            if (floatSize<1000)
            {
                filesize.sprintf("%.3fKb", floatSize);
                return( filesize );
            }
            floatSize = floatSize / 1000.0;
            if (floatSize<1000)
            {
                filesize.sprintf("%.3fMb", floatSize);
                return( filesize );
            }
            floatSize = floatSize / 1000.0;

            if (floatSize<1000)
            {
                filesize.sprintf("%.3fGb", floatSize);
                return( filesize );
            }

            floatSize = floatSize / 1000.0;

            if (floatSize<1000)
            {
                filesize.sprintf("%.3fTb", floatSize);
                return( filesize );
            }

            return "invalid filesize";
        }

        /*static void errorExit(LPTSTR lpszFunction)
        {
            // Retrieve the system error message for the last-error code

            LPVOID lpMsgBuf;
            LPVOID lpDisplayBuf;
            DWORD dw = GetLastError();

            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dw,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lpMsgBuf,
                0, NULL );

            // Display the error message and exit the process

            lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
                (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
            StringCchPrintf((LPTSTR)lpDisplayBuf,
                LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                TEXT("%s failed with error %d: %s"),
                lpszFunction, dw, lpMsgBuf);
            MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

            LocalFree(lpMsgBuf);
            LocalFree(lpDisplayBuf);
            ExitProcess(dw);

        }*/
};
#endif // READABLESIZE_H
