# Serial-Display

In a recent discussion, the idea of re-purposing certain 4-digit LED voltmeter came up. Here is a picture of the voltmeter.

Code available for download here;


Background:

That particular design is based on STM8S003 and has its RX/TX pins available. So I proposed to use them as serial displays, in a daisy-chain fashion. This configuration allows multiple 4-digit displays to be grouped together to form 8/12/16... digit displays, with only 1 line from the controller -> the RX line.

So I ordered two of such units from eBay to experiment with the idea.

While waiting for it, I wrote out the logic and played with it in a simulator. Seems to work fine on a STM8S105/Discovery board.

Then I thought I could run this on a PIC16, especially a PIC16F193x -> the beauty of those chips is that it has 6 consecutive pins on both sides of those chips, RA0..5 on one side, and then RB6..1 on the other side. This allows me to just drop in a 4-digit LED and I can have a serial display with only two components: the PIC16F and the 4-digit LED display.

Software Construct:

The software side of things is quite simple:

the display routine takes data from a buffer, lRAM[4], and puts it to the LEDs one digit at a time;
the display routine is quite simple. The only interesting thing there is the layout of the pins, both on the PIC side and on the display side. They are chosen so that no I can simply mount the LEDs on top of the PIC and the code would work.
the display routine is called from a TIMER0 ISR periodically so that the update of the display is done in the background, fully transparent to the user.
The uart has two components:
The RX side is simple: it polls the receiver to see if there is data. If there is, it processes the data. 
if the data is a valid character ('0'..'9'), the routine updates a 4-char deep circular buffer and outputs the first char to the next display -> this feature allows the display to be daisy-chained;
if the data is a control byte ('\n', or '\r'), the routine dumps the content of the circular buffer into the LED display buffer, AND sends the control byte to the next display -> this feature allows all the displays in the daisy-chain to be updated almost at the same time -> the delay will be for uart transmission of a byte. At 9600bps, that's about 1ms per 4-digit display.
The TX side is even simpler: when called, it simply writes the data to be transmitted into the TX shift register and calls it a day.
