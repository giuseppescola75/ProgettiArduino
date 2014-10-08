/*
SimpleSend
This sketch transmits a short text message using the VirtualWire library
connect the Transmitter data pin to Arduino pin 12
*/
#include <VirtualWire.h>
byte message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message

void setup()
{
// Initialize the IO and ISR
vw_setup(2000); // Bits per sec
}
void loop()
{
  send("Hello there");
  delay(1000);
  Serial.println("Device is ready");
  vw_rx_start(); // Start the receiver
  
  if (vw_get_message(message, &messageLength)) // Non-blocking
  {
  Serial.print("Received: ");
    for (int i = 0; i < messageLength; i++)
    {
      Serial.write(message[i]);
    }
    Serial.println();
  }
}

void send (char *message)
{
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Wait until the whole message is gone
}

