#include "Manager.h"
Manager::Manager(int bpOrder) {

}

Manager::~Manager() {

}

void Manager::run(const char* command_txt) {
    // Open command & log file
    fcmd.open(command_txt);
    flog.open("log.txt");
    if (!fcmd)
    {
        flog << "Fail to open command file" << endl;
        exit(-1);
    }

    flog << "Opening command file: " << command_txt << endl;
    flog.flush();

    // Read and Run command
    string cmd_line;



    while (getline(fcmd, cmd_line))//read command.txt 1 line
    {
        if (cmd_line == "LOAD")//LOAD command
        {
            bool printComment = LOAD();
            if (printComment)
                PrintSuccess("LOAD");
            else
                PrintErrorCode(100);
        }
        else if (cmd_line.find("ADD") != string::npos )
        {
            bool printComment = ADD(cmd_line);
            if (printComment)
                PrintSuccess("ADD");
            else
                PrintErrorCode(300);
        }
        else if (cmd_line == "EXIT")
        {
            PrintSuccess("Exit");

            //command.txt close
            fcmd.close();
            return;
        }
        
    }
}

void Manager::PrintSuccess(const char* cmd)
{
    //Success comment input log.txt
    flog << "===== " << cmd << " =====" << endl;
    flog << "Success" << endl;
    flog << "===============\n" << endl;
}
void Manager::PrintErrorCode(int num)
{
    //error comment input log.txt
    flog << "===== ERROR =====" << endl;
    flog << num << endl;
    flog << "===============\n" << endl;
}


bool Manager::LOAD() {
    if (bp == nullptr)
    {
        bp = new BpTree(&flog, 3);
        return true;
    }
    else
        return false;
}

//bool Manager::VLOAD() {
	//avl->GetVector(Print_vector);

    //return true;
//}

bool Manager::ADD(string line) {
    stringstream inputdata(line);
    vector<string> DATA_;
    string input;

    while (inputdata >> input)
    {
        DATA_.push_back(input);
    }
    FlightData* DATA = new FlightData();
    DATA->SetAirlineName(DATA_[0]);
    DATA->SetFlightNumber(DATA_[1]);
    DATA->SetDestination(DATA_[2]);
    DATA->SetNumberofSeats(atoi(DATA_[3].c_str()));
    DATA->SetStatus(DATA_[4]);
    bp->Insert(DATA);

    return true;
}
/*
bool Manager::PRINT_BP() {

}

bool Manager::SEARCH_BP(string name) {

}

bool Manager::SEARCH_BP(string start, string end) {

}

bool Manager::SEARCH_AVL(string name) {

}

bool Compare(FlightData* flight1, FlightData* flight2) {

}

bool Manager::VPRINT(string type_) {

}

void Manager::printErrorCode(int n) {
	ofstream fout;
	fout.open("log.txt", ofstream::app);
	fout << "========== ERROR ==========" << endl;
	fout << n << endl;
	fout << "===========================" << endl << endl;
	fout.close();
}
*/
