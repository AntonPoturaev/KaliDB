#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_READONLY_HPP__)
#	define __CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_READONLY_HPP__

#include <CppDb/details/MappedFileSkeletons.hpp>
#include <CppDb/details/Bytes.hpp>

namespace CppAbstractDataBase { namespace Details {
  class MappedFileReadOnly final 
    : public MappedFileReadOnlySkeleton<MappedFileReadOnly, tByteConstPtr> 
  {
    typedef MappedFileReadOnlySkeleton<MappedFileReadOnly, tByteConstPtr> tSuper;
    
  public:
    MappedFileReadOnly(boost::filesystem::path const& filePath);
    
    tDataConstPointer GetData() const;
    tDataConstRange GetRange() const;
  };
  
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_READONLY_HPP__