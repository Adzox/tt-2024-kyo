#include "config.hpp"

#include "cmp_load/LoadingScreenLoader.hpp"
#include "rapidjson_scene_parser/RapidJsonSceneParser.hpp"
#include "scripts/Scripts.hpp"
#include "sdl/SdlInput.hpp"
#include "sdl/SdlLogger.hpp"
#include "sdl/SdlTextFileLoader.hpp"
#include "sdl/SdlTextureRenderer.hpp"

#include <Base/BaseGame.hpp>
#include <ComponentLoaders/ScriptLoader.hpp>
#include <Log/Log.hpp>

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

#include <memory>

SdlLogger sdlLogger{};

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* canvas;
std::unique_ptr<Events::EventManager<size_t>> eventManager = nullptr;
std::unique_ptr<SdlInputManager> sdlInputManager = nullptr;
std::unique_ptr<SdlTextFileLoader> textFileLoader = nullptr;
std::unique_ptr<RapidJsonSceneParser> sceneParser = nullptr;
std::unique_ptr<SdlTextureRenderer> gameRenderer = nullptr;

std::unique_ptr<LoadingScreenLoader> loadingScreenLoader = nullptr;

std::unique_ptr<Base::BaseGame> game = nullptr;

unsigned long long lastTicks = 0;
bool closeApp = false;

SDL_AppResult SDL_Fail() {
  SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
  return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return SDL_Fail();
  }

  window = SDL_CreateWindow(TARGET_NAME, TARGET_WIDTH, TARGET_HEIGHT, SDL_WINDOW_RESIZABLE);
  if (!window) {
    return SDL_Fail();
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    return SDL_Fail();
  }

  canvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TARGET_WIDTH, TARGET_HEIGHT);
  SDL_SetTextureScaleMode(canvas, SDL_SCALEMODE_NEAREST);

  SDL_ShowWindow(window);

  SDL_SetHint(SDL_HINT_LOGGING, "app=debug");
  SetGlobalLogger(sdlLogger);
  sdlLogger.SetLogLevel(Log::Level::Debug);

  eventManager = std::make_unique<Events::EventManager<size_t>>();
  sdlInputManager = std::make_unique<SdlInputManager>(*eventManager);
  textFileLoader = std::make_unique<SdlTextFileLoader>();
  sceneParser = std::make_unique<RapidJsonSceneParser>(*textFileLoader);
  gameRenderer = std::make_unique<SdlTextureRenderer>(*renderer);

  game = std::make_unique<Base::BaseGame>(Base::GameConfig{TARGET_NAME, TARGET_WIDTH, TARGET_HEIGHT},
                                          *eventManager,
                                          *gameRenderer.get(),
                                          *gameRenderer.get(),
                                          *sceneParser,
                                          *sdlInputManager);

  loadingScreenLoader = std::make_unique<LoadingScreenLoader>(game->GetSceneLoader());

  game->RegisterCustomComponentLoader(*loadingScreenLoader);

  InitScripts();
  RegisterScripts(*game);

  game->Init("res/common/scenes/game_base.json");

  SDL_LogInfo(SDL_LOG_CATEGORY_CUSTOM, "Application started successfully!");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    closeApp = true;
  }
  sdlInputManager->AddEvent(event);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  float timeSinceLastMainLoopCall = (SDL_GetTicks() - lastTicks) / 1000.f;
  game->Update(timeSinceLastMainLoopCall);
  lastTicks = SDL_GetTicks();

  SDL_SetRenderTarget(renderer, canvas);
  SDL_RenderClear(renderer);
  game->Render();
  SDL_SetRenderTarget(renderer, nullptr);
  SDL_RenderTexture(renderer, canvas, nullptr, nullptr);
  SDL_RenderPresent(renderer);

  return closeApp || game->QuitRequested() ? SDL_AppResult::SDL_APP_SUCCESS : SDL_AppResult::SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  game->Destroy();

  game.reset();
  gameRenderer.reset();

  SDL_DestroyTexture(canvas);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_LogInfo(SDL_LOG_CATEGORY_CUSTOM, "Application quit successfully!");
}
