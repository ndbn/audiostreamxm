#include "AudioStreamXm.h"
#include <godot_cpp/variant/utility_functions.hpp>

Ref<AudioStreamPlayback> AudioStreamXm::_instantiate_playback() const
{

  if (this->get_file_path().is_empty())
  {
    UtilityFunctions::push_error("File path is empty");
    return Ref<AudioStreamXmPlayback>();
  }

  Ref<AudioStreamXmPlayback> playback;
  playback.instantiate();
  playback->xm_stream = Ref<AudioStreamXm>(this);

  PackedByteArray xm = FileAccess::get_file_as_bytes(this->get_file_path());

  int res = xm_create_context_safe(&playback->xm_ctx, reinterpret_cast<const char *>(xm.ptr()), xm.size(), this->get_mix_rate());

  if (res != 0) {
    UtilityFunctions::push_error("Cannot create context for file. Code : ", res);

    if (res == 1) UtilityFunctions::push_error("Module is not sane");
    if (res == 2) UtilityFunctions::push_error("Out of memory");
    if (res > 2) UtilityFunctions::push_error("Error unknown");

    return Ref<AudioStreamXmPlayback>();
  }  

  if (playback->xm_ctx == nullptr) {
    UtilityFunctions::push_error("Cannot create context for file");
    return Ref<AudioStreamXmPlayback>();
  }

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

  if ( !xm_stream->get_loop() && _get_loop_count() > 0){
    // Stop play
    active = false;
    return 0;
  }

  size_t num_samples = MIN(max_samples, p_frames);
  size_t buffer_size = num_samples * 2;
  memset(static_buffer, 0, buffer_size * sizeof(float));

  xm_generate_samples(xm_ctx, static_buffer, num_samples);

  for (int i = 0; i < num_samples; i++)
  {
    p_buffer[i] = {static_buffer[2 * i], static_buffer[2 * i + 1]};
  }
  
  return num_samples;
}

void AudioStreamXmPlayback::_start(double p_from_pos)
{  
  active = true;
	//this->_seek(p_from_pos);
	begin_resample();
}

void AudioStreamXmPlayback::_stop()
{
  active = false;  
}

bool AudioStreamXmPlayback::_is_playing() const
{
    return active;
}

int AudioStreamXmPlayback::_get_loop_count() const
{
  if (xm_ctx != nullptr){
    return xm_get_loop_count(xm_ctx);
  }
  return 0;    
}

AudioStreamXmPlayback::AudioStreamXmPlayback()
{
  //xm_stream = nullptr;
  active = false; 
  xm_ctx = nullptr;

}

AudioStreamXmPlayback::~AudioStreamXmPlayback()
{
  if (xm_ctx != nullptr)
  {
    xm_free_context(xm_ctx);
    xm_ctx = nullptr;
  }
}
