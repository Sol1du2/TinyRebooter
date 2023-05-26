# TinyRebooter
An emergency "REISUBer" for your linux systems.

Have you ever gotten, say, a raspberry pi so frozen you can't even get a picture when connecting a monitor? DON'T UNPLUG POWER AND RISK YOUR FANCY SD CARD!!!! Or maybe you need to reboot a container you lost remote access to and are just too lazy to plug in a keyboard? Well, TinyRebooter is here to (maybe, hopefully) save you! The TinyRebooter has one button and one job only: To send the [Magic SysRq key](https://en.wikipedia.org/wiki/Magic_SysRq_key) command, REISUB, in the hopes of safely rebooting your system even when it is completely unresponsive (**R**eboot **E**ven **I**f **S**ystem **U**tterly **B**roken). Simply plug the device in, press the button, watch the magic show that it is a flashing LED, wait for it to go solid and off you go! Hopefully a safe reboot is on its way!

The project was developed for a [Seeed Studio XIAO SAMD21](https://wiki.seeedstudio.com/Seeeduino-XIAO/), though it would likely still work with any arduino based board that supports Adafruit's TinyUSB library. I.e. it must support presenting itself as a HID device (yes, I said device device, sue me).

# Dependencies
- PlatformIO
- TinyUSB v0.10.5

# Configuration

On top of the `main.c` (really it's pretty much the only useful file) are a few defines you might want to configure depending on your system. The "defaults" are set to work with my use case, your milleage may vary. See, each letter in REISUB is a command. As you might image, a command is not really instant, even if it just takes milliseconds. So I added a delay between each "press". This can be configured along with other shenanigans. Just read the comments, you'll be fine. I tested it with my system and it seems to always be enough to perform all the tasks. But I'm kinda of an idiot so don't take my word and test it yourself. From what I understand you really want to see "Emergency Remount complete" after **U** and before pressing **B**. So make sure of that I guess.

# It doesn't work!!!

- Are you on Windows? Forget it, this is just for linux.
- Does it work on Mac? Honestly, no clue, you tell me.
- Are you on Linux? Check with your distro. Some do not have this Magic key available by default and you need to configure it.

# 3D Prints

Yes, I made a little case for it. No, I have nothing else better to do. You can download the CAD files from my [thingiverse page](). Hate thingiverse? Well, me too, kinda. But I'm too lazy to upload it elsewhere.
