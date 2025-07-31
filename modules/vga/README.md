VGA Module
==========

This module is resposible for interacting with vga text mode hardware on x86.

Message Format
--------------

Messages to this module are send in the form of a packed null-terminated array.
Each command byte is folowed by its arguments and the zero command signals
the end of the array.

Supported Commands
------------------

1.  Print string with current color at current location. Takes one byte string length and a string (can not exceed the size of the message).
2.  Print string with specified color at current location. Takes one byte color code, one byte string length and a string (can not exceed the size of the message).
3.  Print single symbol with current color at current location. Takes one byte symbol code.
4.  Print single symbol with specified color at current location. Takes one byte color code and one byte symbol code.
5.  Same as `3` without advancing the cursor.
6.  Same as `4` without advancing the cursor.
7.  Change current color. Takes one byte color code.
8.  Change current position. Takes one byte column index and one byte row index.
9.  Enable hardware cursor. Takes one byte start scanline index and one byte end scanline index.
10. Disable hardware cursor. Takes no arguments.
11. Clear Screen. Takes no arguments.
12. Not implemented yet.

Invalid commands are ignored.

