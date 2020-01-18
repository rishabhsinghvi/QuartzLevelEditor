#include "Config.h"

#include "json.hpp"

#include<ShlObj.h> // For SHGetKnownFolderPath 

#include<iostream>
#include<sstream>
#include<fstream>
#include<string>


namespace QuartzCreator
{
	const std::string Config::FILENAME = "Config.json";

	void Config::loadConfigFromFile()
	{
		m_ConfigPath = createConfigDirectory();

		std::ifstream file(m_ConfigPath + FILENAME);
		
		if (!file)
		{
			std::cout << "Config file not found. A new config file will be created.\n";
			return;
		}

		using json = nlohmann::json;

		json root;

		file >> root;

		const auto& textures = root["TEXTURE_RECORDS"];

		for (const auto& texture : textures)
		{
			std::string textureName = texture["TEXTURE_NAME"];
			std::string texturePath = texture["TEXTURE_PATH"];

			m_textureRecords.push_back({ std::move(textureName), std::move(texturePath) });
		}

		const auto& tileMaps = root["TILEMAP_RECORDS"];

		for (const auto& tileMap : tileMaps)
		{
			std::string tileMapName = tileMap["TILEMAP_NAME"];
			std::string tileMapTextureName = tileMap["TILEMAP_TEXTURE_NAME"];
			std::string tileMapFilePath = tileMap["TILEMAP_FILE_PATH"];
			m_tileMapRecords.push_back({ std::move(tileMapName), std::move(tileMapTextureName), std::move(tileMapFilePath) });
		}

		std::cout << "Config: Read config successfully\n";

	}

	void Config::addToConfig(const TileMapRecord& record)
	{
		m_tileMapRecords.push_back(record);
	}

	void Config::addToConfig(const TextureRecord& record)
	{
		m_textureRecords.push_back(record);
	}

	void Config::writeConfigToFile() const
	{
		using json = nlohmann::json;

		auto textureArray = json::array();

		for (const auto& x : m_textureRecords)
		{
			json ob;
			ob["TEXTURE_NAME"] = x.m_Name;
			ob["TEXTURE_PATH"] = x.m_PathToFile;

			textureArray.push_back(ob);
		}

		auto mapArray = json::array();

		for (const auto& x : m_tileMapRecords)
		{
			json ob;
			ob["TILEMAP_NAME"] = x.m_Name;
			ob["TILEMAP_TEXTURE_NAME"] = x.m_TextureName;
			ob["TILEMAP_FILE_PATH"] = x.m_PathToFile;
			mapArray.push_back(ob);
		}

		json root;

		root["TEXTURE_RECORDS"] = textureArray;
		root["TILEMAP_RECORDS"] = mapArray;

		std::ofstream file(m_ConfigPath + FILENAME);
		
		file << root;

	}

	const std::vector<Config::TileMapRecord>& Config::getTileMapRecords() const
	{
		return m_tileMapRecords;
	}

	const std::vector<Config::TextureRecord>& Config::getTextureRecords() const
	{
		return m_textureRecords;
	}

	std::string Config::createConfigDirectory() const
	{
		// Not platform agnostic, need to change it later!
		PWSTR path = nullptr;

		HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path);

		std::wstringstream ss;
		ss << path << L"\\QuartzCreator\\";

		CoTaskMemFree(path);

		auto wString = ss.str();

		auto directoryPath = wString.c_str();

		if (CreateDirectory(directoryPath, nullptr) || ERROR_ALREADY_EXISTS == GetLastError())
		{
			return std::string(wString.begin(), wString.end());
		}
		
		// If AppData folder not found, will default to current working directory
		return "";
	}
}