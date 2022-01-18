# x64dbg python

x64dbg python is a x64dbg plugin allow you to run python script to interactive with x64dbg in python3. x64dbg python provide script api similar to `_scriptapi_*.h` header in plugin SDK. You can look at folder "example script" for example

# Installation

- Install both python3 32 bit and 64 bit. Don't forget to add to path environment.
- Copy plugin to x32dbg and x64dbg plugin folder


# Wrapper

x64dbg python made some wrapper for easy to use in python language. If you're going to using one of those function below, look at wrapper definition instead.

**Argument**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool GetInfo(duint addr, ArgumentInfo* info)` | `pyArgumentInfo* GetInfo(duint addr)`       |
| `bool GetList(ListOf(ArgumentInfo) list)`      | `std::vector<pyArgumentInfo>* GetList()`    |

**Assembler**

| Original                                                                             | Wrapper                                                   |
| ------------------------------------------------------------------------------------ | --------------------------------------------------------- |
| `bool Assemble(duint addr, unsigned char* dest, int* size, const char* instruction)` | `py::bytes Assemble(duint addr, const char* instruction)` |
| `bool AssembleEx(duint addr, unsigned char* dest, int* size, const char* instruction, char* error)` | `py::bytes AssembleEx(duint addr, const char* instruction)` |
| `bool AssembleMemEx(duint addr, const char* instruction, int* size, char* error, bool fillnop)` | `bool AssembleMemEx(duint addr, const char* instruction, bool fillnop)` |

*AssembleEx and AssembleMemEx now redirect error to x64dbg log*

**Bookmark**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool GetInfo(duint addr, BookmarkInfo* info)` | `pyBookmarkInfo* GetInfo(duint addr)`       |
| `bool GetList(ListOf(BookmarkInfo) list)`      | `std::vector<pyBookmarkInfo>* GetList()`    |

**Comment**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool Get(duint addr, char* text)`             | `std::string Get(duint addr)`               |
| `bool GetInfo(duint addr, CommentInfo* info)`  | `pyCommentInfo* GetInfo(duint addr)`        |
| `bool GetList(ListOf(CommentInfo) list)`       | `std::vector<pyCommentInfo>* GetList()`     |

**Function**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `GetInfo(duint addr, FunctionInfo* info)`      | `pyFunctionInfo* GetInfo(duint addr)`       |
| `bool GetList(ListOf(FunctionInfo) list)`      | `std::vector<pyFunctionInfo>* GetList()`    |

**Gui**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool SelectionGet(Window window, duint* start, duint* end)` | `std::vector<duint>* SelectionGet(Window window)`
| `bool InputLine(const char* title, char* text)`| `std::string InputLine(const char* title)`  |
| `bool InputValue(const char* title, duint* value)` | `duint InputValue(const char* title)`   |


**Label**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool Get(duint addr, char* text)`             | `std::string Get(duint addr)`               |
| `bool GetInfo(duint addr, LabelInfo* info)`    | `pyLabelInfo* GetInfo(duint addr)`          |
| `bool GetList(ListOf(LabelInfo) list)`         | `std::vector<pyLabelInfo>* GetList()`       |

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

**Symbol**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool GetList(ListOf(SymbolInfo) list)`        | `std::vector<pySymbolInfo>* GetList()`      |


# Credits
 - [pybind11](https://github.com/pybind/pybind11)
 - [VC-LTL](https://github.com/Chuyu-Team/VC-LTL)
 - [x64dbg](https://github.com/x64dbg/x64dbg)