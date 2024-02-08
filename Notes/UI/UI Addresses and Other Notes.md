``7ff6a9d05310`` - Some scaling opcodes that get called on
``7ff6a9a8f12d`` - Battle Jack
``7ff6a9a8f51f`` - Something
``7ff6a9a8f51f`` - Something else
``7ff6a9a92cf5`` - Something else
``7ff6a9a9378f`` - Code Jack
``7ff6a9a9389b`` - Install Genre
``7ff6a9a939a7`` - Summon
``7ff6a9a95582`` - Battle Result
``7ff6a9a96162`` - Something
``7ff6a9a97292`` - Something else
``7ff6a9a97851`` - Level Up Result
``7ff6a9a981cd`` - Something battle related
``7ff6a9a9a073`` - Something
``7ff6a9a9a033`` - Something else interesting (960 & 540?)
``7ff6a9a8eec0`` - Seemingly a function that calls some coordinate stuff, main example being the confirmation window

For the UI, we are going to be replacing the UI stuff to be using a resolution with 1920 or 1080 (based on the size of the aspect ratio), with the other part of the resolution using something that scales with the aspect ratio (for example, 2580x1080 for a 3440x1440 resolution)

Then, we are going to adjust the horizontal (or vertical if an aspect ratio narrower than 16:9) coordinates of individual UI elements to better suit the new aspect ratio.

For game speed stuff, we are going to get the difference between a fixed 60FPS delta time and the current delta time, multiply 1.0 by that difference, and then multiply that by our time dilation value, so these effects still work as intended while in slow motion.

To get the horizontal offset to move some stuff horizontally, you can do 2580 (or whatever the 1080p horizontal aspect ratio is) - 1080, take that difference, and then either add it or subtract it from the positional value.