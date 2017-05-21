#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include <fstream>
#include <sstream>

#include "Utilities.h"

template<typename Derived, typename T>
class ResourceManager
{
public:

	ResourceManager(const std::string& pathFile)
	{
		LoadPaths(pathFile);
	}

	virtual ~ResourceManager()
	{
		PurgeResources();
	}

	T* GetResource(const std::string& id)
	{
		auto resource = Find(id); 
		return resource ? resource->first.get() : nullptr;
	}

	std::string GetPath(const std::string& id)
	{
		auto path = m_path.find(id);
		reeturn path != m_paths.end() ? path->second : "";
	}

	bool RequireResource(const std::string& id)
	{
		//resource already loaded
		auto res = Find(id);
		if (res)
		{
			//then increment the required counter
			++res->second;
			return true;
		}

		//resource not loaded yet so load it
		auto path = m_paths.find(id);
		if (path == m_paths.end())
		{
			std::cerr << "! Failed to find id: " << id << " in paths map" << std::endl;
			return false; //resource path not found
		}

		std::unique_ptr<T> resource(Load(path->second)); //may not work like that
		if (!resource) 
		{
			return false; //loading failed
		}

		//add the new loaded resource 
		m_resources.emplace(id, std::make_pair(std::move(resource), 1));
		return true;
	}

	bool ReleaseResource(const std::string& id)
	{
		auto resource = Find(id);
		//resource not found
		if (!resource)
		{
			return false;
		}

		//decrement required counter
		--resource->second;

		//if the required counter is <= 0 unload the resource
		if (!resource->second)
		{
			Unload(id);
		}
		return true;
	}

	void PurgeResources() //probably not necessary
	{
		while (m_resources.begin() != m_resources.end())
		{
			//delete m_resources.begin()->second.first; //not necessary anymore cause we re using smartpointers
			m_resources.erase(m_resources.begin()); //shouldnt be necessary as well cause its used to iterate through the map
		}
	}

	virtual std::unique_ptr<T> Load(const std::string& path) = 0; //TODO 

	/*

	T* Load(const std::string& path)
	{
		return static_cast<Derived*>(this)->Load(path);
	}
	
	*/

	std::pair<std::unique_ptr<T>, unsigned int>* Find(const std::string& id)
	{
		auto itr = m_resources.find(id);
		return itr != m_resources.end() ? &itr->second : nullptr;
	}

	bool Unload(const std::string& id)
	{
		auto itr = m_resources.find(id);

		if (itr == m_resources.end()) //resource not found
		{
			return false;
		}

		//delete itr->first;
		m_resources.erase(itr);
		return true;
	}

	void LoadPaths(const std::string& pathFile)
	{
		std::ifstream paths;
		paths.open(Utils::GetWorkingDirectory() + pathFile);
		if (paths.is_open()) //file opened
		{
			std::string line;
			while (std::getline(paths, line))
			{
				std::stringstream keystream(line);
				
				std::string pathName;
				std::string path;

				keystream >> pathName;
				keystream >> path;

				m_paths.emplace(pathName, path);
			}
			paths.close();
			return;
		}

		std::cerr << "! Failed to load the path file: " << pathFile << std::endl;
	}

private:
	//key=name value=<pResource,timesRequired>
	//pResource points to objects like textures, fonts, sound files etc.
	std::unordered_map<std::string, std::pair<std::unique_ptr<T>, unsigned int>> m_resources;

	//key=name value=path
	std::unordered_map<std::string, std::string> m_paths;

};