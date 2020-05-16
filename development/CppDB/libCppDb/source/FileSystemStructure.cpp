#include "stdafx.h"
#include <CppDb/FileSystemStructure.hpp>

namespace CppAbstractDataBase {
  FileSystemStructure::NamePolicy const& FileSystemStructure::NamePolicy::GetDefault() noexcept {
    static NamePolicy defaultNamePolicy = { "db", "info", "backup" };
    return defaultNamePolicy;
  }
} /// end namespace CppAbstractDataBase

