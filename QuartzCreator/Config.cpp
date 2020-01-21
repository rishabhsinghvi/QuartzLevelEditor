#include "Config.h"
#include "TextureManager.h"
#include "TileMapManager.h"
#include "AnimationManager.h"

#include "json.hpp"

#include<ShlObj.h> // For SHGetKnownFolderPath 

#include<iostream>
#include<sstream>
#include<fstream>
#include<string>


namespace QuartzCreator
{
	const std::string Config::FILENAME = "Config.json";

	void Config::LoadConfigFromFile()
	{
		m_ConfigPath = CreateConfigDirectory();

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

	void Config::AddToConfig(const TileMapRecord& record)
	{
		m_tileMapRecords.push_back(record);
	}

	void Config::AddToConfig(const TextureRecord& record)
	{
		m_textureRecords.push_back(record);
	}

	void Config::UpdateTextureRecords(TextureManager* tm)
	{
		m_textureRecords.clear();
		const auto& textures = tm->GetTextureList();

		for (const auto& texture : textures)
		{
			TextureRecord record;
			record.m_Name = texture.first;
			record.m_PathToFile = texture.second.m_Path;
			m_textureRecords.push_back(std::move(record));
		}
	}

	void Config::UpdateTileMapRecords(TileMapManager* tm)
	{
		m_tileMapRecords.clear();
		const auto& tilemaps = tm->GetTileMapList();

		for (const auto& tm : tilemaps)
		{
			TileMapRecord t;
			t.m_Name = tm.first;
			t.m_PathToFile = tm.second.m_Path;
			t.m_TextureName = tm.second.m_TextureName;

			m_tileMapRecords.push_back(std::move(t));
		}
	}



	void Config::UpdateAnimationRecords(AnimationManager* am)
	{

	}



	void Config::WriteConfigToFile() const
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

	const std::vector<Config::TileMapRecord>& Config::GetTileMapRecords() const
	{
		return m_tileMapRecords;
	}

	const std::vector<Config::TextureRecord>& Config::GetTextureRecords() const
	{
		return m_textureRecords;
	}

	const std::vector<Config::AnimationRecord>& Config::GetAnimationRecords() const
	{
		return m_animationRecords;
	}

	Config::TextureRecord& Config::GetTextureRecord(const std::string& name)
	{
		for (auto& record : m_textureRecords)
		{
			if (record.m_Name == name)
			{
				return record;
			}
		}
		__debugbreak();
	}

	Config::TileMapRecord& Config::GetTilemapRecord(const std::string& name)
	{
		for (auto& record : m_tileMapRecords)
		{
			if (record.m_Name == name)
			{
				return record;
			}
		}
		__debugbreak();
	}

	Config::AnimationRecord& Config::GetAnimationRecord(const std::string& name)
	{
		for (auto& record : m_animationRecords)
		{
			if (record.m_Name == name)
			{
				return record;
			}
		}
		__debugbreak();
		// TODO: insert return statement here
	}

	void Config::ChangeTextureName(const std::string& prev, const std::string& newN)
	{
		for (auto& x :m_tileMapRecords)
		{
			if (x.m_TextureName == prev)
			{
				x.m_TextureName = newN;
			}
		}
	}

	std::string Config::CreateConfigDirectory() const
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