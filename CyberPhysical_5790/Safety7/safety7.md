Calvin Passmore

A02107892

ECE 5790

# Safety 7

## Part 1

    ivy_check bridge.ivy

Output

    Isolate this:

    OK

## Part 2

    ivy_check bridge_fair.ivy

Output

    Isolate this:

    bridge_fair.ivy: line 100: error: mixin westfairmonitor.west_arrive has wrong number of input parameters for controller.arrive

This one I had trouble with this because this error message was highly misleading, I have matched the number of input arguments that controller.arrive takes. Since there is no error report database or any way for me to debug this, I wasn't able to figure out why this error was occuring before this assignment is due.

The bridge_fair.ivy file does run with

    Isolate this:

    Ok

as the output if you comment out lines 100 and 119.
