#include "stdafx.h"
#include <CppDb/Session.hpp>
#include <CppDb/ControllInformation.hpp>
#include <CppDb/details/BaseDataBase.hpp>

#include <boost/scope_exit.hpp>

namespace CppAbstractDataBase {
  Session::~Session() {
    if (m_Db) {
      m_Db->CloseSession(shared_from_this());
    }
  }
    
  void Session::Transaction(tTransactionProcedure&& doTransaction, tTransactionProcedure&& rollBackTransaction) {
    m_Db->AcquireTransaction();
    
    BOOST_SCOPE_EXIT(this_) {
      this_->m_Db->ReleaseTransaction();
    } BOOST_SCOPE_EXIT_END;
    
    if (doTransaction) {
      try {
        size_t const tryToAllocateCount = m_Db->GetControllInformation().tryToAllocateCount;
        for (size_t i = 0; i < tryToAllocateCount; ++i) {
          try {
            doTransaction(_CtxWeak());
            break;
          }
          catch (boost::interprocess::bad_alloc const&) {
            m_Db->Grow();
            continue;
          }
          catch (std::length_error const&) {
            m_Db->Grow();
            continue;
          }
        }
      } catch(...) {
        if (rollBackTransaction) {
          rollBackTransaction(_CtxWeak());
        }
        
        throw;
      }
    }
  }
  
  Session::Session(tIDataBasePtr&& db) 
    : m_Db(std::forward<tIDataBasePtr>(db))
  {
    if (!m_Db) {
      throw;
    }
  }
  
  Session::tContextWeak Session::_CtxWeak() const {
    return tContextWeak(tContextPtr(m_Db, _PermanentAccess()));
  }
  
  Details::IPermanentAccess* Session::_PermanentAccess() const {
    return static_cast<Details::IPermanentAccess*>(m_Db.get());
  }
} /// end namespace CppAbstractDataBase
