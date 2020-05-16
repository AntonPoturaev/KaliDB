#include "stdafx.h"
#include <CppDb/details/FileSystemPresenter.hpp>
#include <CppDb/FileSystemStructure.hpp>
#include <CppDb/details/ControllInformationHolder.hpp>
#include <CppDb/ControllInformation.hpp>
#include <CppDb/details/AtomUtils.hpp>
#include <CppDb/Exception.hpp>

#include <cassert>

#include <boost/filesystem/operations.hpp>

namespace CppAbstractDataBase { namespace Details {
  namespace {
    FileSystemStructure::NamePolicy const& _ExstractNamePolicy(FileSystemStructure const& fsStruct) {
      return fsStruct.namePolicyOpt ? *fsStruct.namePolicyOpt : FileSystemStructure::NamePolicy::GetDefault();
    }
  } /// end unnamed namespace

  FileSystemPresenter::FileSystemPresenter(FileSystemStructure const& fsStruct) {
    if (fsStruct.rootDir.empty()) {
      CPP_ABSTRACT_DB_THROW_EXCEPTION(BadParam("Path to root directory for DataBase is empty!"));
    }
    
    if (!boost::filesystem::exists(fsStruct.rootDir)) {
      CPP_ABSTRACT_DB_THROW_EXCEPTION(FileSystemError("Path to root directory for DataBase is not exists!"));
    }
    
    if (!boost::filesystem::is_directory(fsStruct.rootDir)) {
      CPP_ABSTRACT_DB_THROW_EXCEPTION(FileSystemError("Path to root directory for DataBase is not directory!"));
    }
    
    auto const& namePolicy = _ExstractNamePolicy(fsStruct);
    
    if ((m_DbName = namePolicy.dbFileName).empty()) {
      CPP_ABSTRACT_DB_THROW_EXCEPTION(BadParam("Bad value of NamePolicy.dbFileName - is empty!"));
    }
    
    if ((m_InfoName = namePolicy.infoFileName).empty()) {
      CPP_ABSTRACT_DB_THROW_EXCEPTION(BadParam("Bad value of NamePolicy.infoFileName - is empty!"));
    }
    
    if ((m_BackupName = namePolicy.backupFileName).empty()) {
      CPP_ABSTRACT_DB_THROW_EXCEPTION(BadParam("Bad value of NamePolicy.backupFileName - is empty!"));
    }
    
    boost::filesystem::path const dbDirPath = fsStruct.rootDir / (m_DbName = fsStruct.dbName);
    
    if (boost::filesystem::exists(dbDirPath)) {
      if (!boost::filesystem::is_directory(dbDirPath)) {
        CPP_ABSTRACT_DB_THROW_EXCEPTION(FileSystemError("Path to DataBase directory is exists, but is not directory!"));
      }
      
      m_DbPath = dbDirPath / namePolicy.dbFileName;
      
      if (boost::filesystem::exists(m_DbPath)) {
        if (!boost::filesystem::is_directory(m_DbPath)) {
          CPP_ABSTRACT_DB_THROW_EXCEPTION(FileSystemError("Path to DataBase file is exists, but is not regular file!"));
        }
      }

      m_InfoPath = dbDirPath / namePolicy.infoFileName;
      
      if (boost::filesystem::exists(m_InfoPath)) {
        if (!boost::filesystem::is_directory(m_InfoPath)) {
          CPP_ABSTRACT_DB_THROW_EXCEPTION(FileSystemError("Path to DataBase information file is exists, but is not regular file!"));
        }
      }
      
      m_BackupPath = dbDirPath / namePolicy.backupFileName;
      
      if (boost::filesystem::exists(m_BackupPath)) {
        if (!boost::filesystem::is_directory(m_BackupPath)) {
          CPP_ABSTRACT_DB_THROW_EXCEPTION(FileSystemError("Path to DataBase backup file is exists, but is not regular file!"));
        }
      }
    } else {
      boost::filesystem::create_directory(dbDirPath);
    }
  }
  
  std::string const& FileSystemPresenter::GetDBName() const {
    return m_DbName;
  }
  
  boost::filesystem::path const& FileSystemPresenter::GetDBFilePath() const {
    return m_DbPath;
  }
  
  const std::string& FileSystemPresenter::GetInfoName() const {
    return m_InfoName;
  }
  
  boost::filesystem::path const& FileSystemPresenter::GetInfoFilePath() const {
    return m_InfoPath;
  }
  
  const std::string& FileSystemPresenter::GetBackupName() const {
    return m_BackupName;
  }
  
  boost::filesystem::path const& FileSystemPresenter::GetBackupFilePath() const {
    return m_BackupPath;
  }
}} /// end namespace CppAbstractDataBase::Details
