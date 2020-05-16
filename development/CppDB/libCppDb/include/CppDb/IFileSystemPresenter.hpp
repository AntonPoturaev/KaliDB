#if !defined(__CPP_ABSTRACT_DATA_BASE_FILE_SYSTEM_PRESENTER_INTERFACE_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_FILE_SYSTEM_PRESENTER_INTERFACE_HPP__

#include <CppDb/IFileSystemPresenterFwd.hpp>

#include <string>

#include <boost/filesystem/path.hpp>

namespace CppAbstractDataBase {
  class IFileSystemPresenter {
  public:
    virtual ~IFileSystemPresenter() = 0;
    
    virtual std::string const& GetDBName() const = 0;
    virtual boost::filesystem::path const& GetDBFilePath() const = 0;
    
  protected:
    virtual std::string const& GetInfoName() const = 0;
    virtual boost::filesystem::path const& GetInfoFilePath() const = 0;
    
    virtual std::string const& GetBackupName() const = 0;
    virtual boost::filesystem::path const& GetBackupFilePath() const = 0;
  };
} /// end namespace CppAbstractDataBase

#endif /// !__CPP_ABSTRACT_DATA_BASE_FILE_SYSTEM_PRESENTER_INTERFACE_HPP__
