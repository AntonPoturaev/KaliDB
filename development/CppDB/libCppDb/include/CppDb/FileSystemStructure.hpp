#if !defined(__CPP_ABSTRACT_DATA_BASE_FILE_SYSTEM_STRUCTURE_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_FILE_SYSTEM_STRUCTURE_HPP__

#include <CppDb/FileSystemStructureFwd.hpp>

#include <string>

#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>

namespace CppAbstractDataBase {
  struct FileSystemStructure final {
  public:
    struct NamePolicy {
    public:
      static NamePolicy const& GetDefault() noexcept;
      
    public:
      std::string dbFileName;
      std::string infoFileName;
      std::string backupFileName;
    };
    
    typedef boost::optional<NamePolicy> tNamePolicyOptional;
    
  public:
    boost::filesystem::path rootDir;
    std::string dbName;
    tNamePolicyOptional namePolicyOpt;
  };
} /// end namespace CppAbstractDataBase

#endif /// !__CPP_ABSTRACT_DATA_BASE_FILE_SYSTEM_STRUCTURE_HPP__
