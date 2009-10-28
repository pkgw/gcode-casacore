//# taql.cc: This program executes TaQL commands
//# Copyright (C) 2009
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This program is free software; you can redistribute it and/or modify it
//# under the terms of the GNU General Public License as published by the Free
//# Software Foundation; either version 2 of the License, or (at your option)
//# any later version.
//#
//# This program is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//# more details.
//#
//# You should have received a copy of the GNU General Public License along
//# with this program; if not, write to the Free Software Foundation, Inc.,
//# 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id$

#include <tables/Tables/TableParse.h>
#include <tables/Tables/Table.h>
#include <tables/Tables/TableDesc.h>
#include <tables/Tables/TableColumn.h>
#include <tables/Tables/ExprNodeArray.h>
#include <casa/Arrays/Matrix.h>
#include <casa/Arrays/Vector.h>
#include <casa/Arrays/ArrayIO.h>
#include <casa/BasicSL/Complex.h>
#include <casa/BasicMath/Math.h>
#include <casa/Utilities/Assert.h>
#include <casa/Exceptions/Error.h>
#include <map>
#include <vector>
#include <casa/iostream.h>

#ifdef HAVE_READLINE
# include <readline/readline.h>
# include <readline/history.h>
#endif

using namespace casa;
using namespace std;

// <summary>
// Execute table commands from user interface
// </summary>

#ifdef HAVE_READLINE
bool readLine (string& line, const string& prompt, bool addToHistory)
{
  char* str = readline(prompt.c_str());
  if (!str) return false;
  line = string(str);
  free(str);
  if (addToHistory) add_history (line.c_str());
  return true;
}
#else
bool readLine (String& line, const String& prompt, bool)
{
  if (!prompt.empty()) cerr << prompt;
  getline (cin, line);
  return cin;
}
#endif

bool readLineSkip (String& line, const String& prompt,
                   const String& commentChars)
{
  Regex lwhiteRE("^[ \t]*");
  Regex rwhiteRE("[ \t]*$");
  bool fnd = false;
  while (!fnd  &&  readLine (line, prompt, false)) {
    // Skip leading and trailing whitespace.
    line.del (lwhiteRE);
    line.del (rwhiteRE);
    if (! line.empty()) {                            
      // non-empty line.
      if (commentChars.empty()  ||  commentChars.size() > line.size()) {
        // Do not test for comment
        fnd = true;
      } else {
        for (uint i=0; i<commentChars.size(); ++i) {
          if (commentChars[i] != line[i]) {
            // non-comment
            fnd = true;
            break;
          }
        }
      }
    }
  }
#ifdef HAVE_READLINE
  add_history (line.c_str());
#endif
  return fnd;
}

// Show the required columns.
// First test if they exist and contain scalars or arrays.
void showTable (const Table& tab, const Vector<String>& colnam)
{
  uInt nrcol = 0;
  PtrBlock<ROTableColumn*> tableColumns(colnam.nelements());
  uInt i;
  for (i=0; i<colnam.nelements(); i++) {
    if (! tab.tableDesc().isColumn (colnam(i))) {
      cout << "Column " << colnam(i) << " does not exist" << endl;
    }else{
      tableColumns[nrcol] = new ROTableColumn (tab, colnam(i));
      if (! tableColumns[nrcol]->columnDesc().isScalar()
      &&  ! tableColumns[nrcol]->columnDesc().isArray()) {
	cout << "Column " << colnam(i)
	     << " contains scalars nor arrays"
	     << endl;
	delete tableColumns[nrcol];
      }else{
	nrcol++;
      }
    }
  }
  if (nrcol == 0) {
    return;
  }
  
  for (i=0; i<tab.nrow(); i++) {
    for (uInt j=0; j<nrcol; j++) {
      if (tableColumns[j]->columnDesc().isArray()) {
	cout << " shape=" << tableColumns[j]->shape (i);
      }else{
	switch (tableColumns[j]->columnDesc().dataType()) {
	case TpBool:
	  cout << " " << tableColumns[j]->asBool (i);
	  break;
	case TpString:
	  cout << " " << tableColumns[j]->asString (i);
	  break;
	case TpComplex:
	case TpDComplex:
	  cout << " " << tableColumns[j]->asDComplex (i);
	  break;
	default:
	  cout << " " << tableColumns[j]->asdouble (i);
	}
      }
    }
    cout << endl;
  }
  
  for (i=0; i<nrcol; i++) {
    delete tableColumns[i];
  }
}


