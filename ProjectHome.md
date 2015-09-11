## Important Update ##
This code is being reworked to meet the following criteria:
  * Software should be run as a background process
  * Software should have the ability to have front-end services for calibration and configuration
    * achieved via local network interface
  * Software should not require it's own GUI software
  * Software should not require complex/diy hardware
Because of the considerable updates, I am removing all the download links (as of Jan 11, 2011), in light of the forth-coming new api.  Please be patient, as my time is getting heavily divided in the next week or so.

## Summary ##
### Current ###
This software's main goal is to provide an abstraction layer from webcam image interpretation to actual interaction with other software.  The idea is to have a daemon/background process to do all the image interpretation, and allow other software to access that information easily.

### History ###
This project was originally based on the [EyeWriter Software](http://www.eyewriter.org/).  The purpose was to simulate the same sort of experience as the EyeWriter Software, without the bloat of [OpenFrameworks](http://www.openframeworks.cc/), and with the portability of ANSI-C.

## Libraries ##
  * [OpenCV 2.1/Win32](https://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.1/) / [OpenCV 2.1/Linux](http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.1/)

## Hardware ##

For building the headset/hardware, check out the [EyeWriter DYI page/instructions](http://www.eyewriter.org/diy/).  The pdf is specifically hosted [here](http://fffff.at/eyewriter/The-EyeWriter.pdf).

I will note that the diagrams seems to be inconsistent with the material list, and I will work on my own instructions that may be a little easier to follow.


---


`*` SDL\_ttf needs to be built against SDL 1.3 - read the README for details on how to do that.  I will see if I can get some binaries for Windows and Linux users posted somewhere soon.