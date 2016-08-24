#include "looper.h"

namespace pixelmap {

Looper* Looper::looper_ = 0;

void Looper::addAnimation(Animation* animation) {
  Log.Info("Adding animation");
  animations_[num_anim_++] = animation;
}

void Looper::addInput(Input* input) {
  Log.Info("Adding input");
  inputs_[num_input_++] = input;
}

void Looper::addVisualization(Visualization* visualization) {
  Log.Info("Adding visualization");
  visualizations_[num_viz_++] = visualization;
}

void Looper::clearAll() {
  Log.Info("Clearing animations, inputs, and visualizations");
  num_anim_ = 0;
  num_input_ = 0;
  num_viz_ = 0;
}

void Looper::loop() {
  Log.Debug("Looping..");

  unsigned long current_time = millis();

  if (current_time > next_update_tick_) {
    next_update_tick_ = current_time + update_millis_per_tick_;
    update_();
  }

  if (current_time > next_draw_tick_) {
    next_draw_tick_ = current_time + draw_millis_per_tick_;
    draw_(draw_millis_per_tick_);
  }

  FastLED.show();
}

void Looper::update_() {
  Log.Debug("Updating..");

  for (int i = 0; i < num_anim_; i++) {
    animations_[i]->update();
  }

  for (int i = 0; i < num_input_; i++) {
    inputs_[i]->update();
  }

  for (int i = 0; i < num_viz_; i++) {
    visualizations_[i]->update();
  }
}

void Looper::draw_(float interp) {
  Log.Debug("Drawing..");

  for (int i = 0; i < num_anim_; i++) {
    animations_[i]->draw(interp);
  }
}

} // end namespace pixelmap
