#include "sztehlo-buttons.h"
#include "sztehlo-video.h"
#include "sztehlo-sound.h"
#include "sztehlo-eeprom.h"


// Standard Arduino headers, use them if needed
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>  // needed for the additional interrupt


// 'smiley', 32x32px
const unsigned char bitmap_smiley[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x60, 0x70, 0x30, 0x30, 0x38, 0x18, 0x18,
  0x18, 0x18, 0x38, 0x30, 0x30, 0x70, 0x60, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xe0, 0xfc, 0x3e, 0x07, 0x01, 0x00, 0x00, 0x30, 0x78, 0x78, 0x30, 0x00, 0x00,
  0x00, 0x00, 0x30, 0x78, 0x78, 0x30, 0x00, 0x00, 0x01, 0x07, 0x3e, 0xfc, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x07, 0x3f, 0x7c, 0xe0, 0x80, 0x00, 0x1c, 0x38, 0x70, 0x60, 0xe0, 0xc0, 0xc0,
  0xc0, 0xc0, 0xe0, 0x60, 0x70, 0x38, 0x1c, 0x00, 0x80, 0xe0, 0x7c, 0x3f, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x06, 0x0e, 0x0c, 0x0c, 0x1c, 0x18, 0x18,
  0x18, 0x18, 0x1c, 0x0c, 0x0c, 0x0e, 0x06, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


// Lawrence of Arabia, main theme
static const uint16_t lawrence_notes[] PROGMEM = {
  NOTE_5_D, NOTE_4_A, NOTE_4_FS, NOTE_4_G, NOTE_4_AS, NOTE_5_CS,
  NOTE_5_D, NOTE_4_A, NOTE_4_FS, NOTE_4_G, NOTE_4_DS, NOTE_4_CS,
  NOTE_4_D, NOTE_4_A, NOTE_4_G, NOTE_5_C, NOTE_4_AS, NOTE_4_A, NOTE_4_B, NOTE_5_CS
};

static const uint8_t lawrence_note_lengths[] PROGMEM = {
  8, 12, 4, 2, 2, 2,
  8, 12, 4, 2, 2, 2,
  8, 8, 8, 8, 8, 16, 2, 2
};


uint8_t sound_enabled = 1;
uint8_t smiley_x_offset = 0;
uint8_t smiley_speed = 1;


void setup() {
  soundInit();
  videoInit();
  buttonsInit();

  videoFillScreen(BLACK_SCREEN_BYTE);
  videoPrintText(10, 0, "button_state: ");
}


void loop() {

  videoDrawBitmap(smiley_x_offset, 4, 32, 4, bitmap_smiley);
  delay(50);
  buttonsReadAllInputs();

  videoPrintText(100, 0, "   ");
  videoPrintNumber(100, 0, button_state);
  videoPrintBits(63, 2, button_state);


  if (BUTTON_PUSHED_ACTION_1) {
    sound_enabled = !sound_enabled;
  }

  if (BUTTON_PUSHED_ACTION_2) {
    videoInvert();
  }

  if (BUTTON_PUSHED_UP) {
    if(smiley_speed < 10)
      smiley_speed++;
  }

  if (BUTTON_PUSHED_DOWN) {
    if(smiley_speed > 0)
      smiley_speed--;
  }

  if (sound_enabled) {
    if (!soundIsPlaying()) soundPlayTones(lawrence_notes, lawrence_note_lengths, 20);
    else soundUpdateTones();
  } else {
    if (soundIsPlaying()) soundStop();
  }

  videoFillRect(smiley_x_offset, 4, 32, 4, BLACK_SCREEN_BYTE);


  if (BUTTON_PUSHED_LEFT) {
    smiley_x_offset = (smiley_x_offset - 16) % 128;
  }

  if (BUTTON_PUSHED_RIGHT) {
    smiley_x_offset = (smiley_x_offset + 16) % 128;
  }

  smiley_x_offset = (smiley_x_offset + smiley_speed) % 128;
}
