// https://github.com/datulab/arduino-keyboard
// https://www.youtube.com/watch?v=Iq3oY91x9Vk

#include <Keyboard.h>

const byte inputs[] = { 21, 20, 19, 18, 15, 14, 16, 10 };  // Columns
const byte outputs[] = { 1, 9, 0, 8, 2, 7, 3, 6, 4, 5 };   // Rows
#define inCount sizeof(inputs) / sizeof(inputs[0])
#define outCount sizeof(outputs) / sizeof(outputs[0])

const int keymap[outCount][inCount] = {
  // 21   20   19   18   15   14   16   10
  { '`', '\\', '=', '-', '0', '9', '8', '7' },                             // 1
  { NULL, '6', '5', '4', '3', '2', '1', KEY_ESC },                          // 9
  { KEY_BACKSPACE, NULL, ']', '[', 'p', 'o', 'i', 'u' },                    // 0
  { NULL, 'y', 't', 'r', 'e', 'w', 'q', KEY_TAB },                          // 8
  { KEY_RETURN, NULL, NULL, '\'', ';', 'l', 'k', 'j' },                      // 2
  { NULL, 'h', 'g', 'f', 'd', 's', 'a', KEY_CAPS_LOCK },                    // 7
  { KEY_KP_ENTER, NULL, KEY_RIGHT_SHIFT, '/', '.', ',', 'm', NULL },       // 3
  { NULL, 'b', 'v', 'c', 'x', 'z', NULL, KEY_LEFT_SHIFT },                   // 6
  { KEY_LEFT_GUI, NULL, KEY_RIGHT_GUI, KEY_RIGHT_ALT, NULL, NULL, NULL, 32 },  // 4
  { NULL, NULL, NULL, NULL, KEY_LEFT_ALT, NULL, KEY_LEFT_GUI, KEY_LEFT_CTRL },  // 5
};

// q, x, s, d, f = broken

// Press speed and delay configuration
int postOutputToLowDelayMicroseconds = 5;
int postOutputToHighDelayMicroseconds = 500;

int repeatsBeforeSecondPress = 90;  // 350;  // Number of repeats a switch encounters when a key is held down before the second press is lodged
int repeatPressDelay = 6;           // 15;           // Number of repeats a switch encounters between each press that is lodged

int currentKeyRepeatCount[outCount][inCount] = { 0 };
bool firstKeyPressFinished[outCount][inCount] = { false };


void setup() {
  for (int i = 0; i < inCount; i++) {
    pinMode(inputs[i], INPUT_PULLUP);
  }

  for (int i = 0; i < outCount; i++) {
    pinMode(outputs[i], OUTPUT);
    digitalWrite(outputs[i], HIGH);
  }

  Serial.begin(9600);
  Serial.println("Connected");
  Keyboard.begin();
}

void loop() {
  for (int i = 0; i < outCount; i++) {
    digitalWrite(outputs[i], LOW);
    delayMicroseconds(postOutputToLowDelayMicroseconds);

    for (int j = 0; j < inCount; j++) {

      // Key pressed, if statement entered numerous times for key held down
      if (digitalRead(inputs[j]) == LOW) {
        // Serial.print("col: ");  // prints every time :(
        // Serial.print(inputs[j]);
        // Serial.print(" row: ");
        // Serial.print(outputs[i]);
        // Serial.print(" key: ");
        // Serial.println(keymap[i][j]);

        // 3 cases
        // first press
        if (currentKeyRepeatCount[i][j] == 0) {
          Serial.println(keymap[i][j]);
          Keyboard.write(keymap[i][j]);
        }
        // time for second press - spam modeI
        else if (firstKeyPressFinished[i][j] && currentKeyRepeatCount[i][j] > repeatPressDelay) {
          Serial.println(keymap[i][j]);
          Keyboard.write(keymap[i][j]);
          currentKeyRepeatCount[i][j] = 1;  // start repeat count again
        }
        // ready for spam mode
        else if (currentKeyRepeatCount[i][j] > repeatsBeforeSecondPress) {
          firstKeyPressFinished[i][j] = true;  // ready for spam
        }
        currentKeyRepeatCount[i][j]++;
      }
      // key recently released so reset
      else if (currentKeyRepeatCount[i][j] != 0) {
        currentKeyRepeatCount[i][j] = 0;
        firstKeyPressFinished[i][j] = false;
      }
    }
    digitalWrite(outputs[i], HIGH);
    delayMicroseconds(postOutputToHighDelayMicroseconds);
  }
}



// PRINT keymap
// for (int i = 0; i < outCount; i++) {
//   for (int j = 0; j < inCount; j++) {
//     // Serial.print(keymap[i][j]);
//     Serial.print(firstKeyPressFinished[i][j]);
//     Serial.print(", ");
//   }
//   Serial.print("\n");
// }
// Serial.print("\n\n\n");
