# IoT first individual assignment

This assignments is a prototipe for an IoT solution to use in university rooms.
By using a photoresistor and an hall sensor we are able to detect in which light condition the projector is open or closed and at what time.
So after making a model we can automatically control the lights with a relay and the curtains with a DC motor to achive the ideal illumination automatically.

In this way the collective intelligence will be something like:
- If (the projector is open **OR** is probable that it will be) **AND** there is too much light **=>** (if lights are on **=>** switch lights off) **AND** if (there is still too much light **AND** curtains are open **=>** close the curtains)
- If (the projector is closed **OR** is probable that it will be) **AND** there is low light **AND** is lecture time **=>** (if curtains are closed **=>** open curtains) **AND** if (there is still too low light **AND** lights are off **=>** switch on the lights)
