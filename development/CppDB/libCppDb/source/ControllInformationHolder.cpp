#include "stdafx.h"
#include <CppDb/details/ControllInformationHolder.hpp>
#include <CppDb/ControllInformation.hpp>
#include <CppDb/Version.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

namespace CppAbstractDataBase { namespace Details {
  namespace {
    /*
     structure of the file:
     -------------------------------------------------------
     |        field         | size of data                 |
     -----------------------|------------------------------|
     | OS bit count         | sizeof(uint8_t)              |
     -----------------------|------------------------------|
     | Version of DB SDK    | sizeof(uint32_t)             |
     -----------------------|------------------------------|
     | CRC 32               | sizeof(tCrcValue)            |
     -----------------------|------------------------------|
     | Controll information | sizeof(ControllInformation)  |
     -------------------------------------------------------
     */
    
    constexpr uint8_t _GetOsBitCount() noexcept {
      return sizeof(void*);
    }
    
    constexpr uint32_t _GetCurrentSDKVersion() noexcept {
      return CPP_ABSTRACT_DATA_BASE_VERSION;
    }
    
    struct _Header final {
      uint8_t osBitCount = _GetOsBitCount();
      uint32_t sdkVersion = _GetCurrentSDKVersion();
      tCrcValue crc { 0 };
    };
    
    struct _CtrlInfo final {
    public:
      explicit _CtrlInfo(tControllInformationOptional&& ctrlInfo) {
        InitHelper(info, std::forward<tControllInformationOptional>(ctrlInfo));
      }

      static void InitHelper(ControllInformation& info, tControllInformationOptional&& ctrlInfo) {
        if (ctrlInfo) {
          info = std::move(*ctrlInfo);
        }
        else {
          info = ControllInformation::GetDefault();
        }
      }

    public:
      _Header header;
      ControllInformation info;
    };

    constexpr size_t _GetTrueSizeOfFileOnThisOS() noexcept {
      return sizeof(_CtrlInfo);
    }

    _CtrlInfo& _GetCtrlInfo(uint8_t* data) noexcept {
      return *static_cast<_CtrlInfo*>(static_cast<void*>(data));
    }
  } /// end unnamed namespace

  _CtrInfo1stStart::_CtrInfo1stStart(
    boost::filesystem::path const& ctrlInfoFilePath
    , tControllInformationOptional&& ctrlInfo
  ) {
    if (!boost::filesystem::exists(ctrlInfoFilePath)) {
      boost::filesystem::ofstream ofs(ctrlInfoFilePath, std::ios::binary | std::ios::out);

      if (!ofs.is_open()) {
        throw std::runtime_error("");
      }

      _CtrlInfo info(std::forward<tControllInformationOptional>(ctrlInfo));
      ofs.rdbuf()->sputn(reinterpret_cast<char const*>(&info), sizeof(_CtrlInfo));
      ofs.close();
    }
  }

  ControllInformation const& ControllInformationHolder::GetControllInformation() const {
    return _GetCtrlInfo(m_Memory.GetData()).info;
  }
  
  uint8_t ControllInformationHolder::GetBitCount() const {
    return _GetCtrlInfo(m_Memory.GetData()).header.osBitCount;
  }
  
  tCrcValue ControllInformationHolder::GetCrc() const {
    return _GetCtrlInfo(m_Memory.GetData()).header.crc;
  }
  
  void ControllInformationHolder::SetCrc(tCrcValue crc) {
    _GetCtrlInfo(m_Memory.GetData()).header.crc = crc;
  }
  
  uint32_t ControllInformationHolder::GetVersion() const {
    return _GetCtrlInfo(m_Memory.GetData()).header.sdkVersion;
  }
  
  ControllInformationHolder::ControllInformationHolder(
    boost::filesystem::path const& ctrlInfoFilePath
    , tControllInformationOptional&& ctrlInfo
  ) : _CtrInfo1stStart(ctrlInfoFilePath, std::forward<tControllInformationOptional>(ctrlInfo))
    , m_Memory(ctrlInfoFilePath)
  {
    if (GetBitCount() != _GetOsBitCount()) {
      throw std::runtime_error("Bad data! This DB created at OS with other bit count!");
    }

    if (GetVersion() != _GetCurrentSDKVersion()) {
      throw std::runtime_error("Bad data! This DB created with other SDK version!");
    }

    _CtrlInfo::InitHelper(
      _GetCtrlInfo(m_Memory.GetData()).info
      , std::forward<tControllInformationOptional>(ctrlInfo)
    );
  }
}} /// end namespace CppAbstractDataBase::Details
