#if !defined(__CPP_ABSTRACT_DATA_BASE_DATA_BASE_INTERFACE_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DATA_BASE_INTERFACE_HPP__

#include <CppDb/Atom.hpp>
#include <CppDb/IDataBaseFwd.hpp>
#include <CppDb/SessionFwd.hpp>
#include <CppDb/IControllInformationHolder.hpp>
#include <CppDb/IFileSystemPresenter.hpp>
#include <CppDb/details/IPermanentAccess.hpp>

namespace CppAbstractDataBase {
  class IDataBase 
    : public IControllInformationHolder
    , public IFileSystemPresenter
    , protected Details::IPermanentAccess
    , public std::enable_shared_from_this<IDataBase> 
  {
    friend class Session;
  public:
    virtual ~IDataBase() = 0;
    
    virtual tAtom GetDbId() const = 0;
    virtual tSessionPtr CreateSession() = 0;
    
  protected:
    virtual void AcquireTransaction() = 0;
    virtual void ReleaseTransaction() = 0;
    
    virtual void Grow() = 0;
    
    virtual void CloseSession(tSessionPtr const& session) = 0;
  };
} /// end namespace CppAbstractDataBase

#endif /// !__CPP_ABSTRACT_DATA_BASE_DATA_BASE_INTERFACE_HPP__
