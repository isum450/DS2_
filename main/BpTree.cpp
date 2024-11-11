#include "BpTree.h"

BpTree::BpTree(ofstream* fout, int order) {
	root = nullptr;
	this->order = order;
	this->fout = fout;

	ifstream ifp;
	ifp.open("C:\\Users\\is104\\DATASTRUCTURE\\main\\main\\input_data.txt");

	if (ifp.fail())//file not connect
	{
		*fout << "====Error====" << endl;
		*fout << "100" << endl;
		*fout << "=============" << endl;
		exit(1);
	}
	else//not error
		*fout << "====LOAD====" << endl;

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

		Insert(DATA);

	}
}

bool BpTree::Insert(FlightData* newData) {
	
	
	if (getRoot() == nullptr)//insert root ó�� ��Ʈ�� ������ ���� ����
	{
		BpTreeDataNode* dataNode = new BpTreeDataNode();
		dataNode->insertDataMap(newData->GetFlightNumber(), newData);
		root = dataNode;
	}
	else
	{
		BpTreeNode* current = getRoot();
		while (current) 
		{
			if (BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(current)) {
				// ���� ��尡 ������ ����� ���
				dataNode->insertDataMap(newData->GetFlightNumber(), newData);
				if (excessDataNode(dataNode)) {
					splitDataNode(dataNode);
				}
				return true;
			}
			else if (BpTreeIndexNode* indexNode = dynamic_cast<BpTreeIndexNode*>(current)) {
				// ���� ��尡 �ε��� ����� ���
				auto it = indexNode->getIndexMap()->lower_bound(newData->GetFlightNumber());
				if (it != indexNode->getIndexMap()->end()) {
					current = it->second;
				}
				else {
					current = indexNode->getMostLeftChild();
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

void BpTree::splitDataNode(BpTreeNode* pDataNode) {

	BpTreeDataNode* newNode = new BpTreeDataNode();
	auto it = pDataNode->getDataMap()->begin();
	advance(it, pDataNode->getDataMap()->size() / 2);
	while (it != pDataNode->getDataMap()->end()) {
		newNode->insertDataMap(it->first, it->second);
		pDataNode->deleteMap(it->first);
		it = pDataNode->getDataMap()->begin();
		advance(it, pDataNode->getDataMap()->size() / 2);
	}

	// ���� �� �ε��� ��带 �����ϰ�, ���ҵ� ������ ��带 �����մϴ�.
	if (pDataNode->getParent() == nullptr) {
		BpTreeIndexNode* newRoot = new BpTreeIndexNode();
		newRoot->insertIndexMap(newNode->getDataMap()->begin()->first, newNode);
		newRoot->setMostLeftChild(pDataNode);
		pDataNode->setParent(newRoot);
		newNode->setParent(newRoot);
		root = newRoot;
	}
	else {
		BpTreeIndexNode* parent = dynamic_cast<BpTreeIndexNode*>(pDataNode->getParent());
		parent->insertIndexMap(newNode->getDataMap()->begin()->first, newNode);
		newNode->setParent(parent);
		if (excessIndexNode(parent)) {
			splitIndexNode(parent);
		}
	}
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	// ���ο� �ε��� ��带 �����մϴ�.
	BpTreeIndexNode* newNode = new BpTreeIndexNode();

	// ���� ����� ���� Ű�� ���ο� ���� �̵��մϴ�.
	auto it = pIndexNode->getIndexMap()->begin();
	std::advance(it, pIndexNode->getIndexMap()->size() / 2);
	while (it != pIndexNode->getIndexMap()->end()) {
		newNode->insertIndexMap(it->first, it->second);
		pIndexNode->deleteMap(it->first);
		it = pIndexNode->getIndexMap()->begin();
		advance(it, pIndexNode->getIndexMap()->size() / 2);
	}

	// ���ο� �ε��� ��带 �θ� ��忡 �����մϴ�.
	if (pIndexNode->getParent() == nullptr) {
		BpTreeIndexNode* newRoot = new BpTreeIndexNode();
		newRoot->insertIndexMap(newNode->getIndexMap()->begin()->first, newNode);
		newRoot->setMostLeftChild(pIndexNode);
		pIndexNode->setParent(newRoot);
		newNode->setParent(newRoot);
		root = newRoot;
	}
	else {
		BpTreeIndexNode* parent = dynamic_cast<BpTreeIndexNode*>(pIndexNode->getParent());
		parent->insertIndexMap(newNode->getIndexMap()->begin()->first, newNode);
		newNode->setParent(parent);
		if (excessIndexNode(parent)) {
			splitIndexNode(parent);
		}
	}
}

BpTreeNode* BpTree::searchDataNode(string name) {
	BpTreeNode* pCur = root;

	while (pCur) {
		if (pCur->getDataMap()->find(name) != pCur->getDataMap()->end()) {
			return pCur;
		}
		pCur = pCur->getNext();
	}
	return nullptr;
}

/*
bool BpTree::SearchModel(string model_name) {

}

bool BpTree::SearchRange(string start, string end) {

}

void BpTree::Print() {

}
*/