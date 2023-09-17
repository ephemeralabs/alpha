# Ring effects

Ring effects is a ready to flash Arduino sketch and electronic circuit (Arduino Uno) developed with the purpose of exploring different materials and NeoPixel light rings possible effects within the scope of Alpha.
It supports 6 different effects:

* Carousel: The lights on the ring turn on sequentially at a fix rate, and then, when all of them are lit, they turn off following the same sequence.
* Blink: The ring blinks.
* Fade: All the lights in the ring fade in at the same time, and then they fade off.
* Fade carousel: Similar to the carousel, but each lit LED lights brighter than the previous one.
* Incremental carousel: Similar to the carousel, but on each complete loop the LEDs brightness increases.

The program starts on an off state, an the circuit includes a button that can be pressed at any time to transition from the current effect to the next one.
Furthermore, the circuit also includes a potentiometer that can be used to adjust the speed at which each of the effects runs.

![video_demo](../../videos/ring_effects_20230917.mp4)
