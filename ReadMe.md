/*
```
    first caculate every note's position
    without doubt it should be with two factors:
    first is the time it should appear 
    the second is the column it should appear
    and here we could define a window that shows from the current time to the future based on the speed the note run 
    the caculation is like this:

    define the windowSize = 10000pts
    the speed the note run = 1.0f pts/ms
    and add a variable which multiply the speed that decide the windowEndTime

    windowEndTime = currentTime + windowSize / speed the note run 

    so now we get a whole window that slips on the time axis and only in the window the notes should be painted 
    using a timer to track the current time and display the notes in the window, and the timer should be the same as the one in audio player and the note checker
    as the timer running per 1ms, the window move and we repaint it in the paintEvent function.
    but it is easy to find that we can change the efficiency of the program by pre-caculation of the notes' positon and store it in a class
    so here we could init the class and just read it when play.

    however , there should be a factor that depends on the note checker return to hide it at the time the keybord was pressed 
    don't forget to add the keybord event to the running function.
    ```
    */

    