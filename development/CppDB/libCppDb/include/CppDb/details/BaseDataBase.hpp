#if !defined(__CPP_ABSTRACT_DATA_BASE_DETAILS_BASE_DATA_BASE_HPP__)
# define __CPP_ABSTRACT_DATA_BASE_DETAILS_BASE_DATA_BASE_HPP__

#include <CppDb/details/BaseDataBaseFwd.hpp>
#include <CppDb/details/PeriodicTimer.hpp>
#include <CppDb/details/Memory.hpp>
#include <CppDb/details/ControllInformationHolder.hpp>
#include <CppDb/details/FileSystemPresenter.hpp>
#include <CppDb/IDataBase.hpp>
#include <CppDb/Session.hpp>

#include <atomic>
#include <set>

#include <boost/noncopyable.hpp>
#include <boost/interprocess/sync/named_recursive_mutex.hpp>

namespace CppAbstractDataBase { namespace Details {
  class BaseDataBase
    : public IDataBase
    , public boost::noncopyable
  {
    typedef boost::interprocess::named_recursive_mutex tTransactionAccess;
    typedef std::set<tSessionPtr> tSessionStore;
    
  public:
    virtual ~BaseDataBase();
    
    virtual tAtom GetDbId() const override final;
    virtual tSessionPtr CreateSession() override final;
    
    virtual ControllInformation const& GetControllInformation() const override final;
    virtual uint8_t GetBitCount() const override final;
    virtual tCrcValue GetCrc() const override final;
    virtual uint32_t GetVersion() const override final;
    
    virtual std::string const& GetDBName() const override final;
    virtual boost::filesystem::path const& GetDBFilePath() const override final;
    virtual std::string const& GetInfoName() const override final;
    virtual boost::filesystem::path const& GetInfoFilePath() const override final;
    virtual std::string const& GetBackupName() const override final;
    virtual boost::filesystem::path const& GetBackupFilePath() const override final;
    
  protected:
    BaseDataBase(FileSystemStructure const& fsStruct, tControllInformationOptional&& ctrlInfo);
    
    virtual tSegmentManager* GetSegmentManager() override final;
    virtual tSegmentManager const* GetSegmentManager() const override final;
    
    virtual void OnGrow() = 0;
    
    virtual void AcquireTransaction() override final;
    virtual void ReleaseTransaction() override final;
    
    virtual void Grow() override final;
    
    virtual void CloseSession(tSessionPtr const& session) override final;
    
    void _OnReleaseTransaction();
    void _CreateBackup();
    void _BackUpTimerTick(boost::system::error_code const& ec);
    void _RestartBackUpTimer();
    
  protected:
    FileSystemPresenter const m_FileSystemPresenter;
    tAtom const m_Id;
    ControllInformationHolder m_ControllInformationHolder;
    tCrcAlgorithm m_CrcAlgorithm;
    tTransactionAccess m_TransactionAccess;
    PeriodicTimer m_BackupTimer;
    std::atomic_bool m_TransactionBusy;
    std::atomic_bool m_NeedCreateBackup;
    tManagedMappedFile m_Memory;
    tSessionStore m_SessionStore;
  };
}} /// end namespace CppAbstractDataBase::Details

#endif /// !__CPP_ABSTRACT_DATA_BASE_DETAILS_BASE_DATA_BASE_HPP__
