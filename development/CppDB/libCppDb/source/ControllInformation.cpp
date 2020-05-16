#include "stdafx.h"
#include <CppDb/ControllInformation.hpp>

namespace CppAbstractDataBase {
  ControllInformation const& ControllInformation::GetDefault() noexcept {
    static constexpr ControllInformation ctrlInfo = { 65536, 3, std::chrono::nanoseconds(1) };
    return ctrlInfo;
  }
} /// end namespace CppAbstractDataBase
