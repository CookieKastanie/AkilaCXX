#include "akila/core/core.hpp"

#include "akila/core/math/math.hpp"

#include "akila/core/memory/ptr.hpp"
#include "akila/core/memory/ref.hpp"

#include "akila/core/ecs/ecs.hpp"

#include "akila/core/resources/resources.hpp"
#include "akila/core/resources/loader.hpp"
#include "akila/core/resources/file_system.hpp"

#include "akila/core/window/window.hpp"
#include "akila/core/window/cursor.hpp"
#include "akila/core/window/window_events.hpp"

#include "akila/core/inputs/inputs.hpp"

#include "akila/core/layer/layers.hpp"

#include "akila/core/time/time.hpp"

#include "akila/core/signals/signals.hpp"
#include "akila/core/signals/listener.hpp"

#include "akila/core/threadpool/threadpool.hpp"

#include "akila/core/rhi/renderer.hpp"
#include "akila/core/rhi/shader.hpp"
#include "akila/core/rhi/texture.hpp"
#include "akila/core/rhi/buffer_object.hpp"
#include "akila/core/rhi/vao.hpp"
#include "akila/core/rhi/framebuffer.hpp"

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

#include "akila/engine/graphics/material.hpp"
#include "akila/engine/graphics/material_factory.hpp"
