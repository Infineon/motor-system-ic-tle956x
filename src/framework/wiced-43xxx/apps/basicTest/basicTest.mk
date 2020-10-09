NAME := App_basicTest

$(NAME)_SOURCES    := basicTest.cpp \

$(NAME)_INCLUDES += ./

$(NAME)_COMPONENTS := drivers/motor-control/TLE9563

VALID_PLATFORMS	:= CYW943907AEVAL1F