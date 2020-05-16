#include "stdafx.h"
#include <CppDb/details/MappedFileReadOnly.hpp>

namespace CppAbstractDataBase { namespace Details {
  MappedFileReadOnly::MappedFileReadOnly(boost::filesystem::path const& filePath)
    : tSuper(filePath, boost::interprocess::read_only)
  {
  }

  MappedFileReadOnly::tDataConstPointer MappedFileReadOnly::GetData() const {
    return static_cast<tDataConstPointer>(GetMappedRegion().get_address());
  }
  
  MappedFileReadOnly::tDataConstRange MappedFileReadOnly::GetRange() const {
    tDataConstPointer const data(GetData());
    return boost::make_iterator_range(data, data + GetDataSize());
  }
}} /// end namespace CppAbstractDataBase::Details
