/************************************************************************/
/* author: xy                                                           */
/* date: 20110512                                                       */
/************************************************************************/
#include "stdafx.h"
#include "xy_logger.h"
#include <fstream>

#ifdef UNICODE

#define XY_TEXT(x)  L##x

#else //UNICODE

#define XY_TEXT(x)  x

#endif

std::ostream& operator<<(std::ostream& os, const POINT& obj)
{
    return os<<"("<<obj.x<<","<<obj.y<<")";
}

std::wostream& operator<<(std::wostream& os, const POINT& obj)
{
    return os<<L"("<<obj.x<<L","<<obj.y<<L")";
}

std::ostream& operator<<(std::ostream& os, const SIZE& obj)
{
    return os<<"("<<obj.cx<<","<<obj.cy<<")";
}

std::wostream& operator<<( std::wostream& os, const SIZE& obj )
{
    return os<<L"("<<obj.cx<<L","<<obj.cy<<L")";
}

std::ostream& operator<<(std::ostream& os, const RECT& obj)
{
    return os<<"("<<obj.left<<","<<obj.top<<","<<obj.right<<","<<obj.bottom<<")";
}

std::wostream& operator<<( std::wostream& os, const RECT& obj )
{
    return os<<L"("<<obj.left<<L","<<obj.top<<L","<<obj.right<<L","<<obj.bottom<<L")";
}

namespace xy_logger
{

#if ENABLE_XY_LOG
    int g_log_once_id=0;
    
    log4cplus::Logger g_logger = log4cplus::Logger::getInstance( XY_TEXT("global_logger_xy") );
#endif

bool doConfigure(const log4cplus::tstring& configFilename)
{
#if ENABLE_XY_LOG
    log4cplus::Hierarchy& h = log4cplus::Logger::getDefaultHierarchy();
    unsigned flags = 0;
    log4cplus::PropertyConfigurator::doConfigure(configFilename, h, flags);
#endif
    return true;
}

bool doConfigure( log4cplus::tistream& property_stream )
{
#if ENABLE_XY_LOG
    log4cplus::PropertyConfigurator p(property_stream);
    p.configure();
#endif
    return true;
}

void write_file(const char * filename, const void * buff, int size)
{
    FILE* out_file = NULL;
    int rv = fopen_s(&out_file, filename,"ab");
    if (rv!=0 || !out_file)
    {
        XY_LOG_ERROR("Failed to open file: "<<filename);
        return;
    }
    fwrite(buff, size, 1, out_file);
    fclose(out_file);
}

void DumpPackBitmap2File(POINT pos, SIZE size, LPCVOID pixels, int pitch, const char *filename)
{
    using namespace std;

    ofstream axxx(filename, ios_base::app);
    int h = size.cy;
    int w = size.cx;
    const BYTE* src = reinterpret_cast<const BYTE*>(pixels);

    axxx<<"pos:("<<pos.x<<","<<pos.y<<") size:("<<size.cx<<","<<size.cy<<")"<<std::endl;
    for(int i=0;i<h;i++, src += pitch)
    {
        const BYTE* s2 = src;
        const BYTE* s2end = s2 + w*4;
        for(; s2 < s2end; s2 += 4)
        {
            axxx<<(int)s2[0]<<","<<(int)s2[1]<<","<<(int)s2[2]<<","<<(int)s2[3]<<",";
        }
        axxx<<std::endl;
    }
    axxx.close();
}

} //namespace xy_logger

