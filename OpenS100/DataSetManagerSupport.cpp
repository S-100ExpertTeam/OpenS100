#include "pch.h"
#include "DataSetManagerSupport.h"
#include "../FeatureCatalog/FeatureCatalogue.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../LibMFCUtil/LibMFCUtil.h"

#include "../GISLibrary/ExchangeCatalogue.h"

#include <string>
#include <pugixml.hpp>


#include <afxwin.h>  // MFC core and standard components
#include <afxdlgs.h> // MFC standard dialog boxes
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <thread>


namespace fs = std::filesystem;

std::unique_ptr<DataSetManagerSupport> DataSetManagerSupport::instance = nullptr;

DataSetManagerSupport::DataSetManagerSupport()
{
	m_db = nullptr;
	ConnectDB();

	CreateFCTable();
	CreatePCTable();

	CreateDSTable();
	CreateSPTable();
}

DataSetManagerSupport::~DataSetManagerSupport()
{
	if (m_db) {
		DisConnectDB();
		m_db = nullptr;
	}
}

void DataSetManagerSupport::CreateCatalogueFile(std::string filePath, S100::ExchangeCatalogue& ec)
{
	fs::path pathObj(filePath);
	fs::path dirPath = pathObj.parent_path();

	if (!dirPath.empty() && !fs::exists(dirPath)) {
		fs::create_directories(dirPath);
	}

	if (fs::exists(filePath)) {
		if (tryDeleteFile(filePath))
		{
			std::ofstream file(filePath);
			if (file.is_open()) {
				file.close();
			}
		}
	}
}


bool DataSetManagerSupport::tryDeleteFile(const std::string& filePath, int maxAttempts, int delaySeconds) {
	for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
		
		if (fs::remove(filePath)) {
			return true;
		}
		else {
			if (attempt < maxAttempts) {
				std::this_thread::sleep_for(std::chrono::seconds(delaySeconds));
			}
		}
	}
	return false;
}



void DataSetManagerSupport::OpenFolder(const CString& path)
{
	CString folderPath = path;

	PathRemoveFileSpec(folderPath.GetBuffer(MAX_PATH));
	folderPath.ReleaseBuffer();

	HINSTANCE result = ShellExecute(NULL, _T("open"), folderPath, NULL, NULL, SW_SHOWDEFAULT);

	if (reinterpret_cast<int>(result) <= 32)
	{
		CString errorMessage;
		errorMessage.Format(_T("Can't open folder. Error Code : %d"), GetLastError());
		AfxMessageBox(errorMessage, MB_ICONERROR);
	}
}



void DataSetManagerSupport::ExtractFileNameAndPath(const std::string& fullPath, std::string& outFileName, std::string& outFilePath) {
	size_t pos = fullPath.find_last_of("\\/");
	if (pos != std::string::npos) {
		outFileName = fullPath.substr(pos + 1);
		outFilePath = fullPath.substr(0, pos);
	}
	else {
		outFileName = fullPath;
		outFilePath = "";
	}
}


std::string DataSetManagerSupport::ExtractPath(const std::string& fullPath) {
	std::string prefix = "file:/";
	size_t start = fullPath.find(prefix);
	if (start == std::string::npos) {
		return "";
	}

	start += prefix.length();

	size_t end = fullPath.rfind('/');
	if (end == std::string::npos || end <= start) {
		return "";
	}

	return fullPath.substr(start, end - start);
}


std::string DataSetManagerSupport::RemoveFileName(const std::string& path) {
	size_t lastSlashPos = path.find_last_of("\\/");
	if (lastSlashPos != std::string::npos) {
		return path.substr(0, lastSlashPos);
	}
	return path; 
}


