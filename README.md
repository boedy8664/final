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
1. Follow the line first.
2. Navigation by apriltag.
3. Follow the line again.
4. Navigation by april tag and stop

## Application
xbee for transport some information and show the step information on screen
line following
april tag for location identification
openMV processing
