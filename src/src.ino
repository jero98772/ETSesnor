#include <FastLED.h>
#define LED_PIN D3
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define amarillo CRGB::Yellow
#define negro CRGB::Black
#define rojo CRGB::Red
#define azul CRGB::Blue
#define morado CRGB::Purple
#define naranja CRGB::OrangeRed
#define verde CRGB::Green
#define aguamarina CRGB::Cyan
#define rosado CRGB::Fuchsia
#define verdedos CRGB::LimeGreen
#define raro CRGB::DarkOrchid
#define rarodos CRGB::DeepPink

const uint8_t matrixWidth = 8;
const uint8_t matrixHeight = 8;
#define NUM_LEDS (matrixWidth * matrixHeight)

#define ESCENAS 3
#define FRAMES 1

int counter=0;
int BRIGHTNESS = 20;

CRGB leds[matrixWidth * matrixHeight];

int animator = 0;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

int inputMin = -2;
int inputMax = 110;

int micMaxValue=0;
int part=0;

int rangeMin = 20;

int rangeMiddleMin = 21;
int rangeMiddleMax = 60;

int rangleMax = 61;

void setup() {
  Serial.begin(115200);
  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

CRGB matrix[ESCENAS][FRAMES][matrixHeight][matrixWidth] = {
  {
    {
    {negro, negro, negro,negro, negro, negro, negro,negro},
    {negro, negro, verde,negro, negro, negro, negro,negro},
    {negro, negro, verde,negro, negro, verde, negro,negro},
    {negro, negro, verde,negro, negro, negro, negro,negro},
    {verde, negro, negro,negro, negro, negro, negro,verde},
    {negro, verde, negro,negro, negro, negro, verde,negro},
    {negro, negro, verde,verde, verde, verde, negro,negro},
    {negro, negro, negro,negro, negro, negro, negro,negro},
    },
  },
  {
    {
    {negro, negro, negro,negro, negro, negro, negro,negro},
    {negro, negro, negro,negro, negro, negro, negro,negro},
    {negro, amarillo, amarillo,negro, negro, amarillo, amarillo,negro},
    {negro, negro, negro,negro, negro, negro, negro,negro},
    {negro, negro, negro,negro, negro, negro, negro,negro},
    {negro, negro, negro,negro, negro, negro, negro,negro},
    {negro, amarillo, amarillo,amarillo, amarillo, amarillo, amarillo,negro},
    {negro, negro, negro,negro, negro, negro, negro,negro},
    },
  },
  {
    {
    {negro, negro, negro,negro, negro, negro, negro,negro},
    {negro, rojo, negro,rojo, rojo, negro, rojo,negro},
    {negro, negro, rojo,negro, negro, rojo, negro,negro},
    {negro, rojo, negro,rojo, rojo, negro, rojo,negro},
    {negro, negro, negro,negro, negro, negro, negro,negro},
    {negro, negro, rojo,rojo, rojo, rojo, negro,negro},
    {negro, rojo, negro,negro, negro, negro, rojo,negro},
    {negro, negro, negro,negro, negro, negro, negro,negro},
  },
 },
};


void cambioLed(int c){
  int primo=103;
  int primo2=29;
  if(micMaxValue<c){
    micMaxValue=800;
    part=micMaxValue/ESCENAS;
   
    }
    counter=0;
    for (int i = 0; i < matrixHeight; i++) {
    for (int j = 0; j < matrixWidth; j++) {
      counter=counter+1;
      if(c<part*1){
        matrix[0][0][j][i]=CRGB(0,128,(primo*counter+c)%255 );
        }
     else if (c>part*1&&c<part*2) {
        matrix[0][0][j][i]=CRGB((primo2*counter+c)%220,0,(primo2*counter+c)%255);
      }else if (c>part*2) {
       matrix[0][0][j][i]=CRGB((primo*counter+c)%255,(primo2*counter+c)%10,0 );
        }
      }
    }
    
    for (int i = 0; i < matrixHeight; i++) {
      for (int j = 0; j < matrixWidth; j++) {
        leds[i * matrixWidth + j] = matrix[0][0][i][j];
      FastLED.show();
     }
    }
}
void loop() {
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(0);
    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample;
      }
      else if (sample < signalMin) {
        signalMin = sample;
      }
    }
  }

  peakToPeak = signalMax - signalMin;
  int changeAnimation = map(peakToPeak, inputMin, inputMax, 0, 100);
  FastLED.setBrightness(80);
  cambioLed(changeAnimation);
  delay(100-((peakToPeak/600)*100));
  animator = (animator + 1) % 100;
  Serial.print("map value: ");
  Serial.println(changeAnimation);
  Serial.print("peak value: ");
  Serial.println(peakToPeak);
}
/*metas
ruido
caritas con animacion 
*/
