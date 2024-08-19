#ifndef AUDIO_STREAM_XM_HPP
#define AUDIO_STREAM_XM_HPP

#include "libxm.hpp"
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_playback_resampled.hpp>


using namespace godot;

class AudioStreamXm : public AudioStream
{
  GDCLASS(AudioStreamXm, AudioStream);
  

  float mix_rate;
  String file_path;
  bool loop;

protected:
  static void _bind_methods()
  {
    ClassDB::bind_method(D_METHOD("set_mix_rate", "hz"), &AudioStreamXm::set_mix_rate);
    ClassDB::bind_method(D_METHOD("get_mix_rate"), &AudioStreamXm::get_mix_rate);

    ClassDB::bind_method(D_METHOD("set_file_path"), &AudioStreamXm::set_file_path);
    ClassDB::bind_method(D_METHOD("get_file_path"), &AudioStreamXm::get_file_path);

    ClassDB::bind_method(D_METHOD("set_loop"), &AudioStreamXm::set_loop);
    ClassDB::bind_method(D_METHOD("get_loop"), &AudioStreamXm::get_loop);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mix_rate", PROPERTY_HINT_RANGE, "20,192000,1,suffix:Hz"), "set_mix_rate", "get_mix_rate");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "file_path", PROPERTY_HINT_FILE, "*.xm"), "set_file_path", "get_file_path");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "get_loop");
  }

public:
  void set_mix_rate(float p_mix_rate) { mix_rate = p_mix_rate; };
  float get_mix_rate() const { return mix_rate; };

  void set_file_path(const String& p_file_path) { file_path = p_file_path; };
  String get_file_path() const { return file_path; };

  void set_loop(const bool p_loop) { loop = p_loop; };
  bool get_loop() const { return loop; };

  Ref<AudioStreamPlayback> _instantiate_playback() const override;
  String _get_stream_name() const override { return ""; };

  double _get_length() const { return 0; };
  bool _is_monophonic() const override { return false; };
  AudioStreamXm();
  ~AudioStreamXm();
};


const int max_samples = 1024;

class AudioStreamXmPlayback : public AudioStreamPlaybackResampled
{
  GDCLASS(AudioStreamXmPlayback, AudioStreamPlaybackResampled);
  friend class AudioStreamXm;

  bool active;
  Ref<AudioStreamXm> xm_stream = nullptr;
  xm_context_t *xm_ctx = nullptr;

  float static_buffer[max_samples * 2];

protected:
  static void _bind_methods()
  {
  };

public:
  virtual int _mix_resampled(AudioFrame *p_buffer, int p_frames) override;
  virtual double _get_stream_sampling_rate() const override { return xm_stream->get_mix_rate(); };

  virtual void _start(double p_from_pos = 0.0) override;
  virtual void _stop() override;
  virtual bool _is_playing() const override;
  virtual int _get_loop_count() const override; // times it looped


  AudioStreamXmPlayback();
  ~AudioStreamXmPlayback();
};

#endif // AUDIO_STREAM_XM_HPP