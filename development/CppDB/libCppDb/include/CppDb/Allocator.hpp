#if !defined(__CPP_ABSTRACT_DATA_BASE_ALLOCATOR_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_ALLOCATOR_HPP__

#include <CppDb/details/Memory.hpp>

#include <boost/interprocess/allocators/allocator.hpp>

namespace CppAbstractDataBase {
  template<typename T>
  using Allocator = boost::interprocess::allocator<T, Details::tSegmentManager>;
} /// end namespace CppAbstractDataBase

#endif /// !__CPP_ABSTRACT_DATA_BASE_ALLOCATOR_HPP__
