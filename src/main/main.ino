#include <Wire.h>           // For the I2C protocol
#include <Adafruit_PN532.h> // For interacting with the PN532 RFID board
//Defining things for I2C
#define PN532_IRQ   (2)
#define PN532_RESET (3)


//Setting up for an I2C connection to the shield
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
// Assign the motor to a pin
int motorPin = 3;
int blueTime [10]={0,0,0,0,0,0,0,0,0,0};
int redTime [10]={0,0,0,0,0,0,0,0,0,0};
int greenTime [10]={0,0,0,0,0,0,0,0,0,0};

//################################################################################################################################\\
//##################################                       setup                           #######################################\\
//################################################################################################################################\\

void setup()
{
  delay(500);
  // Setup code that is run once is in here:
  Serial.begin(115200); // Using 115200 baud to keep everything quick
  delay(30);
  Serial.println(";Connection established.");
  delay(30);
  Serial.println(";Starting program.");

  delay(30);
  Serial.println(";Setting up door motor...");
  pinMode(motorPin, OUTPUT);

  delay(30);
  Serial.println(";Starting nfc.");
  nfc.begin(); // Starts NFC

  delay(30);
  Serial.println(";Searching for the PN532 board.");
  uint32_t versiondata = nfc.getFirmwareVersion();
  if(!versiondata){
    delay(30);
    Serial.println(";Up-to-date PN532 board not found, halting...");
    while(true);
  }

  //If false, PN532 board has been found and we can proceed
  delay(30);
  Serial.println(";PN532 board found, continuing.");
  delay(30);
  Serial.println(";Configuring board to read RFID tags.");
  nfc.SAMConfig();

  delay(30);
  Serial.println(";Setup complete, ready for an RFID card.");
}

//################################################################################################################################\\
//##################################                    Lighting                           #######################################\\
//################################################################################################################################\\

//Controls room lighting and lighting preferences
class Lighting 
{
    String type="lighting"; //preference type
    byte prefBrightness[3]={5*28,5*28,5*28};  //prefered Brightness default ~half brightness all lights.
    byte roomBrightness[3]; //stores current room's brightness
    public:
      String getType() {return type;} //returns the preference type
      void setRoom(int id);//Interaction with output
      void setRoom(byte bright[]);//Interaction with output
      void getMostUsed(int id); //Finds the most used 
      int getGBrightness() {return (int)roomBrightness[0];}
      int getRBrightness() {return (int)roomBrightness[1];}
      int getBBrightness() {return (int)roomBrightness[2];}
      void setBrightness(byte brigh[]);
} ;

void Lighting::setRoom(byte bright[3]) 
{
      //set green LED
      delay(5);
      pinMode(6, OUTPUT);
      analogWrite(6, bright[0]);
      roomBrightness[0]=bright[0];
      delay(30);
      Serial.println(";Green LED is on at brightness " + (String)bright[0]);
      delay(5);
      //set red LED
      pinMode(5, OUTPUT);
      analogWrite(5, bright[1]);
      roomBrightness[1]=bright[1];
      delay(30);
      Serial.println(";Red LED is on at brightness " + (String)bright[1]);
      delay(5);
      //set blue LED
      pinMode(9, OUTPUT);
      analogWrite(9, bright[2]);
      roomBrightness[2]=bright[2];
      delay(30);
      Serial.println(";Blue LED is on at brightness " + (String)bright[2]);     
}

