#if !defined(__CPP_ABSTRACT_DATA_BASE_SESSION_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_SESSION_HPP__

#include <CppDb/SessionFwd.hpp>
#include <CppDb/IDataBaseFwd.hpp>
#include <CppDb/details/BaseDataBaseFwd.hpp>
#include <CppDb/details/IPermanentAccess.hpp>

#include <functional>

#include <boost/noncopyable.hpp>

namespace CppAbstractDataBase {
  class Session final 
    : public boost::noncopyable
    , public std::enable_shared_from_this<Session> 
  {
    friend class Details::BaseDataBase;
  public:
    typedef Details::tIPermanentAccessPtr tContextPtr;
    typedef Details::tIPermanentAccessWeak tContextWeak;
    
    typedef std::function<void(tContextWeak&&)> tTransactionProcedure;
    
  public:
    ~Session();
    
    void Transaction(
      tTransactionProcedure&& doTransaction
      , tTransactionProcedure&& rollBackTransaction = tTransactionProcedure()
    );
    
  private:
    explicit Session(tIDataBasePtr&& db);
    
    tContextWeak _CtxWeak() const;
    Details::IPermanentAccess* _PermanentAccess() const;
    
  private:
    tIDataBasePtr m_Db;
  };
} /// end namespace CppAbstractDataBase

#endif /// !__CPP_ABSTRACT_DATA_BASE_SESSION_HPP__
