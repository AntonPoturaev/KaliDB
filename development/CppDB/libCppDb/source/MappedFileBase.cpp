#include "stdafx.h"
#include <CppDb/details/MappedFileBase.hpp>

namespace CppAbstractDataBase { namespace Details {
  size_t MappedFileBase::GetDataSize() const {
    return m_Region.get_size();
  }
  
  MappedFileBase::MappedFileBase(std::string const& filePath, boost::interprocess::mode_t mode)
    : m_Mapping(filePath.c_str(), mode)
    , m_Region(m_Mapping, mode)
  {
  }
  
  MappedFileBase::tFileMapping const& MappedFileBase::GetFileMapping() const {
    return m_Mapping;
  }
  
  MappedFileBase::tMappedRegion& MappedFileBase::GetMappedRegion() {
    return m_Region;
  }
  
  MappedFileBase::tMappedRegion const& MappedFileBase::GetMappedRegion() const {
    return m_Region;
  }
}} /// end namespace CppAbstractDataBase::Details
