#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>

#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here

    virtual void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    virtual void rotateToRight(AVLNode<Key,Value>* n1);
    virtual void rotateToLeft(AVLNode<Key,Value>* n1);
    virtual void removeFix(AVLNode<Key, Value>* n, int8_t diff); 

}; 



/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::rotateToRight(AVLNode<Key,Value>* n1){
  //set three temp variables: the parent of n1, the right child of the left child of n1, and the left child of n1
    AVLNode<Key, Value>* tempparent = n1->getParent();
    AVLNode<Key, Value>* temp2 = n1->getLeft()->getRight();
    AVLNode<Key, Value>* temp1 = n1->getLeft();

    //if the left child of the right child is null set the left child of n1 to null
    if(temp2 == NULL) n1->setLeft(NULL);
    //otherwise, set the right child of the left child of n1 to null, set the left of n1 to the right child, and the parent of temp2 to temp1
    else{
      temp1->setRight(NULL);
      n1->setLeft(temp2);
      temp2->setParent(n1);
    }
    //if the parent of n1 is null temp 1 becomes the root
    if(tempparent==NULL) {
      this->root_ = temp1;
      temp1->setParent(NULL);
    }
        //if n1 is a left child adjust it correctly
    else if(tempparent->getLeft()==n1){
      temp1->setParent(tempparent);
      tempparent->setLeft(temp1);
    }
    //if n1 is a right child adjust it correctly
    else if(tempparent->getRight() == n1){
      temp1->setParent(tempparent);
      tempparent->setRight(temp1);
    }
    //readjust the n1 and left child of temp1
    temp1->setRight(n1);
    n1->setParent(temp1);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateToLeft(AVLNode<Key,Value>* n1){
  //same thing as rotatetoright just flipped right and left
    AVLNode<Key, Value>* tempparent = n1->getParent();
    AVLNode<Key, Value>* temp2 = n1->getRight()->getLeft();
    AVLNode<Key, Value>* temp1 = n1->getRight();

    if(temp2 == NULL) n1->setRight(NULL);

    else{
      temp1->setLeft(NULL);
      n1->setRight(temp2);
      temp2->setParent(n1);
    }

    if(tempparent==NULL) {
      this->root_ = temp1;
      temp1->setParent(NULL);
    }
    else if(tempparent->getLeft()!=n1){
      temp1->setParent(tempparent);
      tempparent->setRight(temp1);
    }
    else if(tempparent->getLeft() == n1){
      temp1->setParent(tempparent);
      tempparent->setLeft(temp1);

  }
    temp1->setLeft(n1);
    n1->setParent(temp1);
}
 
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n){

  //check if any variable or node we are using is null
  if(p == NULL || p->getParent()==NULL || n == NULL) return ;

  //g is grandparent
    AVLNode<Key, Value>* g = p->getParent();

  if(g->getLeft()==p){
    //update balance of g
    g->updateBalance(-1);
    //if balance of grandparent is 0 return
    if(g->getBalance() == 0) return ;
    //if balance of grandparent is -1 return 
    else if(g->getBalance() == -1) insertFix(g, p);
    //else if balance of grandparent is -2
    else if(g->getBalance() == -2){
      //zig zig condition
      if(p->getLeft() == n){ 
           rotateToRight(g);
           p->setBalance(0);
           g->setBalance(0);
        }
        //zig zag condition
        else{
            rotateToLeft(p); 
            rotateToRight(g);
            //update balances accurately
            if(n->getBalance() == -1){
              p->setBalance(0);
              g->setBalance(1);
              n->setBalance(0);
            
            }
            else if(n->getBalance() == 0){
              p->setBalance(0);
              g->setBalance(0);
              n->setBalance(0);
            }
            else if(n->getBalance() == 1){
              p->setBalance(-1);
              g->setBalance(0);
              n->setBalance(0);
            }
        }
    }
  }
  //same thing as left child just flipped -1/1 and right/left
  else if(g->getRight()==p){
        g->updateBalance(1);

      if(g->getBalance() == 0) return ;
      else if(g->getBalance() == 1) insertFix(g, p);
      else if(g->getBalance() == 2){
      if(p->getRight() == n){
           rotateToLeft(g);
           p->setBalance(0);
           g->setBalance(0);
        }
        else{
            rotateToRight(p);
            rotateToLeft(g);
            if(n->getBalance() == 1){
              p->setBalance(0);
              g->setBalance(-1);
              n->setBalance(0);
            }
            else if(n->getBalance() == 0){
              p->setBalance(0);
              g->setBalance(0);
              n->setBalance(0);
            }
            else if(n->getBalance() == -1){
              p->setBalance(1);
              g->setBalance(0);
              n->setBalance(0);
            }
          }
        }
      }
}
template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key, Value>* n, int8_t diff){
  //check n is not null
  if(n==NULL) return ;
  //set the parent of n
  AVLNode<Key, Value>* p = n->getParent();
  int8_t ndiff = 0;

  //if the parent of n is not null set the balances accurately
  if(p!=NULL){
    if(p->getLeft() == n) ndiff = 1;
    else if(p->getRight() == n) ndiff = -1;
  }
  //if diff is -1
  if(diff == -1){
    //add up the balance to diff == -2
    if(n->getBalance() + diff == -2){
    AVLNode<Key, Value>* c = n->getLeft();
    //update the balances according to the left child of n's balance
    if(c->getBalance() == -1){
      rotateToRight(n);
      n->setBalance(0);
      c->setBalance(0);
      removeFix(p, ndiff);
    }else if(c->getBalance() == 0){
      rotateToRight(n);// error here
      n->setBalance(-1);
      c->setBalance(1);
    }else if(c->getBalance() == 1){
      AVLNode<Key, Value>* g = c->getRight();
      //rotate to left and then right
      rotateToLeft(c);
      rotateToRight(n);
      //update balances according to balances of g
      if(g->getBalance() == 1){
        n->setBalance(0);
        c->setBalance(-1);
        g->setBalance(0);
      }else if(g->getBalance() == 0){
        n->setBalance(0);
        c->setBalance(0);
        g->setBalance(0);    
      }else if(g->getBalance() == -1){
        n->setBalance(1);
        c->setBalance(0);
        g->setBalance(0);    
      }
      //remove fix then recursively
      removeFix(p, ndiff);
    }
  }
  //if the balances of n + diff == -1, set the balances of n to -1 
    else if(n->getBalance() + diff == -1) n->setBalance(-1);
  //if the balances of n + diff == 0, set the balances of n to 0 and call removefix recursivelg

    else if(((n->getBalance()) +diff) == 0){
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }
  //same thing as -1 but just flip -/+ and right/left
  else if(diff == 1){
  if(n->getBalance() + diff == 2){
    AVLNode<Key, Value>* c = n->getRight();
    if(c->getBalance() == 1){
      rotateToLeft(n);
      n->setBalance(0);
      c->setBalance(0);
      removeFix(p, ndiff);
    }else if(c->getBalance() == 0){
      rotateToLeft(n);
      n->setBalance(1);
      c->setBalance(-1);
    }else if(c->getBalance() == -1){
      AVLNode<Key, Value>* g = c->getLeft();
      rotateToRight(c);
      rotateToLeft(n);
      if(g->getBalance() == -1){
        n->setBalance(0);
        c->setBalance(1);
        g->setBalance(0);
      }else if(g->getBalance() == 0){
        n->setBalance(0);
        c->setBalance(0);
        g->setBalance(0);    
      }else if(g->getBalance() == 1){
        n->setBalance(-1);
        c->setBalance(0);
        g->setBalance(0);    
      }
      removeFix(p, ndiff);
    }
  }else if(n->getBalance() + diff == 1) n->setBalance(1);
  else if(n->getBalance() +diff == 0){
    n->setBalance(0);
    removeFix(p, ndiff);
  }
  }
  
}


template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
//if theres nothing in the tree, create a new node and tree
  if(this->root_==nullptr){
     AVLNode<Key, Value>* g = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
     g->setBalance(0);
     this->root_ = g;
     return ;
  }
  //set temp to root 
  AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);

  while(temp!=NULL){
    //if you haven't reached the end of the tree insert it to the apporpriate position
    //this position depends on whether its greater than (right) or less than (left) of the node

      if(new_item.first >temp->getKey()){
          if(temp->getRight()==NULL){
              AVLNode<Key, Value>* n = new AVLNode<Key,Value>(new_item.first, new_item.second, temp);
              temp->setRight(n);
              temp = temp->getRight();
              break ;
          }else{
              temp = temp->getRight();
          }
          
      }
      else if(new_item.first<temp->getKey()){
          if(temp->getLeft()==NULL){
              AVLNode<Key, Value>* g = new AVLNode<Key,Value>(new_item.first, new_item.second, temp);
              temp->setLeft(g);
              temp = temp->getLeft();
              break ; 
          }else{
              temp=temp->getLeft();
          }

      }
      //if its equal to an already existing node, just overwrite it
      else if(new_item.first==temp->getKey()){
          temp->setValue(new_item.second);
          break ;
      }
  }

  temp->setBalance(0);
  if(temp == NULL ||temp->getParent()==NULL) return ;
  //if anything your using is null return it 
  AVLNode<Key, Value>* p = temp->getParent();

