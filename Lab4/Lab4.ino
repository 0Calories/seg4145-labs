#include <Wirefree.h>
#include <WifiClient.h>
WIFI_PROFILE wireless_prof = {
/* SSID */ "stingray",
/* WPA/WPA2 passphrase */ "w1r3l3ss!",
/* Robot IP address */ "10.136.160.15",
/* subnet mask */ "255.255.255.0",
/* Gateway IP */ "10.136.160.1", };
String remote_server = "192.168.1.142"; // peer device IP address.
String remote_port = "9876"; // arbitrary
//Creates a client that can connect to a specified internet IP address and port number
WifiClient client(remote_server, remote_port, PROTO_UDP);
void setup()
{
// connect to AP
Wireless.begin(&wireless_prof);
// if you get a connection, report back via serial. client.connect() connect to the IP address and port specified earlier. It returns true if the connection succeeds, false if not.
if (client.connect()) {
Serial.println("connected"); // prints to serial monitor. Check the Serial Monitor Section at the end of this manual.
// Send message over UDP socket to peer device
client.println("aBcDe"); //Your own message
}
else {
// if connection setup failed:
Serial.println("failed");
}
}
void loop()
{
// if there are incoming bytes available from the peer device, read them and print them:
while (client.available()) {
int in;
while ((in = client.read()) == -1);
Serial.print((char)in);
}
delay(1);
}
