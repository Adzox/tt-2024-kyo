#include "sdl/SdlTextFileLoader.hpp"

#include <Log/Log.hpp>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_stdinc.h>

#include <string>

std::optional<std::string> SdlTextFileLoader::LoadTextFile(std::string_view resourcePath){
    size_t bytesRead = 0;
    auto path = std::string(SDL_GetBasePath()) + std::string(resourcePath);

    void* data = SDL_LoadFile(path.c_str(), &bytesRead);
    if (bytesRead == 0 || !data) {
        Log::LogError("Failed to read file '{}': {}", resourcePath, SDL_GetError());
        return std::nullopt;
    }
    std::string contents((char*)data);
    SDL_free(data);
    return contents;
};
