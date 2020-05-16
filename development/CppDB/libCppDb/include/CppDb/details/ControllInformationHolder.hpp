#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_CONTROLL_INFORMATION_HOLDER_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_CONTROLL_INFORMATION_HOLDER_HPP__

#include <CppDb/IControllInformationHolder.hpp>
#include <CppDb/details/MappedFileReadWrite.hpp>

#include <boost/filesystem/path.hpp>

namespace CppAbstractDataBase { namespace Details {
  class _CtrInfo1stStart {
  protected:
    _CtrInfo1stStart(
      boost::filesystem::path const& ctrlInfoFilePath
      , tControllInformationOptional&& ctrlInfo
    );
  };

  class ControllInformationHolder final
    : public IControllInformationHolder
    , private _CtrInfo1stStart
    , public boost::noncopyable
  {
    typedef MappedFileReadWrite tMemory;
    
  public:
    virtual ~ControllInformationHolder() = default;
    
    ControllInformationHolder(
      boost::filesystem::path const& ctrlInfoFilePath
      , tControllInformationOptional&& ctrlInfo
    );
    
    virtual ControllInformation const& GetControllInformation() const override final;
    virtual uint8_t GetBitCount() const override final;
    
    virtual tCrcValue GetCrc() const override final;
    void SetCrc(tCrcValue crc);
    
    virtual uint32_t GetVersion() const override final;
    
  private:
    mutable tMemory m_Memory;
  };
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_CONTROLL_INFORMATION_HOLDER_HPP__
