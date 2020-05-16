#if !defined(__CPP_ABSTRACT_DATA_BASE_DATA_BASE_INTERFACE_FORWARD_DECLARATION_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DATA_BASE_INTERFACE_FORWARD_DECLARATION_HPP__

#include <memory>

namespace CppAbstractDataBase {
  class IDataBase;
  typedef std::shared_ptr<IDataBase> tIDataBasePtr;
} /// end namespace CppAbstractDataBase

#endif /// !__CPP_ABSTRACT_DATA_BASE_DATA_BASE_INTERFACE_FORWARD_DECLARATION_HPP__
