#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "FlightData.h"
#include "AVLNode.h"
/* class AVL */

class AVLTree {
private:
	AVLNode* root;

public:

	AVLTree() {
		root = NULL;
	}
	~AVLTree() {};
	bool		Insert(FlightData* pFlightData);
	int			getHeight(AVLNode* avlNode);
	int			SetBF(AVLNode* avlNode);
	void		UpdateBF(AVLNode* avlNode);
	AVLNode*	Balance(AVLNode* avlNode);
	AVLNode*	RR(AVLNode* node);
	AVLNode*	LL(AVLNode* node);
	AVLNode*	RL(AVLNode* node);
	AVLNode*	LR(AVLNode* node);
	AVLNode*	getRoot() { return root; };
	void setRoot(AVLNode* node) { root = node; };

	FlightData* Search(string name);
	void GetVector(vector<FlightData*>& v);
};

#endif

