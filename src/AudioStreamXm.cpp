#include "AudioStreamXm.h"
#include <godot_cpp/variant/utility_functions.hpp>

Ref<AudioStreamPlayback> AudioStreamXm::_instantiate_playback() const
{
  Ref<AudioStreamXmPlayback> playback;
  playback.instantiate();
  playback->generator = this;
  return playback;
}

AudioStreamXm::AudioStreamXm()
{
  mix_rate = 44100;
  file_path = String();
  loop = false;
}

AudioStreamXm::~AudioStreamXm()
{
 
}

////////////////
int AudioStreamXmPlayback::_mix_resampled(AudioFrame *p_buffer, int p_frames)
{
  if (!active)
  {
    return 0;
  }

  if ( !generator->get_loop() && _get_loop_count() > 0){
    // Stop play
    active = 0;
    return 0;
  }

  size_t num_samples = MIN(max_samples, p_frames);
  size_t buffer_size = num_samples * 2;

  //float *buffer = new float[buffer_size];

  xm_generate_samples(xm_ctx, static_buffer, num_samples);

  for (int i = 0; i < num_samples; i++)
  {
    AudioFrame f = {0.0f, 0.0f};
    p_buffer[i] = {static_buffer[2 * i], static_buffer[2 * i + 1]};
  }

  //delete buffer;

  return num_samples;
}

void AudioStreamXmPlayback::_start(double p_from_pos)
{
  if (xm_ctx != nullptr){
    xm_free_context(xm_ctx);
  }

  if (generator->get_file_path().is_empty())
  {
    UtilityFunctions::push_error("File path is empty");
    return;
  }

  PackedByteArray xm = FileAccess::get_file_as_bytes(generator->get_file_path());

  auto res = xm_create_context_safe(&xm_ctx, reinterpret_cast<const char *>(xm.ptr()), xm.size(), generator->get_mix_rate());

  if (res != 0) {
    UtilityFunctions::push_error("Cannot create context for file. Code : ", res);

    if (res == 1) UtilityFunctions::push_error("Module is not sane");
    if (res == 2) UtilityFunctions::push_error("Out of memory");
    if (res > 2) UtilityFunctions::push_error("Error unknown");

    return;
  }  

  if (xm_ctx == nullptr) {
    UtilityFunctions::push_error("Cannot create context for file");
    return;
  }

  if (active == false)
  {
    begin_resample();
  }
  active = true;
}

AudioStreamXmPlayback::AudioStreamXmPlayback()
{
  generator = nullptr;
  active = false; 
  xm_ctx = nullptr;
}

AudioStreamXmPlayback::~AudioStreamXmPlayback()
{
  if (xm_ctx)
  {
    xm_free_context(this->xm_ctx);
  }
}
