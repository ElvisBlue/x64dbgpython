# x64dbg python

x64dbg python is a x64dbg plugin allow you to run python script to interactive with x64dbg in python3. x64dbg python provide script api similar to `_scriptapi_*.h` header in plugin SDK. You can look at folder "Sample script" for example

# Wrapper

x64dbg python made some wrapper to fit with python language.

**Argument**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool GetInfo(duint addr, ArgumentInfo* info)` | `pyArgumentInfo* GetInfo(duint addr)`       |
| `bool GetList(ListOf(ArgumentInfo) list)`      | `std::vector<pyArgumentInfo>* GetList()`    |




**Module**

| Original                                                | Wrapper                                        |
| --------------------------------------------------------| ---------------------------------------------- |
| `bool InfoFromAddr(duint addr, ModuleInfo* info)`       | `pyModuleInfo* InfoFromAddr(duint addr)`       |
| `bool InfoFromName(const char* name, ModuleInfo* info)` | `pyModuleInfo* InfoFromName(const char* name)` |
| `bool NameFromAddr(duint addr, char* name)`             | `std::string NameFromAddr(duint addr)`         |
| `bool PathFromAddr(duint addr, char* path)`             | `std::string PathFromAddr(duint addr)`         |
| `bool PathFromName(const char* name, char* path)`       | `std::string PathFromName(const char* name)`   |
| `bool SectionFromAddr(duint addr, int number, ModuleSectionInfo* section)` | `pyModuleSectionInfo* SectionFromAddr(duint addr, int number)`   |
| `bool SectionFromName(const char* name, int number, ModuleSectionInfo* section)` | `pyModuleSectionInfo* SectionFromName(const char* name, int number)` |
| `bool SectionListFromAddr(duint addr, ListOf(ModuleSectionInfo) list)` | `std::vector<pyModuleSectionInfo>* SectionListFromAddr(duint addr)`  |
| `bool SectionListFromName(const char* name, ListOf(ModuleSectionInfo) list)` | `std::vector<pyModuleSectionInfo>* SectionListFromName(const char* name)` |
| `bool GetMainModuleInfo(ModuleInfo* info)`              | `pyModuleInfo* GetMainModuleInfo()`            |
| `bool GetMainModuleName(char* name)`                    | `std::string GetMainModuleName()`              |
| `bool GetMainModulePath(char* path)`                    | `std::string GetMainModulePath()`              |
| `bool GetMainModuleSectionList(ListOf(ModuleSectionInfo) list)` | `std::vector<pyModuleSectionInfo>* GetMainModuleSectionList()` |
| `bool GetList(ListOf(ModuleInfo) list)`                 | `std::vector<pyModuleInfo>* GetList()`         |
| `bool GetExports(const ModuleInfo* mod, ListOf(ModuleExport) list)` | `std::vector<pyModuleExport>* GetExports(const pyModuleInfo* mod)` |
| `bool GetImports(const ModuleInfo* mod, ListOf(ModuleImport) list)` | `std::vector<pyModuleImport>* GetImports(const pyModuleInfo* mod)` |