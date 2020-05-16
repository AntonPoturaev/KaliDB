#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_BASE_HPP__)
#	define __CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_BASE_HPP__

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/noncopyable.hpp>

#include <string>

namespace CppAbstractDataBase { namespace Details {
  class MappedFileBase {
  protected:
    typedef boost::interprocess::file_mapping tFileMapping;
    typedef boost::interprocess::mapped_region tMappedRegion;
    
  public:
    size_t GetDataSize() const;

    tFileMapping const& GetFileMapping() const;
    
    tMappedRegion& GetMappedRegion();
    tMappedRegion const& GetMappedRegion() const;
    
  protected:
    MappedFileBase(std::string const& filePath, boost::interprocess::mode_t mode);
    
  private:
    tFileMapping const m_Mapping;
    tMappedRegion m_Region;
  };
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_BASE_HPP__
