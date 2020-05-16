#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_FILE_SYSTEM_PRESENTER_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_FILE_SYSTEM_PRESENTER_HPP__

#include <CppDb/IFileSystemPresenter.hpp>
#include <CppDb/FileSystemStructureFwd.hpp>
#include <CppDb/ControllInformationFwd.hpp>

#include <boost/noncopyable.hpp>

namespace CppAbstractDataBase { namespace Details {
  class FileSystemPresenter final
    : public IFileSystemPresenter
    , public boost::noncopyable 
  {
  public:
    virtual ~FileSystemPresenter() = default;
    FileSystemPresenter(FileSystemStructure const& fsStruct);
    
    virtual std::string const& GetDBName() const override final;
    virtual boost::filesystem::path const& GetDBFilePath() const override final;

    virtual std::string const& GetInfoName() const override final;
    virtual boost::filesystem::path const& GetInfoFilePath() const override final;
    
    virtual std::string const& GetBackupName() const override final;
    virtual boost::filesystem::path const& GetBackupFilePath() const override final;
    
  private:
    std::string m_DbName;
    boost::filesystem::path m_DbPath;
    
    std::string m_InfoName;
    boost::filesystem::path m_InfoPath;
    
    std::string m_BackupName;
    boost::filesystem::path m_BackupPath;
  };
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_FILE_SYSTEM_PRESENTER_HPP__
