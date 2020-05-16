#if !defined(__CPP_ABSTRACT_DATA_BASE_MANAGER_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_MANAGER_HPP__

#include <CppDb/details/ConcreteDataBase.hpp>
#include <CppDb/details/SingletonTemplate.hpp>

#include <mutex>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace CppAbstractDataBase {
  class Manager final : public Details::Singleton<Manager> {
    friend class Details::Singleton<Manager>;
    
    typedef std::lock_guard<std::mutex> const tLock;

    struct TagBy {
      struct Id {};
      struct Instance {};
      struct DbPath {};
    };
    
    typedef boost::multi_index_container<
      tIDataBasePtr
      , boost::multi_index::indexed_by<
          boost::multi_index::ordered_unique<
            boost::multi_index::tag<TagBy::Id>
              , boost::multi_index::const_mem_fun<
                IDataBase, tAtom, &IDataBase::GetDbId
              >
          >
          , boost::multi_index::ordered_unique<
              boost::multi_index::tag<TagBy::Instance>
              , boost::multi_index::identity<tIDataBasePtr>
          >
          , boost::multi_index::hashed_unique<
              boost::multi_index::tag<TagBy::DbPath>
              , boost::multi_index::const_mem_fun<
                IFileSystemPresenter, boost::filesystem::path const&, &IFileSystemPresenter::GetDBFilePath
              >
          >
      >
    > tStore;
    
    typedef tStore::index<TagBy::Id>::type tStoreViewByDbId;
    typedef tStore::index<TagBy::Instance>::type tStoreViewByDataBase;
    typedef tStore::index<TagBy::DbPath>::type tStoreViewByDbPath;
    
  public:
    template<typename ContainerT>
    tIDataBasePtr AcquireDataBase(FileSystemStructure const& fsStruct
                                  , tControllInformationOptional&& ctrlInfo = tControllInformationOptional())
    {
      tLock lock(m_Access);

      auto const found = GetStoreViewByDbPath().find(fsStruct.rootDir / fsStruct.dbName);
      if (found != GetStoreViewByDbPath().end()) {
        return *found;
      }
      
      auto const result = GetStoreViewByDataBase().emplace(
        new Details::ConcreteDataBase<ContainerT>(fsStruct, std::forward<tControllInformationOptional>(ctrlInfo))
      );
      
      return result.second ? *result.first : nullptr;
    }
    
    tIDataBasePtr AcquireDataBase(tAtom dbId) const;
    void ReleaseDataBase(tAtom dbId);
    
  private:
    Manager() = default;
    
    tStoreViewByDbId& GetStoreViewByDbId();
    tStoreViewByDbId const& GetStoreViewByDbId() const;
    tStoreViewByDataBase& GetStoreViewByDataBase();
    tStoreViewByDataBase const& GetStoreViewByDataBase() const;
    tStoreViewByDbPath& GetStoreViewByDbPath();
    tStoreViewByDbPath const& GetStoreViewByDbPath() const;
    
  private:
    tStore m_Store;
    std::mutex mutable m_Access;
  };
} /// end namespace CppAbstractDataBase

#endif /// !__CPP_ABSTRACT_DATA_BASE_MANAGER_HPP__
