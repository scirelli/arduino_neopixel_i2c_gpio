#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_NeoPixel.h>

#define RED        5
#define GREEN      4
#define BLUE       3
#define LED_PIN    6
#define LED_COUNT  8

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_ADS1015 ads1015;  	// Construct an ads1015 
Adafruit_ADS1115 ads1115;	// Construct an ads1115 

unsigned long startTime = 0, elapsed = 0;
unsigned long count = 0;
long firstPixelHue = 0;


void setup() {
  // Setup GPIO
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  
  // Setup i2c
  Serial.begin(9600);
  ads1015.begin(0x48);// Initialize ads1015 at the default address 0x48

  // Setup NeoPixel
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)


  startTime = micros();
}

// the loop function runs over and over again forever
void loop() {
  elapsed = micros() - startTime;
  int16_t results;

  // Do some GPIO
  digitalWrite(RED, LOW);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(GREEN, HIGH); 
  digitalWrite(BLUE, LOW); 
  digitalWrite(BLUE, HIGH);

  //Read some i2c
  results = ads1015.readADC_Differential_0_1();
  results = ads1015.readADC_Differential_0_1();
  results = ads1015.readADC_Differential_0_1();
  results = ads1015.readADC_Differential_0_1();
  
  results = ads1015.readADC_Differential_0_1();
  results = ads1015.readADC_Differential_0_1();
  results = ads1015.readADC_Differential_0_1();
  results = ads1015.readADC_Differential_0_1();

  // Do a theater marquee effect in various colors...
  //theaterChase(strip.Color(127, 127, 127), 0); // White, half brightness
  //theaterChase(strip.Color(127,   0,   0), 0); // Red, half brightness
  //theaterChase(strip.Color(  0,   0, 127), 0); // Blue, half brightness
  //rainbow(0);
  strip.rainbow(firstPixelHue);
  strip.show(); // Update strip with new contents
  if(firstPixelHue > 5*65536) {
    firstPixelHue = 0;
  }
  firstPixelHue += 256;

  // See how many iterations can be done in one second
  if(elapsed >= 1000000){
    Serial.print("Iterations: ");
    Serial.print(count);
     Serial.println("/1 sec");
    count = 0;
    startTime = micros();
  }
  //delay(1);
  count++;
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}