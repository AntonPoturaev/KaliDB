#include "stdafx.h"
#include <CppDb/details/MappedFileReadWrite.hpp>

namespace CppAbstractDataBase { namespace Details {
  MappedFileReadWrite::MappedFileReadWrite(boost::filesystem::path const& filePath)
    : tSuper(filePath, boost::interprocess::read_write)
  {
  }
    
  MappedFileReadWrite::tDataPointer MappedFileReadWrite::GetData() {
    return static_cast<tDataPointer>(GetMappedRegion().get_address());
  }
  
  MappedFileReadWrite::tDataRange MappedFileReadWrite::GetRange() {
    tDataPointer const data(GetData());
    return boost::make_iterator_range(data, data + GetDataSize());
  }
  
}} /// end namespace CppAbstractDataBase::Details
