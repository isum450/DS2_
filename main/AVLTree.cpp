#include "AVLTree.h"

bool AVLTree::Insert(FlightData* pFlightData) {
	AVLNode* newNode = new AVLNode();
	newNode->setFlightData(pFlightData);
	newNode->getFlightData()->SetNumberofSeats(0);
	newNode->setLeft(nullptr);
	newNode->setRight(nullptr);
	newNode->setBF(0);

	if (root == nullptr)//root create
	{
		root = newNode;
		return true;
	}

	AVLNode* parent = nullptr;
	AVLNode* cur = root;
		
	while (cur != nullptr)
	{
		parent = cur;
		if (cur->getFlightData()->GetFlightNumber() > pFlightData->GetFlightNumber())//go left
		{
			cur = cur->getLeft();
		}
		else if (cur->getFlightData()->GetFlightNumber() < pFlightData->GetFlightNumber())//go right
		{
			cur = cur->getRight();
		}
	}

	//newNode insert
	if (pFlightData->GetFlightNumber() < parent->getFlightData()->GetFlightNumber())
		parent->setLeft(newNode);
	else
		parent->setRight(newNode);

	root = Balance(root);//balance factor set

	return true;
}

int AVLTree::getHeight(AVLNode* avlNode)//get height (recursive)
{
	if (avlNode != nullptr)
	{
		int left_height = getHeight(avlNode->getLeft());
		int right_height = getHeight(avlNode->getRight());

		if (left_height > right_height)
			return left_height + 1;
		else
			return right_height + 1;
	}

	return 0;//nullptr
}

int AVLTree::SetBF(AVLNode* avlNode)
{
	int left = getHeight(avlNode->getLeft());
	int right = getHeight(avlNode->getRight());
	
	return left - right;//balance factor
}

void AVLTree::UpdateBF(AVLNode* avlNode)//recursive
{
	if (avlNode == nullptr)
		return;
	avlNode->setBF(SetBF(avlNode));//set balance factor
	UpdateBF(avlNode->getLeft());
	UpdateBF(avlNode->getRight());
}

AVLNode* AVLTree::Balance(AVLNode* avlNode)
{
	if (avlNode == nullptr)
	{
		return avlNode;
	}
	UpdateBF(avlNode);

	//unbalanced AVL tree -> Rotation
	if (avlNode->getBF() > 1) {
		if (avlNode->getLeft()!=nullptr && avlNode->getLeft()->getBF() < 0) {
			avlNode->setLeft(RR(avlNode->getLeft())); //   LR
		}
		return LL(avlNode); // LL
	}
	if (avlNode->getBF() < -1) {
		if (avlNode->getRight()!=nullptr&&avlNode->getRight()->getBF() > 0) {
			avlNode->setRight(LL(avlNode->getRight())); // RL
		}
		return RR(avlNode); // RR
	}

	return avlNode;//after rotate
}

AVLNode* AVLTree::LL(AVLNode* node) {//LL Rotation
	AVLNode* parent = node->getLeft();
	if (parent == nullptr) {
		return node;
	}
	node->setLeft(parent->getRight());
	parent->setRight(node);

	//balance factor update 
	node->setBF(SetBF(node));
	parent->setBF(SetBF(parent));

	return parent;
}

AVLNode* AVLTree::RR(AVLNode* node) {//RR Rotation
	AVLNode* parent = node->getRight();
	if (parent == nullptr) {
		return node;
	}

	node->setRight(parent->getLeft());
	parent->setLeft(node);

	//balance factor update 
	node->setBF(SetBF(node));
	parent->setBF(SetBF(parent));

	return parent;
}


void AVLTree::GetVector(vector<FlightData*>& v) {

	queue<AVLNode*> nodeQueue;
	nodeQueue.push(root); // root node insert queue

	while (!nodeQueue.empty()) {
		AVLNode* currentNode = nodeQueue.front();
		nodeQueue.pop();

		//insert vector
		v.push_back(currentNode->getFlightData());

		//insert queue
		if (currentNode->getLeft() != nullptr) {
			nodeQueue.push(currentNode->getLeft());
		}

		//insert queue
		if (currentNode->getRight() != nullptr) {
			nodeQueue.push(currentNode->getRight());
		}
	}
}


FlightData* AVLTree::Search(string name) {

	AVLNode* currentNode = root;

	while (currentNode != nullptr) {
		if (currentNode->getFlightData()->GetFlightNumber() == name)
		{
			//name ==AVL_DATA
			return currentNode->getFlightData();
		}
		else if (name < currentNode->getFlightData()->GetFlightNumber()) // name < AVL_DATA
		{
			currentNode = currentNode->getLeft();
		}
		else //name > AVL_DATA 
		{
			currentNode = currentNode->getRight();
		}
	}

	return nullptr;

}
