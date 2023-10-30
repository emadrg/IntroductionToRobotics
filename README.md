# IntroductionToRobotics
This repo reflects my learning journey in the field of robotics, so I'll be uploading here the projects I make for university (2023-2024) in order to understand the basic concepts.

# Homework 2
For this project, I connected an RGB led to 3 potentiometers, which allow me to control the 3 colors of the led: Red, Green and Blue. 
I connected the cathods of the LED to the ports ~3, ~5 and ~6, making sure to add a resistor for each (values ranging from 100 to 220 Ohms), while the potentiometers are connected to the A0, A1 and A2 ports. In the end I made sure they're also connected to GND, as well as to the power source (ports VIN and GND).
When I connect the project to the source code, each potentiometer controls its own color, having the possibility to play with light intensities simultaneously, also creating white light when all 3 colors are at their maximum intesity.

Here's the video where you can see how my project works: https://youtu.be/gTTg6-AfAUg?si=IjdEQBx7E5JBRRrm


![Homework#1](https://github.com/emadrg/IntroductionToRobotics/assets/115634320/e217a64b-4cf8-46f6-9c12-9bf43fc8d5cd)


# Homework 3
Elevator simulator

  This project simulates an elevator in movement. I used 3 buttons, one for each floor and 4 LEDs: 3 LEDs for the floors and one for the operational state, as well as a buzzer, which gives us an auditory feedback on the elevator's state.
  The purpose of this circuit is to offer the impression of an elevator working for a 3-floor building. The floor LEDs indicate the presence of the lift on a certain floor by being ON (elevator present) or OFF (elevator absent), while the operational state LED shows the user that the elevator is in movement by blinking and remaining ON when the elevator is stationary. Worth memntioning is the fact that when the elevator is already on the desired floor, nothing happens. For each LED, I used a 220 Ohms resistor.
  I've also added a buzzer which makes a specific sound while the elevator is moving and beeping when the elevator is on its desired position, connecting to it a 100 Ohms resistor.
The main purpose of my project was understanding debouncing techniques, as well as learning how to coordinate multiple parts of the circuit.
  I've implemented debouncing for the action of pushing a button ("calling the elevator") in order to avoid unintentional repeated button press. This way, the circuit doesn't "get confused" by accidental state changes created by electrical arches in the button.
  For coordination, the main challange was making the operational state led blink while the floor LEDs also get turned on one by one. In order to make these actions parallel, I used two separate functions for each type of LEDs, alongside a function that adds the buzzer activity specific for each state of the elevator.
  All in all, this project was a little challanging for me, since it was more complex than what I was used to. However, the circuit itself was a lot easier that writing the code, which I found to be the most difficult part. 
  To conclude, this little homework was a very good way to understand debouncing, working with millis() and understanding how to make multiple parts of the circuit run simultaneously. 

Here's the video where you can see how my project works: https://youtu.be/d4HAe15tKeQ?si=pGSumkI3E9IJJzQi
