#ifndef MEMORY_H
#define MEMORY_H
#include <QObject>
#include <windows.h>
/*
enum limits{
    QUOTA_LIMITS_HARDWS_MIN_DISABLE=0x00000002,
    QUOTA_LIMITS_HARDWS_MIN_ENABLE=0x00000001,
    QUOTA_LIMITS_HARDWS_MAX_DISABLE=0x00000008,
    QUOTA_LIMITS_HARDWS_MAX_ENABLE=0x00000004
};


class memory_t: public QObject
{
    Q_OBJECT
    public:
        bool enableLock;
        size_t bytes;
        size_t pointers;
        memory_t():enableLock(true),bytes(0),pointers(0)
        {
            SIZE_T sizeToAsk = 50*1024*1024;
            size_t WorkingSetLimitSizeDecrement;
            typedef BOOL (WINAPI *PFN_SetProcessWorkingSetSizeEx)(HANDLE, SIZE_T, SIZE_T, DWORD);

            HMODULE hKernel32 = ::GetModuleHandle(Ltr("kernel32.dll")ll"));
            PFN_SetProcessWorkingSetSizeEx pfnSetProcessWorkingSetSizeEx =
                    (PFN_SetProcessWorkingSetSizeEx) ::GetProcAddress(hKernel32, tr("SetProcessWorkingSetSizeEx")Ex"));
            if (pfnSetProcessWorkingSetSizeEx != NULL)
            {
                qDebug("SetProcessWorkingSetSizeEx is present");

                DWORD dwQuotaFlags = QUOTA_LIMITS_HARDWS_MIN_ENABLE|QUOTA_LIMITS_HARDWS_MAX_DISABLE;
                while (!pfnSetProcessWorkingSetSizeEx(::GetCurrentProcess(), sizeToAsk, sizeToAsk, dwQuotaFlags))
                {
                    //Ensure we don't overflow:
                    if (sizeToAsk <= WorkingSetLimitSizeDecrement)
                    {
                        qDebug("Failed to set working set size limits for host.");
                        break;
                    }

                    if (GetLastError() == ERROR_INVALID_PARAMETER)
                    {
                        //MIN_ENABLE and MAX_DISABLE are not support, revert to:
                        dwQuotaFlags = QUOTA_LIMITS_HARDWS_MIN_DISABLE|QUOTA_LIMITS_HARDWS_MAX_DISABLE;
                        //and retry:
                        continue;
                    }
                    if (GetLastError() == ERROR_NO_SYSTEM_RESOURCES)
                    {
                        //Need to decrease min request:
                        sizeToAsk -= WorkingSetLimitSizeDecrement;
                        continue;
                    }

                    qDebug("Failed to set working set size limit to %i bytes: %d", sizeToAsk, GetLastError());
                    break;
                }
            }
            else
            {
                while (!SetProcessWorkingSetSize(::GetCurrentProcess(), sizeToAsk, sizeToAsk))
                {
                    //Ensure we don't overflow:
                    if (sizeToAsk <= WorkingSetLimitSizeDecrement)
                    {
                        qDebug("Failed to set working set size limits for host.");
                        break;
                    }

                    if (GetLastError() == ERROR_NO_SYSTEM_RESOURCES)
                    {
                        //Need to decrease min request:
                        sizeToAsk -= WorkingSetLimitSizeDecrement;
                        continue;
                    }

                    qDebug("Failed to set working set size limit to %i bytes: %d", sizeToAsk, GetLastError());
                    break;
                }
            }
            qDebug("Successfully set working set size limit to %i bytes", sizeToAsk);
        }
        void* operator_new(size_t size)
        {
            void *mem = malloc(size);
            pointers++;
            if(enableLock)
            {
                bool ok = VirtualLock(mem,size);
                bytes+=size;
                if(!ok)
                    qWarning("failed to lock mem");
            }
            return mem;
        }

        void operator_delete(void *mem)
        {
            pointers--;
            free(mem);
        }

    public slots:
        void disableLock()
        {
            enableLock = false;
            qDebug("locking disbled");
        }


};
*/
#endif // MEMORY_H
