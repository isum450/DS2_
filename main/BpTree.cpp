#include "BpTree.h"

BpTree::BpTree(ofstream* fout, int order) {

	//initializing
	root = nullptr;
	this->order = order;
	this->fout = fout;

	//input_data.txt open
	ifstream ifp;
	ifp.open("input_data.txt");

	if (ifp.fail())//file not open
	{
		*fout << "====Error====" << endl;
		*fout << "100" << endl;
		*fout << "=============" << endl;
		exit(1);
	}
	else//LOAD success
	{
		*fout << "========LOAD========" << endl;
		*fout << "Success" << endl;
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

		Insert(DATA);//bp->insert

	}

	ifp.close();//close inpout_data.txt
	*fout << "====================" << endl;
}

bool BpTree::Insert(FlightData* newData) {
	
	
	if (getRoot() == nullptr)//insert root
	{
		BpTreeDataNode* dataNode = new BpTreeDataNode();
		dataNode->insertDataMap(newData->GetFlightNumber(), newData);//data insert
		root = dataNode;//root create
	}
	else
	{
		BpTreeNode* current = getRoot();

		while (current) 
		{
			if (BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(current)) {
				// current == datanode
				auto datamap = current->getDataMap();

				current->insertDataMap(newData->GetFlightNumber(), newData);//insert data

				if (excessDataNode(current)) //if(data size > 2) 
				{
					splitDataNode(current);//data split
				}
				return true;
			}
			else if (BpTreeIndexNode* indexNode = dynamic_cast<BpTreeIndexNode*>(current)) {
				// current == indexnode
				auto idxmap = indexNode->getIndexMap();
				string key[2];
				BpTreeNode* node[2];
				int i = 0;
				for (const auto& pair : *idxmap) {
					key[i] = pair.first;
					node[i] = pair.second;
					i++;
				}
				string it = newData->GetFlightNumber();
				if (i == 1)//index size == 1
				{
					if (key[0] > it) {//first key > insert data key
						current = indexNode->getMostLeftChild();
					}
					else if (key[0] < it) {//second key < insert data key
						current = node[0];
					}
				}
				else if (i == 2)//index size == 2
				{
					if (key[0] > it) {////first key > insert data key
						current = indexNode->getMostLeftChild();
					}
					else if ((key[0] < it) && (key[1] > it))//second key > insert data key > first key
					{
						current = node[0];
					}
					else if (key[1] < it) {//second key < insert data key
						current = node[1];
					}
				}

			}
		}

	}
	return true;
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > order - 1)return true;//order is equal to the number of elements 
	else return false;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {//split datanode

	BpTreeDataNode* newNode = new BpTreeDataNode();

	auto datamap = pDataNode->getDataMap();

	string key[3];
	FlightData* node[3];
	int i = 0;

	for (const auto& pair : *datamap) {
		key[i] = pair.first;
		node[i] = pair.second;
		i++;
	}
	//i == 2
	newNode->insertDataMap(key[0], node[0]);
	pDataNode->deleteMap(key[0]);
	newNode->insertDataMap(key[1], node[1]);
	pDataNode->deleteMap(key[1]);
	//pDataNode -> newNode(size2) - pDataNode(size1)

	if (pDataNode->getPrev() != nullptr)//next, prev connect
	{
		pDataNode->getPrev()->setNext(newNode);
		newNode->setPrev(pDataNode->getPrev());
	}
	newNode->setNext(pDataNode);
	pDataNode->setPrev(newNode);

	// splited data node connection
	if (pDataNode->getParent() == nullptr) //parent == root
	{
		BpTreeIndexNode* newRoot = new BpTreeIndexNode();

		newRoot->insertIndexMap(key[1], pDataNode);
		newRoot->setMostLeftChild(newNode);
		pDataNode->setParent(newRoot);
		newNode->setParent(newRoot);
		root = newRoot;
	}
	else//parent !- root
	{
		BpTreeIndexNode* parent = dynamic_cast<BpTreeIndexNode*>(pDataNode->getParent());
		parent->insertIndexMap(key[1], pDataNode);
		parent->setMostLeftChild(newNode);
		newNode->setParent(parent);
		pDataNode->setParent(parent);
		if (excessIndexNode(parent)) {
			splitIndexNode(parent);
		}
	}

}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	// new indexnode create
	BpTreeIndexNode* newNode = new BpTreeIndexNode();

	auto idxmap = pIndexNode->getIndexMap();
	vector<pair<string, BpTreeNode*>>tempIndex(idxmap->begin(), idxmap->end());

	newNode->setMostLeftChild(pIndexNode->getMostLeftChild());
	newNode->insertIndexMap(tempIndex[0].first, tempIndex[0].second);
	newNode->insertIndexMap(tempIndex[1].first, nullptr);

	//child - new_indexnode connection
	if (tempIndex[0].second != nullptr) {
		tempIndex[0].second->setParent(newNode);
	}
	if (pIndexNode->getMostLeftChild() != nullptr) {
		pIndexNode->getMostLeftChild()->setParent(newNode);
	}
	pIndexNode->deleteMap(tempIndex[0].first);
	pIndexNode->setMostLeftChild(tempIndex[1].second);
	pIndexNode->deleteMap(tempIndex[1].first);

	if (pIndexNode->getMostLeftChild() != nullptr )
	{
		pIndexNode->getMostLeftChild()->setParent(pIndexNode);
	}

	if (pIndexNode->getParent() == nullptr) {//parent node == root
		BpTreeIndexNode* newRoot = new BpTreeIndexNode();
		newRoot->insertIndexMap(tempIndex[1].first, pIndexNode);
		newRoot->setMostLeftChild(newNode);
		pIndexNode->setParent(newRoot);
		newNode->setParent(newRoot);
		root = newRoot;
	}
	else {//parent node != root
		BpTreeIndexNode* parent = dynamic_cast<BpTreeIndexNode*>(pIndexNode->getParent());
		parent->insertIndexMap(tempIndex[1].first, pIndexNode);
		parent->setMostLeftChild(newNode);
		newNode->setParent(parent);
		pIndexNode->setParent(parent);

		if (excessIndexNode(parent)) {//if(parent size > 2)
			splitIndexNode(parent);//split parent node
		}
	}
}

