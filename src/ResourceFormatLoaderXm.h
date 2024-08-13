#ifndef RFL_XM_HPP
#define RFL_XM_HPP

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource_format_loader.hpp>

#include "AudioStreamXm.h"

using namespace godot;

class ResourceFormatLoaderXm : public ResourceFormatLoader
{
  GDCLASS(ResourceFormatLoaderXm, ResourceFormatLoader);

protected:
  static void _bind_methods() {}

public:
  virtual PackedStringArray _get_recognized_extensions() const override { return {"xm"}; };
  virtual bool _handles_type(const StringName &p_type) const override {
     return ClassDB::is_parent_class(p_type, "AudioStream");
  };
  virtual String _get_resource_type(const String &p_path) const override
  {
    if (p_path.get_extension().to_lower() == "xm") return "AudioStreamXm";    
    return "";
  };
  virtual String _get_resource_script_class(const String &p_path) const override { return "AudioStreamXm"; };
  virtual Variant _load(const String &p_path, const String &p_original_path, bool p_use_sub_threads, int32_t p_cache_mode) const override
  {
    AudioStreamXm *stream = memnew(AudioStreamXm);
    stream->set_file_path(StringName(p_path));

    Ref<AudioStreamXm> xm_stream = Ref<AudioStreamXm>(stream);

    return xm_stream;
  };

  // virtual bool _recognize_path(const String &p_path, const StringName &p_type) const;
  // virtual int64_t _get_resource_uid(const String &p_path) const;
  // virtual PackedStringArray _get_dependencies(const String &p_path, bool p_add_types) const;
  // virtual Error _rename_dependencies(const String &p_path, const Dictionary &p_renames) const;
  // virtual bool _exists(const String &p_path) const;
  // virtual PackedStringArray _get_classes_used(const String &p_path) const;

  // ResourceFormatLoaderXm();
};

#endif // RFL_XM_HPP