#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h>
#include <limits>
#include <cmath>
#include <time.h>
using namespace std;


const bool debug = false;


std::fstream& GotoLine(std::fstream& file, unsigned int num){
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}


class Wp
{
public:
	unsigned int line = 0;
	string name = "";
	Wp(string n, unsigned int l)
	{
		name = n;
		line = l; 	
	}
};

class var
{
	public:
	string type;
	double fval;
	string sval;
	int index = 0;
	string name;
	var(string n)
	{
		
		if (n.substr(0,5) == "rand:")
		{
			srand((unsigned)time(0));
			name = n.substr(0,4);
			fval = rand() % stoi(n.substr(5));
			//cout << "DEBUG: " << fval << endl;
			get();
		}else
		{
			name = n;
		}
		
		
	}

	void updateType()
	{
		if (sval != "")
		{
			type = "string";
		}else
		{
			type = "num";
		}
	}

	void operator =(string val)
	{
		if (val.substr(0,5) == "rand:")
		{
			srand((unsigned)time(0));
			fval = rand() % stoi(val.substr(5));
			//cout << "DEBUG: " << fval << endl;
			get();
			return;
		}
		try
		{
			to_string(stod(val));
			fval = stod(val);
			type = "num";
		}catch(const exception&)
		{
			sval = val;
			type = "string";
		}
		if (debug)
		{
			cout << "Debug: = operator used: type:" << type << endl;
		}
		
	}

	void operator =(var a)
	{
		updateType();
		if (type == "num")
		{
			fval = a.fval;
		}else
		{
			sval = a.sval;
		}
	}


	void operator +=(string val)
	{
		try
		{
			to_string(stod(val));
			fval += stod(val);
		}catch(const exception&)
		{
			sval += val;
		}
		
	}

	void operator +=(var a)
	{
		updateType();
		if (type == "num")
		{
			fval += a.fval;
		}else
		{
			sval += a.sval;
		}
	}

	void operator *=(string val)
	{
		try
		{
			to_string(stod(val));
			fval *= stod(val);
		}catch(const exception&)
		{
			
		}
		
	}

	void operator *=(var a)
	{
		updateType();
		if (type == "num")
		{
			fval *= a.fval;
		}else
		{
			
		}
	}

	void operator /=(string val)
	{
		try
		{
			to_string(stod(val));
			fval /= stod(val);
		}catch(const exception&)
		{
			
		}
		
	}

	void operator /=(var a)
	{
		updateType();
		if (type == "num")
		{
			fval /= a.fval;
		}else
		{
			
		}
	}

		void operator -=(string val)
	{
		try
		{
			to_string(stod(val));
			fval -= stod(val);
		}catch(const exception&)
		{
			
		}
		
	}

	void operator -=(var a)
	{
		updateType();
		if (type == "num")
		{
			fval -= a.fval;
		}else
		{
			
		}
	}

	string get()
	{
		updateType();
		if (type == "num")
		{
			return to_string(fval);
		}else
		{
			return sval;
		}
	}

	void copy(var a)
	{
		name = a.name;
		fval = a.fval;
		sval = a.sval;
		type = a.type;
		index = a.index;
	}

	bool operator ==(var a)
	{
		if (type == "num")
		{
			return (fval==a.fval);
		}else
		{
			return (sval == a.sval);
		}
	}

	bool operator <=(var a)
	{
		if (type == "num")
		{
			return (fval <= a.fval);
		}else
		{
			return (sval <= a.sval);
		}
	}

	bool operator >=(var a)
	{
		if (type == "num")
		{
			return (fval>=a.fval);
		}else
		{
			return (sval >= a.sval);
		}
	}

	bool operator <(var a)
	{
		if (type == "num")
		{
			return (fval<a.fval);
		}else
		{
			return (sval < a.sval);
		}
	}

	bool operator >(var a)
	{
		if (type == "num")
		{
			return (fval>a.fval);
		}else
		{
			return (sval>a.sval);
		}
	}

	bool operator !=(var a)
	{
		if (type == "num")
		{
			return (fval!=a.fval);
		}else
		{
			return (sval!=a.sval);
		}
	}

	void operator %(var a)
	{
		if (type == "num")
		fval = ((int)fval%(int)a.fval);
	}

	void operator %(string a)
	{
		if (type == "num"){
			int k = stod(a);
			fval = ((int)fval%(int)k);
		}
	}
	

};

vector<var> vars;
vector<Wp> wps;

