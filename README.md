# final
## Setup
xbee : TX<->D10, RX<->D9
openMV : P4<->D0, P5<->D1
Ping : D12
continuous servo : D5、D6

## File description
main.cpp for all operating function
control.py for receive information from xbee
main for my openMV code

## My design
Follow the line first.
Navigation by apriltag.
Follow the line again.
Navigation by april tag and stop

## Application
xbee for transport some information and show the step information on screen
line following
april tag for location identification
openMV processing
