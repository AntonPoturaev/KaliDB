#if !defined(__CPP_ABSTRACT_DATA_BASE_EXCEPTION_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_EXCEPTION_HPP__

#include <exception>
#include <string>

#include <boost/exception/exception.hpp>

namespace CppAbstractDataBase {
  /// @class Exception - base for all level of the CppAbstractDataBase exceptions
  class Exception 
    : virtual public std::exception
    , virtual public boost::exception
  {
  public:
    virtual ~Exception() noexcept;
    
    virtual const char* what() const noexcept override final;
    
  protected:
    Exception(std::string const& type, std::string const& message);
    
  private:
    std::string const m_Message;
  };
  
  /// @{ Exception lelvels definition...
  
  /// @class Warning - base for all Warnings
  class Warning : public Exception {
  protected:
    Warning(std::string const& type, std::string const& message);
  };
 
  /// @class Error - base for all Error
  class Error : public Exception {
  protected:
    Error(std::string const& type, std::string const& message);
  };
  
  /// @class FatalError - base for all FatalError
  class FatalError : public Exception {
  protected:
    FatalError(std::string const& type, std::string const& message);
  };
  
  /// @}
  
  /// @{ Warnings definition ...
  
  struct BadParam final : public Warning {
    explicit BadParam(std::string const& message);
  };
  
  /// @}
  
  /// @{ Errors definition ...
  
  struct BadLogic final : public Error {
    explicit BadLogic(std::string const& message);
  };
  
  struct FileSystemError final : public Error {
    explicit FileSystemError(std::string const& message);
  };
  
  struct UndefinedException final : public Error {
    UndefinedException();
  };
  
  /// @}
  
  /// @{ FatalErrors definition ...
  struct BadAlloc final : public FatalError {
    explicit BadAlloc(std::string const& message);
  };
  
  /// @}
} /// end namespace CppAbstractDataBase

#include <boost/throw_exception.hpp>
#include <boost/exception_ptr.hpp>

//#define CPP_ABSTRACT_DB_THROW_EXCEPTION(exception) BOOST_THROW_EXCEPTION(exception)
#define CPP_ABSTRACT_DB_THROW_EXCEPTION(exception)

#endif /// !__CPP_ABSTRACT_DATA_BASE_EXCEPTION_HPP__
