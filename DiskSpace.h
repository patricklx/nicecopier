#ifndef DISKSPACE_H
#define DISKSPACE_H

#include <QDir>
#ifdef _WIN32
#include <windows.h>
#else // linux stuff
#include <sys/vfs.h>
#include <sys/stat.h>
#endif // _WIN32

static bool getFreeTotalSpace(QString sDirPath,double* fTotal, double* fFree)
{
#ifdef _WIN32

    QString sCurDir = QDir::current().absolutePath();
    QDir::setCurrent( sDirPath );

    ULARGE_INTEGER free,total;
    bool bRes = ::GetDiskFreeSpaceExA( 0 , &free , &total , NULL );
    if ( !bRes ) return false;

    QDir::setCurrent( sCurDir );

    if(fFree != NULL)
        *fFree = static_cast<double>( static_cast<__int64>(free.QuadPart) );

    if(fTotal != NULL)
        *fTotal = static_cast<double>( static_cast<__int64>(total.QuadPart) ) ;

#else //linux

    struct stat stst;
    struct statfs stfs;

    if ( ::stat(sDirPath.local8Bit(),&stst) == -1 ) return false;
    if ( ::statfs(sDirPath.local8Bit(),&stfs) == -1 ) return false;

    fFree = stfs.f_bavail * ( stst.st_blksize  );
    fTotal = stfs.f_blocks * ( stst.st_blksize );

#endif // _WIN32

    return true;
}


#endif // DISKSPACE_H
