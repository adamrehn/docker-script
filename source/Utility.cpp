/*
	docker-script: run script files inside Docker containers
	Copyright (c) 2017, Adam Rehn
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
#include "Utility.h"
#include <cstdlib>
#include <cstring>
#include <memory>
using std::unique_ptr;

//Adapted from a string search and replace function from <http://snipplr.com/view/1055/find-and-replace-one-string-with-another>
//and the code from the comment by "icstatic" that fixes the infinite loop bugs present in the original.
string Utility::strReplace(const string& find, const string& replace, string subject)
{
	size_t uReplaceLen = replace.length();
	size_t uFindLen = find.length();
	size_t uPos = 0;
	
	if (uFindLen == 0) {
		return subject;
	}
	
	while ((uPos = subject.find(find, uPos)) != string::npos)
	{
		subject.replace(uPos, uFindLen, replace);
		uPos += uReplaceLen;
	}
	
	return subject;
}

#ifdef _WIN32
	
	#include <windows.h>
	#include <direct.h>
	
	string Utility::realPath(const string& path)
	{
		char buf[4096];
		if (GetFullPathName(path.c_str(), sizeof(buf), buf, nullptr) != 0) {
			return Utility::strReplace("\\", "/", buf);
		}
		
		//Failed to resolve the real path
		return "";
	}
	
	string Utility::dirName(const string& path)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		
		//Strip the trailing slash from the path
		_splitpath(path.c_str(), drive, dir, fname, ext);
		string directory = dir;
		directory = directory.substr(0, directory.length() - 1);
		return string(drive) + directory;
	}
	
	string Utility::baseName(const string& path)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		
		_splitpath(path.c_str(), drive, dir, fname, ext);
		return string(fname) + string(ext);
	}
	
	string Utility::cwd()
	{
		char buf[4096];
		_getcwd(buf, sizeof(buf));
		return Utility::strReplace("\\", "/", buf);
	}
	
#else
	
	#include <unistd.h>
	#include <libgen.h>
	
	string Utility::realPath(const string& path)
	{
		char* resolved = ::realpath(path.c_str(), nullptr);
		if (resolved != nullptr)
		{
			string result(resolved);
			free(resolved);
			return result;
		}
		
		//Failed to resolve the real path
		return "";
	}
	
	string Utility::dirName(const string& path)
	{
		//Copy the string into a buffer, since dirname may modify the input string
		unique_ptr<char[]> buf( new char[path.size()] );
		memcpy(buf.get(), path.c_str(), path.size());
		return ::dirname(buf.get());
	}
	
	string Utility::baseName(const string& path)
	{
		//Copy the string into a buffer, since basename may modify the input string
		unique_ptr<char[]> buf( new char[path.size()] );
		memcpy(buf.get(), path.c_str(), path.size());
		return ::basename(buf.get());
	}
	
	string Utility::cwd()
	{
		char buf[4096];
		getcwd(buf, sizeof(buf));
		return string(buf);
	}
	
#endif
