#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_MEMORY_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_MEMORY_HPP__

#include <boost/interprocess/managed_mapped_file.hpp>

namespace CppAbstractDataBase { namespace Details {
  typedef boost::interprocess::managed_mapped_file tManagedMappedFile;
  typedef tManagedMappedFile::segment_manager tSegmentManager;
  typedef boost::uintmax_t tMemorySize;
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_MEMORY_HPP__
