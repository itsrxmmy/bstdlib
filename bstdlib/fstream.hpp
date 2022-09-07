
/**
 * @file fstream.hpp
 * @author Rammy Aly
 * @brief Simple version of fstream with additional features
 * @version 1.4
 * @date 2022-09-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include <fstream>
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

namespace bstd {
	namespace fstream {
		class file {
			private:
			std::string fp;
			std::string content;
			char* fp_cstr;

			public:
			std::vector<std::string> lines;


			file (std::string fp) : fp(fp) {}
			file (char* fp_cstr) : fp(fp_cstr) {}

			std::vector<std::string> scan () {
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


			std::string scan_line (int num) {
				// scan, fetch line
				return this->scan()[num-1];
			}

			bool exists () {
				std::filesystem::path p{this->fp.c_str()};
				return std::filesystem::exists(p);
			}

			void create () {
				// create file
				std::fstream fs(this->fp.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
				fs.close();
				this->scan();
			}

			int delete_ () {
				return std::remove(this->fp.c_str());
			}
			
			int remove () {
				return this->delete_();
			}

			void write (std::vector<std::string> lines) {
				std::fstream fs(this->fp.c_str(), std::fstream::out);
				for (int i=0; i<lines.size(); i++)
					// write line
					fs << lines[i].c_str() << "\n";
				fs.close();
				this->scan();
			}

			void write (std::string lines[]) {
				std::fstream fs(this->fp.c_str(), std::fstream::out);
				int range = sizeof(lines) / sizeof(std::string);
				for (int i=0; i<range; i++)
					// write line
					fs << lines[i].c_str() << "\n";
				fs.close();
				this->scan();
			}


			void write (char* lines[]) {
				std::fstream fs(this->fp.c_str(), std::fstream::out);
				int range = sizeof(lines) / sizeof(std::string);
				for (int i=0; i<range; i++)
					// write line
					fs << lines[i] << "\n";
					fs << std::endl;
				fs.close();
				this->scan();
			}


			void write (std::string content, bool endline=true) {
				std::fstream fs(this->fp.c_str(), std::fstream::out);
				std::string final_ = (endline) ? "\n" : "";
				fs << content << final_;
				this->scan();
			}

			void edit_line (int num, std::string content) {
				auto original = this->scan();
				std::vector<std::string> new_;
				for (int i=0; i<original.size(); i++) {
					if (i==num-1)
						new_.push_back(content);
					else
						new_.push_back(original[i]);
				}

				this->write(new_);
			}

			void clear_line (int num) {
				auto original = this->scan();
				std::vector<std::string> new_;
				for (int i=0; i<original.size(); i++) {
					if (i==num-1)
						new_.push_back(STR_BLANK);
					else {
						new_.push_back(original[i]);
					}
				}

				this->write(new_);
			}

			void erase_line (int num) {
				this->ClearLine(num);
			}

			void delete_line (int num) {
				auto original = this->scan();
				std::vector<std::string> new_;
				for (int i=0; i<original.size(); i++) {
					if (i!=num-1)
						new_.push_back(original[i]);
				}

				this->write(new_);
			}

			void remove_line (int num) {
				this->DeleteLine(num);
			}

			void write_line (int num, std::string content) {
				auto original = this->scan();
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

				this->write(new_);
			}

			void clear () {
				this->write(STR_BLANK);
			}

			void erase () {
				this->Clear();
			}

			file clone () {
				std::string fp_new = this->fp+CLONE_FILE_FILL;
				file fp_clone(fp_new);
				fp_clone.create();
				fp_clone.write(this->lines);
			}

			file clone_to (std::string fp_new=CLONED_FILE_NULL) {
				file fp_clone(fp_new);
				fp_clone.create();
				fp_clone.write(this->lines);
				return fp_clone;
			}

			file move_to (std::string fp_new) {
				file fp_moved(fp_new);
				fp_moved.create();
				fp_moved.write(this->lines);
				this->delete_();
				return fp_moved;
			}

			void print () {
				auto content = this->scan();
				for (int i=0; i<content.size(); i++) {
					std::cout << content[i] << std::endl;
				}
			}

			void echo () {
				this->print();
			}

			std::string get_content () {
				this->scan();
				return this->content;
			}

			std::vector<std::string> get_lines () {
				return this->scan();
			}
		};

		void mkdir (std::string path) {
			std::string task = "mkdir " + path;
			std::system(task.c_str());
		}

		void mkdir (char* path) {
			std::string path_(path);
			std::string task = "mkdir " + path_;
			std::system(task.c_str());
		}

		void rmdir (std::string path) {
			std::string task = "rmdir -r " + path;
			std::system(task.c_str());
		}

		void mkdir (char* path) {
			std::string path_(path);
			std::string task = "rm -r " + path_;
			std::system(task.c_str());
		}

		void cp_r (std::string path, std::string dest=".") {
			std::string task = "cp -r " + path + " " + dest;
			std::system(task.c_str());
		}

		void cp_r (char* path, char* dest=".") {
			std::string path_str(path);
			std::string dest_str(dest);
			std::string task = "cp -r " + path_str + " " + dest_str;
			std::system(task.c_str());
		}

		void cp (std::string path, std::string dest=".") {
			std::string task = "cp " + path + " " + dest;
			std::system(task.c_str());
		}

		void cp (char* path, char* dest=".") {
			std::string path_str(path);
			std::string dest_str(dest);
			std::string task = "cp " + path_str + " " + dest_str;
			std::system(task.c_str());
		}
	}
}



