# Serial-Display

In a recent discussion, the idea of re-purposing certain 4-digit LED voltmeter came up. Here is a picture of the voltmeter.

Code available for download here;


Background:

That particular design is based on STM8S003 and has its RX/TX pins available. So I proposed to use them as serial displays, in a daisy-chain fashion. This configuration allows multiple 4-digit displays to be grouped together to form 8/12/16... digit displays, with only 1 line from the controller -> the RX line.

So I ordered two of such units from eBay to experiment with the idea.

While waiting for it, I wrote out the logic and played with it in a simulator. Seems to work fine on a STM8S105/Discovery board.

Then I thought I could run this on a PIC16, especially a PIC16F193x -> the beauty of those chips is that it has 6 consecutive pins on both sides of those chips, RA0..5 on one side, and then RB6..1 on the other side. This allows me to just drop in a 4-digit LED and I can have a serial display with only two components: the PIC16F and the 4-digit LED display.

EDIT: I completed a trial of the code on two PIC18F25K20 and one PIC16F1936. The code worked flawlessly on both chips.

For the PIC16/PIC18 version, see here: https://dannyelectronics.wordpress.com/2017/06/25/worlds-simplest-serial-display-pic16f/
