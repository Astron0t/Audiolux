#include <pixl.h>
#include <FastLED.h>
#include <Logging.h>

// Uncomment to turn off assertions
#define NDEBUG

// Weird thing I had to do to get the Logging library working with teensy
extern "C"{
  int _getpid(){ return -1;}
  int _kill(int pid, int sig){ return -1; }
  int _write(){return -1;}
}

#define LOGLEVEL LOG_LEVEL_INFOS

using namespace pixl;

Input* input;

// Triangle
LEDStrip triangle_strip = LEDStrip(165);
int triangle_points[] = {37, 0, 19};
int triangle_edges[] = {18, 19, 18};
LEDStrip* triangle_strips[] = {&triangle_strip};
LEDs triangle_led = LEDs(3, triangle_strips, triangle_points, triangle_edges);
LEDs* triangle_leds[] = {&triangle_led};
TriangleAnimation* triangle_anim;
CircleAnimation* circle_anim;

// Circle
LEDStrip circle_strip = LEDStrip(150);
LEDs circle_led = LEDs(circle_strip, 0, 150);
LEDs* circle_leds[] = {&circle_led};

Visualization* viz;

// Audio shield setup
AudioInputI2S audio;
//AudioAnalyzePeak peak;
//AudioAnalyzeNoteFrequency note;
//AudioConnection patchCord1(audio, peak);
//AudioConnection patchCord2(audio, note);
AudioControlSGTL5000 audioShield;
AudioAnalyzeFFT1024 fft;
AudioConnection patchCord1(audio, 0, fft, 0);

void setup() {
  Log.Init(LOGLEVEL, 9600);
  delay(1000);
  Log.Info("Starting setup()\n");
  Serial.flush();
  delay(1000);

  AudioMemory(12);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.lineInLevel(15);
  //audioShield.inputSelect(AUDIO_INPUT_MIC);
  //audioShield.micGain(100);
  //note.begin(.99);

  fft.windowFunction(AudioWindowHanning1024);

  //input = new AudioShieldInput(&peak, &note);
  input = new FFTInput(&fft);

  viz = new RippleVisualization(input, 50, 1, true);
  triangle_anim = new TriangleAnimation(viz, triangle_leds, 1);
  circle_anim = new CircleAnimation(viz, circle_leds, 1);

  triangle_anim->init(1.0);
  circle_anim->init(1.0);

  FastLED.addLeds<WS2811, 2, GRB>(triangle_strip.leds, 165);
  FastLED.addLeds<WS2811, 2, GRB>(circle_strip.leds, 150);
  FastLED.setBrightness(255);

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(triangle_anim);
  looper->addAnimation(circle_anim);
  looper->setUpdatesPerSecond(30);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  AudioNoInterrupts();
  Looper::instance()->loop();
  AudioInterrupts();
}