void showExpr(const TableExprNode& expr)
{
  // Print the index if possible.
  // Get internal node.
  const TableExprNodeArrayPart* nodePtr =
               dynamic_cast<const TableExprNodeArrayPart*>(expr.getNodeRep());
  if (nodePtr != 0) {
    // The node represents a part of an array; get its index node.
    const TableExprNodeIndex* inxNode = nodePtr->getIndexNode();
    // If a constant index accessing a single element,
    // get the Slicer defining the index.
    if (inxNode->isConstant()  &&  inxNode->isSingle()) {
      const Slicer& indices = inxNode->getConstantSlicer();
      // Extract the index from it.
      cout << "Index: " << indices.start() << endl;
    }
  }
  const Unit& unit = expr.unit();
  if (! unit.empty()) {
    cout << "Unit: " << unit.getName() << endl;
  }
  if (expr.isScalar()) {
    switch (expr.getColumnDataType()) {
    case TpBool:
      cout << expr.getColumnBool();
      break;
    case TpUChar:
      cout << expr.getColumnuChar();
      break;
    case TpShort:
      cout << expr.getColumnShort();
      break;
    case TpUShort:
      cout << expr.getColumnuShort();
      break;
    case TpInt:
      cout << expr.getColumnInt();
      break;
    case TpUInt:
      cout << expr.getColumnuInt();
      break;
    case TpFloat:
      cout << expr.getColumnFloat();
      break;
    case TpDouble:
      cout << expr.getColumnDouble();
      break;
    case TpComplex:
      cout << expr.getColumnComplex();
      break;
    case TpDComplex:
      cout << expr.getColumnDComplex();
      break;
    case TpString:
      cout << expr.getColumnString();
      break;
    default:
      cout << "Unknown expression scalar type " << expr.getColumnDataType();
    }
    cout << endl;
  } else {
    for (uInt i=0; i<expr.nrow(); i++) {
      cout << "  row " << i << ":  ";
      switch (expr.dataType()) {
      case TpBool:
	{
	  Array<Bool> arr;
	  expr.get (i, arr);
	  cout << arr;
	  break;
	}
      case TpDouble:
	{
	  Array<Double> arr;
	  expr.get (i, arr);
	  cout << arr;
	  break;
	}
      case TpDComplex:
	{
	  Array<DComplex> arr;
	  expr.get (i, arr);
	  cout << arr;
	  break;
	}
      case TpString:
	{
	  Array<String> arr;
	  expr.get (i, arr);
	  cout << arr;
	  break;
	}
      default:
	cout << "Unknown expression array type " << expr.dataType();
      }
      cout << endl;
    }
  }
}


// Sort and select data.
Table doCommand (const String& str, const vector<const Table*>& tempTables)
{
  Table tabp;
  uInt i;
  Vector<String> colNames;
  String cmd;
  TaQLResult result;
  result = tableCommand (str, tempTables, colNames, cmd);
  cout << "    has been executed" << endl;
  if (result.isTable()) {
    tabp = result.table();
    cout << "    " << cmd << " result of " << tabp.nrow()
	 << " rows" << endl;
    if (colNames.nelements() > 0) {
      // Show the selected column names.
      cout << colNames.nelements() << " selected columns: ";
      for (i=0; i<colNames.nelements(); i++) {
        cout << " " << colNames(i);
      }
      cout << endl;
      // Show the contents of the columns.
      showTable (tabp, colNames);
    }
  } else {
    showExpr (result.node());
  }
  return tabp;
}


void showHelp()
{
  cerr << endl;
  cerr << "Any TaQL command can be given. TaQL is described at" << endl;
  cerr << "  www.astron.nl/casacore/trunk/casacore/notes/199.html" << endl;
  cerr << "The result of a CALC command and selected columns will be printed."
       << endl;
  cerr << "For other commands the number of selected rows is printed." << endl;
  cerr << endl;
  cerr << "It is possible to save the table resulting from a selection" << endl;
  cerr << "by assigning it like:" << endl;
  cerr << "     var = command" << endl;
  cerr << "Thereafter var can be used in another TaQL command like:" << endl;
  cerr << "     select from $var where ..." << endl;
  cerr << endl;
}

