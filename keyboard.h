/**
  ******************************************************************************
  * @file    keyboard.hs
  * @author  Brett Brotherton
  * @version V1.0.0
  * @date    2013-11-03
  * @brief   Library to emulate a USB HID keyboard using 
  * STM32F4-Discovery sample code and drivers
  ******************************************************************************
  * @attention
  * This code was mostly adapted from aurdiuno USB library 
  * https://github.com/arduino/Arduino
  * 
  * Copyright (c) 2010, Peter Barrett   
  * Permission to use, copy, modify, and/or distribute this software for  
  * any purpose with or without fee is hereby granted, provided that the  
  * above copyright notice and this permission notice appear in all copies.  
  * 
  * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL  
  * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  
  * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR  
  * BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES  
  * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
  * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  
  * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS  
  * SOFTWARE.
  * ******************************************************************************
*/
#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

//================================================================================
//================================================================================
//        Keyboard

#define KEY_LEFT_CTRL                0x80
#define KEY_LEFT_SHIFT                0x81
#define KEY_LEFT_ALT                0x82
#define KEY_LEFT_GUI                0x83
#define KEY_RIGHT_CTRL                0x84
#define KEY_RIGHT_SHIFT                0x85
#define KEY_RIGHT_ALT                0x86
#define KEY_RIGHT_GUI                0x87

#define KEY_UP_ARROW                0xDA
#define KEY_DOWN_ARROW                0xD9
#define KEY_LEFT_ARROW                0xD8
#define KEY_RIGHT_ARROW                0xD7
#define KEY_BACKSPACE                0xB2
#define KEY_TAB                                0xB3
#define KEY_RETURN                        0xB0
#define KEY_ESC                                0xB1
#define KEY_INSERT                        0xD1
#define KEY_DELETE                        0xD4
#define KEY_PAGE_UP                        0xD3
#define KEY_PAGE_DOWN                0xD6
#define KEY_HOME                        0xD2
#define KEY_END                                0xD5
#define KEY_CAPS_LOCK                0xC1
#define KEY_F1                                0xC2
#define KEY_F2                                0xC3
#define KEY_F3                                0xC4
#define KEY_F4                                0xC5
#define KEY_F5                                0xC6
#define KEY_F6                                0xC7
#define KEY_F7                                0xC8
#define KEY_F8                                0xC9
#define KEY_F9                                0xCA
#define KEY_F10                                0xCB
#define KEY_F11                                0xCC
#define KEY_F12                                0xCD

//        Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
        uint8_t modifiers;
        uint8_t reserved;
        uint8_t keys[6];
} KeyReport;


/* 
 * keyboardInit:
 * Initializes keyboard HID library 
 * 
 * corehandle: pointer to the USB_OTG_CORE_HANDLE that should be used to
 * send keypresses (the function USBD_HID_SendReport will be used to send keypresses)
 * 
 * return: none
 */
void keyboardInit(USB_OTG_CORE_HANDLE *corehandle);

/* 
 * keyboardPress:
 * Send a single keypress and keeps the key pressed until released
 * used to send combinations of keys.  You can have up to 6 simultaneous
 * keys pressed by subsequent calls to keyboardPress.s
 * 
 * k: an ascii character or one of the keys defined above
 * 
 * return: number of characters sent (0 if error)
 */
int keyboardPress(uint8_t k);

/* 
 * keyboardRelease:
 * Releases the key from the list of keys being pressed 
 *
 * k: an ascii character or one of the keys defined above
 * 
 * return: number of characters sent (0 if error)
 */
int keyboardRelease(uint8_t k);

/* 
 * keyboardRelease:
 * Releases all keys from the list of keys being pressed 
 *
 * return: none
 */
void keyboardReleaseAll(void);

/* 
 * keyboardWrite:
 * Send a single character then send that the key was released
 * 
 * c: character or keycode to send
 * 
 * return: number of keys sent (0 if error)
 */
int keyboardWrite(uint8_t c);

/* 
 * keyboardPutString:
 * Send a string of characters
 * 
 * str: ascii string to send
 * 
 * return: number of keys sent (0 if error)
 */
int keyboardPutString(char *str);

#endif
