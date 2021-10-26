#include "MPU9250.h"
#include "Vector2D.h"
#include <Adafruit_NeoPixel.h>

MPU9250 mpu;
Vector2D magnet;

//----- COLOURS -----//
uint32_t YELLOW = 65535/6;
uint32_t GREEN = 65535/3;
uint32_t CYAN = 65535/2;
uint32_t BLUE = 65535/3*2;
uint32_t MAGENTA = 65535/6*5;
uint32_t RED = 65535;
uint8_t target = 0;

//----- SET DIFFICULTY -----//
#define DELAY_DIFFICULTY 100
#define MIN_LOAD_DIFFICULTY 50
#define MAX_LOAD_DIFFICULTY 100
#define PIXEL_RED_TARGET_COLOR true

//Change RED_TARGET_COLOR WITH OTHER OPTION

#define RED_TARGET_COLOR RED
#if PIXEL_RED_TARGET_COLOR
  #define TARGET_COLOR RED_TARGET_COLOR
#else
  #define TARGET_COLOR GREEN
#endif

//Magnetometer Inverted

//----- Neopixel Configuration -----//
#define NUM_LEDS_PER_STRIP 40
#define PIXEL_PIN 2 //Defines the Datapin for NeoPixel
#define BRIGHTNESS 50

class Neopixel_Controller : public Adafruit_NeoPixel{
  public:
    uint8_t oldLED = 0;
    uint8_t currentLED = 0;
    uint8_t diff_LED = 0;
    uint32_t timestamp;
    uint16_t duration = 3;
    
    uint32_t currentColor = 65536/3;
    long firstPixelHue;
    uint16_t speed = 400;
    
    long fromColor = GREEN;
    long toColor = RED;
    int16_t colorOffset = -4000;
    
    Neopixel_Controller ( uint16_t n, uint16_t pin , neoPixelType type=NEO_GRB+NEO_KHZ800) : Adafruit_NeoPixel( n, pin, type ){

    }

    void colors( ){
          long color_step = abs( toColor - fromColor )/( this->numPixels()/2 ) ;
          long pixelHue = ( fromColor + color_step*diff_LED + colorOffset ) % (65535);

          setPixelColor( (currentLED+diff_LED) % ( this->numPixels() ), gamma32( ColorHSV(pixelHue)));
          
          if( ( currentLED - diff_LED ) < 0 ){
            setPixelColor( ( this->numPixels()+(currentLED-diff_LED)) % ( this->numPixels() ), gamma32( ColorHSV(pixelHue)));
          }else{
            setPixelColor( (currentLED-diff_LED) % ( this->numPixels() ), gamma32( ColorHSV(pixelHue)));
          }
          
          diff_LED ++;
          if( diff_LED > ( NUM_LEDS_PER_STRIP/2 ) ){ 
            diff_LED = 0;
          }
          show();
    }
    
    void pixelRun( int pixel ){
      if( currentLED != pixel ){
        currentLED = pixel;
        Serial.print("Pixel: ");Serial.print(pixel);Serial.print("\t current: ");Serial.print(currentLED);Serial.print("\t old: ");Serial.println(oldLED);
        setPixelColor( currentLED , currentColor  );
        setPixelColor( oldLED,  Color( 0, 0 ,0 )    );
        show();
        oldLED = currentLED;
      }
    }

    void update(){
      if ( millis() - timestamp > duration ) {
          timestamp = millis();
      }
    }
};

Neopixel_Controller strip = Neopixel_Controller(NUM_LEDS_PER_STRIP, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
void setup() {
    Serial.begin(115200);
    Wire.begin();
    strip.begin();
    strip.setBrightness ( BRIGHTNESS );
    delay(2000);

    if (!mpu.setup(0x68)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }
    // Set RandomSeed from analog Read
    randomSeed(analogRead(0));
    loadTarget();
    
}

void loop() {
    if (mpu.update()) {
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 25) {
            //print_mag();
            vectorize();
            pixel_angle();
            prev_ms = millis();
        }
    }
}

void calibrate(){
    // calibrate anytime you want to
    Serial.println("Accel Gyro calibration will start in 5sec.");
    Serial.println("Please leave the device still on the flat plane.");
    mpu.verbose(true);
    delay(5000);
    mpu.calibrateAccelGyro();

    Serial.println("Mag calibration will start in 5sec.");
    Serial.println("Please Wave device in a figure eight until done.");
    delay(5000);
    mpu.calibrateMag();

    print_calibration();
    mpu.verbose(false);
}

void vectorize(){
    //Magnetometer is inverted
    magnet.x = -mpu.getMagX();
    magnet.y = mpu.getMagY();
}

void print_mag() {
    Serial.print("mX ");
    Serial.print(mpu.getMagX(), 2);
    Serial.print(", mY ");
    Serial.print(mpu.getMagY(), 2);
    Serial.print(", mZ ");
    Serial.println(mpu.getMagZ(), 2);
}

void print_magnitude() {
    Serial.println(magnet.Magnitude(), 2);
}

void pixel_angle() {
    int pixel = map( (int)magnet.Angle(), -180, 180, 0, 40);
    strip.pixelRun( pixel );
    if ( pixel == target ) {
      Serial.println("WIN");
      loadTarget();
    }
}

void loadTarget(){
  target = random( NUM_LEDS_PER_STRIP );
  Serial.print("Target : ");Serial.println( target );
  int loadloop = random( MIN_LOAD_DIFFICULTY, MAX_LOAD_DIFFICULTY );
  
  for ( int i = 0 ; i < loadloop ; i++ ){
    int randomLED = random( 0, NUM_LEDS_PER_STRIP );
    strip.setPixelColor( randomLED, strip.gamma32( strip.ColorHSV(GREEN))  );
    strip.show();
    delay( random( DELAY_DIFFICULTY ) );
    strip.setPixelColor( randomLED, strip.Color(0,0,0) );
    strip.show();
  }

  strip.setPixelColor( target, strip.gamma32( strip.ColorHSV( TARGET_COLOR ))  );
  strip.show();
  delay(10);
  strip.setPixelColor( target,  strip.Color(0,0,0)   );
  strip.show();
}

void print_calibration() {
    Serial.println("< calibration parameters >");
    Serial.println("accel bias [g]: ");
    Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.println();
    Serial.println("gyro bias [deg/s]: ");
    Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.println();
    Serial.println("mag bias [mG]: ");
    Serial.print(mpu.getMagBiasX());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasY());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasZ());
    Serial.println();
    Serial.println("mag scale []: ");
    Serial.print(mpu.getMagScaleX());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());
    Serial.println();
}
