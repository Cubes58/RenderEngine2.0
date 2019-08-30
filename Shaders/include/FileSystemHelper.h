#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

struct FileInformation {
	std::string m_Name;
	std::string m_Extension;
	std::string m_Location;
};

class FileSystemHelper {
private:
	FileSystemHelper() = default;
	~FileSystemHelper() = default;

	struct PathLeafInformation {
		FileInformation operator()(const std::filesystem::directory_entry &p_Entry) const {
			FileInformation fileInformation;

			// Add the Name of the file.
			fileInformation.m_Name = p_Entry.path().filename().string();
			RemoveFileExtension(fileInformation.m_Name);

			// Add the Extension of the file.
			fileInformation.m_Extension = p_Entry.path().extension().string();

			// Add the path of the file.
			fileInformation.m_Location = p_Entry.path().string();

			return fileInformation;
		}
	};

public:
	static std::vector<FileInformation> GetFilesInFolder(const std::string &p_FolderLocation, bool p_SearchSubDirectories = true) {
		std::vector<FileInformation> files;

		if (!DoesLocationExist(p_FolderLocation))
			return files;

		std::filesystem::path pathLocation(p_FolderLocation);
		if (!p_SearchSubDirectories) {
			std::filesystem::directory_iterator start(pathLocation);
			std::filesystem::directory_iterator end;
			std::transform(start, end, std::back_inserter(files), PathLeafInformation());
		}
		else {
			std::filesystem::recursive_directory_iterator rStart(pathLocation);
			std::filesystem::recursive_directory_iterator rEnd;
			std::transform(rStart, rEnd, std::back_inserter(files), PathLeafInformation());
		}

		return files;
	}

	static void RetainRemoveFilesWithExtensions(std::vector<std::string> &p_Files, const std::vector<std::string> &p_FileExtensions, bool p_KeepFile = true) {
		p_Files.erase(std::remove_if(p_Files.begin(), p_Files.end(), [&](std::string p_File)->bool {

			size_t extensionStartPosition = p_File.find_last_of(".");
			// Enusre it's a file, otherwise it won't have a file type.
			if (extensionStartPosition == std::string::npos) {
				return true;
			}

			std::string fileExtension = p_File.substr(extensionStartPosition, p_File.size() - 1);
			for (auto &extension : p_FileExtensions) {
				if (fileExtension == extension && p_KeepFile)
					return false;
				if (fileExtension == extension && !p_KeepFile)
					return true;
			}

			return p_KeepFile;
		}), p_Files.end());
	}

	static void RetainRemoveFilesWithExtensions(std::vector<FileInformation> &p_Files, const std::vector<std::string> &p_FileExtensions, bool p_KeepFile = true) {
		p_Files.erase(std::remove_if(p_Files.begin(), p_Files.end(), [&](FileInformation p_File)->bool {
			for (auto &extension : p_FileExtensions) {
				if (p_File.m_Extension == extension && p_KeepFile)
					return false;
				if (p_File.m_Extension == extension && !p_KeepFile)
					return true;
			}

			return p_KeepFile;
		}), p_Files.end());
	}

	static bool RemoveFileExtension(std::string &p_FileName) {
		size_t firstPosition = p_FileName.find_last_of('.');

		if (firstPosition == std::string::npos)
			return false;

		p_FileName = p_FileName.substr(0, firstPosition);
		return true;
	}

	static bool RemoveFilePath(std::string &p_FileName) {
		size_t firstPosition = p_FileName.find_last_of('/');
		size_t firstPositionOther = p_FileName.find_last_of("\\");

		if (firstPosition == std::string::npos && firstPositionOther == std::string::npos)
			return false;

		bool canCheckFirstPosition = false;
		if (firstPosition != std::string::npos)
			canCheckFirstPosition = true;

		bool canCheckFirstPositionOther = false;
		if (firstPositionOther != std::string::npos)
			canCheckFirstPositionOther = true;

		if ((canCheckFirstPosition && canCheckFirstPositionOther) && (firstPosition >= firstPositionOther))
			p_FileName = p_FileName.substr(firstPosition + 1, p_FileName.size() - firstPosition);
		else if ((canCheckFirstPosition && canCheckFirstPositionOther) && (firstPosition <= firstPositionOther))
			p_FileName = p_FileName.substr(firstPositionOther + 1, p_FileName.size() - firstPosition);
		else if (canCheckFirstPosition)
			p_FileName = p_FileName.substr(firstPosition + 1, p_FileName.size() - firstPosition);
		else
			p_FileName = p_FileName.substr(firstPositionOther + 1, p_FileName.size() - firstPosition);

		return true;
	}

	static std::string GetNameFromFile(std::string p_FileName) {
		RemoveFileExtension(p_FileName);
		RemoveFilePath(p_FileName);

		return p_FileName;
	}

	static bool DoesLocationExist(const std::string &p_FileName) {
		struct stat info;
		if (stat(p_FileName.c_str(), &info) != 0) {
			std::cerr << p_FileName << " cannot be accessed." <<
				"\nMake sure it exists." << std::endl;

			return false;
		}
		return true;
	}

	// Delete the copy and assignment operators.
	FileSystemHelper(FileSystemHelper const&) = delete; //!< Copy operator, deleted.
	FileSystemHelper& operator=(FileSystemHelper const&) = delete; //!< Assignment operator, deleted.
};