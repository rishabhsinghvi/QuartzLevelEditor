#ifndef CONFIG_H
#define CONFIG_H

#include<vector>
#include<string>


namespace QuartzCreator
{
	class Config
	{
	public:
		struct TileMapRecord
		{
			std::string m_Name;
			std::string m_TextureName;
			std::string m_PathToFile;

			TileMapRecord() = default;

			TileMapRecord(const std::string& name, const std::string& tex, const std::string& path) :
				m_Name(name), m_TextureName(tex), m_PathToFile(path)
			{

			}
		};

		struct TextureRecord
		{
			std::string m_Name;
			std::string m_PathToFile;

			TextureRecord() = default;

			TextureRecord(const std::string& name, const std::string& path):
				m_Name(name), m_PathToFile(path)
			{

			}
		};

		// TODO: Add entity record
	
	private:
		static const std::string FILENAME;
		std::vector<TileMapRecord> m_tileMapRecords;
		std::vector<TextureRecord> m_textureRecords;
		std::string m_ConfigPath;

	public:

		void loadConfigFromFile();
	
		void addToConfig(const TileMapRecord& record);

		void addToConfig(const TextureRecord& record);

		void writeConfigToFile() const;

		const std::vector<TileMapRecord>& getTileMapRecords() const;

		const std::vector<TextureRecord>& getTextureRecords() const;

		TextureRecord& getTextureRecord(const std::string& name);

		TileMapRecord& getTilemapRecord(const std::string& name);

		void changeTextureName(const std::string& prev, const std::string& newN);

	private:

		std::string createConfigDirectory() const;


	};
}

#endif