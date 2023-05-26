#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

// Configuration defines.

#define SERIAL_BAUD_RATE 115200 // Baud rate for serial prints.
#define BUTTON_PIN 3 // Arduino PIN for the button to start the commands.
#define CLICK_DURATION 200 // How long a command key is held.

// The total delay between commands (including reboot) is
// delay + 2 * LED_BLINK_DELAY
#define COMMAND_SEQUENCE_START_DELAY 500 // Delay after SYSRQ key is pressed.
#define COMMAND_DELAY 1600 // Delay after a command, except reboot.
#define REBOOT_COMMAND_DELAY 2600 // Delay after the final reboot command.

#define LED_BLINK_DELAY 200 // Dealy between the blinking LED (twice per blink).

#define COMMAND_SEQUENCE_LENGTH 7 // Number of commands in the sequence
// (SYSRQ + REISUB).

// Struct definitions.
struct Command {
  uint8_t key;
  unsigned long pressDelay;
};

// Function definitions.
void clickKey(const Command* const cmd);
void blink(unsigned long blinkDelay);

// Report IDs.
enum
{
  RID_KEYBOARD_HOLD = 1,
  RID_KEYBOARD_COMMAND, 
};

// HID report descriptor using TinyUSB's template.
uint8_t const descHIDReport[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD_HOLD)),
  TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD_COMMAND)),
};

Adafruit_USBD_HID usbHID;
bool keyHeld;

Command cmdSequence[COMMAND_SEQUENCE_LENGTH] = {
  {HID_KEY_PRINT_SCREEN, COMMAND_SEQUENCE_START_DELAY},
  {HID_KEY_R, COMMAND_DELAY},
  {HID_KEY_E, COMMAND_DELAY},
  {HID_KEY_I, COMMAND_DELAY},
  {HID_KEY_S, COMMAND_DELAY},
  {HID_KEY_U, REBOOT_COMMAND_DELAY},
  {HID_KEY_B, 0}
};

uint8_t cmdIndex;

void setup() {
  // Set up button, pullup.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // Set up built in LED.
  pinMode(LED_BUILTIN, OUTPUT);

  usbHID.setPollInterval(2);
  usbHID.setReportDescriptor(descHIDReport, sizeof(descHIDReport));

  usbHID.begin();

  Serial.begin(SERIAL_BAUD_RATE);

  keyHeld = false;
  cmdIndex = 0;

  // Wait until device mounted.
  while(!USBDevice.mounted()) {
    delay(1);
  }

  Serial.println("Emergency Reboot Ready");
}

void loop() {
  // LOW == button pressed.
  if (digitalRead(BUTTON_PIN) == LOW && !keyHeld) {
    // Remote wakeup.
    if (USBDevice.suspended()) {
      // Wake up host if we are in suspend mode
      // and REMOTE_WAKEUP feature is enabled by host.
      USBDevice.remoteWakeup();
    }

    if (usbHID.ready()) {
      Serial.println("Starting key sequence");
      uint8_t keycode[6] = {HID_KEY_NONE};
      
      keycode[0] = HID_KEY_ALT_LEFT;
      usbHID.keyboardReport(RID_KEYBOARD_HOLD, HID_KEY_NONE, keycode);
      delay(COMMAND_SEQUENCE_START_DELAY);
      
      keyHeld = true;
    }
  } else if (keyHeld) {
    if (cmdIndex < COMMAND_SEQUENCE_LENGTH) {
      clickKey(&cmdSequence[cmdIndex]);
      blink(LED_BLINK_DELAY);

      cmdIndex++;
    } else {
      // Fast blink to indicate end of command sequence.
      blink(LED_BLINK_DELAY);
      blink(LED_BLINK_DELAY);
      blink(LED_BLINK_DELAY);
      blink(LED_BLINK_DELAY);

      digitalWrite(LED_BUILTIN, LOW); // Leave LED on when finished.

      usbHID.keyboardRelease(RID_KEYBOARD_HOLD);

      keyHeld = false;
      cmdIndex = 0;

      Serial.println("Key sequence finished");
    }
  }
}

void clickKey(const Command* const cmd) {
  uint8_t keycode[6] = {HID_KEY_NONE};

  keycode[0] = cmd->key;
  
  usbHID.keyboardReport(RID_KEYBOARD_COMMAND, HID_KEY_NONE, keycode);
  delay(CLICK_DURATION);
  usbHID.keyboardRelease(RID_KEYBOARD_COMMAND);
  delay(cmd->pressDelay);
}

void blink(unsigned long blinkDelay) {
  digitalWrite(LED_BUILTIN, LOW);
  delay(blinkDelay);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(blinkDelay);
}
