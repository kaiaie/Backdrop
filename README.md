# Backdrop

A simple Win32 program that displays a window of a given 
colour (by default, white)


## Why?

When taking screenshots in Windows, it usually suffices to press
Alt+PrtScn. However, if capturing multiple overlapping windows, 
the icons and wallpaper on the desktop can intrude. The purpose 
of this program is to provide a blank background that screenshots 
can be taken against without having to rearrange one's desktop 
to do so.


## Usage

If run from the command-line, it will accept a HTML-style RGB 
hex triplet of the form #rrggbb as an argument. If run from 
Explorer, it will present a standard colour picker.


## Caveats

This was thrown together in a hurry. The command-line parser
is particularly egregious. It does the job but is not very 
robust.


## Requirements

Built with MingW32.
