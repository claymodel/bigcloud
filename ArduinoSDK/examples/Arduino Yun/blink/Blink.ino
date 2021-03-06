#include <Parse.h>
#include <Bridge.h>

String ledState;
const int BUFSIZE = 200;

// set LED state and send state to Parse
// This method must be called after parse.initialize()
void setLedState(char state[]) {
  ledState = state;

  // send current led state to parse
  ParseObjectCreate create;
  create.setClassName("Event");
  create.add("installationId", Parse.getInstallationId());
  create.add("alarm", true);
  String value = "{\"state\":\"";
  value += state;
  value += "\"}";
  create.addJSONValue("value", value);
  ParseResponse response = create.send();

  String eventId;
  Serial.println("\nSetting LED state...");
  Serial.print(response.getJSONBody());
  if (!response.getErrorCode()) {
    eventId = response.getString("objectId");
    Serial.print("Event id:");
    Serial.println(eventId);
  } else {
    Serial.println("Failed to notify Parse the LED state.");
  }
  response.close();
}

void setup() {
  // Initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  // Initialize Bridge (with linux)
  Bridge.begin();

  // Initialize Serial
  Serial.begin(9600);

  while (!Serial); // wait for a console connection

  // appId and clientKey will be provided in provisioning
  Parse.begin(NULL, NULL);

  // do provisioning now
  Serial.println("Please go to arduino.local/parse_config.html to complete device provisioning. Press y when you are done.");
  while (1) {
    if(Serial.available()) {
      char c = (char)Serial.read();
      if (c == 'y') {
        break; // start blink example
      }
    }
  }

  Serial.println("Parse blinky example started");
  Serial.println(Parse.getInstallationId());
  Serial.println(Parse.getSessionToken());

  // Turn off LED
  setLedState("off");

  /* start push service */
  if(Parse.startPushService()) {
    Serial.println("\nParse push started\n");
  }
}

void loop() {
  // set LED light in board based on state
  if (ledState == "off") {
    digitalWrite(13, LOW); // turn the LED off (LOW is the voltage level)
  } else if (ledState == "on") {
    digitalWrite(13, HIGH); // turn the LED on (HIGH is the voltage level)
  } else if (ledState == "blink") {
    digitalWrite(13, HIGH);
    delay(500);  // wait half a second
    digitalWrite(13, LOW);
    delay(500);
  }

  // check if any new push message
  if (Parse.pushAvailable()) {
    ParsePush push = Parse.nextPush();

    // print whole JSON body
    Serial.print("New push message content: ");
    Serial.println(push.getJSONBody());

    // set LED state correspondingly
    String command = push.getString("alert");
    if(command == "on" || command == "off" || command == "blink") {
      Serial.print("LED state set to: ");
      Serial.println(command);
      ledState = command;
    } else {
      Serial.println("Unknown message");
    }
    push.close();
  }
}
