/** 
 * @file        BLDCM-logger.hpp
 * @brief       BLDCM Logger
 * @date        January 2021
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef TLE9563_LOGGER_HPP_
#define TLE9563_LOGGER_HPP_

#include "../config/tle9xxx-conf.hpp"

#if (TLE9XXX_LOGGER_ENABLED == 1)

#include <stdint.h>
#include "../pal/logger.hpp"

class Logger
{
    public:

                Logger              (LoggerPAL *logpal);
               ~Logger              ();
        void    init                ();
        void    deinit              ();
        void    print               (const char * format,
                                     ...);
        void    printfModule        (const char * format,
                                     const char * module,
                                     const char * color,
                                     ...);
        void    printModuleHex      (const uint8_t  * array,
                                           uint32_t   length,
                                     const char     * module,
                                     const char     * color);

    private:
        LoggerPAL * logpal;
};

/**
 * @brief Library logger
 */
extern Logger tle9563_log;

/** 
 * Logger colors options
 */
#define TLE9563_LOGGER_COLOR_RED                  "\x1b[31m"
#define TLE9563_LOGGER_COLOR_GREEN                "\x1b[32m"
#define TLE9563_LOGGER_COLOR_YELLOW               "\x1b[33m"
#define TLE9563_LOGGER_COLOR_BLUE                 "\x1b[34m"
#define TLE9563_LOGGER_COLOR_MAGENTA              "\x1b[35m"
#define TLE9563_LOGGER_COLOR_CYAN                 "\x1b[36m"
#define TLE9563_LOGGER_COLOR_LIGHT_GREY           "\x1b[90m"
#define TLE9563_LOGGER_COLOR_LIGHT_RED            "\x1b[91m"
#define TLE9563_LOGGER_COLOR_LIGHT_GREEN          "\x1b[92m"
#define TLE9563_LOGGER_COLOR_LIGHT_YELLOW         "\x1b[93m"
#define TLE9563_LOGGER_COLOR_LIGHT_BLUE           "\x1b[94m"
#define TLE9563_LOGGER_COLOR_LIGHT_MAGENTA        "\x1b[95m"
#define TLE9563_LOGGER_COLOR_LIGHT_CYAN           "\x1b[96m"
#define TLE9563_LOGGER_COLOR_DEFAULT              "\x1b[0m"

/**
 * @brief TLE9563 logger module
 */ 

/**
 *  Logger color for different error types
 */
#define TLE9563_LOGGER_ERROR_COLOR               TLE9563_LOGGER_COLOR_RED
#define TLE9563_LOGGER_WARNING_COLOR             TLE9563_LOGGER_COLOR_YELLOW


#define TLE9563_LOG_INIT()\
{\
    tle9563_log.init();\
}

#define TLE9563_LOG_DEINIT()\
{\
    tle9563_log.deinit();\
}

#define TLE9563_LOGGER_SERVICE               "[tle9563]        : "
#define TLE9563_LOGGER_COLOR                 TLE9563_LOGGER_COLOR_GREEN

#define TLE9563_LOG_MSG(str)\
{\
    tle9563_log.printfModule(str, TLE9563_LOGGER_SERVICE, TLE9563_LOGGER_COLOR);\
}

#define TLE9563_LOG_MSG_VAL(str, val)\
{\
    tle9563_log.printfModule(str, TLE9563_LOGGER_SERVICE, TLE9563_LOGGER_COLOR, val);\
}

#define TLE9563_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        tle9563_log.printfModule("fail with return code %i", TLE9563_LOGGER_SERVICE, TLE9563_LOGGER_ERROR_COLOR, ret);\
    }\
    else\
    {\
        tle9563_log.printfModule("pass", TLE9563_LOGGER_SERVICE, TLE9563_LOGGER_COLOR);\
    }\
}

/**
 * @brief TLE9563 Register module
 */ 
#if (TLE9XXX_REG_LOGGER_ENABLED == 1)

#define TLE9563_REG_LOGGER_SERVICE               "[tle9563 reg]    : " 
#define TLE9563_REG_LOGGER_COLOR                 TLE9563_LOGGER_COLOR_GREEN

#define TLE9563_REG_LOG_MSG(str)\
{\
    tle9563_log.printfModule(str, TLE9563_REG_LOGGER_SERVICE, TLE9563_REG_LOGGER_COLOR);\
}

#define TLE9563_REG_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        tle9563_log.printfModule("fail with return code %i", TLE9563_REG_LOGGER_SERVICE, TLE9563_LOGGER_ERROR_COLOR, ret);\
    }\
    else\
    {\
        tle9563_log.printfModule("pass", TLE9563_REG_LOGGER_SERVICE, TLE9563_REG_LOGGER_COLOR);\
    }\
}