void Lighting::setRoom(int id) 
{
      delay(30);
      id = abs(id);
      Serial.println(";ID: "+ (String)id);
      delay(200);
      String str;
      while(Serial.available())
      {
        str = Serial.read();
      }
      Serial.println("RFILE:"+(String)id+" gPref");
      delay(200);
      str = "";
      while(Serial.available())
      {
        str += (char) Serial.read();
        //Serial.println(";Green String bytes: " + str);
      }
      int gPref=str.toInt();
      delay(200);
      Serial.println("RFILE:"+(String)id+" rPref");
      delay(200);
      str = "";
      while(Serial.available())
      {
        str += (char) Serial.read();
        //Serial.println(";Red String bytes: " + str);
      }
      int rPref = str.toInt();
      delay(200);
      Serial.println("RFILE:"+(String)id+" bPref");
      delay(200);
      str = "";
      while(Serial.available())
      {
        str += (char) Serial.read();
        //Serial.println(";Blue String bytes: " + str);
      }
      int bPref = str.toInt();
      Serial.println(";RGB: [" + (String) rPref + "," + (String) bPref + "," + (String) gPref + "]");
      byte prevLightPref[3]={gPref, rPref, bPref};
      for(int i=0;i<sizeof(prevLightPref);i++)
      prefBrightness[i] = prevLightPref[i];//Set the previous brightness as stored in file from last run.
      //set green LED
      analogWrite(6, prefBrightness[0]);
      roomBrightness[0]=prefBrightness[0];
      //set red LED
      //pinMode(7, OUTPUT);
      analogWrite(5, prefBrightness[1]);
      roomBrightness[1]=prefBrightness[1];
      //set blue LED
      //pinMode(8, OUTPUT);
      analogWrite(9, prefBrightness[2]);
      roomBrightness[2]=prefBrightness[2];    
}



void Lighting::getMostUsed(int id)
{
  int bMax=0;
  int rMax=0;
  int gMax=0;
  for(int i=0;i<10;i++)
  {
    if(blueTime[i]>bMax) { bMax=i*28; }
    if(redTime[i]>rMax)  { rMax=i*28; }
    if(greenTime[i]>gMax)  { gMax=i*28; }
  }
  delay(30);
  Serial.println("SETWFILE:"+id);
  delay(30);
  Serial.println("WFILE:gPref="+(String)gMax);
  delay(30);
  Serial.println("WFILE:rPref="+(String)rMax);
  delay(30);
  Serial.println("WFILE:bPref="+(String)bMax);
  for(int i = 0;i<10;i++) 
  {greenTime[i]=0;
  blueTime[i]=0;
  redTime[i]=0;}
}

//################################################################################################################################\\
//##################################                    Person                             #######################################\\
//################################################################################################################################\\

class Person{
  String fullName;
  int id;
  public :Lighting lightPref;
  bool here=false; 
  public:
    Person(String fullName1, int id1); //Constructor
    getRoom(); //Get current state of room
    int getID() {return id;} // get ID name
    String getName() {return fullName;} //getFullName
    void setName(String fullName1) {fullName=fullName1;}
    void setID(int id1) {id=id1;} 
    void toggleHere() {here=!here;} 
    setLight(byte[]);
};

Person::Person(String fullName1, int id1)
{
  fullName=fullName1;
  id=id1;
}


//################################################################################################################################\\
//##################################                      Room                             #######################################\\
//################################################################################################################################\\


class Room{
  public:
    Person currentUser =Person("null",-1);
} ;

//################################################################################################################################\\
//##################################                      Loop                             #######################################\\
//################################################################################################################################\\

unsigned digit = 0;

