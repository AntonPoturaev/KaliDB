#if !defined(__CPP_ABSTRACT_DATA_BASE_CONTROLL_INFORMATION_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_CONTROLL_INFORMATION_HPP__

#include <CppDb/ControllInformationFwd.hpp>

#include <cstddef>
#include <chrono>

namespace CppAbstractDataBase {
  struct ControllInformation final {
  public:
    static ControllInformation const& GetDefault() noexcept;
    
  public:
    size_t pageSize;
    size_t tryToAllocateCount;
    std::chrono::nanoseconds backupTimeout;
  };
} /// end namespace CppAbstractDataBase

#endif /// !__CPP_ABSTRACT_DATA_BASE_CONTROLL_INFORMATION_HPP__
