#include <FastLED.h>
#include <IRremote.hpp>

// --- 1. LED MATRIX SETTINGS ---
#define LED_PIN         D0   // Data pin connected to D0
#define NUM_LEDS        64  // Total number of LEDs (8x8 = 64)
#define MATRIX_WIDTH    8
#define MATRIX_HEIGHT   8
#define BRIGHTNESS      80  // Brightness (0-255). Start low!
#define LED_TYPE        WS2812
#define COLOR_ORDER     GRB
CRGB leds[NUM_LEDS];

// --- 2. IR RECEIVER SETTINGS ---
#define IR_RECEIVE_PIN  D1   // Signal pin connected to D1

// --- 3. STATE VARIABLE ---
int currentAnimation = 0; // This variable stores which animation is active


// --- NEW IMAGE PIXEL DATA (8x8) ---

// Heart ❤️ (For Button 0)
const CRGB heart[NUM_LEDS] = {
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Red,   CRGB::Red,   CRGB::Black, CRGB::Black, CRGB::Red,   CRGB::Red,   CRGB::Black,
  CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,
  CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,
  CRGB::Black, CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Red,   CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Red,   CRGB::Red,   CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};

// Umbrella ☂️ (For Button 1)
const CRGB umbrella[NUM_LEDS] = {
  CRGB::Black, CRGB::Black, CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Black,
  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gray,  CRGB::Gray,  CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gray,  CRGB::Gray,  CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gray,  CRGB::Gray,  CRGB::Gray,  CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Gray,  CRGB::Gray,  CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};

// Star ⭐ (For Button 2)
const CRGB star[NUM_LEDS] = {
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Yellow,CRGB::Yellow,CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Black, CRGB::Black,
  CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,
  CRGB::Black, CRGB::Black, CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Yellow,CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Yellow,CRGB::Yellow,CRGB::Black, CRGB::Black, CRGB::Yellow,CRGB::Yellow,CRGB::Black,
  CRGB::Black, CRGB::Yellow,CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Yellow,CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};

// Cloud ☁️ (For Button 3)
const CRGB cloud[NUM_LEDS] = {
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::White, CRGB::White, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::White, CRGB::White, CRGB::White, CRGB::White, CRGB::White, CRGB::Black, CRGB::Black,
  CRGB::White, CRGB::White, CRGB::White, CRGB::White, CRGB::White, CRGB::White, CRGB::White, CRGB::Black,
  CRGB::Black, CRGB::White, CRGB::White, CRGB::White, CRGB::White, CRGB::White, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};

// Half Moon 🌙 (For Button 4) - Now White and more crescent-shaped
const CRGB halfMoon[NUM_LEDS] = {
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::White, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::White, CRGB::White, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::White, CRGB::White, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::White, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::White, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::White, CRGB::White, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::White, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};

// Leaf 🍃 (For Button 5)
const CRGB leaf[NUM_LEDS] = {
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Green, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Black, CRGB::Black,
  CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Green, CRGB::Green, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::SaddleBrown, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::SaddleBrown, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};

// Rain Drops 💧 (For Button 6)
const CRGB rainDrops[NUM_LEDS] = {
  CRGB::Black, CRGB::Blue,  CRGB::Black, CRGB::Black, CRGB::Blue,  CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Blue,  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Blue,  CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Blue,  CRGB::Black, CRGB::Black, CRGB::Blue,  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Blue,  CRGB::Black, CRGB::Blue,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};


void setup() {
  Serial.begin(115200);
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  
  Serial.println("Multi Image Controller Ready!");
  animationOff(); // Start with LEDs off
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.print("IR Code: 0x");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    // --- IR BUTTON LOGIC ---
    switch (IrReceiver.decodedIRData.decodedRawData) {
      
      // Use the code for your Power button
      case 0xFFA25D: 
        Serial.println("Command: OFF");
        currentAnimation = 0;
        break;

      // --- REPLACE THESE PLACEHOLDER CODES WITH YOURS ---
      case 0xE916FF00: // Button '0'
        Serial.println("Command: Heart");
        currentAnimation = 1;
        break;

      case 0xF30CFF00: // Button '1'
        Serial.println("Command: Umbrella");
        currentAnimation = 2;
        break;

      case 0xE718FF00: // Button '2'
        Serial.println("Command: Star");
        currentAnimation = 3;
        break;
        
      case 0xA15EFF00: // Button '3'
        Serial.println("Command: Cloud");
        currentAnimation = 4;
        break;
        
      case 0xF708FF00: // Button '4'
        Serial.println("Command: Half Moon");
        currentAnimation = 5;
        break;

      case 0xE31CFF00: // Button '5'
        Serial.println("Command: Leaf");
        currentAnimation = 6;
        break;
      
      case 0xAD52FF00: // Button '6' (Example placeholder)
        Serial.println("Command: Rain Drops");
        currentAnimation = 7;
        break;
    }
    
    IrReceiver.resume(); 
  }

  // Run the currently selected animation
  switch (currentAnimation) {
    case 0: animationOff(); break;
    case 1: animationHeart(); break;
    case 2: animationUmbrella(); break;
    case 3: animationStar(); break;
    case 4: animationCloud(); break;
    case 5: animationHalfMoon(); break;
    case 6: animationLeaf(); break;
    case 7: animationRainDrops(); break;
  }

  FastLED.show();
  delay(30); 
}


// --- ANIMATION FUNCTIONS ---

void animationOff() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void animationHeart() {
  memcpy(leds, heart, sizeof(heart));
}

void animationUmbrella() {
  memcpy(leds, umbrella, sizeof(umbrella));
}

void animationStar() {
  memcpy(leds, star, sizeof(star));
}

void animationCloud() {
  memcpy(leds, cloud, sizeof(cloud));
}

void animationHalfMoon() {
  memcpy(leds, halfMoon, sizeof(halfMoon));
}

void animationLeaf() {
  memcpy(leds, leaf, sizeof(leaf));
}

void animationRainDrops() {
  memcpy(leds, rainDrops, sizeof(rainDrops));
}