String substituteName (const String& name, const map<String,Table>& tables,
                       vector<const Table*>& tabs)
{
  map<String,Table>::const_iterator fnd = tables.find(name);
  if (fnd == tables.end()) {
    return name;
  }
  tabs.push_back (&(fnd->second));
  return String::toString (tabs.size());
}

vector<const Table*> replaceVars (String& str, const map<String,Table>& tables)
{
  vector<const Table*> tabs;
  // Initialize some variables.
  Bool backslash = False;
  Bool dollar = False;
  Bool squote = False;
  Bool dquote = False;
  String name;
  String out;
  out.reserve (str.size());
  // Loop through the entire string.
  for (uInt i=0; i<str.size(); ++i) {
    char tmp = str[i];
    // If a dollar was found, we might have a name.
    // Alphabetics and underscore are always part of name.
    if (dollar) {
      if (tmp=='_'  ||  (tmp>='a' && tmp<='z')  ||  (tmp>='A' && tmp<='Z')) {
        name += tmp;
        continue;
      } else if (tmp>='0' && tmp<='9' && !name.empty()) {
        // Numerics are only part if not first character.
        name += tmp;
        continue;
      } else {
        // End of name found. Try to substitute.
        dollar = False;
        out += substituteName(name, tables, tabs);
      }
    }
    // Handle possible single or double quotes.
    if (tmp == '"'  &&  !squote) {
      dquote = !dquote;
    } else if (tmp == '\''  &&  !dquote) {
      squote = !squote;
    } else if (!dquote && !squote) {
      // Set a switch if we have a dollar (outside quoted)
      // that is not preceeded by a backslash.
      if (tmp == '$'  &&  !backslash) {
        dollar = True;
        name = String();
      }
    }
    // Set a switch if we have a backslash (not preceeded by a backslash).
    backslash = (!backslash && tmp == '\\');
    // Add the character to output.
    out += tmp;
  }
  // The entire string has been handled.
  // Substitute a possible last name.
  // Insert a possible incomplete eval string as such.
  if (dollar) {
    out += substituteName(name, tables, tabs);
  }
  str = out;
  return tabs;
}

// Ask and execute commands till quit or ^D is given.
void askCommands()
{
  Regex varassRE("^[a-zA-Z_][a-zA-Z0-9_]*[ \t]*=");
  Regex assRE("[ \t]*=");
  Regex whiteRE("^[ \t]*");
  map<String,Table> tables;
  while (True) {
    String str;
    if (! readLineSkip (str, "TaQL> ", "#")) {
      cerr << endl;
      break;
    }
    // Remove leading whitespace.
    str.del (whiteRE);
    if (str.empty()) {
      cerr << "h or help gives help info" << endl;
    } else {
      if (str == "h"  ||  str == "help") {
        showHelp();
      } else if (str == "exit"  ||  str == "quit"  ||  str == "q") {
        break;
      } else {
#ifdef HAVE_READLINE
        add_history (str.c_str());
#endif
        try {
          String varName;
          String::size_type assLen = varassRE.match (str.c_str(), str.size());
          if (assLen != String::npos) {
            varName = str.before(assLen);
            str = str.from(assLen);
            varName.del (assRE);
            if (varName.empty()) {
              throw AipsError ("Variable name before command is empty");
            }
          }
          str.del (whiteRE);
          if (str.empty()) {
            // Remove variable.
            tables.erase (varName);
          } else {
            map<String,Table>::const_iterator it = tables.find (str);
            if (it != tables.end()) {
              cout << "  " << str << " has " << it->second.nrow()
                   << " rows" << endl;
            } else {
              if (!varName.empty()) {
                cout << varName << " = ";
              }
              cout << str << endl;
              vector<const Table*> tabs = replaceVars (str, tables);
              Table tab = doCommand (str, tabs);
              if (!varName.empty()  &&  !tab.isNull()) {
                tables[varName] = tab;
              }
            }
          }
        } catch (AipsError& x) {
          cerr << x.getMesg() << endl;
        }
      }
    }
  }
}


int main (int argc, const char* argv[])
{
  try {
    if (argc > 1) {
      // Execute the given command.
      doCommand (argv[1], vector<const Table*>());
    } else {
    // Do some interactive tests.
      askCommands();
    }
  } catch (AipsError& x) {
    cout << "\nCaught an exception: " << x.getMesg() << endl;
    return 1;
  } 
  return 0;               // successfully executed
}