//adjust balance of p 
  if(p->getBalance() == -1 || p->getBalance() == 1){
    p->setBalance(0);
  }
  //if its 0, set the balance appropriately based on right, left child and call insertfix
  else if(p->getBalance() == 0){
    if(p->getLeft() == temp) p->setBalance(-1);
    if(p->getRight() == temp) p->setBalance(1);
    insertFix(p, temp);
  }



}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  //if theres nothing in the tree just return 
  if(this->root_==NULL) return ;
    AVLNode<Key, Value> *temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(temp == NULL) return ;
    //temp is now at appropriate position

//node swap if n has 2 children with the predecessor
  if(temp->getRight()!=NULL && temp->getLeft()!=NULL){
    nodeSwap(temp, static_cast<AVLNode<Key, Value>*>(this->predecessor(temp)));

  }
  int8_t diff;
//update diff apporpriately
  AVLNode<Key,Value>* p = temp->getParent();
  if(p!=NULL){
    if(p->getLeft() == temp) diff = 1;
    else if(p->getRight() == temp) diff = -1;
    
  }

//explanation: 
//child = child of temp
//we're deleting temp
//t is temps parents
//change the pointers of p to point to child

  AVLNode<Key, Value>* child;
    if(temp->getRight()!=NULL && temp->getLeft()==NULL) child = temp->getRight();
    
    else if(temp->getRight()==NULL && temp->getLeft()!=NULL) child = temp->getLeft();
    
    else if(temp->getRight() == NULL && temp->getLeft() == NULL) child = NULL;
    


    AVLNode<Key, Value> *t = temp->getParent();
    if(t!=NULL){
      if(temp->getParent()->getRight()==temp) t->setRight(child); 
      else if(temp->getParent()->getLeft()==temp) t->setLeft(child);
    }
    else{
      this->root_ = child;
    }

//if child is not null set the parent of child to t
  if(child !=NULL) child->setParent(t);
  
  //delete the in between(temp) and removefix it 
  delete temp;
  removeFix(p, diff);

}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
