# NYFW

Static library for creating applications on the Raspberry Pi, running Pi OS Lite. Written in C using platform and standard libraries.

---

The library **NYFW**, a very clever play on **GLFW** is a simple library which I plan to use to build apps and games on my Raspberry Pi,
using platform libraries and C. So far, the library is able to open a "window" and clear the framebuffer **(/dev/fb0)** and set the system
to graphics mode. The system also handles reading input devices **(/dev/input/eventX)** to read keyboard and mouse input (controller support
eventually). 

There are also several other modules for related tasks, that will probably be moved to separate libraries in the future. These include text drawing,
a custom image format (loading & saving files), a set color palette, and more. 

The graphics system is mainly built around the NYFW\_Canvas struct. This struct provides a representation for a 2D array of pixels. The framebuffer, mapped
to the program, is represented by a Canvas, as are any other images. This allows for easy copying and drawing across the program. 

---

*(A warning for anyone looking)* This library is very much a **work in progress**, and will probably not work on other systems. Reading input devices was 
a bit of a challenge, and I ended up hardcoding the specific mouse and keyboard I'm using. So, unless you have my exact mouse & keyboard, or change the devices
in "src/input.c" and rebuild, this will probably freeze and you'll have to reboot your computer. So, good luck with that. Hopefully, in the near future, I'll
fix that issue and it won't be a problem. But, as long as this paragraph is still here, be wary. 

---

**Planned Changes:** I plan to make some changes to the structure of this guy, especially moving the nonessential stuff to separate libraries that are 
dependent on **NYFW**. I think **Canvas** can be on its own, though maybe with some other drawing modules (like the image one and text one, plus shapes and stuff, idk). 
In the end, I think, **NYFW** should only really be for input and the window. 


