alarm-notification
==================

License
==================
Copyright 2013 Brett Brotherton http://www.bbrotherton.com

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

Description
==================

This code in conjunction with [STM32F4DISCOVERY board firmware package](http://www.st.com/web/en/catalog/tools/PF257904)
implements a simple HID keyboard device designed for brute force attacks
against an Android phone.

How It Works
==================

This simply quickly goes through brute and tries to brute force unlocking an android phone
by enumerating as a USB keyboard and sending keystrokes to the phone.  As of this writing
November 2013 android does not lock out after a certain number of guesses it just makes you 
wait 30 seconds after 5 incorrect guesses.  The basic break down of the guessing algorithm is:

1. Make 5 guesses
2. Send the following sequence 

```
[Window+g] cyouremail@yourdomain.com\t[last guess]\t[last guess]\t\t\n".
```
This complicated string attempts to launch gmail and send you an email with the last guess that was made.
3. Wait 30 seconds and then repeat

The idea is that you will hopefully get and email with a guess and then you can work back and narrow it down
to the correct one.  It was decided to only try to send the email every 5 guess in order to speed things up
and minimize the chance of getting into a wierd state on the keypad.  Sending an email was my attempt to automate this 
more, other brute forec attacks I had seen did not do this and therefore you would need to watch the device
and see the screen unlock.

This was a quick prototype designed to solve a problem for a friend it is not
a polished cracking tool.  It is designed for fun and for benign, I am not responsible if
you use this software to break any laws!  It does have the caveat that it doesn't detect if usb is unplugged
so if the phone battery dies it will keep on guessing and you will have to start over.  Also the email sending
sequence does not work 100% reliably sometimes the message did not send.  In these cases it was saved in the drafts
folder since I was trying to unlock a phone with a forgotton passcode that was still a sufficient sucess in my case
so I did not spend further time getting this working perfectly.



It was designed to be able to hone down your choices if you know some information
about the pin.  For example I designed this to help a friend unlock their 
phone when they forgot the PIN.  We started with the following information:

1. The pin was 6 digits
2. The pin does not have the numbers 1, 2, or 3 in it
3. The pin did not repeat any digits (eg 456986 has 6 twice so that should not be attempted)
4. The pin likely did not start with 0.

Tweaking based on this information can all be done in main.c

First we set a start guess of 4 followed by five zeros (since it is believed
the pin does not start with 0) and it is a 6 digit pin.

```
strcpy(guess, "400000");
```
Now that we have our initial guess we simply call nextPermutation after 
each guess.  The first argument is our guess string we will modify this
to obtain our next guess.  The second argument is a string with exclusion 
characters (next perumation is designed only for pins with digits 0-9), 
any permutation that has a character in the exclusion list will not be 
tried.  The final argument is a flag that we set to 1 saying don't allow
any digit to repeat, if this were set to zero then numbers with repeated
digit would be tried as well.

```
nextPermutation(guess, "123", 1);
```

Using this we have narrowed down our guesses from approx 45,000 to around
5,000 at just under 10 guesses per minute due to 30 second blockout after 
5 wrong attmempts that is a run time of under 8 hours...not too bad!

If you want to modify to brute force any 4 digit pin 10,000 different
permutations and about 16 hours of time worst case you would modify the 
above lines to be:

```
strcpy(guess, "0000");
nextPermutation(guess, NULL, 0);
```

Compiling and Flashing
==================
Requirements:
* Linux system with arm toolchain and stlink installed and running see http://liviube.wordpress.com/2013/04/22/blink-for-stm32f4-discovery-board-on-linux-with-makefile/  for setup information
* ST32F4 Discovery Board
* [STM32F4DISCOVERY board firmware package] [5]

1. Download and extract the [STM32F4DISCOVERY board firmware package](http://www.st.com/web/en/catalog/tools/PF257904)
2. in the same directory where you extracted the firmware package pull this git repo
3. cd to the git repo
4. vi Makefile and if necessary modify TOOLS_DIR and STM_ROOTs
5. run: make and then make flash to push it to the board via stlink (Note stlink executables must be in your path or you need to modify the Makefile).

Using
==================

1. Power the board via a mini-usb cable LED 3 should light up
2. Connect the micro usb side of the board to the phone via USB OTG adapter and micro usb cable.
3. LED 3 should turn off indicating the micro is connected to your phone
4. press the user button to start the guessing! LED4 will light up to indicate guessing is active and LED6 will toggle as guesses are made
5. If you wish to pause you can hold the user button till the green (LED 4) turns off.
6. press the user button again to continue guessing.
  

Software used in this Project
==================
They Modifier keys and conversion from ascii to keycodes came from  the
[Arduino](https://github.com/arduino/Arduino) library.

[Liveiu Beraru](http://liviube.wordpress.com/2013/04/22/blink-for-stm32f4-discovery-board-on-linux-with-makefile/) had an excellent blog entry on setting up the toolchain
to a great example makefile in his [Github Project](https://github.com/Malkavian/tuts/tree/master/stm/blinky).

[ST Micro](http://www.st.com/web/en/catalog/tools/FM116/SC959/SS1532/PF252419#) had a great software kit that provided all the low level
drivers and was pretty easy to modify their demo project to get this 
working.