void loop(void) {
  
  Room ourRoom;
  Person jared ("Jared Nussbaum", 1940156574);
  Person chris ("Christopher Thierauf", 217541214);
  Person daniel ("Daniel Coley", 1870188);
  Person leo ("Leonardo DeCosta", 1856019);
  Person nothing("null", -1);
  int currPotentiometer[3];

  uint8_t success;                        // Will return true if the card is readable
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer for the UID, max of 7 char
  uint8_t uidLength;                      // Length of the UID, should return either 4 or 7
  uint32_t cardidentifier = 0;
 
  // Waiting for a card, when found, will attempt to discern if Mifare Classic
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success){   // If the card is readable via the PN532, will return 1 and run this if statement

    delay(30);
    Serial.println(";Found a card, gathering information...\n\n");

    // Creates the proper structure of the ID number to create a human-readable number
    
    cardidentifier = uid[3];
    cardidentifier <<= 8;
      cardidentifier |= uid[2];
    cardidentifier <<= 8;
      cardidentifier |= uid[1];
    cardidentifier <<= 8;
      cardidentifier |= uid[0];

    delay(30);
    Serial.println(";" + cardidentifier); // Print information to the screen
    
    if(cardidentifier == 217541214){ // This is the card labeled 'Christopher'
      delay(30);
      Serial.println(";Read user Christopher. \n"); // Inform the user
      if(ourRoom.currentUser.getID()==chris.getID()){
        ourRoom.currentUser = chris;
        ourRoom.currentUser.toggleHere();
      }
      else{
        ourRoom.currentUser.lightPref.getMostUsed(chris.getID());
        ourRoom.currentUser.toggleHere();
        ourRoom.currentUser=nothing;
      }
      currPotentiometer[0]=analogRead(A0);
      currPotentiometer[1]=analogRead(A1);
      currPotentiometer[2]=analogRead(A2);
    
      
    }else if(cardidentifier == 1870188){   // This is the card labeled 'Daniel'
      delay(30);
      Serial.println(";Read user Daniel. \n");
      if(ourRoom.currentUser.getID()!=daniel.getID()){
        ourRoom.currentUser = daniel;
        ourRoom.currentUser.toggleHere();
      }
      else{
        ourRoom.currentUser.lightPref.getMostUsed(daniel.getID());
        ourRoom.currentUser.toggleHere();
        ourRoom.currentUser=nothing;
      }
      currPotentiometer[0]=analogRead(A0);
      currPotentiometer[1]=analogRead(A1);
      currPotentiometer[2]=analogRead(A2);

      
    }else if(cardidentifier == 1940156574){ // This is the card labeled 'Jared'
      delay(30);
      Serial.println(";Read user Jared. \n");
      if(ourRoom.currentUser.getID()!=jared.getID()){
        ourRoom.currentUser = jared;
        ourRoom.currentUser.toggleHere();
      }
      else{
        ourRoom.currentUser.lightPref.getMostUsed(jared.getID());
        ourRoom.currentUser.toggleHere();
        ourRoom.currentUser=nothing;
      }
      currPotentiometer[0]=analogRead(A0);
      currPotentiometer[1]=analogRead(A1);
      currPotentiometer[2]=analogRead(A2);
      // Toggles boolean 'jaredHere'
 
    
    }else if(cardidentifier == 1856019){ // This is the card labeled 'Leo'
      delay(30);
      Serial.println(";Read user Leo. \n");
      if(ourRoom.currentUser.getID()!=leo.getID()){
        ourRoom.currentUser = leo;
        ourRoom.currentUser.toggleHere();
      }
      else{
        ourRoom.currentUser.lightPref.getMostUsed(leo.getID());
        ourRoom.currentUser.toggleHere();
        ourRoom.currentUser=nothing;
      }
      currPotentiometer[0]=analogRead(A0);
      currPotentiometer[1]=analogRead(A1);
      currPotentiometer[2]=analogRead(A2);

      
    }else{ // This is the card with no label

      delay(30);
      Serial.println(";This user is not registered, access denied! \n");
    }
    if ((cardidentifier == 1856019)||(cardidentifier == 1940156574)||(cardidentifier == 1870188)||(cardidentifier == 217541214)){
    // Opening the door
        analogWrite(motorPin, 225); // Move to open
        delay(1000);                // Wait for open
        analogWrite(motorPin, 0);   // Stop
        delay(2000);                // Hold position
        analogWrite(motorPin, -225);// Move back closed
        delay(1000);                // Wait to close
        analogWrite(motorPin, 0);   // Stop
    }
    delay(2500);
    
    }
      while(1){
      if(abs(currPotentiometer[0]-analogRead(A0))<3 && abs(currPotentiometer[1]-analogRead(A1))<3 && abs(currPotentiometer[2]-analogRead(A2))<3)
        {ourRoom.currentUser.lightPref.setRoom(ourRoom.currentUser.getID());
        Serial.println("hi");} 
      else
      {
        int v1=analogRead(A0);
        v1=map(v1,0,1023, 0,225);
        int v2=analogRead(A1);
        v2=map(v2,0,1023, 0,225);
        int v3=analogRead(A2);
        v3=map(v3,0,1023, 0,225);
        byte potVals[3]={v1,v2,v3};
        ourRoom.currentUser.lightPref.setRoom(potVals);
        }
      blueTime[ourRoom.currentUser.lightPref.getBBrightness()/26]+=1;
      redTime[ourRoom.currentUser.lightPref.getRBrightness()/26]+=1;
      greenTime[ourRoom.currentUser.lightPref.getGBrightness()/26]+=1;
      if(Serial.readString()=="break")
      break;
      delay(10);
  }
  //pingTest();
  //readTest();
 }

