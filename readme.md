Commonly Used Raspberry Pi Classes

Date:       11/20/2013
Author:     K. Loux
Copyright:  Kerry Loux 2013
Licence:    MIT (see LICENSE file)

This collection of classes are used in several of my Raspberry Pi projects, so I decided to break them out as a submodule for ease of maintenance.  This file contains instructions for setting up a brand new Raspberry Pi, setting up a cross-compiler for Raspberry Pi projects on a Linux machine, and also some notes about using the classes contained in this repository.

Note that the GPIO and PWMOutput classes rely on the Wiring Pi library, available at http://wiringpi.com (instructions for compiling and installing are below).

The TimingUtility class relyies on librt.  To link a project that uses the TimingUtility class, you'll need to add -lrt to your linker flags.

=== SETTING UP A BRAND NEW RASPBERRY PI ===

Starting with a brand new Raspberry Pi, follow these instructions to get started.

1. Follow these instructions to prepare Raspberry Pi:
http://elinux.org/RPi_Easy_SD_Card_Setup

After setup, you can log into the RPi via ssh - no monitor required.  After logging in for the first time, if you read the messages, you'll see that they recommend running 'sudo raspi-config', which allows you to easily change things like password and hostname.

2. Follow these instructions in order to set up cross compiling for the RPi:
http://www.bootc.net/archives/2012/05/26/how-to-build-a-cross-compiler-for-your-raspberry-pi/

I had to download a half dozen missing packages in order to get configure to complete, then I had to install subversion to get build to complete.  You may or may not run into the same situation.

When configuring for the build (his instructions #s 3-14 under "To actually create your toolchain...", look for and select the option to build a c++ compiler, too.

After going through all of this, I'm not sure it wouldn't be easier to just install gcc on the RPi.  I haven't done that yet, but getting the cross-compiling stuff working took a lot longer than I would have guessed (allow a couple hours).  But I was also juggling a baby...

If you use vi as your text editor, you'll probably want to create a .vimrc file and add "set nocp" (by default, arrow keys don't navigate in insert mode, instead they insert A, B, etc.)

3.  Install wiringPi library by following the instructions here:
http://wiringpi.com/download-and-install/.  I was originally planning on writing the minimal amount of low-level stuff myself, but two things changed my mind:  Lack of documentation on GPIO functions (you'll find documentation that tells you it is possible to set a pull-up or pull-down for each input pin, but good luck figuring out how to do it - the easiest way I could figure out was by looking at the source code for wiringPi, which implements this feature - and then after I got looking at Wiring Pi, I found that it's not a very big library, implementing only a few more things than we were going to need for this project.

Again, cross-compiling makes stuff more difficult.  The instructions above are fine if you're compiling on the RPi.  If you want to cross-compile, you'll need to make the following changes:
- change the makefile (wiringPi/wiringPi/Makefile), line 36 to use the cross-compiler built in step 2 above.  If you follow exactly as the instructions are written, the compiler would be arm-unknown-linux-gnueabi-g++.  Also, the makefile doesn't define macros for the archiver and ranlib.  You'll need to define those yourself and use those found in the x-tools binary directory you built, then use them in place of ar and ranlib below.
- the INSTALL file has instructions for building a statically-linked library.  If we cross-compile, our options are either link it statically, or install it on the Pi, too, in order to use the dynamic library (which is what builds by default).  I used static linking, and it seems to be working just fine.

4.  There are several guides online for using git submodules.  Here is my summary of the most useful git commands.
- To add rpi to your project as a submodule:
```
$ cd <root directory of your superproject>
$ git submodule add https://github.com/KerryL/rpi.git
```

NOTE:  To add a submodule within another directory, the destination must be specified following the repository url, so instead of the last step above, it would be:
```
$ git submodule add https://github.com/KerryL/rpi.git <desired path>/rpi
```

- Cloning a repository using a submodule now requires an extra step:
````
$ git clone ...
$ cd <project directory created by above clone command>
$ git submodule update --init --recursive
````
