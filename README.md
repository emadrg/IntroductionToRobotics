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

![Homework#5](https://github.com/emadrg/IntroductionToRobotics/blob/main/homework4_stopwatch.jpeg)


# Homework 6
Pseudo-smart environment monitor

This project has the functionality of monitoring the environment, using a ultrasonic sensor (measuring distance) and a LDR sensor (measuring light), being able to interact with it by using a menu. I will paste the instructions given by my proffesor, since I can't find a better way to summarize the purpose of this project: "This system will utilize various sensors to gather environmental data, log this data intoEEPROM, and provide both visual feedback via an RGB LED and user interaction through a Serial Menu.  The project focuses on integrating sensor readings, memory management, Serial Communication and the general objective of building a menu."
I'll briefly go through the menu, sebmenus and their functionality:

1. Sensor settings
	1.1. Sensors sampling interval (the interval at wich the sensors should gather data from the environment)
	1.2. Ultrasonic alert threshold (here you can insert the minimum value that is "okay" for the ultrasonic sensor. See 4.2. for more info)
	1.3. LDR alert threshold (same as above, but for the LDR sensor)
	1.4. Back (to the main manu, not to the time when the most difficult task we got at school was to color inside the lines :') )

2. Reset logger data
	2.1. YES (resets all the logged data)
	2.2. NO 

3.System status
	3.1. Current sensors readings (displays the current sensor readings continuousely (from both sensors), until you press a random key)
	3.2. Current sensors settings (displays the settings made for the sesnsors data, but if not yet modified it displays the default values)
	3.3. Displays logged data (aka last 10 sensor readings)
	3.4. Back

4. RGB Led control
	4.1. Manual Color control (select the desired color for the LED: red, green, blue or off)
	4.2.Toggle Automatic mode ON/OFF (If ON: if the read values are bigger than the min threshold, the LED is geen, if not the LED is red and you also get a sound warning from the buzzer. If OFF, nothing special happens)
	4.3.Back

Sounds cool, right? Yeah sure, but the implementation was a bit of a coding challange. The physical setup itself was a piece of cake, since I used the following:
-ultrasonic sensor
-LDR sensor (with a 10K Ohms resistor)
-buzzer
-RGB Led (with 3 330 Ohms resistors, one for each color)
-wires as needed
Okay so now that we got the circuit part straight, let's dive in into the scary part: the code.
What I firstly needed to do, was to find a way to insert multiple values into the serial monitor, one by one, in order to navigate the cascading menu. This was the most difficult part for me since I had to work with EEPROM, in order to store and read the inputs, but in the end I got the idea to create a reading function to do just that, and it worked wonderfully (thank God). After that, I created a function that displays the main menu and the submenus, and using a "switch" structure in the loop() function, I was able to display the correct submenu, based on the input. 
Each submenu has its own options, which have their own functions as well. These functions modify certain parameters, according to what the user inputs, in order to modify the fucntionality of the sensors.
Lastly, I created a sensors() function. This is a function that combines the behaviour of both sensors, based on the parameters I can modify. These parameters are initialized with default values, so that the sensors can work just fine even if I only modified a few of the parameters, or maybe none. 
This project seemed pretty overwhelming for me at first, especially because of the menu, but I think it turned out pretty well in the end. What made the implementation much easier was creating a lot of functions, since they allowed me to take everything "one by one", instead of having a lot of code in the loop() and be impossible to debug. 


![Homework#6](https://github.com/emadrg/IntroductionToRobotics/blob/main/Homework6_environment_motinor.jpeg)

Here's the video where you can see how my project works: [https://youtu.be/QGPPslQAHVE?si=MYaJMI3td7oAE4R2](https://youtu.be/QGPPslQAHVE?si=MYaJMI3td7oAE4R2)https://youtu.be/QGPPslQAHVE?si=MYaJMI3td7oAE4R2



# Homework 7
Mini matrix game

This little project is the beginning of my matrix game. For this homework, I made sure to correctly connect the 8x8 matrix to the driver, as well as connecting the joystick to the whole project. The idea of this game was pretty simple: you have a player (a LED that blinks) that can shoot different so called 'obstacles'. As for now, the game doesn't really end and it doesn't get competitive either, but it's a good start for what's about to come. 
The main challange for this was correctly connecting the matrix to the driver, since there are a lot of wires and one wrong connection can make everything not work anymore. After I made sure that the hardware was working just fine, I could start writing my code. 
The obstacles were defined by simply creating a matrix that would contain them, so nothing fancy over here. I then needed a player that can shoot lasers up in order to destroy the obstacles, so I created functions for the player as well as the laser shooting. The most difficult part was implementing the logic for displaying the obstacles. What I wanted to happen was to display the obstacles one by one, but only after they were destroyed. What I mean by this is that the 2nd obstacle would only be displayed after the 1st was completely destroyed (all the leds in the obstacle would turn off after being shot). The player's movement on the matrix was done by up-cycling the code from one of my previous assignments, which i added to a separate function for a more "clean" aspect of the code.
I'm sorry if this homework is a little disappointing since it's not that interesting for the user (the player of the game), but it was useful to me as a first step to a more complex game. 


![Homework#6](https://github.com/emadrg/IntroductionToRobotics/blob/main/Homework_7_mini_matrix_game.jpeg)

Here's the video where you can see how my project works: https://youtu.be/dmLaou6sOoU?si=2ul3Njsd1MAaARNR



# Homework 8 
Matrix checkpoint

This project comes as an addition to the last one, since Homework #7 was the starting point. The main purpose of this project was to implement a menu on the LCD, that the user can navigate using a joystick and makling selections by pressing on the joystick. 
I implemented a main menu using a list, every displayable option being reffered to as main_menu[index]. This is helpful, since I only displayed one option at a time on the LCD, and when I press the joystick button the program knows what I chose, thanks to the index. Depending on the option chosen, the game can start (first you have to select a difficulty), or you can adjust the LCD brightness and the matrix brightness (haven't yet implemented these, since I'm having issues implementing the submenus), or displaying some information in the "About" section.
I also implemented a game over functionality, so the program knows when the game ends. The idea of this game is to shoot as many LEDS from the obstacles as possible, while they're dissapearing at a certain rate, depending on the difficulty chosen, therefore the game ends when there is an obstacle that has completely dissapeared by itself and the player hasn't shot any LEDs from that specific obstacle. During gameplay, the LCD displays the number of LEDs that have been shot in total, which will come in handy later on, when implementing a highscore option.
Right now, this project seems a little unstable to me, there are a lot of stuff that I need to fix. For now, my #1 priority is trying to fix the submenus, since they don't take any inputs and I can't modify the LCD or the matrix brightness just yet.

The setup is the same as last time, so I didn't take a new picture.
Here's the video where you can see how my project works: https://youtu.be/8JqMgnbWhA8