#define TLE9563_REG_VALUE_HEX(addr_ptr)\
{\
    tle9563_log.printModuleHex(addr_ptr, 1, TLE9563_REG_LOGGER_SERVICE, TLE9563_REG_LOGGER_COLOR);\
}

#define TLE9563_REG_BITF_VALUE(map, addr, bitf)\
{\
    tle9563_log.printfModule("%x :: 0x%02x -> bfield 0x%02x", TLE9563_REG_LOGGER_SERVICE, TLE9563_REG_LOGGER_COLOR, addr, map[addr], bitf);\
}

#define TLE9563_REG_MAP_HEX(map, length, addr)\
{\
    tle9563_log.print("%s%s", TLE9563_REG_LOGGER_COLOR, TLE9563_REG_LOGGER_SERVICE);\
    tle9563_log.print("%x :: 0x%02x\r\n", 0, map[0]);\
    for(uint8_t i = 1; i < length; i++)\
    {\
        tle9563_log.print("%17x :: ", i);\
        tle9563_log.print("0x%02x", map[i]);\
        if(i == addr)\
            tle9563_log.print("<---");\
            	                    \
        tle9563_log.print("\r\n");\
    }\
    tle9563_log.print("%s", TLE9563_LOGGER_COLOR_DEFAULT);\
}

#else

#define TLE9563_REG_VALUE_HEX(addr_ptr)          {   } 
#define TLE9563_REG_BITF_VALUE(map, addr, bitf)  {   }
#define TLE9563_REG_MAP_HEX(map, length, addr)   {   }   


#endif /* TLE9XXX_REG_LOGGER_ENABLED */

/**
 * @brief TLE9563 Application module
 */ 
#if (TLE9XXX_APP_LOGGER_ENABLED == 1)

/**
 *  Logger color for different error types
 */
#define TLE9563_APP_LOGGER_ERROR_COLOR               TLE9563_LOGGER_COLOR_RED
#define TLE9563_APP_LOGGER_WARNING_COLOR             TLE9563_LOGGER_COLOR_YELLOW


#define TLE9563_APP_LOGGER_SERVICE               "[tle9563 app]    : "
#define TLE9563_APP_LOGGER_COLOR                 TLE9563_LOGGER_COLOR_MAGENTA

#define TLE9563_APP_LOG_MSG(str)\
{\
    tle9563_log.printfModule(str, TLE9563_APP_LOGGER_SERVICE, TLE9563_APP_LOGGER_COLOR);\
}

#define TLE9563_APP_LOG_VAR(str, var)\
{\
    tle9563_log.printfModule(str, TLE9563_APP_LOGGER_SERVICE, TLE9563_APP_LOGGER_COLOR, var);\
}\

#define TLE9563_APP_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        tle9563_log.printfModule("fail with return code %i", TLE9563_APP_LOGGER_SERVICE, TLE9563_APP_LOGGER_ERROR_COLOR, ret);\
    }\
    else\
    {\
        tle9563_log.printfModule("pass", TLE9563_APP_LOGGER_SERVICE, TLE9563_APP_LOGGER_COLOR);\
    }\
}

#else

#define TLE9563_APP_LOG_MSG(str)        {}
#define TLE9563_APP_LOG_VAR(str, var)   {}
#define TLE9563_APP_LOG_RETURN(ret)     {}

#endif /* TLE9XXX_APP_LOGGER_ENABLED */

#else

/** 
 * Logger not enabled. All macros are empty.
 */

#define TLE9563_LOG_INIT()                       {   }
#define TLE9563_LOG_DEINIT()                     {   }

#define TLE9563_LOG_MSG(str)                     {   }
#define TLE9563_LOG_RETURN(ret)                  {   }
#define TLE9563_LOG_MSG_VAL(str, val)            {   }

#define TLE9563_REG_LOG_MSG(str)                 {   }
#define TLE9563_REG_LOG_RETURN(ret)              {   }
#define TLE9563_REG_VALUE_HEX(addr_ptr)          {   } 
#define TLE9563_REG_BITF_VALUE(map, addr, bitf)  {   }
#define TLE9563_REG_MAP_HEX(map, length, addr)   {   } 

#define TLE9563_APP_LOG_MSG(str)                 {   }
#define TLE9563_APP_LOG_VAR(str, var)            {   }
#define TLE9563_APP_LOG_RETURN(ret)              {   }

#endif /* TLE9XXX_LOGGER_ENABLED */

#endif /* TLE9563_LOGGER_HPP_ */