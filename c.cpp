/*
	パッケージ マネージャーのゆうか バージョン 1.PRE-RELEASE-2
	Duca package manager version 1.PRE-RELEASE-2

	This package manager based on Vtuber name: Hibiki Duca (a VNVTuber)

	and it's come with an operating system call: Hibiki Duwuca Linux System
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <new>
#include <stdio.h>
#include <string>
#include <string.h>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
 
using namespace std;

enum PackageType {
	source_package,
	pre_package
};

//save the installed package into a file
void save_database(string package) {
	ofstream sw;
	sw.open("/opt/duca/installedpackage");
	sw << package << endl;
	sw.close();
}
//load the installed package from file
int load_database(string package) {
	ifstream sw;
	sw.open("/opt/duca/installedpackage");
	string packagelist;
	while(getline(sw, packagelist)) {
		if (packagelist.find(package) != string::npos) {
			cout << "[*] Package " << packagelist << " is already installed." <<endl;
			return 0;
		} else {
			return 1;
		}
	}
	sw.close();
	return 1;
}


int download_package(string package_links, PackageType packagetype) {

	if (packagetype == source_package) {

		cout << "[*] Downloading package " + package_links <<endl;

		string command = "wget --no-check-certificate https://raw.githubusercontent.com/CodeCs1/Hibiki-Duwuca-repos/main/packages/" + package_links + ".pckzip";
		const char* command0 = command.c_str();

		int returncode = system(command0);

		return returncode;
	} else if (packagetype == pre_package) {
		cout << "[*] Downloading package " + package_links <<endl;

		string command = "wget --no-check-certificate https://raw.githubusercontent.com/CodeCs1/Hibiki-Duwuca-repos/main/pre-packages/" + package_links + ".dupck";
		const char* command0 = command.c_str();

		int returncode = system(command0);

		return returncode;
	}
	return 0;
}

int check_package(string package_links) {
	cout << "[*] Check if " << package_links << " is exist" <<endl;
	string command = "wget -q --no-check-certificate --method=HEAD " + package_links;
	const char* command1 = command.c_str();
	int errorcode = system(command1);
	if (errorcode != 0) {
		cout << "[X] Package not exist in repository" <<endl;
		return 1;
	} else {
		return 0;
	}
	return 0;
}


void pre_packge_install(string package) {
	cout << "[*] Installing in root device..." <<endl;
	string command = "tar xvf " + package + ".dupck -C /";
	const char* command1 = command.c_str();
	system(command1);
	cout << "[*] Cleaning Up..." <<endl;
	string command2 = "rm -rf " + package + ".dupck";
	const char* command3 = command2.c_str();
	system(command3);
	cout << "[!] Install is completed." <<endl;
}
void pre_packge_install(string package, string path) {
	cout << "[*] Installing in " << path << " prefix directory..."<<endl;
	string command = "tar xvf " + package + ".dupck -C " + path;
	const char* command1 = command.c_str();
	system(command1);
	cout << "[*] Cleaning Up..." <<endl;
	string command2 = "rm -rf " + package + ".dupck";
	const char* command3 = command2.c_str();
	system(command3);
	cout << "[!] Install is completed." <<endl;
}

bool isFileExist(const char* filename);
 
bool isFileExist(const char* filename) {
	FILE* file;
	if ((file = fopen(filename, "r"))) {
		fclose(file);
		return true;
	} 
	return false;
}

static char version_prefix = '1';

static string version_name = ".PRE-RELEASE-2";

void version() {
	cout << "Duca Package Manager version " << version_prefix << version_name << endl;
}


bool check_permission() {
	if (getuid()) 
		return false;
	else 
		return true;
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("\n");
	} else {
		cout << "[*] Check if the database is created..." <<endl;

		if (isFileExist("/opt/duca/installedpackage")) {
			cout << "[*] Database exist" <<endl;
		} else {
			if(check_permission()) {
				struct stat st = {0};
				if (stat("/opt/duca", &st) == -1) {
					mkdir("/opt/duca", 0700);
				}
				cout << "[*] Database not exist, creating..." <<endl;
			
				ofstream st1;
				st1.open("/opt/duca/installedpackage");
				st1 << "" << endl;
				st1.close();
			} else {
				cout << "[!] Please run this as administrator (root) to create database" <<endl;
			}
			
		}
		for (int i = 1; i < argc; i++) {
			string args1 = argv[i];
			if (args1 == "-i") {
				string args2 = argv[i+1];
				string args4 = argv[i+2];
				if (args4 == "-s") {
					if(check_permission()) {
						if (load_database(args2) == 0) {
							return 0;
						} else {

							download_package(args2, source_package);

							cout << "[*] Extracting archive..." <<endl;

							string command="tar xvf " + args2 + ".pckzip";
							const char* str2 = command.c_str();
							system(str2);
							string instscr;
							ifstream Insrscript(args2 + "/" + args2 + ".pckscr");

							ifstream PackageInstallerScript(args2+"/"+args2+".instscr", ofstream::out);

							ofstream replace(args2+"/"+args2+"-path.instscr");
							if (argc == 6) {
								if (string(argv[4]) == "--prefix") {
									while(getline(Insrscript, instscr)) {
										size_t pos = instscr.find("URL=");
										size_t pos1 = instscr.find(" ");
										string extracted = instscr.substr(pos + 4, pos1 - pos);
										cout << extracted << endl;
										cout << "[*] Starting download script..." <<endl;
										string command2 = "wget --no-check-certificate -O 1.tar.gz " + extracted;
										const char* linko = command2.c_str();
										system(linko);

										string commandremoveold = "rm -rf " + args2 + "/" + args2 + ".instscr";

										system(commandremoveold.c_str());

										cout << "[*] Replacing file location..." <<endl;
										string location = argv[5];
										string tmp;

										string command = "make DESTDIR=" + location + " install";
										string command1 = "make install";

										while (getline(PackageInstallerScript, tmp)) {
											if (tmp == command1) {
												tmp = command;
											}
											cout << tmp << endl;
											replace << tmp << endl;
										}

										cout << "[*] Starting installer script..." <<endl;
										string command3 = "sh " + args2 + "/" + args2 + "-path.instscr 1.tar.gz";
										const char* command4 = command3.c_str();
										system(command4);
									}
								}
							} else if (argc == 4) {

								while (getline(Insrscript, instscr)) {
									size_t pos = instscr.find("URL=");
									size_t pos1 = instscr.find(" ");
									string extracted = instscr.substr(pos + 4, pos1 - pos);
									cout << extracted << endl;
									cout << "[*] Starting download script..." <<endl;
									string command2 = "wget --no-check-certificate -O 1.tar.gz " + extracted;
									const char* linko = command2.c_str();
									system(linko);
									cout << "[*] Starting installer script..." <<endl;
									string command3 = "sh " + args2 + "/" + args2 + ".instscr 1.tar.gz";
									const char* command4 = command3.c_str();
									system(command4);
								}
							}
							save_database(args2);
							Insrscript.close();
						}

						cout << "Done in installing " << args2  <<endl;
					} else {
						return 1;
					}
				} else if (args4 == "-p") {
					const char* args5c = argv[4];
					const char* prefixc = argv[5];

					if (load_database(args2) == 0) {
						return 0;
					} else {
						if (check_permission()) {
							cout << "[*] Installing " << args2 <<endl;


							string repositorylink;
							string requited = args2 + "/requitement.txt";

							const char* filerequited =requited.c_str();

							if (isFileExist(filerequited)) {
								cout << "[*] Installing requitements for this package..." <<endl;
								ifstream requitedconfig(requited);
								string str;
								string links;
								while(getline(requitedconfig, str)) {
									cout << " [->] " << str << endl;
									sleep(3);
									//const char* linksc = links.c_str();
									//cout << linksc << endl;
									int returncode = check_package("https://raw.githubusercontent.com/CodeCs1/Hibiki-Duwuca-repos/main/pre-packages/" + str + ".dupck");
									if (returncode == 0) {
										download_package(str, pre_package);
										if (argc == 6) {
											if (string(args5c) == "--prefix") 
												pre_packge_install(str, string(prefixc));						
										} else if (argc == 4) {
											pre_packge_install(str);
										}
									} else {

										//if the package is not found, maybe the package is source package

										//e.g: bash requited glibc and glibc is source package

										cout << "[*] Installing using source-package based..." <<endl;

										links = "https://raw.githubusercontent.com/CodeCs1/Hibiki-Duwuca-repos/main/packages/" + str + ".pckzip";

										int check2 = check_package(links);

										if (check2 == 0) {
											

											if (returncode != 0) {
												cout << "[!] Errror in install package." <<endl;
												exit(1);
											} else {
												cout << "Extracting archive..." <<endl;

												string command="tar xvf " + str + ".pckzip";
												const char* str2 = command.c_str();
												system(str2);
												string instscr;
												ifstream Insrscript(str+ "/" + str + ".pckscr");

												while (getline(Insrscript, instscr)) {
													size_t pos = instscr.find("URL=");
													size_t pos1 = instscr.find(" ");
													string extracted = instscr.substr(pos + 4, pos1 - pos);
													cout << extracted << endl;
													cout << "[*] Starting download script..." <<endl;
													string command2 = "wget --no-check-certificate -O 1.tar.gz " + extracted;
													const char* linko = command2.c_str();
													system(linko);
													cout << "[*] Starting installer script..." <<endl;
													string command3 = "sh " + str + "/" + str + ".instscr 1.tar.gz";
													const char* command4 = command3.c_str();
													int ret = system(command4);
													if (WEXITSTATUS(ret) != 0x10) {
														cout << "[!]: Installation failed." <<endl;
														return 1;
													}
												}
											}

										} else {
											cout << "[!] Error in install package." <<endl;
											exit(1);
										}
									}
								}
							} else {
								cout << "(!) No requiements found in " << args2 << " package, starting install..." <<endl;
							}

							int returncode = check_package("https://raw.githubusercontent.com/CodeCs1/Hibiki-Duwuca-repos/main/pre-packages/" + args2 + ".dupck");

							if (returncode == 0) {
								download_package(args2, pre_package);
							} else {
								cout << "[X] Error in download the package" <<endl;
								return 1;
							}
							/*Fix Optional Arguments*/
							if (argc == 6) {
								if (string(args5c) == "--prefix") {
									pre_packge_install(args2, string(prefixc));
									save_database(args2);		
								}				
							} else if (argc == 4) {
								pre_packge_install(args2);
								save_database(args2);
							}
						} else {
							return 1;
						}
					}
				}
			} else if (args1 == "-v") {
				version();
			} else if (args1 == "-h") {
				version();
				cout << "Help utilities:" <<endl;
				cout << "-h: Show this menu." <<endl;
				cout << "-i <package_name>: Install a package." <<endl;
				cout << "	-s: install using source. (run with -i flag)" <<endl;
				cout << "		--prefix <PATH>: install in specified path. (optional, run with -s flag)" <<endl;
				cout << "	-p: install using pre-compiled package. (run with -i flag)" <<endl;
				cout << "		--prefix <PATH>: install in specified path. (optional, run with -p flag)" <<endl;
				cout << "-r <package_name>: Reinstall a package. [still in development]" <<endl;
				cout << "-u <package_name>: Uninstall a package. [still in development]" <<endl;
				cout << "-U <package_name>: Update a package.    [still in development]" <<endl;
				cout << "-F <package_name>: find a package." <<endl;
				cout << "-v: show version" <<endl;
			} else if (args1 == "-F") {
				if (argc == 3) {
					string args2 = argv[i+1];
					string links = "https://raw.githubusercontent.com/CodeCs1/Hibiki-Duwuca-repos/main/packages/" + args2 + ".pckzip";
					int ret = check_package(links);
					if (ret == 0) {
						cout << "\n[*] Package " << args2 << " exist (packages)" <<endl;
						return 0;
					} else {
						cout << "[*] Package not found in packages section, checking in pre-packages section..." <<endl;

						string links = "https://raw.githubusercontent.com/CodeCs1/Hibiki-Duwuca-repos/main/pre-packages/" + args2 + ".dupck";
						int ret = check_package(links);
						if (ret == 0) {
							cout << "\n[*] Package " << args2 << " is exist (pre-packages)" <<endl;
							return 0;
						} else {
							cout << "\n[!] Package " << args2 << " not exist" <<endl;
						}
						return 1;
					}
				} else {
					cout << "Usage: duca -F <package_name>" <<endl;
					cout << "      Find a package" <<endl;
					return 0;
				}
			}else /*if (args1 != "-h" || args1 != "-i" && args1 != "-F")*/ {
				cout << "[!] The command " << args1 << " is not found." <<endl;
				return 1;
			}
		}
	}
	return 0;
}
