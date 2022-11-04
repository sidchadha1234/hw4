#include "equal-paths.h"
#include <iostream>
using namespace std;



int helper(Node * root)
{

	//if the root itself is null, return 0
	if(root==nullptr){
		return 0;
	}
//count the height all the way to left and right
	int countleft=helper(root->left);
	int countright=helper(root->right);
	//return the max of them 
	return max(countleft+1, countright+1);
}

bool equalPaths(Node * root){
	if(root==nullptr) return true;
  //if left child and right child aren't null, compare the two and make sure they're the same height
	if(root->right!=NULL && root->left!=NULL){
			int r = helper(root->right);
			int l = helper(root->left);
			return r==l;
	}
  //if the right child isn't null and left is look at the left child and its children
  else if(root->right==NULL && root->left!=NULL){
			return equalPaths(root->left);
	}
  //same as previous just with right child
  else if(root->right!=NULL && root->left==NULL){
			return equalPaths(root->right);

	}
//both children are null - return true
  else{
			return true;
	}
	
}