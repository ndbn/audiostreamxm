#ifndef RFL_XM_HPP
#define RFL_XM_HPP

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource_format_loader.hpp>
#include <godot_cpp/classes/resource_format_saver.hpp>

#include "AudioStreamXm.h"

using namespace godot;

class ResourceFormatLoaderXm : public ResourceFormatLoader
{
  GDCLASS(ResourceFormatLoaderXm, ResourceFormatLoader);

protected:
  static void _bind_methods() {}

public:
  virtual PackedStringArray _get_recognized_extensions() const override { 
    return {"xm"}; 
  };

  virtual bool _handles_type(const StringName &p_type) const override {
     return ClassDB::is_parent_class(p_type, "AudioStream");
  };

  virtual String _get_resource_type(const String &p_path) const override
  {
    return "AudioStreamXm";
  };

  virtual String _get_resource_script_class(const String &p_path) const override { 
    return "AudioStreamXm"; 
  };
  
  virtual Variant _load(const String &p_path, const String &p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const override
  {
    Ref<AudioStreamXm> xm_stream;
    xm_stream.instantiate();
    xm_stream->set_file_path(StringName(p_path));
    return xm_stream;
  };
};

#endif // RFL_XM_HPP