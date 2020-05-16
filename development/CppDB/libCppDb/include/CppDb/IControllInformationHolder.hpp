#if !defined(__CPP_ABSTRACT_DATA_BASE_CONTROLL_INFORMATION_HOLDER_INTERFACE_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_CONTROLL_INFORMATION_HOLDER_INTERFACE_HPP__

#include <CppDb/IControllInformationHolderFwd.hpp>
#include <CppDb/ControllInformationFwd.hpp>
#include <CppDb/details/CrcTypes.hpp>

namespace CppAbstractDataBase {
  struct IControllInformationHolder {
    virtual ~IControllInformationHolder() = 0;
    
    virtual ControllInformation const& GetControllInformation() const = 0;
    virtual uint8_t GetBitCount() const = 0;
    virtual tCrcValue GetCrc() const = 0;
    virtual uint32_t GetVersion() const = 0;
  };
} /// end namespace CppAbstractDataBase

#endif /// !__CPP_ABSTRACT_DATA_BASE_CONTROLL_INFORMATION_HOLDER_INTERFACE_HPP__
