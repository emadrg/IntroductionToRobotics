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

![Homework#3](https://github.com/emadrg/IntroductionToRobotics/blob/main/Homework3_elevator_simulator.jpeg)


# Homework 4
7 segment display drawing

The purpose of this homework was to control a 7-segment display using a joystick. The initial position should be on the decimal point (DP), and the current position should always blink, irrespective of whether the segment is on or off. The joystick is used to move to neighboring positions, and a short press of the button toggles the segment state from ON to OFF or from OFF to ON. Easy, right? Well, I initially thought so, but in reality, I spent quite some time on what seemed like an easy project.

For the physical part of the project, I used a 7-segment display to which I connected eight resistors of 330 Ohms, one for each segment (even though we also have the decimal point, so the name "7-segment display" is a lie; there are eight segments). I then connected each segment to the corresponding pins on the Arduino Uno, making sure to connect the GND of the display to the board's GND as well. Lastly, I connected the joystick with two connections for Vin and GND, and three connections for the X and Y axes, and also the button. The joystick has an internal resistor, so that's none of my concern. The circuit itself was relatively easy to create, and now we're ready to dive into the code.

For the code, I started by defining how the joystick movements should be interpreted. I determined the threshold values that indicate a movement on the X or Y axis sufficient to be considered one of four directions: UP, DOWN, LEFT, or RIGHT. 

Regarding the 7-segment display, I realized that I needed a blink function since the selected segment should blink regardless of its state. The blink function was simple and effective. However, I encountered a challenge: how could I make the display remember the state of its segments when I pressed the button? It took some trial and error, but I concluded that I needed a separate array to keep track of the segment states. With this insight, for every button press, the selected element in the array would change its state from LOW to HIGH or vice versa, and then I could display everything. By separating the blinking and the states, they were not influenced by each other, and the project worked as intended, and I was finally able to sleep.

That being said, this is my small project. I believe it turned out quite well, and it helped me understand how to separate different parts of the code to achieve the desired behavior. Even though it took more time than I initially expected, this is the first robotics project on which I had to perform debugging.

Here's the video where you can see how my project works:https://youtu.be/bqevUxLTv-4?si=czq3UiVVwehYA0jo

![Homework#4](https://github.com/emadrg/IntroductionToRobotics/blob/main/homework4_7_segment_display_drawing.jpeg)




# Homework 5
4-7-segment display stopwatch

This Arduino project is designed to create a digital stopwatch timer with lap functionality using a 4-digit 7-segment display and a shift register. The timer can be started, paused, and reset using physical buttons. Additionally, the user can save lap times during the timer operation. The circuit uses interrupts to handle button presses and features debouncing to ensure accurate input. The 7-segment display is multiplexed to show the current time or lap time, and the lap times are stored in an array for later reference. The project includes code for handling the display, button inputs, and timing functionality. It provides a versatile and customizable platform for creating a digital stopwatch timer with lap recording capabilities.

The phisycal circuit looks a bit complicated because of the wires, but this is the "light" version, thanks to the shift register which allows us to make far fewer connections. I made sure to correctly connect all the wires of the display to the register, as well as connecting everything necessary to the Arduino board. The 3 buttons each require a 1k Ohms resistor, as well as Arduino, 5V and GND connections. 

This project helped me understand how the register works, since it makes the curcuit much lighter, allowing fewer connections. 
Here's the video where you can see how my project works: https://youtu.be/1FbD3kzXKlc?feature=shared

![Homework#4](https://github.com/emadrg/IntroductionToRobotics/blob/main/homework4_stopwatch.jpeg)
