#pragma once

// Lightweight frame profiler.
// Enable with: add_compile_definitions(FLUIDSIM_PROFILE) or -DFLUIDSIM_PROFILE

#ifdef FLUIDSIM_PROFILE

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>

namespace FluidSimProfiler {
    enum class Section : uint8_t {
        NeighbourSearch,
        Gravity,
        Predict,
        Relaxation,
        Boundary,
        Velocity,
        Count
    };

    inline constexpr const char *sectionName(Section s) {
        switch (s) {
            case Section::NeighbourSearch: return "neighbourSearch";
            case Section::Gravity: return "applyGravity";
            case Section::Predict: return "predictPosition";
            case Section::Relaxation: return "doubleDensityRelaxation";
            case Section::Boundary: return "worldBoundary";
            case Section::Velocity: return "computeNextVelocity";
            default: return "unknown";
        }
    }

    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    inline std::array<double, static_cast<size_t>(Section::Count)> g_accumMs{};
    inline uint64_t g_frames = 0;

    struct Scope {
        Section section;
        TimePoint start;

        explicit Scope(Section s) : section(s), start(Clock::now()) {}
        ~Scope() {
            const auto end = Clock::now();
            const double ms = std::chrono::duration<double, std::milli>(end - start).count();
            g_accumMs[static_cast<size_t>(section)] += ms;
        }
    };

    inline void endFrame(uint32_t printEveryNFrames = 120) {
        ++g_frames;
        if (printEveryNFrames == 0 || (g_frames % printEveryNFrames) != 0) return;

        std::fprintf(stderr, "--- frame profiler (%llu frames) ---\n", static_cast<unsigned long long>(g_frames));
        for (size_t i = 0; i < g_accumMs.size(); ++i) {
            std::fprintf(stderr, "  %-24s : %8.3f ms/frame\n",
                sectionName(static_cast<Section>(i)),
                g_accumMs[i] / static_cast<double>(printEveryNFrames));
            g_accumMs[i] = 0.0;
        }
    }
}

#else

#include <cstdint>

namespace FluidSimProfiler {
    enum class Section : uint8_t {
        NeighbourSearch,
        Gravity,
        Predict,
        Relaxation,
        Boundary,
        Velocity,
        Count
    };

    struct Scope {
        explicit Scope(Section) {}
    };

    inline void endFrame(uint32_t = 0) {}
}

#endif