void test(string arg, fstream& read)
{
	static int globalIndex = 0;
	static unsigned int currentLine = 0;
	currentLine++;
	string str;

	if (debug)
	{
		cout << "Debug: arg: " << arg << endl;
	}
	if (arg.substr(0,2) == "//")
	{
		if (debug)
		{
			cout << "Debug: Found // substring\n";
		}
		getline(read,str);
		return;
	}
	
	if (arg == "var")
	{
		
		read >> str;
		if (debug)
		{
			cout << "Debug: found var, param: " << str << endl;
		}
		var n(str);
		vars.push_back(n);
		vars[vars.size()-1].index = globalIndex;
		globalIndex++;
		return;
	}

	if (arg == "mov" || arg == "add" || arg == "sub" || arg == "mul" || arg == "div" || arg == "rem")
	{
		read >> str;
		if (debug)
		{
			cout << "Debug: found " << arg << " param:" << str << endl;
		}
		var to("null");
		var from("null");
		for (int i = 0; i < vars.size(); i++)
		{
			if (str == vars[i].name)
			{
				to.copy(vars[i]);
				break;
			}
		}
		if (to.name == "null")
		{
			return;
		}
		if (debug)
		{
			cout << "Debug: to.name: "<< to.name << "\n";
		}
		read >> str;
		for (int i = 0; i < vars.size(); i++)
		{
			if (str == vars[i].name)
			{
				from.copy(vars[i]);
				break;
			}
		}

		
		if (from.name == "null")
		{
			if (arg == "mov")
					to = str;
			if (arg == "add")
				to += str;
			if (arg == "sub")
				to -= str;
			if (arg == "mul")
				to *= str;
			if (arg == "div")
				to /= str;
			if (arg == "rem")
				to % (str);
		}else
		{
			if (arg == "mov")
					to = from;
			if (arg == "add")
				to += from;
			if (arg == "sub")
				to -= from;
			if (arg == "mul")
				to *= from;
			if (arg == "div")
				to /= from;
			if (arg == "rem")
				to % from;
		}
		if (debug)
		{
			cout << "Debug: from.name: " << from.name << endl;
			cout << "Debug: val: " << to.get() << endl;
		}
		vars[to.index].copy(to);

		
		return;
	}

	if (arg == "wp")
	{
		read >> str;
		Wp n(str, currentLine+1);
		wps.push_back(n);
		return;
	}

	if (arg == "goto")
	{
		read >> str;
		for (int i = 0; i < wps.size(); i++)
		{
			if (str == wps[i].name)
			{
				GotoLine(read,wps[i].line);
				return;
			}
		}
		GotoLine(read,stoi(str));
		return;
	}
	if (arg == "print")
	{
		read >> str;
		if (debug)
		{
			cout << "Debug: str " << str << endl;
		}
		for (int i = 0; i < vars.size(); i++)
		{
			if (str == vars[i].name)
			{
				if (debug)
				{
					cout << "Debug: match\n";
				}
				cout << vars[i].get() << endl;
				return;
			}
		}
		string full;
		getline(read, full);
		str +=full;
		if (debug)
		{
			cout << "Debug: " << str << endl;
		}
		cout << str << endl;
		return;
	}

	if (arg == "if")
	{
		if (debug)
		{
			cout << "Debug: if found\n";
		}
		var var1("null");
		var var2("null");
		string compare;
		read >> str;
		read >> compare;
		
		for (int i = 0; i < vars.size(); i++)
		{
			if (str == vars[i].name)
			{
				if (debug)
				{
					cout << "Debug: match\n";
				}
				var1.copy(vars[i]);
				break;
			}
		}
		if (var1.name == "null")
		{
			var1 = str;
		}
		read >> str;
		for (int i = 0; i < vars.size(); i++)
		{
			if (str == vars[i].name)
			{
				if (debug)
				{
					cout << "Debug: match\n";
				}
				var2.copy(vars[i]);
				break;
			}
		}
		if (var2.name == "null")
		{
			var2 = str;
		}
		bool result;
		//compare
		if (compare == "==")
		{
			result = (var1 == var2);
		}
		if (compare == ">=")
		{
			result = (var1 >= var2);
		}
		if (compare == "<=")
		{
			result = (var1 <= var2);
		}
		if (compare == "<")
		{
			result = (var1 < var2);
		}
		if (compare == ">")
		{
			result = (var1 > var2);
		}
		if (compare == "!=")
		{
			result = (var1 != var2);
		}
		if (!result)
		{
			int balance = 1;
			while (balance > 0)
			{
				
				read >> str;
				if (str == "if")
				{
					balance++;
				}else if (str == "end")
				{
					balance--;
				}
			}
			return;
		}
	}

	if (arg == "ask")
	{
		read >> str;
		for (int i = 0; i < vars.size();i++)
		{
			if (str == vars[i].name)
			{
				string input;
				cout << "?";
				cin >> input;
				vars[i] = input;
				return;
			}
		}
	}

	if (arg == "abs")
	{
		read >> str;
		
		for (int i = 0; i < vars.size();i++)
		{
			if (str == vars[i].name)
			{
				
				if (vars[i].fval < 0){
					vars[i].fval *=-1;
				}
				
				return;
			}
		}
	}

	
	
}


void run(string name)
{
	fstream read;
	if (name.find(".")==string::npos)
	{
		name+=".txt";
	}
	read.open(name);
	assert(read.good());

	string str;
	while(!read.eof())
	{
		read >> str;
		test(str, read);
	}
	read.close();
}


int main() {
  run("program.txt");
}