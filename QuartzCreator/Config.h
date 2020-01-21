#ifndef CONFIG_H
#define CONFIG_H

#include<vector>
#include<string>


namespace QuartzCreator
{
	class TextureManager;
	class AnimationManager;
	class TileMapManager;

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
		
		struct AnimationRecord
		{
			struct AnimFrameInfo
			{
				unsigned int m_X;
				unsigned int m_Y;
				unsigned int m_W;
				unsigned int m_H;
				unsigned int m_Dur;
			};

			std::string m_Name;
			bool m_Looped;
			std::vector<AnimFrameInfo> m_FrameInfo;
		};

		// TODO: Add entity record
	
	private:
		static const std::string FILENAME;
		std::vector<TileMapRecord> m_tileMapRecords;
		std::vector<TextureRecord> m_textureRecords;
		std::vector<AnimationRecord> m_animationRecords;
		std::string m_ConfigPath;

	public:

		void LoadConfigFromFile();
	
		void AddToConfig(const TileMapRecord& record);

		void AddToConfig(const TextureRecord& record);

		void UpdateTextureRecords(TextureManager* tm);

		void UpdateTileMapRecords(TileMapManager* tm);

		void UpdateAnimationRecords(AnimationManager* am);

		void WriteConfigToFile() const;

		const std::vector<TileMapRecord>& GetTileMapRecords() const;

		const std::vector<TextureRecord>& GetTextureRecords() const;

		const std::vector<AnimationRecord>& GetAnimationRecords() const;

		TextureRecord& GetTextureRecord(const std::string& name);

		TileMapRecord& GetTilemapRecord(const std::string& name);

		AnimationRecord& GetAnimationRecord(const std::string& name);

		void ChangeTextureName(const std::string& prev, const std::string& newN);

	private:

		std::string CreateConfigDirectory() const;


	};
}

#endif