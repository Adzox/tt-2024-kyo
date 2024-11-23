#include "scripts/Scripts.hpp"

#include "scripts/InitScript.hpp"
#include "scripts/OnClickedSwitchSceneScript.hpp"

#include <ComponentLoaders/ScriptLoader.hpp>

#include <memory>

std::unique_ptr<ComponentLoaders::ScriptLoader<InitScript>> initScriptLoader;
std::unique_ptr<ComponentLoaders::ScriptLoader<OnClickedSwitchSceneScript>> gotoGameLoader;

void InitScripts() {
    initScriptLoader = std::make_unique<ComponentLoaders::ScriptLoader<InitScript>>();
    gotoGameLoader = std::make_unique<ComponentLoaders::ScriptLoader<OnClickedSwitchSceneScript>>();
}

void RegisterScripts(Base::BaseGame& baseGame) {
    baseGame.RegisterCustomComponentLoader(*initScriptLoader);
    baseGame.RegisterCustomComponentLoader(*gotoGameLoader);
}