std::vector<std::shared_ptr<DatasetClass>> DataSetManagerSupport::Dataset_LoadFile(std::string filepath)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filepath.c_str());
	pugi::xml_node nodeList = doc.first_child();
	std::vector<std::shared_ptr<DatasetClass>> vec;

	const pugi::char_t* instructionName = nodeList.name();
	std::string str = instructionName;
	if ((std::string::npos != str.find("S100_ExchangeCatalogue")) || (std::string::npos != str.find("S-101_ExchangeCatalogue")))
	{
		S100::ExchangeCatalogue ex;
		ex.Open(filepath);
		
		for (int i = 0; i < ex.DatasetDiscoveryMetadata.size(); i++)
		{
			DatasetClass dc;

			std::string folderPath = RemoveFileName(filepath);

			std::string adjustedRelativePath = ex.DatasetDiscoveryMetadata[i].FileName;
			size_t prefixPos = adjustedRelativePath.find("file:/");
			if (prefixPos != std::string::npos) {
				adjustedRelativePath.erase(prefixPos, 6);
			}
			std::replace(adjustedRelativePath.begin(), adjustedRelativePath.end(), '/', '\\');

			string file;
			string path;
			ExtractFileNameAndPath(ex.DatasetDiscoveryMetadata[i].FileName, file, path);

			dc.fileName = file;
			dc.filePath = folderPath + "\\" + adjustedRelativePath;
			dc.DS_SP = false;

			vec.push_back(make_shared<DatasetClass>(dc));
		}


		for (int i = 0; i < ex.SupportFileDiscoveryMetadata.size(); i++)
		{
			DatasetClass dc;

			std::string folderPath = RemoveFileName(filepath);

			std::string adjustedRelativePath = ex.SupportFileDiscoveryMetadata[i].FileName;
			size_t prefixPos = adjustedRelativePath.find("file:/");
			if (prefixPos != std::string::npos) {
				adjustedRelativePath.erase(prefixPos, 6); 
			}
			std::replace(adjustedRelativePath.begin(), adjustedRelativePath.end(), '/', '\\');

			string file;
			string path;
			ExtractFileNameAndPath(ex.SupportFileDiscoveryMetadata[i].FileName, file, path);

			dc.fileName = file;
			dc.filePath = folderPath + "\\" + adjustedRelativePath;
			dc.DS_SP = true;

			vec.push_back(make_shared<DatasetClass>(dc));
		}

		return vec;
	}
	return vec;
}


std::shared_ptr<FC_PC_DatasetClass> DataSetManagerSupport::FC_PC_LoadFile(std::string filepath)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filepath.c_str());
	pugi::xml_node nodeList = doc.first_child();

	const pugi::char_t* instructionName = nodeList.name();
	std::string str = instructionName;
	if (std::string::npos != str.find("S100_FC_FeatureCatalogue"))
	{
		FeatureCatalogue fc;
		auto tempfilepath = LibMFCUtil::StringToWString(filepath);
		fc.Read(tempfilepath);

		FC_PC_DatasetClass dc;
		//FC
		dc.FC_PC = false;
		dc.product = LibMFCUtil::WStringToString(fc.GetProductId());
		auto tmp = fc.getVersion();
		dc.version = tmp.source;

		return std::make_shared<FC_PC_DatasetClass>(dc);
	}
	else if (std::string::npos != str.find("portrayalCatalog"))
	{
		PortrayalCatalogue pc;
		auto tempfilepath = LibMFCUtil::StringToWString(filepath);
		pc.Read(tempfilepath);

		FC_PC_DatasetClass dc;
		//PC
		dc.FC_PC = true;
		dc.product = LibMFCUtil::WStringToString(pc.GetProduct());
		dc.version = LibMFCUtil::WStringToString(pc.GetVersion());

		return std::make_shared<FC_PC_DatasetClass>(dc);
	}

	return nullptr;
}


