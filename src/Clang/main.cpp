#include <iostream>
#include <clang-c/Index.h>
#include <sstream>      // std::stringstream

using namespace std;

// Global state, capturing changes the visitor signature
stringstream os;

static inline string Convert(const CXString& s)
{
    string result = clang_getCString(s);
    clang_disposeString(s);
    return result;
}

void print_func_bundle(CXCursor cursor, stringstream& os) {
    auto type = clang_getCursorType(cursor);
    auto function_name = Convert(clang_getCursorSpelling(cursor));
    auto return_type   = Convert(clang_getTypeSpelling(clang_getResultType(type)));

    int num_args = clang_Cursor_getNumArguments(cursor);

	// begin function JSON record
	os << "{ \"name\": \"" << function_name << "\", \"type\" : \"" << return_type << "\", \"args\" : [ ";
    for (int i = 0; i < num_args; ++i)
    {
        auto arg_cursor = clang_Cursor_getArgument(cursor, i);
        auto arg_name = Convert(clang_getCursorSpelling(arg_cursor));
        if (arg_name.empty()) {
            arg_name = "_";
        }
        auto arg_data_type = Convert(clang_getTypeSpelling(clang_getArgType(type, i)));
		// append function arguments
		os << "[\"" << arg_data_type << "\", \"" << arg_name << "\"]";
		if (i < num_args - 1) {
			os << ", ";
		}
    }
	os << " ]}, " << endl;
}

int main()
{
  CXIndex index = clang_createIndex(0, 0);

  // Set clang arguments
  constexpr const char* defaultArguments[] = {
    "-std=c++0x",
    "-fsyntax-only"
  };

  // translate
  CXTranslationUnit unit = clang_parseTranslationUnit( index,
                                              "nat.hpp",
                                              defaultArguments,
                                              std::extent<decltype(defaultArguments)>::value,
                                              0,
                                              0,
                                              CXTranslationUnit_None );
  if (unit == nullptr)
  {
    cerr << "Unable to parse translation unit. Quitting." << endl;
    exit(-1);
  }

  // Begin top level object
  os << "{" << endl;

  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
    cursor,
    [](CXCursor c, CXCursor parent, CXClientData client_data)
    {
      if(clang_Location_isFromMainFile(clang_getCursorLocation(c)) == 0)
        return CXChildVisit_Continue;

      if (clang_getCursorKind(c) == CXCursor_Namespace) {
        auto ns = Convert(clang_getCursorSpelling(c));
        os << "\"namespace\" : \"" << ns << "\", \"functions\" : [ " << endl;
      }
	  if (clang_getCursorKind(c) == CXCursor_FunctionDecl) {
        print_func_bundle(c, os);
      }

      return CXChildVisit_Recurse;
    },
    nullptr);
  // Erase last comma
  os.seekp(-3, os.cur);
  os << endl << "]}";
  cout << os.str() << endl;

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
}

