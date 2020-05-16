#include "stdafx.h"
#include <CppDb/details/BaseDataBase.hpp>
#include <CppDb/details/AtomUtils.hpp>
#include <CppDb/details/MappedFileReadOnly.hpp>
#include <CppDb/Session.hpp>
#include <CppDb/ControllInformation.hpp>
#include <CppDb/Exception.hpp>

#define BOOST_NO_CXX11_SCOPED_ENUMS
# include <boost/filesystem/operations.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace CppAbstractDataBase { namespace Details {
  namespace {
    std::string _GenerateNameForTransactionAccess(std::string const& dbName) {
      return boost::filesystem::path(dbName).filename().string() 
             + "_Transaction_Access_"
             + boost::uuids::to_string(boost::uuids::random_generator()())
      ;
    }
  } /// end unnamed namespace
  
  BaseDataBase::~BaseDataBase() {
    tManagedMappedFile::shrink_to_fit(GetDBFilePath().string().c_str());
  }

  tAtom BaseDataBase::GetDbId() const {
    return m_Id;
  }
  
  tSessionPtr BaseDataBase::CreateSession() {
    auto const result = m_SessionStore.emplace(new Session(shared_from_this()));
    return result.second ? *result.first : nullptr;
  }
  
  void BaseDataBase::CloseSession(tSessionPtr const& session) {
    m_SessionStore.erase(session);
  }
  
  ControllInformation const& BaseDataBase::GetControllInformation() const {
    return m_ControllInformationHolder.GetControllInformation();
  }
  
  uint8_t BaseDataBase::GetBitCount() const {
    return m_ControllInformationHolder.GetBitCount();
  }
  
  tCrcValue BaseDataBase::GetCrc() const {
    return m_ControllInformationHolder.GetCrc();
  }
  
  uint32_t BaseDataBase::GetVersion() const {
    return m_ControllInformationHolder.GetVersion();
  }
  
  std::string const& BaseDataBase::GetDBName() const {
    return m_FileSystemPresenter.GetDBName();
  }
  
  boost::filesystem::path const& BaseDataBase::GetDBFilePath() const {
    return m_FileSystemPresenter.GetDBFilePath();
  }
  
  std::string const& BaseDataBase::GetInfoName() const {
    return m_FileSystemPresenter.GetInfoName();
  }
  
  boost::filesystem::path const& BaseDataBase::GetInfoFilePath() const {
    return m_FileSystemPresenter.GetInfoFilePath();
  }
  
  std::string const& BaseDataBase::GetBackupName() const {
    return m_FileSystemPresenter.GetBackupName();
  }
  
  boost::filesystem::path const& BaseDataBase::GetBackupFilePath() const {
    return m_FileSystemPresenter.GetBackupFilePath();
  }
  
  tSegmentManager* BaseDataBase::GetSegmentManager() {
    return m_Memory.get_segment_manager();
  }
  
  tSegmentManager const* BaseDataBase::GetSegmentManager() const {
    return m_Memory.get_segment_manager();
  }
  
  void BaseDataBase::AcquireTransaction() {
    m_TransactionAccess.lock();
    m_TransactionBusy = true;
  }
  
  void BaseDataBase::ReleaseTransaction() {
    m_TransactionBusy = false;
    _OnReleaseTransaction();
    m_TransactionAccess.unlock();
  }
  
  void BaseDataBase::Grow() {
    tManagedMappedFile::grow(GetDBFilePath().string().c_str(), GetControllInformation().pageSize);
    OnGrow();
  }

  tMemorySize _GetTrueMemorySize(boost::filesystem::path const& filePath, tMemorySize defaultPageSize) {
    return boost::filesystem::exists(filePath) ? boost::filesystem::file_size(filePath) : defaultPageSize;
  }

  BaseDataBase::BaseDataBase(FileSystemStructure const& fsStruct, tControllInformationOptional&& ctrlInfo)
    : m_FileSystemPresenter(fsStruct)
    , m_Id(CreateAtom(GetDBFilePath().string()))
    , m_ControllInformationHolder(GetInfoFilePath(), std::forward<tControllInformationOptional>(ctrlInfo))
    , m_TransactionAccess(
        boost::interprocess::open_or_create
        , _GenerateNameForTransactionAccess(GetDBName()).c_str()
      )
    , m_TransactionBusy(false)
    , m_NeedCreateBackup(false)
    , m_Memory(
        boost::interprocess::open_or_create
        , GetDBFilePath().string().c_str()
        , boost::numeric_cast<tManagedMappedFile::size_type>(
            _GetTrueMemorySize(GetDBFilePath(), GetControllInformation().pageSize)
        )
    )
  {
    _RestartBackUpTimer();
  }
  
  void BaseDataBase::_OnReleaseTransaction() {
    if (m_NeedCreateBackup) {
      _CreateBackup();
    }
  }
  
  void BaseDataBase::_CreateBackup() {
    try {
      boost::filesystem::copy_file(
        GetDBFilePath(), GetBackupFilePath()
        , boost::filesystem::copy_option::overwrite_if_exists
      );
    } catch (boost::filesystem::filesystem_error const& er) {
      (void)er;
      CPP_ABSTRACT_DB_THROW_EXCEPTION(FileSystemError(""));
    }

    {
      MappedFileReadOnly file(GetBackupFilePath());
      m_CrcAlgorithm.process_bytes(file.GetData(), file.GetDataSize());
      m_ControllInformationHolder.SetCrc(m_CrcAlgorithm.checksum());
    }
    
    m_CrcAlgorithm.reset();
    m_NeedCreateBackup = false;
  }
  
  void BaseDataBase::_BackUpTimerTick(boost::system::error_code const& ec) {
    if (ec) {
      return;
    }
    
    if (m_TransactionBusy) {
      m_NeedCreateBackup = true;
    } else {
      boost::interprocess::scoped_lock<tTransactionAccess> const lock(m_TransactionAccess);
      _CreateBackup();
    }
  }
  
  void BaseDataBase::_RestartBackUpTimer() {
    m_BackupTimer.Wait(
      std::bind(&BaseDataBase::_BackUpTimerTick, this, std::placeholders::_1)
      , GetControllInformation().backupTimeout
    );
  }
  
}} /// end namespace CppAbstractDataBase::Details
