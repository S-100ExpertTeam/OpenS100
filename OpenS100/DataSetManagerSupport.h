#pragma once
#include <string>
#include <memory>
#include <sqlite3.h>
#include <vector>

namespace S100
{
	class ExchangeCatalogue;
	class DataCoverage;
	class ProductSpecification;
	class EX_GeographicBoundingBox;
}


class FC_PC_DatasetClass
{
public:
	int idx;
	bool OnOff;
	std::string fileName;
	std::string filePath;
	std::string product;
	std::string version;
	bool FC_PC;
	bool ExistPath;
};


class DatasetClass
{
public:
	std::string fileName;
	std::string filePath;
	bool DS_SP;
	bool ExistPath;

	std::string Name;
	std::string commonfilePath;
	std::vector<S100::DataCoverage> datacoverage;
	std::shared_ptr <S100::ProductSpecification> productSpecification;
	std::shared_ptr<S100::EX_GeographicBoundingBox> BoundingBox;
	
};

//class DatasetFileClass : public DatasetClass
//{
//public:
//	
//};
//
//class SupportFileClass : public DatasetClass
//{
//public :
//};


class DataSetManagerSupport
{
private:
	static std::unique_ptr<DataSetManagerSupport> instance; 
	sqlite3* m_db;

	DataSetManagerSupport();
	DataSetManagerSupport(const DataSetManagerSupport&) = delete;
	DataSetManagerSupport& operator=(const DataSetManagerSupport&) = delete;

public:

	~DataSetManagerSupport();

	static DataSetManagerSupport& GetInstance() {
		if (!instance) {
			instance = std::unique_ptr<DataSetManagerSupport>(new DataSetManagerSupport());
		}
		return *instance;
	}
	//Path
	CString m_FolderPath = L"..\\TEMP\\";
	CString m_FileName = L"IniCatalog.XML";



	std::string RemoveFileName(const std::string& path);
	void OpenFolder(const CString& folderPath);
	void ExtractFileNameAndPath(const std::string& fullPath, std::string& outFileName, std::string& outFilePath);
	std::string ExtractPath(const std::string& fullPath);

	// FC_PC
	std::shared_ptr<FC_PC_DatasetClass> FC_PC_LoadFile(std::string filepath);
	std::vector<std::shared_ptr<DatasetClass>> Dataset_LoadFile(std::string filepath);

	// init
	std::vector<std::string> initFCList();
	std::vector<std::string> initPCList();
	std::vector<std::string> initDataFileList();

	// Catalogue.xml
	void CreateCatalogueFile(std::string folderPath, S100::ExchangeCatalogue& ec);
	bool tryDeleteFile(const std::string& filePath, int maxAttempts = 3, int delaySeconds = 5);

	void DeleteUnlistedFilesAndFolders(const CString& baseFolderPath, const std::vector<CString>& keepRelativePaths);
	void DeleteContents(const CString& folderPath, const std::vector<CString>& keepPaths);
	bool CopyFileWithChecks(const CString& sourcePath, const CString& targetPath);
	bool CreateNestedDirectory(const CString& directoryPath);
	void mkdirs(const CString directory);



	// DB
	std::vector<std::shared_ptr<FC_PC_DatasetClass>>  LoadFC_PC_DB();

	void ConnectDB();
	void DisConnectDB();

	void CreateFCTable();
	void CreatePCTable();
	void CreateDSTable(); 
	void CreateSPTable();

	void insertData(std::string table, const std::string& fileName, const std::string& filePath, const std::string& product, const std::string& version);
	void insertData(std::string table, const std::string& fileName, const std::string& filePath);

	//void updateData(std::string table, const std::string& fileName, const std::string& filePath, const std::string& product, const std::string& version);
	//void deleteData(std::string table, const std::string& filePath);
	void deleteAllDataFromTable(const std::string& table);

	std::vector<std::shared_ptr<FC_PC_DatasetClass>> selectAllData(std::string table);
	std::vector<std::shared_ptr<DatasetClass>> selectAllDataDS_SP(std::string table);

	std::shared_ptr<FC_PC_DatasetClass> selectDataByFilePath(std::string table, const std::string& targetFilePath);
	
};


