#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_SKELETONS_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_SKELETONS_HPP__

#include <CppDb/details/MappedFileBase.hpp>

#include <type_traits>

#include <boost/filesystem/path.hpp>
#include <boost/range/iterator_range_core.hpp>

namespace CppAbstractDataBase { namespace Details {
  template<typename DerivedT, typename DataConstPointerT>
  class MappedFileReadOnlySkeleton : public MappedFileBase {
  public:
    typedef DataConstPointerT tDataConstPointer;
    typedef boost::iterator_range<tDataConstPointer> tDataConstRange;
    
  public:
    tDataConstPointer GetData() const {
      return static_cast<DerivedT const*>(this)->GetData();
    }
    
    tDataConstRange GetRange() const {
      return static_cast<DerivedT const*>(this)->GetRange();
    }
    
  protected:
    MappedFileReadOnlySkeleton(boost::filesystem::path const& filePath, boost::interprocess::mode_t mode)
      : MappedFileBase(filePath.string(), mode)
    {
    }
  };
  
  template<typename T>
  struct _MakeConstPtr
    : public std::add_pointer<
        typename std::add_const<
          typename std::remove_pointer<T>::type
        >::type
      >
  {
  };
  
  template<typename DerivedT, typename DataPointerT>
  class MappedFileReadWriteSkeleton 
    : public MappedFileReadOnlySkeleton<DerivedT, typename _MakeConstPtr<DataPointerT>::type>
  {
    typedef MappedFileReadOnlySkeleton<DerivedT, typename _MakeConstPtr<DataPointerT>::type> tSuper;
  public:
    typedef DataPointerT tDataPointer;
    typedef boost::iterator_range<tDataPointer> tDataRange;
    
  public:
    tDataPointer GetData() {
      return static_cast<DerivedT*>(this)->GetData();
    }
    
    tDataRange GetRange() {
      return static_cast<DerivedT*>(this)->GetRange();
    }
    
  protected:
    MappedFileReadWriteSkeleton(boost::filesystem::path const& filePath, boost::interprocess::mode_t mode)
      : tSuper(filePath, mode)
    {
    }
  };
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_MAPPED_FILE_SKELETONS_HPP__
