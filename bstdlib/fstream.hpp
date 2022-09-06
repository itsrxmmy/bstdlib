#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <filesystem>

#define LINES_LAST -1
#define LINES_NEW -1
#define LINES_END -1
#define LINES_FIRST -2
#define LINES_START -2
#define STR_BLANK " "
#define CLONE_FILE_FILL "clone_"
#define CLONED_FILE_NULL "cloned_file_null_"


class File {
	private:
	std::string fp;
	std::string content;
	char* fp_cstr;

	public:
	std::vector<std::string> lines;


	File (std::string fp) : fp(fp) {}
	File (char* fp_cstr) : fp(fp_cstr) {}

	std::vector<std::string> Scan () {
		// open file
		std::ifstream fs(this->fp.c_str());
		// initialize new lines vector and content string
		std::vector<std::string> lines_new;
		std::string content;
		// scan line by line
		for (std::string line; std::getline(fs, line);) {
			// update lines vector
			lines_new.push_back(line);
			content+=(line+"\n");
		}

		fs.close();
		// save changes
		this->lines = lines_new;
		this->content = content;
		return lines_new;
	}

	void Mkdir (std::string path) {
		std::string task = "mkdir " + path;
		std::system(task.c_str());
	}

	void Mkdir (char* path) {
		std::string path_(path);
		std::string task = "mkdir " + path_;
		std::system(task.c_str());
	}

	std::string ScanLine (int num) {
		// scan, fetch line
		return this->Scan()[num-1];
	}

	bool Exists () {
		return std::filesystem::exists(this->fp.c_str());
	}

	void Create () {
		// create file
		std::fstream fs(this->fp.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
		// ensure file is closed
		fs.close();
		this->Scan();
	}

	int Delete () {
		return std::remove(this->fp.c_str());
	}

	void Write (std::vector<std::string> lines) {
		std::fstream fs(this->fp.c_str(), std::fstream::out);
		for (int i=0; i<lines.size(); i++)
			// write line
			fs << lines[i].c_str() << "\n";
		fs.close();
		this->Scan();
	}

	void Write (std::string lines[]) {
		std::fstream fs(this->fp.c_str(), std::fstream::out);
		int range = sizeof(lines) / sizeof(std::string);
		for (int i=0; i<range; i++)
			// write line
			fs << lines[i].c_str() << "\n";
		fs.close();
		this->Scan();
	}


	void Write (char* lines[]) {
		std::fstream fs(this->fp.c_str(), std::fstream::out);
		int range = sizeof(lines) / sizeof(std::string);
		for (int i=0; i<range; i++)
			// write line
			fs << lines[i] << "\n";
			fs << std::endl;
		fs.close();
		this->Scan();
	}


	void Write (std::string content, bool endline=true) {
		std::fstream fs(this->fp.c_str(), std::fstream::out);
		std::string final_ = (endline) ? "\n" : "";
		fs << content << final_;
		this->Scan();
	}

	void EditLine (int num, std::string content) {
		auto original = this->Scan();
		std::vector<std::string> new_;
		for (int i=0; i<original.size(); i++) {
			if (i==num-1)
				new_.push_back(content);
			else
				new_.push_back(original[i]);
		}

		this->Write(new_);
	}

	void ClearLine (int num) {
		auto original = this->Scan();
		std::vector<std::string> new_;
		for (int i=0; i<original.size(); i++) {
			if (i==num-1)
				new_.push_back(STR_BLANK);
			else {
				new_.push_back(original[i]);
			}
		}

		this->Write(new_);
	}

	void EraseLine (int num) {
		this->ClearLine(num);
	}

	void DeleteLine (int num) {
		auto original = this->Scan();
		std::vector<std::string> new_;
		for (int i=0; i<original.size(); i++) {
			if (i!=num-1)
				new_.push_back(original[i]);
		}

		this->Write(new_);
	}

	void RemoveLine (int num) {
		this->DeleteLine(num);
	}

	void WriteLine (int num, std::string content) {
		auto original = this->Scan();
		std::vector<std::string> new_;
		if (num==LINES_END) {
			for (int i=0; i<original.size(); i++) {
				new_.push_back(original[i]);
			}
			new_.push_back(content);
		}
		else if (num==LINES_START) {
			new_.push_back(content);
			for (int i=0; i<original.size(); i++) {
				new_.push_back(original[i]);
			}
		}
		else {
			for (int i=0; i<original.size(); i++) {
				if (i==num-1) {
					new_.push_back(content);
					new_.push_back(original[i]);
				}
				else {
					new_.push_back(original[i]);
				}
			}
		}

		this->Write(new_);
	}

	void Clear () {
		this->Write(STR_BLANK);
	}

	void Erase () {
		this->Clear();
	}

	File Clone () {
		std::string fp_new = this->fp+CLONE_FILE_FILL;
		File fp_clone(fp_new);
		fp_clone.Create();
		fp_clone.Write(this->lines);
	}

	File CloneTo (std::string fp_new=CLONED_FILE_NULL) {
		File fp_clone(fp_new);
		fp_clone.Create();
		fp_clone.Write(this->lines);
		return fp_clone;
	}

	File MoveTo (std::string fp_new) {
		File fp_moved(fp_new);
		fp_moved.Create();
		fp_moved.Write(this->lines);
		this->Delete();
		return fp_moved;
	}

	void Print () {
		auto content = this->Scan();
		for (int i=0; i<content.size(); i++) {
			std::cout << content[i] << std::endl;
		}
	}

	std::string GetContent () {
		this->Scan();
		return this->content;
	}

	std::vector<std::string> GetLines () {
		return this->Scan();
	}


};



