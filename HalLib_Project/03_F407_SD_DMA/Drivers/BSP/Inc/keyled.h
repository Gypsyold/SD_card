#ifndef KEYLED_H_
#define KEYLED_H_

#include "main.h"

typedef enum 
{
	KEY_NONE = 0,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN
} KEYS;

#define KEY_WAIT_ALWAYS		0

KEYS ScanPressedKey(uint32_t timeout);



#endif // KEYLED_H_
