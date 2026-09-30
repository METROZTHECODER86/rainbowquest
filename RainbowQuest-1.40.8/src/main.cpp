// RainbowQuest - port for Beat Saber 1.40.8 (Scotland2 / bs-cordl / BSML)
// Original mod by Unifox.
//
// Approach: instead of mutating ColorScheme fields every frame (field names change
// between game versions), hook the ColorScheme colour getters and return a
// time-based rainbow colour. Left/right are kept 180 degrees apart.
#include "main.hpp"
#include "ModConfig.hpp"

#include "UnityEngine/Color.hpp"
#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Transform.hpp"
#include "HMUI/ViewController.hpp"
#include "GlobalNamespace/ColorScheme.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML-Lite.hpp"

#include <cmath>
#include <cstdlib>
#include <ctime>

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};

static float obstacleOffset = 90.0f;

static UnityEngine::Color RainbowColor(float offsetDegrees, float alpha) {
    // ~90 degrees/second at speed 1.0 (matches the old "+1 hue per frame at 90 fps")
    float hue = std::fmod(UnityEngine::Time::get_time() * getRainbowConfig().RainbowSpeed.GetValue() * 90.0f + offsetDegrees, 360.0f);
    if (hue < 0.0f) hue += 360.0f;
    auto rgb = UnityEngine::Color::HSVToRGB(hue / 360.0f, 1.0f, 1.0f);
    UnityEngine::Color c;
    c.r = UnityEngine::Mathf::GammaToLinearSpace(rgb.r);
    c.g = UnityEngine::Mathf::GammaToLinearSpace(rgb.g);
    c.b = UnityEngine::Mathf::GammaToLinearSpace(rgb.b);
    c.a = alpha;
    return c;
}

#define RAINBOW_HOOK(NAME, OFFSET)                                                              \
    MAKE_HOOK_MATCH(ColorScheme_##NAME, &GlobalNamespace::ColorScheme::get_##NAME,              \
                    UnityEngine::Color, GlobalNamespace::ColorScheme* self) {                   \
        auto original = ColorScheme_##NAME(self);                                               \
        if (!getRainbowConfig().RainbowEnabled.GetValue()) return original;                     \
        return RainbowColor(OFFSET, original.a);                                                \
    }

RAINBOW_HOOK(saberAColor, 0.0f)
RAINBOW_HOOK(saberBColor, 180.0f)
RAINBOW_HOOK(obstaclesColor, obstacleOffset)
RAINBOW_HOOK(environmentColor0, 0.0f)
RAINBOW_HOOK(environmentColor1, 180.0f)

static void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (!firstActivation) return;

    auto container = BSML::Lite::CreateScrollableSettingsContainer(self->get_transform());

    BSML::Lite::CreateToggle(container->get_transform(), "Rainbow Enabled",
        getRainbowConfig().RainbowEnabled.GetValue(),
        [](bool value) { getRainbowConfig().RainbowEnabled.SetValue(value); });

    BSML::Lite::CreateIncrementSetting(container->get_transform(), "Rainbow Speed", 1, 0.1f,
        getRainbowConfig().RainbowSpeed.GetValue(), 0.0f, 3.0f,
        [](float value) { getRainbowConfig().RainbowSpeed.SetValue(value); });
}

MOD_EXTERN_FUNC void setup(CModInfo* info) noexcept {
    *info = modInfo.to_c();
    Paper::Logger::RegisterFileContextId(MOD_ID);
    getRainbowConfig().Init(modInfo);
    PaperLogger.info("Setup complete");
}

MOD_EXTERN_FUNC void late_load() noexcept {
    il2cpp_functions::Init();
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    obstacleOffset = static_cast<float>(std::rand() % 360);

    BSML::Init();
    BSML::Register::RegisterSettingsMenu("Rainbow Quest", DidActivate);

    INSTALL_HOOK(PaperLogger, ColorScheme_saberAColor);
    INSTALL_HOOK(PaperLogger, ColorScheme_saberBColor);
    INSTALL_HOOK(PaperLogger, ColorScheme_obstaclesColor);
    INSTALL_HOOK(PaperLogger, ColorScheme_environmentColor0);
    INSTALL_HOOK(PaperLogger, ColorScheme_environmentColor1);
    PaperLogger.info("Installed RainbowQuest hooks");
}
