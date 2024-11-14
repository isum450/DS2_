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
            if (!printComment)
                PrintErrorCode(100);
        }
        else if (cmd_line.find("ADD") != string::npos )
        {
            bool printComment = ADD(cmd_line);
            if (!printComment)
                PrintErrorCode(300);
        }
        else if (cmd_line == "PRINT_BP")
        {
            bool printComment = PRINT_BP();
            if (!printComment)
                PrintErrorCode(700);
        }
        else if (cmd_line == "EXIT")
        {
            //command.txt close
            fcmd.close();
            return;
        }
        
    }
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

bool Manager::ADD(string line) {//BpTree add command
    stringstream inputdata(line);
    vector<string> DATA_;
    string input;

    while (inputdata >> input)
    {
        DATA_.push_back(input);
    }
    if (DATA_.size() != 5)
        return false;
    FlightData* DATA = new FlightData();
    DATA->SetAirlineName(DATA_[1]);
    DATA->SetFlightNumber(DATA_[2]);
    DATA->SetDestination(DATA_[3]);
    DATA->SetStatus(DATA_[4]);

    

    BpTreeNode* addData = bp->searchDataNode(DATA->GetFlightNumber());
    if (addData == nullptr)//DATA search failed
    {
        bp->Insert(DATA);

        flog << "======== ADD ========" << endl;
        flog << DATA->GetFlightNumber() << " | " << DATA->GetAirlineName() << " | " << DATA->GetDestination() << " | " << DATA->GetNumberofSeats() << " | " << DATA->GetStatus() << endl;
        flog << "=====================" << endl;

        return true;
    }
    else if(BpTreeDataNode* curr = dynamic_cast<BpTreeDataNode*>(addData))//DATA search
    {
        auto it = curr->getDataMap()->find(DATA->GetFlightNumber());
        string state;
        if (it != curr->getDataMap()->end())
        {
            state = it->second->GetStatus();
        }

        if ((state == "Cancelled" && DATA->GetStatus() == "Boarding") ||
            (state == "Boarding" && DATA->GetStatus() == "Boarding") ||
            (state == "Delayed" && DATA->GetStatus() == "Delayed") )
        {
            if (it->second->GetNumberofSeats() > 0)
            {
                it->second->SetStatus(DATA->GetStatus());
                it->second->SetSeatsDec();

                flog << "======== ADD ========" << endl;
                flog << it->second->GetFlightNumber() << " | " << it->second->GetAirlineName() << " | " << it->second->GetDestination() << " | " << it->second->GetNumberofSeats() << " | " << it->second->GetStatus() << endl;
                flog << "=====================" << endl;

                return true;
            }
            else if (it->second->GetNumberofSeats() == 0)
            {
                //AVL TREE
                return false;
            }
            
        }
        else if (DATA->GetStatus() == "Departure")
        {
            if (it->second->GetNumberofSeats() == 0)
            {
                //AVL TREE
            }
            else 
                it->second->SetSeatsDec();

            flog << "======== ADD ========" << endl;
            flog << it->second->GetFlightNumber() << " | " << it->second->GetAirlineName() << " | " << it->second->GetDestination() << " | " << it->second->GetNumberofSeats() << " | " << it->second->GetStatus() << endl;
            flog << "=====================" << endl;

            return true;
        }
        else
            return false;
        
    }
}

bool Manager::PRINT_BP() {
    if (bp->getRoot() == nullptr)
    {
        return false;
    }
    bp->Print();
    return true;
}
/*
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
