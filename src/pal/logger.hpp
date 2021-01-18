/** 
 * @file        logger.hpp
 * @brief       TLE9563 Logger Platform Abstraction Layer
 * @date        January 2021
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef TLE9563_PAL_LOGGER_HPP_
#define TLE9563_PAL_LOGGER_HPP_

#include "tle9563-conf.hpp"

#if (TLE9563_LOGGER_ENABLED == 1)

#include <stdint.h>

class LoggerPAL
{
    public:
    
    	enum Error_t
		{
			OK          =  0,    /**< No error */
			INTF_ERROR  = -1,    /**< Interface error */
			CONF_ERROR  = -2,    /**< Configuration error */
			READ_ERROR  = -3,    /**< Read error */
			WRITE_ERROR = -4,    /**< Write error */
		};

        /**
         * @brief       Initializes the Logger interface
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t     init    () = 0;
        
        /**
         * @brief       Deinitializes the logger interface 
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t     deinit  () = 0;
        
        /**
         * @brief       Writes in the logger interface
         * @param[in]   *logData    Pointer to the array holding the data to be logged
         * @param[in]   length      Length in bytes of the data to be written 
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t     write   (const uint8_t * logData, uint32_t length) = 0;
};


#endif /* TLE9563_LOGGER_ENABLED */
#endif /**< TLE9563_PAL_LOGGER_HPP_ */