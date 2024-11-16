#include "Manager.h"
Manager::Manager(int bpOrder) {
    avl = new AVLTree();
    avl->setRoot(nullptr);
}

Manager::~Manager() {

}

void Manager::run(const char* command_txt) {
    // Open command & log file
    fcmd.open(command_txt);
    flog.open("C:\\Users\\leeso\\DS2\\DS2_\\main\\log.txt");
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
        if (cmd_line == "LOAD")//LOAD command
        {
            bool printComment = LOAD();
            if (!printComment)
                PrintErrorCode(200);
        }
        else if (cmd_line == "VLOAD")
        {
            bool printComment = VLOAD();
            if (!printComment)
                PrintErrorCode(100);
        }
        else if (cmd_line.find("ADD") != string::npos )
        {
            bool printComment = ADD(cmd_line);
            if (!printComment)
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

            if (DATA_.size() == 3)
            {
                SEARCH_BP(DATA_[1], DATA_[2]);
            }
            else if (DATA_.size() == 2)
            {
                SEARCH_BP(DATA_[1]);
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
            
            if (flightnumber.size() != 2)
                PrintErrorCode(500);

            bool printComment = SEARCH_AVL(flightnumber[1]);

            if (!printComment)
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
            if (type_[1] == "A")
            {
                printComment = VPRINT("A");
            }
            else if (type_[1] == "B")
            {
                printComment = VPRINT("B");
            }
            
            if (!printComment)
                PrintErrorCode(600);
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

bool Manager::VLOAD() {
	avl->GetVector(Print_vector);

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
            it->second->SetStatus(DATA->GetStatus());
            it->second->SetSeatsDec();

            flog << "======== ADD ========" << endl;
            flog << it->second->GetFlightNumber() << " | " << it->second->GetAirlineName() << " | " << it->second->GetDestination() << " | " << it->second->GetNumberofSeats() << " | " << it->second->GetStatus() << endl;
            flog << "=====================" << endl;

            if (it->second->GetNumberofSeats() == 0)           
                avl->Insert(DATA);//AVL TREE
            return true;
            
            
        }
        else if (DATA->GetStatus() == "Departure")
        {
            it->second->SetSeatsDec();

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

bool Manager::PRINT_BP() {

    if (bp->getRoot() == nullptr)
    {
        return false;
    }

    bp->Print();
    return true;
}

bool Manager::SEARCH_BP(string name) {
    
    bool printComment = bp->SearchModel(name);
    if (!printComment)
        PrintErrorCode(400);

    return true;
}

bool Manager::SEARCH_BP(string start, string end) {
    bool printComment = bp->SearchRange(start, end);
    if (!printComment)
        PrintErrorCode(400);

    return true;
}

bool Manager::SEARCH_AVL(string name)
{
    if (avl->getRoot() == nullptr)
        return false;

    AVLNode* currentNode = avl->getRoot();

    while (currentNode != nullptr) {
        if (currentNode->getFlightData()->GetFlightNumber() == name)
        {
            flog << "====== SEARCH_AVL ======" << endl;
            flog << currentNode->getFlightData()->GetFlightNumber() << " | " << currentNode->getFlightData()->GetAirlineName() << " | " << currentNode->getFlightData()->GetDestination() << " | " << currentNode->getFlightData()->GetNumberofSeats() << " | " << currentNode->getFlightData()->GetStatus() << endl;
            return true;
        }
        else if (name < currentNode->getFlightData()->GetFlightNumber()) 
        {
            currentNode = currentNode->getLeft();
        }
        else 
        {
            currentNode = currentNode->getRight();
        }
    }

    return false;
}


bool compareA(FlightData* a, FlightData* b) {
    if (a->GetAirlineName() != b->GetAirlineName()) {
        return a->GetAirlineName() < b->GetAirlineName(); // 항공사명 오름차순
    }
    if (a->GetDestination() != b->GetDestination()) {
        return a->GetDestination() < b->GetDestination(); // 도착지명 오름차순
    }
    return a->GetStatus() > b->GetStatus(); // 상태 정보 내림차순
}

// 조건 B에 따라 정렬하는 함수
bool compareB(FlightData* a, FlightData* b) {
    if (a->GetDestination() != b->GetDestination()) {
        return a->GetDestination() < b->GetDestination(); // 도착지명 오름차순
    }
    if (a->GetStatus() != b->GetStatus()) {
        return a->GetStatus() < b->GetStatus(); // 상태 정보 오름차순
    }
    return a->GetAirlineName() > b->GetAirlineName(); // 항공사명 내림차순
}

bool Manager::VPRINT(string type_) {
    if (Print_vector.empty()) {
        return false;
    }
    if (type_ == "A")
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
    else if (type_ == "B")
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
