#pragma once

#include <Assets/ITextFileLoader.hpp>

class SdlTextFileLoader : public Assets::ITextFileLoader {

  std::optional<std::string> LoadTextFile(std::string_view resourcePath) override;
};
