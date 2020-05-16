#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_PERMANENT_ACCESS_INTERFACE_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_PERMANENT_ACCESS_INTERFACE_HPP__

#include <CppDb/AbstractContainerDefinition.hpp>
#include <CppDb/details/Memory.hpp>

#include <memory>

namespace CppAbstractDataBase { namespace Details {
  struct IPermanentAccess {
    virtual ~IPermanentAccess() = 0;
    
    virtual tAbstractContainerPtr GetAbstractContainer() = 0;
    virtual tAbstractContainerConstPtr GetAbstractContainer() const = 0;
    
    virtual tSegmentManager* GetSegmentManager() = 0;
    virtual tSegmentManager const* GetSegmentManager() const = 0;
  };
  
  typedef std::shared_ptr<IPermanentAccess> tIPermanentAccessPtr;
  typedef std::weak_ptr<IPermanentAccess> tIPermanentAccessWeak;
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_PERMANENT_ACCESS_INTERFACE_HPP__
