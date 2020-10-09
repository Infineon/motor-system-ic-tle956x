NAME := App_speedControl

$(NAME)_SOURCES    := speedControl.cpp

$(NAME)_INCLUDES += ./

$(NAME)_COMPONENTS := drivers/motor-control/TLE9563

VALID_PLATFORMS	:= CYW943907AEVAL1F