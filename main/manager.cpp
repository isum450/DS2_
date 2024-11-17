#include "Manager.h"
Manager::Manager(int bpOrder) {
    avl = new AVLTree();
    avl->setRoot(nullptr);
}

Manager::~Manager() {
    delete(avl);
    delete(bp);
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

    flog.flush();

    // Read and Run command
    string cmd_line;


    while (getline(fcmd, cmd_line))//read command.txt 1 line
    {
        if (cmd_line == "LOAD")//LOAD
        {
            bool printComment = LOAD();//BpTree LOAD

            if (!printComment)//error
            {
                PrintErrorCode(100);
                exit(-1);
            }
            
        }
        else if (cmd_line == "VLOAD")
        {
            bool printComment = VLOAD();//AVL tree->Vector
            if (!printComment)//error
                PrintErrorCode(200);
        }
        else if (cmd_line.find("ADD") != string::npos )
        {
            bool printComment = ADD(cmd_line);//BpTree ADD DATA
            if (!printComment)//error
                PrintErrorCode(300);
        }
        else if (cmd_line.find("SEARCH_BP") != string::npos)
        {
            stringstream ss(cmd_line);
            vector<string> DATA_;
            string input;

            while (getline(ss, input, '\t'))
            {
                DATA_.push_back(input);
            }

            if (DATA_.size() == 3)//Range search
            {
                SEARCH_BP(DATA_[1], DATA_[2]);
            }
            else if (DATA_.size() == 2)
            {
                SEARCH_BP(DATA_[1]);//flightNumber search
            }
        }
        else if (cmd_line.find("SEARCH_AVL") != string::npos)
        {
            stringstream ss(cmd_line);
            vector<string> flightnumber;
            string input;

            while (getline(ss, input, '\t'))
            {
                flightnumber.push_back(input);
            }
            
            if (flightnumber.size() != 2)//command size error
                PrintErrorCode(500);

            bool printComment = SEARCH_AVL(flightnumber[1]);

            if (!printComment)//AVL error
                PrintErrorCode(500);
        }
        else if (cmd_line.find("VPRINT") != string::npos)
        {
            stringstream ss(cmd_line);
            vector<string> type_;
            string input;

            while (getline(ss, input, '\t'))
            {
                type_.push_back(input);
            }
            bool printComment;
            if (type_[1] == "A")//AVL Print type A
            {
                printComment = VPRINT("A");
            }
            else if (type_[1] == "B")//AVL Print type B
            {
                printComment = VPRINT("B");
            }
            
            if (!printComment)//error
                PrintErrorCode(600);
        }
        else if (cmd_line == "PRINT_BP")
        {
            bool printComment = PRINT_BP();
            if (!printComment)//error
                PrintErrorCode(700);
        }
        else if (cmd_line == "EXIT")//program exit
        {
            //command.txt close
            flog << "========EXIT========" << endl;
            flog << "Success" << endl;
            flog << "===================" << endl;
            flog.close();
            fcmd.close();
            return;
        }
        else
        { 
            PrintErrorCode(800);
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


bool Manager::LOAD() {//input_data.txt->BpTree

    if (bp == nullptr)
    {
        bp = new BpTree(&flog, 3);//bp initialize

        //input_data.txt open
        ifstream ifp;
        ifp.open("input_data.txt");

        if (ifp.fail())//file not open
        {
            return false;
        }

        string line;//read input_data.txt 1 line

        while (getline(ifp, line))
        {
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


            //Exception handling
            if (DATA->GetAirlineName() == "JEJU" && DATA->GetDestination() != "CJU")
            {
                delete(bp);
                bp = nullptr;
                return false;
            }
            
            if (DATA->GetAirlineName() != "KoreanAir" && DATA->GetAirlineName() != "ASIANA" && DATA->GetAirlineName() != "JEJU" && DATA->GetAirlineName() != "JeanAir" && DATA->GetAirlineName() != "AirKwangwoon")
            {
                delete(bp);
                bp = nullptr;
                return false;
            }
            
            if ((DATA->GetAirlineName() == "KoreanAir" || DATA->GetAirlineName() == "ASIANA") && DATA->GetNumberofSeats() > 7)
            {
                delete(bp);
                bp = nullptr;
                return false;
            }
            else if ((DATA->GetAirlineName() == "JEJU" || DATA->GetAirlineName() == "JeanAir") && DATA->GetNumberofSeats() > 5)
            {
                delete(bp);
                bp = nullptr;
                return false;
            }
            else if (DATA->GetAirlineName() == "AirKwangwoon" && DATA->GetNumberofSeats() > 6)
            {
                delete(bp);
                bp = nullptr;
                return false;
            }

            bp->Insert(DATA);//bp->insert

        }
        ifp.close();//close inpout_data.txt

        flog << "========LOAD========" << endl;
        flog << "Success" << endl;
        flog << "====================" << endl;

        return true;
    }
    else
    {
        delete(bp);
        bp = nullptr;
        return false;//already bp exist
    }
        
}

bool Manager::VLOAD() {
    if (avl->getRoot() == nullptr)//AVL tree empty error
        return false;

	avl->GetVector(Print_vector);//AVL Tree -> Vector

    return true;
}

bool Manager::ADD(string line) {//BpTree add command
    
    stringstream ss(line);
    vector<string> DATA_;
    string input;

    while (getline(ss, input, '\t'))
    {
        DATA_.push_back(input);
    }
    if (DATA_.size() != 5)//command size error
        return false;

    FlightData* DATA = new FlightData();
    DATA->SetAirlineName(DATA_[1]);
    DATA->SetFlightNumber(DATA_[2]);
    DATA->SetDestination(DATA_[3]);
    DATA->SetStatus(DATA_[4]);


    //Exception handling
    //jeju
    if (DATA->GetAirlineName() == "JEJU" && DATA->GetDestination() != "CJU")
    {
        return false;
    }
    
    if (DATA->GetAirlineName() != "KoreanAir" && DATA->GetAirlineName() != "ASIANA" && DATA->GetAirlineName() != "JEJU" && DATA->GetAirlineName() != "JeanAir" && DATA->GetAirlineName() != "AirKwangwoon")
    {
        return false;
    }


    //DATA

    BpTreeNode* addData = bp->searchDataNode(DATA->GetFlightNumber());

    if (addData == nullptr)//DATA not exist in BpTree
    {
        bp->Insert(DATA);//insert data

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

        //ADD condition
        if ((state == "Cancelled" && DATA->GetStatus() == "Boarding") ||
            (state == "Boarding" && DATA->GetStatus() == "Boarding") ||
            (state == "Delayed" && DATA->GetStatus() == "Delayed") ||
            (state == "Boarding" && DATA->GetStatus() == "Cancelled"))
        {
            if ((state == "Boarding" && DATA->GetStatus() == "Boarding") ||
                (state == "Delayed" && DATA->GetStatus() == "Delayed"))
            {
                it->second->SetSeatsDec();
            }
            else if (state == "Cancelled" && DATA->GetStatus() == "Boarding" )
            {
                it->second->SetStatus(DATA->GetStatus());
                it->second->SetSeatsDec();
            }
            else if (state == "Boarding" && DATA->GetStatus() == "Cancelled")
                it->second->SetStatus(DATA->GetStatus());

            //input comment log.txt
            flog << "======== ADD ========" << endl;
            flog << it->second->GetFlightNumber() << " | " << it->second->GetAirlineName() << " | " << it->second->GetDestination() << " | " << it->second->GetNumberofSeats() << " | " << it->second->GetStatus() << endl;
            flog << "=====================" << endl;

            if (it->second->GetNumberofSeats() == 0)           
                avl->Insert(DATA);//AVL TREE
            return true;
        }
        else
            return false;
        
    }
}

bool Manager::PRINT_BP() 
{

    if (bp->getRoot() == nullptr)//bp empty
    {
        return false;
    }

    bp->Print();//bp->print
    return true;
}

bool Manager::SEARCH_BP(string name) {
    if (bp->getRoot() == nullptr)
    {
        PrintErrorCode(400);
        return false;
    }
    bool printComment = bp->SearchModel(name);//bp print
    if (!printComment)//error
        PrintErrorCode(400);

    return true;
}

bool Manager::SEARCH_BP(string start, string end) {
    if (bp->getRoot() == nullptr)
    {
        PrintErrorCode(400);
        return false;
    }

    bool printComment = bp->SearchRange(start, end);//bp range search -> print
    if (!printComment)//error
        PrintErrorCode(400);

    return true;
}

bool Manager::SEARCH_AVL(string name)
{
    if (avl->getRoot() == nullptr)//AVL empty
        return false;

    FlightData* printData = avl->Search(name);

    if (printData == nullptr)
        return false;

    flog << "====== SEARCH_AVL ======" << endl;
    flog << printData->GetFlightNumber() << " | " << printData->GetAirlineName() << " | " << printData->GetDestination() << " | " << printData->GetNumberofSeats() << " | " << printData->GetStatus() << endl;

    return true;
}


bool compareA(FlightData* a, FlightData* b) {
    if (a->GetAirlineName() != b->GetAirlineName()) 
    {
        return a->GetAirlineName() < b->GetAirlineName(); // arilinename ascending
    }
    if (a->GetDestination() != b->GetDestination()) 
    {
        return a->GetDestination() < b->GetDestination(); // departure ascending
    }
    return a->GetStatus() > b->GetStatus(); // status descending
}

bool compareB(FlightData* a, FlightData* b) {
    if (a->GetDestination() != b->GetDestination()) {
        return a->GetDestination() < b->GetDestination(); // departure ascending
    }
    if (a->GetStatus() != b->GetStatus()) {
        return a->GetStatus() < b->GetStatus(); // status ascending
    }
    return a->GetAirlineName() > b->GetAirlineName(); // airlinename descending
}

bool Manager::VPRINT(string type_) {
    if (Print_vector.empty()) //vector empty
    {
        return false;
    }
    if (type_ == "A")//VPRINT A
    {
        sort(Print_vector.begin(), Print_vector.end(), compareA);
        flog << "======= VPRINT A =======\n";
        for (auto& flight : Print_vector) {
            flog << flight->GetAirlineName() << " | "
                << flight->GetFlightNumber() << " | "
                << flight->GetDestination() << " | "
                << flight->GetStatus() << "\n";
        }
        flog << "======================\n";
    }
    else if (type_ == "B")//VPRINT B
    {
        sort(Print_vector.begin(), Print_vector.end(), compareB);
        flog << "======= VPRINT B =======\n";
        for (auto& flight : Print_vector) {
            flog << flight->GetAirlineName() << " | "
                << flight->GetFlightNumber() << " | "
                << flight->GetDestination() << " | "
                << flight->GetStatus() << "\n";
        }
        flog << "======================\n";
    }
}
