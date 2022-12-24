#include "akila/core/core.hpp"

#include "akila/core/math/math.hpp"

#include "akila/core/memory/ptr.hpp"
#include "akila/core/memory/ref.hpp"

#include "akila/core/ecs/ecs.hpp"

#include "akila/core/resource/resources.hpp"
#include "akila/core/resource/loader.hpp"
#include "akila/core/resource/file_system.hpp"

#include "akila/core/window/window.hpp"
#include "akila/core/window/cursor.hpp"
#include "akila/core/window/window_events.hpp"

#include "akila/core/inputs/inputs.hpp"

#include "akila/core/layer/layers.hpp"

#include "akila/core/time/time.hpp"

#include "akila/core/signal/signals.hpp"
#include "akila/core/signal/listener.hpp"

#include "akila/core/threadpool/threadpool.hpp"

#include "akila/core/renderer/renderer.hpp"
#include "akila/core/renderer/shader.hpp"
#include "akila/core/renderer/material.hpp"
#include "akila/core/renderer/texture.hpp"
#include "akila/core/renderer/buffer_object.hpp"
#include "akila/core/renderer/vao.hpp"
#include "akila/core/renderer/shader_preproc.hpp"
#include "akila/core/renderer/framebuffer.hpp"

#include "akila/core/audio/audio.hpp"
#include "akila/core/audio/audio_source.hpp"
#include "akila/core/audio/audio_emitter.hpp"

#include "akila/core/coroutine/coroutine.hpp"

#pragma warning(push, 0)
#include <imgui/imgui.h>
#pragma warning(pop)

///

#include "akila/engine/engine.hpp"

#include "akila/engine/math_extension/distribution.hpp"
#include "akila/engine/math_extension/easing.hpp"
#include "akila/engine/math_extension/transform.hpp"

#include "akila/engine/containers/small_vector.hpp"
#include "akila/engine/containers/image.hpp"
