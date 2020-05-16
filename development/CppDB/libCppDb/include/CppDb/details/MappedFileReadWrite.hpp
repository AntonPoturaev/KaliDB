#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_READWRITE_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_READWRITE_HPP__

#include <CppDb/details/MappedFileSkeletons.hpp>
#include <CppDb/details/Bytes.hpp>

namespace CppAbstractDataBase { namespace Details {
  class MappedFileReadWrite final 
    : public MappedFileReadWriteSkeleton<MappedFileReadWrite, tBytePtr> 
  {
    typedef MappedFileReadWriteSkeleton<MappedFileReadWrite, tBytePtr> tSuper;
    
  public:
    MappedFileReadWrite(boost::filesystem::path const& filePath);
    
    tDataPointer GetData();
    tDataRange GetRange();
  };
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_READWRITE_HPP__
