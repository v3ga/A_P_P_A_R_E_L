/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */
 
import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress myRemoteLocation;

void setup() {
  size(400,400);
  frameRate(25);
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this,1235);
  
  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  myRemoteLocation = new NetAddress("127.0.0.1",12000);
}


void draw() {
  background(0);  
}

void mousePressed() {
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage myMessage = new OscMessage("/test");
  
  myMessage.add(123); /* add an int to the osc message */

  /* send the message */
  oscP5.send(myMessage, myRemoteLocation); 
}


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  println("### received an osc message.");
  print("    addrpattern: "+theOscMessage.addrPattern());
  println("    typetag: "+theOscMessage.typetag());
  if (theOscMessage.typetag().equals("ssf"))
  {
    println("    [0]="+theOscMessage.get(0).stringValue());
    println("    [1]="+theOscMessage.get(1).stringValue());
    println("    [2]="+theOscMessage.get(2).floatValue());
  }
  else
  if (theOscMessage.typetag().equals("sssf"))
  {
    println("    [0]="+theOscMessage.get(0).stringValue());
    println("    [1]="+theOscMessage.get(1).stringValue());
    println("    [2]="+theOscMessage.get(2).stringValue());
    println("    [3]="+theOscMessage.get(3).floatValue());
  }
}