void DataSetManagerSupport::CreateFCTable()
{
	const char* sqlCreateTable =
		"CREATE TABLE IF NOT EXISTS FeatureCatalogue ("
		"idx INTEGER PRIMARY KEY AUTOINCREMENT, "
		"fileName TEXT NOT NULL, "
		"filePath TEXT NOT NULL UNIQUE, "
		"product TEXT NOT NULL, "
		"version TEXT NOT NULL);";

	char* errMsg = nullptr;
	int rc = sqlite3_exec(m_db, sqlCreateTable, nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		TRACE("SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
	}
	else {
		TRACE("FeatureCatalogue Table created successfully");
	}
}


void DataSetManagerSupport::CreatePCTable()
{
	const char* sqlCreateTable =
		"CREATE TABLE IF NOT EXISTS PortrayalCatalogue ("
		"idx INTEGER PRIMARY KEY AUTOINCREMENT, "
		"fileName TEXT NOT NULL, "
		"filePath TEXT NOT NULL UNIQUE, "
		"product TEXT NOT NULL, "
		"version TEXT NOT NULL);";

	char* errMsg = nullptr;
	int rc = sqlite3_exec(m_db, sqlCreateTable, nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		TRACE("SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
	}
	else {
		TRACE("PortrayalCatalogue Table created successfully");
	}
}


void DataSetManagerSupport::CreateDSTable()
{
	const char* sqlCreateTable =
		"CREATE TABLE IF NOT EXISTS DatasetFiles ("
		"idx INTEGER PRIMARY KEY AUTOINCREMENT, "
		"fileName TEXT NOT NULL, "
		"filePath TEXT NOT NULL UNIQUE "
		");";

	char* errMsg = nullptr;
	int rc = sqlite3_exec(m_db, sqlCreateTable, nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		TRACE("SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
	}
	else {
		TRACE("FeatureCatalogue Table created successfully");
	}
}

void DataSetManagerSupport::CreateSPTable()
{
	const char* sqlCreateTable =
		"CREATE TABLE IF NOT EXISTS SupportFiles ("
		"idx INTEGER PRIMARY KEY AUTOINCREMENT, "
		"fileName TEXT NOT NULL, "
		"filePath TEXT NOT NULL UNIQUE "
		");";

	char* errMsg = nullptr;
	int rc = sqlite3_exec(m_db, sqlCreateTable, nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		TRACE("SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
	}
	else {
		TRACE("FeatureCatalogue Table created successfully");
	}
}

void DataSetManagerSupport::insertData(std::string table, const std::string& fileName, const std::string& filePath) {
	std::string sqlInsert =
		"INSERT INTO " + table + " (fileName, filePath) "
		"VALUES (?, ?);";

	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(m_db, sqlInsert.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_text(stmt, 1, fileName.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, filePath.c_str(), -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		TRACE("Could not insert data: %s", sqlite3_errmsg(m_db));
	}
	else {
		TRACE("Data inserted successfully");
	}
	sqlite3_finalize(stmt);
}



void DataSetManagerSupport::insertData(std::string table, const std::string& fileName, const std::string& filePath, const std::string& product, const std::string& version) {
	std::string sqlInsert =
		"INSERT INTO "+ table +" (fileName, filePath, product, version) "
		"VALUES (?, ?, ?, ?);";

	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(m_db, sqlInsert.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_text(stmt, 1, fileName.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 2, filePath.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, product.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 4, version.c_str(), -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		TRACE("Could not insert data: %s", sqlite3_errmsg(m_db));
	}
	else {
		TRACE("Data inserted successfully");
	}
	sqlite3_finalize(stmt);
}


std::shared_ptr<FC_PC_DatasetClass>  DataSetManagerSupport::selectDataByFilePath(std::string table, const std::string& targetFilePath) {
	std::string  sqlSelect = "SELECT * FROM "+ table +" WHERE filePath = ?;";
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(m_db, sqlSelect.c_str(), -1, &stmt, nullptr);

	if (rc != SQLITE_OK) {
		TRACE("Cannot prepare select statement: %s",sqlite3_errmsg(m_db));
	}

	sqlite3_bind_text(stmt, 1, targetFilePath.c_str(), -1, SQLITE_TRANSIENT);

	if (sqlite3_step(stmt) == SQLITE_ROW) {

		FC_PC_DatasetClass dt;

		dt.fileName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		dt.filePath = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		dt.product = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		dt.version = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
		dt.FC_PC = table == "FeatureCatalogue" ? false : true;

		sqlite3_finalize(stmt);
		return std::make_shared<FC_PC_DatasetClass>(dt);

	}
	else {
		TRACE("No data found for the specified filePath: %s", targetFilePath);
	}

	sqlite3_finalize(stmt);
	return nullptr;
}



std::vector<std::shared_ptr<DatasetClass>> DataSetManagerSupport::selectAllDataDS_SP(std::string table) {
	std::vector<std::shared_ptr<DatasetClass>> data;

	std::string sqlSelect = "SELECT * FROM " + table + ";";
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(m_db, sqlSelect.c_str(), -1, &stmt, nullptr);

	if (rc != SQLITE_OK) {
		TRACE("Cannot prepare select statement: %s", sqlite3_errmsg(m_db));
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {

		DatasetClass dt;

		dt.fileName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		dt.filePath = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		dt.DS_SP = table == "DatasetFiles" ? false : true;

		data.push_back(std::make_shared<DatasetClass>(dt));
	}

	sqlite3_finalize(stmt);
	return data;
}


std::vector<std::shared_ptr<FC_PC_DatasetClass>> DataSetManagerSupport::selectAllData(std::string table) {
	std::vector<std::shared_ptr<FC_PC_DatasetClass>> data;

	std::string sqlSelect = "SELECT * FROM " + table + ";";
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(m_db, sqlSelect.c_str(), -1, &stmt, nullptr);

	if (rc != SQLITE_OK) {
		TRACE("Cannot prepare select statement: %s", sqlite3_errmsg(m_db));
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {

		FC_PC_DatasetClass dt;

		dt.fileName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		dt.filePath = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		dt.product = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		dt.version = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
		dt.FC_PC = table == "FeatureCatalogue" ? false : true;

		data.push_back(std::make_shared<FC_PC_DatasetClass>(dt));
	}

	sqlite3_finalize(stmt);
	return data;
}


void DataSetManagerSupport::deleteAllDataFromTable(const std::string& table) {
	std::string sqlDelete = "DELETE FROM " + table + ";";

	sqlite3_stmt* stmt;
	if (sqlite3_prepare_v2(m_db, sqlDelete.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		TRACE("Could not prepare delete statement: %s", sqlite3_errmsg(m_db));
		return;
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		TRACE("Could not delete data: %s", sqlite3_errmsg(m_db));
	}
	else {
		TRACE("All data deleted successfully from %s", table.c_str());
	}

	sqlite3_finalize(stmt);
}

void DataSetManagerSupport::ConnectDB()
{
	char* errMsg = 0;
	int rc;

	rc = sqlite3_open("..\\TEMP\\DatasetManager.db", &m_db);
	if (rc) {
		TRACE("Fail Connect Database: %s\n", sqlite3_errmsg(m_db));
	}
	else {
		TRACE("Success Connect Database\n");
	}
}


void DataSetManagerSupport::DisConnectDB()
{
	if (m_db != nullptr)
		sqlite3_close(m_db);
}


std::vector<std::shared_ptr<FC_PC_DatasetClass>> DataSetManagerSupport::LoadFC_PC_DB()
{
	std::vector<std::shared_ptr<FC_PC_DatasetClass>> data;

	//SetFC_PC_DBFrame()

	return data;
}


std::vector<std::string> DataSetManagerSupport::initFCList()
{
	std::vector<std::string> vec;

	auto temp = selectAllData("FeatureCatalogue");
	for (int i = 0; i < temp.size(); i++)
	{
		if (PathFileExists(LibMFCUtil::StringToWString(temp[i]->filePath).c_str()))
			vec.push_back(temp[i]->filePath);
	}

	return vec;
}


std::vector<std::string> DataSetManagerSupport::initPCList()
{
	std::vector<std::string> vec;

	auto temp = selectAllData("PortrayalCatalogue");
	for (int i = 0; i < temp.size(); i++)
	{
		if (PathFileExists(LibMFCUtil::StringToWString(temp[i]->filePath).c_str()))
			vec.push_back(temp[i]->filePath);
	}

	return vec;
}


std::vector<std::string> DataSetManagerSupport::initDataFileList()
{
	std::vector<std::string> vec;

	auto temp = selectAllDataDS_SP("DatasetFiles");
	for (int i = 0; i < temp.size(); i++)
	{
		if (PathFileExists(LibMFCUtil::StringToWString(temp[i]->filePath).c_str()))
			vec.push_back(temp[i]->filePath);
	}

	return vec;
}
