**Argument**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool GetInfo(duint addr, ArgumentInfo* info)` | `std::optional<pyArgumentInfo> GetInfo(duint addr)`       |
| `bool GetList(ListOf(ArgumentInfo) list)`      | `std::optional<std::vector<pyArgumentInfo>> GetList()`    |

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
| `bool GetInfo(duint addr, BookmarkInfo* info)` | `std::optional<pyBookmarkInfo> GetInfo(duint addr)`       |
| `bool GetList(ListOf(BookmarkInfo) list)`      | `std::optional<std::vector<pyBookmarkInfo>> GetList()`    |

**Comment**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool Get(duint addr, char* text)`             | `std::string Get(duint addr)`               |
| `bool GetInfo(duint addr, CommentInfo* info)`  | `std::optional<pyCommentInfo> GetInfo(duint addr)`        |
| `bool GetList(ListOf(CommentInfo) list)`       | `std::optional<std::vector<pyCommentInfo>> GetList()`     |

**Function**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `GetInfo(duint addr, FunctionInfo* info)`      | `std::optional<pyFunctionInfo> GetInfo(duint addr)`       |
| `bool GetList(ListOf(FunctionInfo) list)`      | `std::optional<std::vector<pyFunctionInfo>> GetList()`    |

**Gui**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool SelectionGet(Window window, duint* start, duint* end)` | `std::optional<std::vector<duint>> SelectionGet(Window window)`|
| `bool Gui::Disassembly::SelectionGet(duint* start, duint* end)` | `std::optional<std::vector<duint>> PyWrapper::Gui::Disassembly::SelectionGet()`|
| `bool Gui::Dump::SelectionGet(duint* start, duint* end)` | `std::optional<std::vector<duint>> PyWrapper::Gui::Dump::SelectionGet()`|
| `bool Gui::Stack::SelectionGet(duint* start, duint* end)` | `std::optional<std::vector<duint>> PyWrapper::Gui::Stack::SelectionGet()`|
| `bool InputLine(const char* title, char* text)`| `std::string InputLine(const char* title)`  |
| `bool InputValue(const char* title, duint* value)` | `duint InputValue(const char* title)`   |


**Label**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool Get(duint addr, char* text)`             | `std::string Get(duint addr)`               |
| `bool GetInfo(duint addr, LabelInfo* info)`    | `std::optional<pyLabelInfo> GetInfo(duint addr)`          |
| `bool GetList(ListOf(LabelInfo) list)`         | `std::optional<std::vector<pyLabelInfo>> GetList()`       |
| `bool FromString(const char* label, duint* addr)`| `duint FromString(std::string label)`       |


**Memory**

| Original                                                                     | Wrapper                                     |
| ---------------------------------------------------------------------------- | ------------------------------------------- |
| `bool Read(duint addr, void* data, duint size, duint* sizeRead);`            | `py::bytes Read(duint addr, duint size)`    |
| `bool Write(duint addr, const void* data, duint size, duint* sizeWritten)`   | `duint Write(duint addr, std::string data)` |

**Misc**

| Original                                                                     | Wrapper                                     |
| ---------------------------------------------------------------------------- | ------------------------------------------- |
| `bool ParseExpression(const char* expression, duint* value)`                 | `duint ParseExpression(std::string expression)`|


**Module**

| Original                                                | Wrapper                                        |
| --------------------------------------------------------| ---------------------------------------------- |
| `bool InfoFromAddr(duint addr, ModuleInfo* info)`       | `std::optional<pyModuleInfo> InfoFromAddr(duint addr)`       |
| `bool InfoFromName(const char* name, ModuleInfo* info)` | `std::optional<pyModuleInfo> InfoFromName(const char* name)` |
| `bool NameFromAddr(duint addr, char* name)`             | `std::string NameFromAddr(duint addr)`         |
| `bool PathFromAddr(duint addr, char* path)`             | `std::string PathFromAddr(duint addr)`         |
| `bool PathFromName(const char* name, char* path)`       | `std::string PathFromName(const char* name)`   |
| `bool SectionFromAddr(duint addr, int number, ModuleSectionInfo* section)` | `std::optional<pyModuleSectionInfo> SectionFromAddr(duint addr, int number)`   |
| `bool SectionFromName(const char* name, int number, ModuleSectionInfo* section)` | `std::optional<pyModuleSectionInfo> SectionFromName(const char* name, int number)` |
| `bool SectionListFromAddr(duint addr, ListOf(ModuleSectionInfo) list)` | `std::optional<std::vector<pyModuleSectionInfo>> SectionListFromAddr(duint addr)`  |
| `bool SectionListFromName(const char* name, ListOf(ModuleSectionInfo) list)` | `std::optional<std::vector<pyModuleSectionInfo>> SectionListFromName(const char* name)` |
| `bool GetMainModuleInfo(ModuleInfo* info)`              | `std::optional<pyModuleInfo> GetMainModuleInfo()`            |
| `bool GetMainModuleName(char* name)`                    | `std::string GetMainModuleName()`              |
| `bool GetMainModulePath(char* path)`                    | `std::string GetMainModulePath()`              |
| `bool GetMainModuleSectionList(ListOf(ModuleSectionInfo) list)` | `std::optional<std::vector<pyModuleSectionInfo>> GetMainModuleSectionList()` |
| `bool GetList(ListOf(ModuleInfo) list)`                 | `std::optional<std::vector<pyModuleInfo>> GetList()`         |
| `bool GetExports(const ModuleInfo* mod, ListOf(ModuleExport) list)` | `std::optional<std::vector<pyModuleExport>> GetExports(const pyModuleInfo* mod)` |
| `bool GetImports(const ModuleInfo* mod, ListOf(ModuleImport) list)` | `std::optional<std::vector<pyModuleImport>> GetImports(const pyModuleInfo* mod)` |

**Symbol**

| Original                                       | Wrapper                                     |
| ---------------------------------------------- | ------------------------------------------- |
| `bool GetList(ListOf(SymbolInfo) list)`        | `std::optional<std::vector<pySymbolInfo>> GetList()`      |