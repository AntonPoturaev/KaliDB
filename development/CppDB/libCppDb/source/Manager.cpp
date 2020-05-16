#include "stdafx.h"
#include <CppDb/Manager.hpp>
#include <CppDb/Exception.hpp>

#include <boost/format.hpp>

namespace CppAbstractDataBase {
  tIDataBasePtr Manager::AcquireDataBase(tAtom dbId) const {
    tLock lock(m_Access);
    
    auto const found = GetStoreViewByDbId().find(dbId);
    return (found == GetStoreViewByDbId().end()) ? nullptr : *found;
  }
  
  void Manager::ReleaseDataBase(tAtom dbId) {
    tLock lock(m_Access);
    GetStoreViewByDbId().erase(dbId);
  }
  
  Manager::tStoreViewByDbId& Manager::GetStoreViewByDbId() {
    return m_Store.get<TagBy::Id>();
  }
  
  Manager::tStoreViewByDbId const& Manager::GetStoreViewByDbId() const {
    return m_Store.get<TagBy::Id>();
  }
  
  Manager::tStoreViewByDataBase& Manager::GetStoreViewByDataBase() {
    return m_Store.get<TagBy::Instance>();
  }
  
  Manager::tStoreViewByDataBase const& Manager::GetStoreViewByDataBase() const {
    return m_Store.get<TagBy::Instance>();
  }
  
  Manager::tStoreViewByDbPath& Manager::GetStoreViewByDbPath() {
    return m_Store.get<TagBy::DbPath>();
  }
  
  Manager::tStoreViewByDbPath const& Manager::GetStoreViewByDbPath() const {
    return m_Store.get<TagBy::DbPath>();
  }

} /// end namespace CppAbstractDataBase
