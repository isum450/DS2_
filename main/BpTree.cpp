#include "BpTree.h"

BpTree::BpTree(ofstream* fout, int order) {
	root = nullptr;
	this->order = order;
	this->fout = fout;

	ifstream ifp;
	ifp.open("C:\\Users\\is104\\DATASTRUCTURE\\main\\DS2\\DS2\\main\\input_data.txt");

	if (ifp.fail())//file not connect
	{
		*fout << "====Error====" << endl;
		*fout << "100" << endl;
		*fout << "=============" << endl;
		exit(1);
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

		Insert(DATA);

	}
}

bool BpTree::Insert(FlightData* newData) {
	
	
	if (getRoot() == nullptr)//insert root ó�� ��Ʈ�� ������ ���� ����
	{
		BpTreeDataNode* dataNode = new BpTreeDataNode();
		dataNode->insertDataMap(newData->GetFlightNumber(), newData);
		root = dataNode;
		cout << newData->GetFlightNumber() << endl;
	}
	else
	{
		BpTreeNode* current = getRoot();
		while (current) 
		{
			if (BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(current)) {
				// ���� ��尡 ������ ����� ���
				auto datamap = current->getDataMap();
				string key[3];
				FlightData* node[3];
				int i = 0;
				for (const auto& pair : *datamap) {
					key[i] = pair.first;
					node[i] = pair.second;
					i++;
				}


				dataNode->insertDataMap(newData->GetFlightNumber(), newData);
				if (excessDataNode(dataNode)) {
					splitDataNode(dataNode);
				}
				cout << newData->GetFlightNumber() << endl;
				return true;
			}
			else if (BpTreeIndexNode* indexNode = dynamic_cast<BpTreeIndexNode*>(current)) {
				// ���� ��尡 �ε��� ����� ���
				auto idxmap = indexNode->getIndexMap();
				string key[3];
				BpTreeNode* node[3];
				int i = 0;
				for (const auto& pair : *idxmap) {
					key[i] = pair.first;          
					node[i] = pair.second;    
					i++;
				}
				string it = newData->GetFlightNumber();
				if (key[0]>it) {//ù��° Ű �����ٵ� ���� ��
					current = indexNode->getMostLeftChild();
				}
				else if ((key[0]<it) && (key[1] > it))
				{
					current = node[0];
				}
				else if(key[1] <it ){//�ι�° Ű �����ٵ� ū ���
					current = node[1];
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

	auto datamap = pDataNode->getDataMap();
	string key[3];
	FlightData* node[3];
	int i = 0;
	for (const auto& pair : *datamap) {
		key[i] = pair.first;
		node[i] = pair.second;
		i++;
	}
	//i�� 2�̾�� ��.
	newNode->insertDataMap(key[0], node[0]);
	pDataNode->deleteMap(key[0]);
	newNode->insertDataMap(key[1], node[1]);
	pDataNode->deleteMap(key[1]);

	

	// ���ο� �ε��� ��带 �����ϰ�, ���ҵ� ������ ��带 �����մϴ�.
	if (pDataNode->getParent() == nullptr) {
		BpTreeIndexNode* newRoot = new BpTreeIndexNode();

		newRoot->insertIndexMap(key[1], pDataNode);
		newRoot->setMostLeftChild(newNode);
		pDataNode->setParent(newRoot);
		newNode->setParent(newRoot);
		root = newRoot;
	}
	else {
		BpTreeIndexNode* parent = dynamic_cast<BpTreeIndexNode*>(pDataNode->getParent());
		parent->insertIndexMap(key[1], pDataNode);
		newNode->setParent(parent);
		pDataNode->setParent(parent);
		if (excessIndexNode(parent)) {
			splitIndexNode(parent);//�����ϰ� mostleftchild �����ؾ� ��
		}
	}
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	// ���ο� �ε��� ��带 �����մϴ�.
	BpTreeIndexNode* newNode = new BpTreeIndexNode();

	auto idxmap = pIndexNode->getIndexMap();
	string key[3];
	BpTreeNode* node[3];
	int i = 0;
	for (const auto& pair : *idxmap) {
		key[i] = pair.first;
		node[i] = pair.second;
		i++;
	}

	newNode->insertIndexMap(key[0], node[0]);
	pIndexNode->deleteMap(key[0]);
	newNode->insertIndexMap(key[1], node[1]);
	pIndexNode->deleteMap(key[1]);

	// ���ο� �ε��� ��带 �θ� ��忡 �����մϴ�.
	if (pIndexNode->getParent() == nullptr) {
		BpTreeIndexNode* newRoot = new BpTreeIndexNode();
		newRoot->insertIndexMap(key[1], pIndexNode);
		newRoot->setMostLeftChild(newNode);
		pIndexNode->setParent(newRoot);
		newNode->setParent(newRoot);
		root = newRoot;
	}
	else {
		BpTreeIndexNode* parent = dynamic_cast<BpTreeIndexNode*>(pIndexNode->getParent());
		parent->insertIndexMap(key[1], newNode);
		newNode->setParent(parent);
		pIndexNode->setParent(parent);
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


bool BpTree::SearchModel(string model_name) {

	BpTreeNode* pCur = root;

	while (pCur) {
		if (pCur->getDataMap()->find(model_name) != pCur->getDataMap()->end()) {
			return true;
		}
		pCur = pCur->getNext();
	}
	return false;
}

/*
bool BpTree::SearchRange(string start, string end) {

}
*/

void BpTree::Print() {
	BpTreeNode* current = root;
	while (current->getMostLeftChild() != nullptr)
		current = current->getMostLeftChild();
	
	while (current->getNext() != nullptr)
	{
		auto datamap = current->getDataMap();
		for (const auto& pair : *datamap) {
			cout << pair.first << " ";
		}
		cout << endl;

		current = current->getNext();
	}

}