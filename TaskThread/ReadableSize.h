#ifndef READABLESIZE_H
#define READABLESIZE_H

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
};
#endif // READABLESIZE_H
