#include "AVLTree.h"

bool AVLTree::Insert(FlightData* pFlightData) {
	AVLNode* newNode = new AVLNode();
	newNode->setFlightData(pFlightData);
	newNode->setLeft(nullptr);
	newNode->setRight(nullptr);
	newNode->setBF(0);
	if (root == nullptr)
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

	if (pFlightData->GetFlightNumber() < parent->getFlightData()->GetFlightNumber())
		parent->setLeft(newNode);
	else
		parent->setRight(newNode);

	root = Balance(root);

	return true;
}

int AVLTree::getHeight(AVLNode* avlNode)
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

	return 0;
}

int AVLTree::SetBF(AVLNode* avlNode)
{
	int left = getHeight(avlNode->getLeft());
	int right = getHeight(avlNode->getRight());
	
	return left - right;
}

void AVLTree::UpdateBF(AVLNode* avlNode)
{
	if (avlNode == nullptr)
		return;
	avlNode->setBF(SetBF(avlNode));
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

	if (avlNode->getBF() > 1) {
		if (avlNode->getLeft()->getBF() > 0) {
			avlNode->setLeft(LR(avlNode->getLeft())); // LR
		}
		return LL(avlNode); // LL
	}
	if (avlNode->getBF() < -1) {
		if (avlNode->getRight()->getBF() < 0) {
			avlNode->setRight(RL(avlNode->getRight())); // RL
		}
		return RR(avlNode); // RR
	}
	return avlNode;
}

AVLNode* AVLTree::LL(AVLNode* node) {
	AVLNode* parent = node->getLeft();
	node->setLeft(parent->getRight());
	parent->setRight(node);

	// 회전 후 밸런스 팩터 업데이트
	node->setBF(SetBF(node));
	parent->setBF(SetBF(parent));

	return parent;
}

AVLNode* AVLTree::RR(AVLNode* node) {
	AVLNode* parent = node->getRight();
	node->setRight(parent->getLeft());
	parent->setLeft(node);

	// 회전 후 밸런스 팩터 업데이트
	node->setBF(SetBF(node));
	parent->setBF(SetBF(parent));

	return parent;
}

AVLNode* AVLTree::LR(AVLNode* node) {
	node->setLeft(RR(node->getLeft()));
	return LL(node);
}

AVLNode* AVLTree::RL(AVLNode* node) {
	node->setRight(LL(node->getRight()));
	return RR(node);
}


void AVLTree::GetVector(vector<FlightData*>& v) {
	if (root == nullptr) return; // 트리가 비어 있는 경우

	std::queue<AVLNode*> nodeQueue;
	nodeQueue.push(root); // 루트 노드를 큐에 삽입

	while (!nodeQueue.empty()) {
		AVLNode* currentNode = nodeQueue.front();
		nodeQueue.pop();

		// 현재 노드의 데이터를 벡터에 추가
		v.push_back(currentNode->getFlightData());

		// 왼쪽 자식이 있다면 큐에 추가
		if (currentNode->getLeft() != nullptr) {
			nodeQueue.push(currentNode->getLeft());
		}

		// 오른쪽 자식이 있다면 큐에 추가
		if (currentNode->getRight() != nullptr) {
			nodeQueue.push(currentNode->getRight());
		}
	}
}

/*
FlightData* AVLTree::Search(string name) {
	AVLNode* cur = root;


}
*/