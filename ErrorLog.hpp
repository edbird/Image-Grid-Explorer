#ifndef ERROR_LOG_HPP
#define ERROR_LOG_HPP


// C++ headers
#include <fstream>



class ErrorLog
{

    public:

    ErrorLog(const std::string& filename)
    {
        error_log.open(filename.c_str(), std::ios::trunc);
    }


    virtual
    ~ErrorLog()
    {
        error_log.flush();
        error_log.close();
    }


    void Print(const std::string& message)
    {
        error_log << message << std::endl;
    }



    protected:

    std::ofstream error_log;

};


#endif // ERROR_LOG_HPP
