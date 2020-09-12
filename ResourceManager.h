#pragma once
#include <string>
#include <unordered_map>

//when class use template, all its contest should be written as header altogether
template<typename Derived, typename T>
class ResourceManager
{
public:
	ResourceManager(const std::string& l_pathsFIle)
	{
		LoadPaths(l_pathsFile);
	}

	virtual ~ResourceManager() { PurgeResources(); }

	T* GetResource(const std::string& l_id)
	{
		auto res = Find(l_id);
		return(res ? res->first : nullptr);
	}

	std::string GerPath(const std::string& l_id)
	{
		auto path = m_paths.find(l_id);
		return(path != m_paths.end() ? path->second : " ");
	}

	bool RequireResource(const std::string& l_id)
	{
		auto res = Find(l_id);
		if (res)
		{
			++res->second;
			return true;
		}
		auto path = m_paths.find(l_id);
		if (path == m_paths.end()) { return false; }
		T* resource = Load(path->second);
		if (!resource) { return false; }
		m_resources.emplace(l_id, std::make_pair(resource, 1));
		return true;
	} 

	bool ReleaseResource(const std::string& l_id)
	{
		auto res = Find(l_id);
		if (!res) { return false; }
		--res->second;
		if (!res->second) { Unload(l_id); }
		return true;
	}

	void PurgeResources()
	{
		while (m_resources.begin() != m_resources.end())
		{
			delete m_resource.begin()->second.first;
			m_resources.erase(m_resources.begin());
		}
	}

private:
	std::unordered_map<std::string, 
		std::pair<T*, unsigned int>> m_resources;
	std::unordered_map<std::string, std::string> m_paths;
};