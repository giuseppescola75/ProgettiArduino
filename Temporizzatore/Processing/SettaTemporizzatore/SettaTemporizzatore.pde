import processing.serial.*;
import controlP5.*;

ControlP5 cp5;
Serial myPort; 

void setup() {
  //Inizio grafica
  size(700, 400);
  PFont font = createFont("arial", 20);
  cp5 = new ControlP5(this);

  cp5.addTextfield("Lunedi")
    .setPosition(20, 100)
      .setSize(100, 40)
        .setFont(font)
          .setFocus(true)
            .setColor(color(255, 0, 0))
              ;

  cp5.addTextfield("Lunedi2")
    .setPosition(240, 100)
      .setSize(100, 40)
        .setFont(font)
          .setColor(color(255, 0, 0))
            ;              

  cp5.addTextfield("Martedi")
    .setPosition(420, 100)
      .setSize(100, 40)
        .setFont(createFont("arial", 20))
          .setAutoClear(false)
            ; 

  cp5.addTextfield("Martedi2")
    .setPosition(590, 100)
      .setSize(100, 40)
        .setFont(createFont("arial", 20))
          .setAutoClear(false)
            ;         

  cp5.addTextfield("Mercoledi")
    .setPosition(20, 170)
      .setSize(100, 40)
        .setFont(createFont("arial", 20))
          .setAutoClear(false)
            ; 

  cp5.addTextfield("Mercoledi2")
    .setPosition(240, 170)
      .setSize(100, 40)
        .setFont(createFont("arial", 20))
          .setAutoClear(false)
            ;   

  cp5.addTextfield("Giovedi")
    .setPosition(420, 170)
      .setSize(100, 40)
        .setFont(createFont("arial", 20))
          .setAutoClear(false)
            ; 

  cp5.addTextfield("Giovedi2")
    .setPosition(590, 170)
      .setSize(100, 40)
        .setFont(createFont("arial", 20))
          .setAutoClear(false)
            ; 

  cp5.addTextfield("Venerdi")
    .setPosition(20, 240)
      .setSize(100, 40)
        .setFont(createFont("arial", 20))
          .setAutoClear(false)
            ; 

  cp5.addTextfield("Venerdi2")
    .setPosition(240, 240)
      .setSize(100, 40)
        .setFont(createFont("arial", 20))
          .setAutoClear(false)
            ;   

  cp5.addTextfield("Sabato")
    .setPosition(420, 240)
      .setSize(100, 40)
        .setFont(createFont("arial", 20))
          .setAutoClear(false)
            ; 

  cp5.addTextfield("Sabato2")
    .setPosition(590, 240)
      .setSize(100, 40)
        .setFont(createFont("arial", 20))
          .setAutoClear(false)
            ; 

  cp5.addBang("Submit")
    .setPosition(340, 320)
      .setSize(80, 40)
        .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
          ;  

  textFont(font);
  intro ();

  // List all the available serial ports:
  //println(Serial.list());
  println(Serial.list ().length);
  cp5.addTextlabel("ElencoPorte")
    .setText("Porte disponibili")
      .setPosition(100, 10 )
        .setColorValue(0xffffff00)
          .setFont(createFont("Georgia", 20))
            ;

//  int numeroPorte = Serial.list ().length;
//  for (int i = 0; i < numeroPorte; i++) {
//
//    //    cp5.addTextlabel("label" + i)
//    //      .setText(Serial.list()[i])
//    //        .setPosition(100 +(i*100), 50 )
//    //          .setColorValue(0xffffff00)
//    //            .setFont(createFont("Georgia", 20))
//    //              ;
//
//    // Invio codice di verifica ad Arduino per verificare se trovo la porta corretta
//    try {
//      //println(Serial.list()[i]);
//      Serial myTempPort =  new Serial(this, Serial.list()[i], 57600);
//      //myTempPort.clear();
//      //println("Porta  " + Serial.list()[i].toString());
//      //myTempPort.write("##");
//      //myPort=myTempPort;
//      println("Porta  utilizzabile " + Serial.list()[i].toString());
//    }
//    catch (Exception e)
//    {
//      println(e);
//      println("Porta non utilizzabile " + Serial.list()[i].toString());
//    }
//  }

  //println(Serial.list().length);
  // You may need to change the number in [ ] to match 
  // the correct port for your system
  myPort = new Serial(this, Serial.list()[7], 57600);
  //myPort.bufferUntil('\n');
}

void Submit()
{
  myPort.write("##");
  String lunedi = cp5.get(Textfield.class, "Lunedi").getText();
  String lunedi1 = cp5.get(Textfield.class, "Lunedi2").getText();
  String martedi = cp5.get(Textfield.class, "Martedi").getText();
  String martedi1 = cp5.get(Textfield.class, "Martedi2").getText();
  String mercoledi = cp5.get(Textfield.class, "Mercoledi").getText();
  String mercoledi1 = cp5.get(Textfield.class, "Mercoledi2").getText();
  String giovedi = cp5.get(Textfield.class, "Giovedi").getText();
  String giovedi1 = cp5.get(Textfield.class, "Giovedi2").getText();
  String venerdi = cp5.get(Textfield.class, "Venerdi").getText();
  String venerdi1 = cp5.get(Textfield.class, "Venerdi2").getText();
  String sabato = cp5.get(Textfield.class, "Sabato").getText();
  String sabato1 = cp5.get(Textfield.class, "Sabato2").getText();  
  //Setto la variabile da inviare ad arduino
  //#P01000110015001900    è lunedi dalle 10 alle 11 dalle 15 alle 19
  //#P11000110015001900    è martedi dalle 10 alle 11 dalle 15 alle 19
  String serializedToArduinoLunedi = "#P0" + lunedi +lunedi1 ;
  String serializedToArduinoMartedi = "#P1" + martedi +martedi1 ;
  String serializedToArduinoMercoledi = "#P2" + mercoledi +mercoledi1 ;
  String serializedToArduinoGiovedi = "#P3" + giovedi +giovedi1 ;
  String serializedToArduinoVenerdi = "#P4" + venerdi +venerdi1 ;
  String serializedToArduinoSabato = "#P5" + sabato +sabato1 ;
  println("Invio Lunedi " + serializedToArduinoLunedi);
  println("Invio Martedi " + serializedToArduinoMartedi);
  myPort.write(serializedToArduinoLunedi);
  println(serializedToArduinoMartedi);
  myPort.write(serializedToArduinoLunedi);
}


void draw() {  

  /*while (myPort.available () > 0) {
   int inByte = myPort.read();
   println(inByte);
   }*/
}


void intro () {

  background(0);
  fill(255);
  text(cp5.get(Textfield.class, "Lunedi").getText(), 360, 130);
  //text(textValue, 360, 180);
}

// Called whenever there is something available to read
void serialEvent(Serial myPort) {
  println("ricevendo...");
  // Data from the Serial port is read in serialEvent() using the readStringUntil() function with * as the end character.
  //println(myPort.read());
  String input = myPort.readStringUntil('\n'); 

  if (input != null) {
    // Print message received
    println( "Receiving:" + input);      
    if (input.charAt(0) =='#' && input.charAt(1) =='O' && input.charAt(2) =='K' ) {    //Il responde dovrebbe essere nel formato :#POK1
      switch(input.charAt(3)) {
      case '0':      //Lunedi 
        println("Lunedi arrivato");  
        break;
      case 1:     //Martedi
        println("Martedi arrivato");
        break;
      } 
      myPort.clear();      
      //myPort.write("A");
      println("contact");
    }
  }

  // When finished ask for values again
  //port.write(65);
}

