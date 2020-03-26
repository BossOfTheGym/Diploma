#pragma once

#include <iostream>
#include <fstream>

#include <utility>
#include <thread>
#include <memory>

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>


//some STL aliases 
namespace alias
{
	using String = std::string;

	using IStream = std::istream;
	using OStream = std::ostream;

	using IFStream = std::ifstream;
	using OFStream = std::ofstream;
}