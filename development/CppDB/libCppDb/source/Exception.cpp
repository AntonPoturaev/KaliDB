#include "stdafx.h"
#include <CppDb/Exception.hpp>

#include <boost/format.hpp>

namespace CppAbstractDataBase {
  Exception::~Exception() noexcept {}
    
  const char* Exception::what() const noexcept {
    return m_Message.c_str();
  }
  
  Exception::Exception(std::string const& type, std::string const& message)
    : m_Message(boost::str(boost::format("CppAbstractDataBase exception (type: %1%). Reason: %2%") % type % message))
  {
  }
  
  Warning::Warning(std::string const& type, std::string const& message)
    : Exception(boost::str(boost::format("Warning [%1%]") % type), message)
  {
  }
  
  Error::Error(std::string const& type, std::string const& message)
    : Exception(boost::str(boost::format("Error [%1%]") % type), message)
  {
  }
  
  FatalError::FatalError(std::string const& type, std::string const& message)
    : Exception(boost::str(boost::format("FatalError [%1%]") % type), message)
  {
  }
  
  BadParam::BadParam(std::string const& message)
    : Warning("Warning", message)
  {
  }
  
  BadLogic::BadLogic(std::string const& message)
    : Error("Error", message)
  {
  }
  
  FileSystemError::FileSystemError(std::string const& message)
    : Error("Error", message)
  {
  }
  
  UndefinedException::UndefinedException()
    : Error("Error", "Undefined type of exception")
  {
  }
  
  BadAlloc::BadAlloc(std::string const& message)
    : FatalError("FatalError", message)
  {
  }
} /// end namespace CppAbstractDataBase