BpTreeNode* BpTree::searchDataNode(string name) {//data search
	BpTreeNode* pCur = root;

	while (pCur->getMostLeftChild() != nullptr)//move to data node
		pCur = pCur->getMostLeftChild();

	while (pCur) {//data node search
		if (pCur->getDataMap()->find(name) != pCur->getDataMap()->end()) {
			return pCur;
		}
		pCur = pCur->getNext();
	}
	return nullptr;
}


bool BpTree::SearchModel(string model_name) {

	BpTreeNode* search_name = searchDataNode(model_name);

	if (search_name != nullptr)
	{

		*fout << "========SEARCH_BP========" << endl;

		while (search_name != nullptr)
		{

			auto datamap = search_name->getDataMap();

			for (const auto& pair : *datamap) {
				FlightData foutData = *pair.second;
				*fout << pair.first << " | " << foutData.GetAirlineName() << " | " << foutData.GetDestination() << foutData.GetFlightNumber() << " | " << foutData.GetNumberofSeats() << " | " << foutData.GetStatus() << endl;
			}

			search_name = search_name->getNext();

		}
		*fout << "========================" << endl;
		return true;
	}

	return false;
}


bool BpTree::SearchRange(string start, string end) {//searchrange
	BpTreeNode* current = root;

	while (current->getMostLeftChild() != nullptr) //move to datanode
	{
		current = current->getMostLeftChild();
	}

	*fout << "========SEARCH_RANGE========" << endl;

	bool foundData = false;

	while (current != nullptr) {
		auto datamap = current->getDataMap();

		for (const auto& pair : *datamap) {
			string flightNumber = pair.first;

			if (flightNumber[0] >= start[0] && flightNumber[0] <= end[0]) //condition
			{
				FlightData foutData = *pair.second;

				*fout << flightNumber << " | "
					<< foutData.GetAirlineName() << " | "
					<< foutData.GetDestination() << " | "
					<< foutData.GetFlightNumber() << " | "
					<< foutData.GetNumberofSeats() << " | "
					<< foutData.GetStatus() << endl;

				foundData = true;
			}
		}

		current = current->getNext();
	}

	*fout << "=============================" << endl;

	return foundData;
}


void BpTree::Print() {
	BpTreeNode* current = root;
	while (current->getMostLeftChild() != nullptr)//move to datanode
		current = current->getMostLeftChild();
	
	//input log.txt
	*fout << "========PRINT_BP========" << endl;
	while (current!= nullptr)
	{
		auto datamap = current->getDataMap();


		for (const auto& pair : *datamap) {
			FlightData foutData = *pair.second;
			*fout << pair.first <<" | " << foutData.GetAirlineName() << " | " << foutData.GetDestination() << foutData.GetFlightNumber() << " | " << foutData.GetNumberofSeats() << " | " << foutData.GetStatus() << endl;
		}

		current = current->getNext();
	}
	*fout << "========================" << endl;
}
